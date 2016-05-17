/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "BundleAdjustment.h"
#include "Dms.h"
#include "Point.h"
#include "RayTester.h"
#include "InteriorOrientation.h"
#include "SparseMatrix.h"
#include "ConvertionsSystems.h"
#include "math.h"
#include "Image.h"
#include "Sensor.h"
#include "Flight.h"

#include <iostream>
#include <QTime>


//#define FRANC
#define PAULO

#define MAXRESIDUO 0.0001
#define ESPARSA
//#define TIMES

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


BundleAdjustment::BundleAdjustment(std::deque<Image*>listSelectedImages, std::deque<Point*> listSelectedPoints)
{
    listImages=listSelectedImages;
    listPoints=listSelectedPoints;
    //	fliDir=flightDirection;

    numImages=listImages.size();
    numPoints=listPoints.size();
    numControlPoints=0;
    numFotogrametricPoints=0;

    zeroingCoordinatesPhotogrammetrics();

    for (int i=0;i<numPoints;i++)
    {
        if(listPoints.at(i)->getType() == Point::CONTROL)
        {
            listControlPoints.push_back(listPoints.at(i));
            numControlPoints++;
        }
        else if(listPoints.at(i)->getType() == Point::PHOTOGRAMMETRIC)
        {
            listPhotogrammetricPoints.push_back(listPoints.at(i));
            numFotogrametricPoints++;
        }
    }

    fillDetectorCoordinates();
    numEquations=numberOfEquations();
    numUnknows=6*numImages+3*numFotogrametricPoints;

    /** serÃÂ´a eliminiado em breve*/
    c= listImages.at(0)->getSensor()->getFocalDistance();
    xsi0=listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getXi();
    eta0=listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getEta();

    userInitialValues = hasAllImagesInitialValues();
    //userInitialValues= false;

    /*
        for (int i=0;i<listPhotogrammetricPoints.size();i++)
                qDebug("%s",listPhotogrammetricPoints.at(i)->getPointId().c_str());

        //qDebug("Numero de todos os pontos: %d",listPoints.size());*/
    //qDebug("%s",printAll().c_str());

}

int BundleAdjustment::numberOfEquations()
{
    int n=0;
    for (int i=0;i<numImages;i++)
        n+=numberControlPoints(listImages.at(i))+numberPhotogrammetricPoints(listImages.at(i));
    return 2*n;
}

Point *BundleAdjustment::getPointFrom(int imageIndex, int pointIndex)
{
    return listImages.at(imageIndex)->getPointAt(pointIndex);
}

void BundleAdjustment::fillDetectorCoordinates()
{
    for (int i=0; i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        for (int j=0;j<pnts;j++)
        {
            double col=getPointFrom(i,j)->getImageCoordinate(listImages.at(i)->getId()).getCol();
            double lin=getPointFrom(i,j)->getImageCoordinate(listImages.at(i)->getId()).getLin();

            //Matrix coord=digitalToAnalog(listImages.at(i)->getIO(),lin,col);
            RayTester ray(listImages.at(i));
            ray.setIOParameters(listImages.at(i)->getIO()->getXa());
            DetectorSpaceCoordinate coordinates = ray.imageToDetector(col,lin);

            DetectorSpaceCoordinate aux(listImages.at(i)->getId());
            aux.setXi(coordinates.getXi());
            aux.setEta(coordinates.getEta());
            getPointFrom(i,j)->putDetectorCoordinate(aux);
        }
    }
}

//Professor Nunes mandou matar a iteraÃÂ§ao sobre os residuos para evitar os valores "NANs"
bool BundleAdjustment::calculate(bool makeReport)
{
    if (!userInitialValues)
    {
        calculateInicialsValues();
        //matInitialValues.show('f',5,"mat Inicial Values");
        for (unsigned int i=0;i<listImages.size();i++)
        {
            Image *img=listImages.at(i);
            if(img->isInsAvailable() && img->getInsType()=="Initial")
            {
                matInitialValues.set(i+1,1,img->getInsOmega());
                matInitialValues.set(i+1,2,img->getInsPhi());
                matInitialValues.set(i+1,3,img->getInsKappa());
            }
            if(img->isGnssAvailable() && img->getGnssType()=="Initial")
            {
                matInitialValues.set(i+1,4,img->getGnssX0());
                matInitialValues.set(i+1,5,img->getGnssY0());
                matInitialValues.set(i+1,6,img->getGnssZ0());
            }
        }
    }

    //double range=0.0;
    //double diff=0.0;
    //normalize(listPoints,range,diff);

    matAdjust=matInitialValues;

    //qDebug("%s",printAll().c_str());

    matAdjust.show('f',5,"matAdjust Inicial Values");

    //matAdjust=convertToGeocentric(matAdjust,WGS84,-1,23);
    //matAdjust.show('f',5,"matAdjust Inicial Values em Geocentricas");

    /*
        matAdjust=convertToUTM(matAdjust,WGS84,);
        matAdjust.show('f',5,"matAdjust Inicial Values em UTM");
        */

    P.identity(numEquations);
    //P.show();
    //bool resOk=false;
    //int changePesos=0;
    //Matrix tempRes;
    totalIterations=0;

    QTime ptime;
    if (numFotogrametricPoints!=0)
    {
        //while(!resOk && totalIterations<maxIterations)
        //{
        int conv=0;
        while(totalIterations<maxIterations && conv!=1)
            //while(convIterations<maxIterations && conv!=1)
        {
            ptime.start();
            createA1();
            //int A1time=ptime.restart();
            //A1.show('f',3,"A1");

            createA2();
            //A2.show('f',3,"A2");
            //int A2time=ptime.restart();

            createL0();
            //L0.show('f',3,"L0");
            //int l0time=ptime.restart();

            createLb();
            //Lb.show('f',3,"Lb");
            //int lbtime=ptime.restart();

            Matrix l=Lb-L0;

            ptime.start();
            /*Matrix*/ n11=getN11();
            //int n11time=ptime.restart();
            //n11.show('f',3,"N11=A1^T*P*A1");


            /*Matrix*/ n12=getN12();
            //n12.show('f',3,"N12=A1^T*P*A2");
            //int n12time=ptime.restart();

            /*Matrix*/ n22=getN22();
            //n22.show('f',3,"N22==A2^T*P*A2");
            //int n22time=ptime.restart();


            //int invN22time=ptime.restart();

            /*Matrix*/ n1=getn1(l);
            //n1.show('f',3,"n1=A1^T*P*L");
            //int n1time=ptime.restart();

            /*Matrix*/ n2=getn2(l);
            //n2.show('f',3,"n2==A2^T*P*L");
            //int n2time=ptime.restart();
#ifdef PAULO
            setInverseN11(n11);
            //int invN11time=ptime.restart();
            setx2(n12,n22,n2,n1);
            //int x2time=ptime.restart();
            //x2.show();

            setx1(n12,n1);
            //int x1time=ptime.restart();
            //x1.show();
#endif

#ifdef FRANC

            setInverseN22(n22);
            int invN22time=ptime.restart();
            setx1(n12,n1);
            int x1time=ptime.restart();

            setx2(n12,n22,n2,n1);
            int x2time=ptime.restart();
            //x2.show();

#endif

#ifdef TIMES
            qDebug("Tempo para executar a %d iteracao",totalIterations);
            qDebug("Para criar A1: %.3f",A1time/1000.0);
            qDebug("Para criar A2: %.3f",A2time/1000.0);
            qDebug("Para criar L0: %.3f",l0time/1000.0);
            qDebug("Para criar Lb: %.3f",lbtime/1000.0);
            qDebug("Para calcular N11: %.3f",n11time/1000.0);
            qDebug("Para calcular N12: %.3f",n12time/1000.0);
            qDebug("Para calcular N22: %.3f",n22time/1000.0);
#ifdef PAULO
            qDebug("Para calcular inversaN11: %.3f",invN11time/1000.0);
#endif

#ifdef FRANC
            qDebug("Para calcular inversaN11: %.3f",invN22time/1000.0);
#endif
            qDebug("Para calcular n1: %.3f",n1time/1000.0);
            qDebug("Para calcular n2: %.3f",n2time/1000.0);
            qDebug("Para calcular x1: %.3f",x1time/1000.0);
            qDebug("Para calcular x2: %.3f",x2time/1000.0);
#endif
            //x1.show('f',3,"x1");
            //x2.show('f',3,"x2");

            conv=testConverged();
            if (conv ==-2)
            {
                std::cout << "Apareceu valor infinito" << std::endl;
                break;
            }
            if (conv ==-1)
            {
                std::cout << "Apareceu valor NAN" << std::endl;
                break;
            }
            //matAdjust.show('f',5,"MatAdjus antes do update do loop");
            updateMatAdjust();
            //matAdjust.show('f',5,"MatAdjus depois do update do loop");
            updateCoordFotog();

            //matAdjust.show('f',5,"matAdjust Iterando");
            totalIterations++;

            if(makeReport)
            {
                calculateResiduos();
                listRMSE.push_back(calculateRMSE());
            }

            qDebug("iteration %d/%d",totalIterations,maxIterations);
        }
        //qDebug("numero iterations %d=%d",changePesos,iterations);

        //matAdjust.show('f',5,"MatAdjus depois da iteracao");
        calculateResiduos();

        //matRes.show('f',5,"MatRes depois da iteracao");
        /*
                        resOk=testResiduo();
                        if (changePesos==0)
                                tempRes.resize(matRes.getRows(),1);
                        calculatePeso();
                        matRes=matRes-tempRes;
                        tempRes=matRes;
                        changePesos++;
                        //totalIterations+=iterations;
                        //matRes.show('f',5,"MatRes");
                }*/
    }
    else
    {
        //qDebug("Sem pontos Fotogrametricos");
        /*while(!resOk)
                {*/
        totalIterations=0;
        int conv=0;
        while(totalIterations<maxIterations && conv!=1)
        {
            createA1();
            createL0();
            createLb();
            Matrix l=Lb-L0;
            Matrix n11=getN11();
            Matrix n1=getn1(l);
            //n11.show('f',3,"N11");
            setInverseN11(n11);
#ifdef ESPARSA
            x1=SparseMatrix(inverseN11)*n1;
#endif
            //x1.show('f',3,"x1");
            //matAdjust.show('f',5,"MatAdjus antes do update do loop");

            conv=testConverged();
            if (conv ==-2)
            {
                std::cout << "Apareceu valor infinito" << std::endl;
                break;
            }
            if (conv ==-1)
            {
                std::cout << "Apareceu valor NAN" << std::endl;
                break;
            }
            totalIterations++;
            qDebug("iteration %d",totalIterations);
            updateMatAdjust();
            //matAdjust.show('f',5,"MatAdjus depois do update do loop");
        }
        //qDebug("numero iterations %d=%d",changePesos,iterations);

        //matAdjust.show('f',7,"MatAdjus depois da iteracao");
        calculateResiduos();
        //matRes.show('f',5,"MatRes depois da iteracao");
        //resOk=testResiduo();
        //if (changePesos==0)
        //tempRes.resize(matRes.getRows(),1);
        //calculatePeso();
        //matRes=matRes-tempRes;
        //tempRes=matRes;
        //changePesos++;
        //totalIterations+=iterations;
        //matRes.show('f',5,"MatRes");
        //}
    }
    //printf("Numero de troca de pesos: %d\n",changePesos);
    calculateResiduos();
    //matRes.show('f',8,"MatResCalculate");

    /*
        matAdjust=convertToGeocentric(matAdjust,WGS84,-1,23);
        matAdjust.show('f',5,"matAdjust Inicial Values em Geocentricas");
        */	//matAdjust=convertToUTM(matAdjust,WGS84);

    //desnormalize(listPoints,range,diff);

    //matAdjust.show('f',5,"matAdjust em UTM");

    //convertToUTM(listPoints,WGS84);
    setAFP();
    return true;
}

/* metodos de calculo*/
Matrix BundleAdjustment::getM11(Matrix M1)
{
#ifdef ESPARSA
    return SparseMatrix(M1.transpose())*M1;
#endif
    //Matrix temp=M1.transpose();
    return M1.transpose()*M1;
}

Matrix BundleAdjustment::getM12(Matrix M1, Matrix M2)
{
    //Matrix temp=M1.transpose();
    //return temp*M2;
#ifdef ESPARSA
    return SparseMatrix(M1.transpose())*M2;
#endif
    return M1.transpose()*M2;
}

Matrix BundleAdjustment::getM22(Matrix M2)
{

#ifdef ESPARSA
    return SparseMatrix(M2.transpose())*M2;
#endif
    return M2.transpose()*M2;

}

Matrix BundleAdjustment::getm1(Matrix M1, Matrix L1)
{

#ifdef ESPARSA
    return SparseMatrix(M1.transpose())*L1;
#endif
    return (M1.transpose())*L1;
}

Matrix BundleAdjustment::getm2(Matrix M2, Matrix L1)
{

#ifdef ESPARSA
    return SparseMatrix(M2.transpose())*L1;
#endif
    return (M2.transpose())*L1;
}

Matrix BundleAdjustment::getPAf(Matrix M12,Matrix m1, Matrix xypf)
{

#ifdef ESPARSA
    /*	SparseMatrix q1=SparseMatrix(SparseMatrix(M12)*M22.inverse());
        return	SparseMatrix((M11-q1*M12.transpose()).inverse() )*( m1-q1*m2 );*/
    SparseMatrix temp1=SparseMatrix(inverseM11);
    return temp1*m1-SparseMatrix(temp1*M12)*xypf;
#endif

    //	return ( (M11-M12*M22.inverse()*M12.transpose()).inverse() )*( m1-M12*M22.transpose()*m2 );
    //return ( (M11-M12*M22.inverse()*M12.transpose()).inverse() )*( m1-M12*M22.inverse()*m2 );
}

// AproximaÃÂ§ao inicial para as coordenadas planimetricas(XY) dos pontos fotogrametricos
Matrix BundleAdjustment::getXYpf(Matrix M12, Matrix M22, Matrix m1,Matrix m2)
{
#ifdef ESPARSA
    /*
        SparseMatrix q1=SparseMatrix(M22.inverse());
        return q1*m2-SparseMatrix(q1*M12.transpose())*paf;*/
    SparseMatrix temp1=SparseMatrix(SparseMatrix(M12.transpose())*inverseM11);
    return SparseMatrix((M22-temp1*M12).inverse())*(m2-temp1*m1);
#endif

    //return M22.inverse()*m2-M22.inverse()*M12.transpose()*paf;
}

Matrix BundleAdjustment::getN11()
{
#ifdef ESPARSA
    return SparseMatrix(SparseMatrix(A1.transpose())*P)*A1;
#endif
    return A1.transpose()*P*A1;
}

Matrix BundleAdjustment::getN12()
{
#ifdef ESPARSA
    return SparseMatrix(SparseMatrix(A1.transpose())*P)*A2;
#endif
    return A1.transpose()*P*A2;
}

Matrix BundleAdjustment::getN22()
{

#ifdef ESPARSA
    return SparseMatrix(A2.transpose())*A2;
#endif

    return A2.transpose()*A2;
}

Matrix BundleAdjustment::getn1(Matrix L)
{

#ifdef ESPARSA
    return SparseMatrix(SparseMatrix(A1.transpose())*P)*L;
#endif
    return A1.transpose()*P*L;

}

Matrix BundleAdjustment::getn2(Matrix L)
{
#ifdef ESPARSA
    return SparseMatrix(A2.transpose())*L;
#endif
    return A2.transpose()*L;

}

void BundleAdjustment::setx1(Matrix N12,Matrix n1)
{

#ifdef PAULO
    SparseMatrix temp1=SparseMatrix(inverseN11);
    x1=temp1*n1-SparseMatrix(temp1*N12)*x2;
#endif

#ifdef FRANC
    SparseMatrix temp1=SparseMatrix(SparseMatrix(n12)*inverseN22);
    //x1=SparseMatrix((n11-temp1*n12.transpose()).inverse())*(n1-temp1*n2);
    x1=SparseMatrix((n11-temp1*n12.transpose()).osuInverse())*(n1-temp1*n2);
#endif

}

void BundleAdjustment::setx2(Matrix N12, Matrix N22, Matrix n2, Matrix n1)
{

    /*
#ifdef ESPARSA
        SparseMatrix temp1=SparseMatrix(SparseMatrix(N12.transpose())*inverseN11);
        //Matrix mat=(N22-temp1*N12);
        //mat.show('f',5,"N22-temp1*N12");
        x2=SparseMatrix((N22-temp1*N12).inverse())*(n2-temp1*n1);
#endif
*/
#ifdef PAULO
    SparseMatrix temp1=SparseMatrix(SparseMatrix(N12.transpose())*inverseN11);
    //Matrix mat=(N22-temp1*N12);
    //mat.show('f',5,"N22-temp1*N12");
    //x2=SparseMatrix((N22-temp1*N12).inverse())*(n2-temp1*n1);
    x2=SparseMatrix((N22-temp1*N12).osuInverse())*(n2-temp1*n1);
#endif

#ifdef FRANC
    SparseMatrix temp1=SparseMatrix(inverseN22);
    x2=temp1*n2-SparseMatrix(temp1*n12.transpose())*x1;
#endif
    //return x2;
}

/*
void BundleAdjustment::setInverseN22(Matrix n22)
{
        int rows=n22.getRows();
        inverseN22=n22;
        //n22.show('f',4,"n22");
        for (int i=1;i<rows;i+=3)
        {
                Matrix unit=n22.sel(i,i+2,i,i+2);
                //unit.show('f',4,"unidade n22");
                inverseN22.putMatrix(unit.inverse(),i,i);
                //unit.inverse().show('f',4,"unidade inversa n22");
        }
        //inverseN22.show('f',4,"Inversa n22 ");
        //return n22;
}

*/
void BundleAdjustment::setInverseN11(Matrix n11)
{
    int rows=n11.getRows();
    inverseN11=n11;
    for (int i=1;i<rows;i+=6)
    {
        Matrix unit=n11.sel(i,i+5,i,i+5);
        inverseN11.putMatrix(unit.inverse(),i,i);
    }
}

void BundleAdjustment::setInverseN22(Matrix n22)
{
    int rows=n22.getRows();
    inverseN22=n22;
    for (int i=1;i<rows;i+=3)
    {
        Matrix unit=n22.sel(i,i+2,i,i+2);
        inverseN22.putMatrix(unit.inverse(),i,i);
    }
}

void BundleAdjustment::setInverseM11(Matrix m11)
{
    int rows=m11.getRows();
    inverseM11=m11;
    for (int i=1;i<rows;i+=3)
    {
        Matrix unit=m11.sel(i,i+2,i,i+2);
        inverseM11.putMatrix(unit.inverse(),i,i);
    }
}


void BundleAdjustment::setRot(double omega, double phi, double kappa)
{
    r11=cos(phi) * cos(kappa);
    r12=-cos(phi)*sin(kappa);
    r13=sin(phi);
    r21=cos(omega) * sin(kappa) + sin(omega) * sin(phi) * cos(kappa);
    r22=cos(omega) * cos(kappa) - sin(omega) * sin(phi) * sin(kappa);
    r23=-sin(omega) * cos(phi);
    r31=sin(omega) * sin(kappa) - cos(omega) * sin(phi) * cos(kappa);
    r32=sin(omega) * cos(kappa) + cos(omega) * sin(phi) * sin(kappa);
    r33=cos(omega) * cos(phi);
}

Matrix BundleAdjustment::getCoordinatesEqColin(double X, double Y, double Z, int imageId)
{
    Matrix coord(1,2);
    //double omega=matAdjust.get(imageId,1);
    //double phi=matAdjust.get(imageId,2);
    //double kappa=matAdjust.get(imageId,3);
    //double X0=matAdjust.get(imageId,4);
    //double Y0=matAdjust.get(imageId,5);
    //double Z0=matAdjust.get(imageId,6);

    double X0=getXAdjus(imageId);
    double Y0=getYAdjus(imageId);
    double Z0=getZAdjus(imageId);

    setRot(getOmegaAdjus(imageId),getPhiAdjus(imageId),getKappaAdjus(imageId));

    //qDebug("getCoordieqColLin\tomega %f phi %f kappa %f",getOmegaAdjus(imageId),getPhiAdjus(imageId),getKappaAdjus(imageId));
    double xi=xsi0-c*(r11*(X-X0)+r21*(Y-Y0)+r31*(Z-Z0))/(r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));
    double eta=eta0-c*(r12*(X-X0)+r22*(Y-Y0)+r32*(Z-Z0))/(r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));

    coord.set(1,1,xi);
    coord.set(1,2,eta);

    return coord;
}

Matrix BundleAdjustment::getJacobianaControl(double X, double Y, double Z, int imageIndex)
{
    Matrix JacCtrl(2,6);

    double omega=matAdjust.get(imageIndex,1);
    double phi=matAdjust.get(imageIndex,2);
    double kappa=matAdjust.get(imageIndex,3);
    double X0=matAdjust.get(imageIndex,4);
    double Y0=matAdjust.get(imageIndex,5);
    double Z0=matAdjust.get(imageIndex,6);

    //qDebug("omega %f phi %f kappa %f X0 %f Y0 %f Z0 %f",omega,phi,kappa,X0,Y0, Z0);


    JacCtrl.set(1,1,c*cos(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*sin(phi));
    JacCtrl.set(1,2,-c*(-sin(kappa)*cos(omega)-cos(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*sin(omega));
    JacCtrl.set(1,3,-c*(-sin(kappa)*sin(omega)+cos(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*cos(omega));
    JacCtrl.set(1,4,-c*((-sin(kappa)*sin(omega)+cos(kappa)*sin(phi)*cos(omega))*(Y-Y0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(-cos(phi)*cos(omega)*(Y-Y0)-cos(phi)*sin(omega)*(Z-Z0)));
    JacCtrl.set(1,5,-c*(-cos(kappa)*sin(phi)*(X-X0)+cos(kappa)*cos(phi)*sin(omega)*(Y-Y0)-cos(kappa)*cos(phi)*cos(omega)*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(cos(phi)*(X-X0)+sin(phi)*sin(omega)*(Y-Y0)-sin(phi)*cos(omega)*(Z-Z0)));
    JacCtrl.set(1,6,-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)));

    JacCtrl.set(2,1,-c*sin(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*sin(phi));
    JacCtrl.set(2,2,-c*(-cos(kappa)*cos(omega)+sin(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*sin(omega));
    JacCtrl.set(2,3,-c*(-cos(kappa)*sin(omega)-sin(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*cos(omega));
    JacCtrl.set(2,4,-c*((-cos(kappa)*sin(omega)-sin(kappa)*sin(phi)*cos(omega))*(Y-Y0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(-cos(phi)*cos(omega)*(Y-Y0)-cos(phi)*sin(omega)*(Z-Z0)));
    JacCtrl.set(2,5,-c*(sin(kappa)*sin(phi)*(X-X0)-sin(kappa)*cos(phi)*sin(omega)*(Y-Y0)+sin(kappa)*cos(phi)*cos(omega)*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(cos(phi)*(X-X0)+sin(phi)*sin(omega)*(Y-Y0)-sin(phi)*cos(omega)*(Z-Z0)));
    JacCtrl.set(2,6,-c*(-cos(kappa)*cos(phi)*(X-X0)+(-sin(kappa)*cos(omega)-cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(-sin(kappa)*sin(omega)+cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)));

    return JacCtrl;
}

Matrix BundleAdjustment::getJacobianaFotogrametric(double X, double Y, double Z, int imageIndex)
{
    Matrix JacFoto(2,3);

    double omega=matAdjust.get(imageIndex,1);
    double phi=matAdjust.get(imageIndex,2);
    double kappa=matAdjust.get(imageIndex,3);
    double X0=matAdjust.get(imageIndex,4);
    double Y0=matAdjust.get(imageIndex,5);
    double Z0=matAdjust.get(imageIndex,6);


    JacFoto.set(1,1,-c*cos(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*sin(phi));
    JacFoto.set(1,2,-c*(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*sin(omega));
    JacFoto.set(1,3,-c*(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*cos(omega));

    JacFoto.set(2,1,c*sin(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*sin(phi));
    JacFoto.set(2,2,-c*(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*sin(omega));
    JacFoto.set(2,3,-c*(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*cos(omega));

    return JacFoto;
}

//retorna os parametros da imagem numero imageID
Matrix BundleAdjustment::getPTA(Matrix PAf, int imageId)
{
    Matrix result(6,1);
    for (int i=6*imageId+1,j=1 ; i<=6*(imageId+1) ; i++,j++)
        result.set(j,1,PAf.get(i,1));
    return result;
}

/* metodos auxialiares para buscar dados*/
double BundleAdjustment::getdOmegax1(int imageId)
{
    return x1.get(6*(imageId-1)+4,1);
}

double BundleAdjustment::getdPhix1(int imageId)
{
    return x1.get(6*(imageId-1)+5,1);
}

double BundleAdjustment::getdKappax1(int imageId)
{
    return x1.get(6*(imageId-1)+6,1);
}

double BundleAdjustment::getdXx1(int imageId)
{
    return x1.get(6*(imageId-1)+1,1);
}

double BundleAdjustment::getdYx1(int imageId)
{
    return x1.get(6*(imageId-1)+2,1);
}

double BundleAdjustment::getdZx1(int imageId)
{
    return x1.get(6*(imageId-1)+3,1);
}

double BundleAdjustment::getdXx2(int fotogrPointId)
{
    return x2.get(3*(fotogrPointId-1)+1,1);
}

double BundleAdjustment::getdYx2(int fotogrPointId)
{
    return x2.get(3*(fotogrPointId-1)+2,1);
}

double BundleAdjustment::getdZx2(int fotogrPointId)
{
    return x2.get(3*(fotogrPointId-1)+3,1);
}

double BundleAdjustment::getOmegaAdjus(int imageId)
{
    return matAdjust.get(imageId,1);
}

double BundleAdjustment::getPhiAdjus(int imageId)
{
    return matAdjust.get(imageId,2);
}

double BundleAdjustment::getKappaAdjus(int imageId)
{
    return matAdjust.get(imageId,3);
}

double BundleAdjustment::getXAdjus(int imageId)
{
    return matAdjust.get(imageId,4);
}

double BundleAdjustment::getYAdjus(int imageId)
{
    return matAdjust.get(imageId,5);
}

double BundleAdjustment::getZAdjus(int imageId)
{
    return matAdjust.get(imageId,6);
}

// Procura onde esta o ponto na lista de todos os pontos(ou seja na lista de pontos selecionados) e retorna o seu indice na lista
int BundleAdjustment::whereInPoints(Point *pnt)
{
    for (int i=0;i<numPoints;i++)
        if (pnt==listPoints.at(i))
            return i;
    //qDebug("Nao achou o ponto %d na lista de pontos selecionados ",pnt->getId());
    return -1;//se nao achar retorna -1
}

int BundleAdjustment::whereInPhotogrammetricPoints(Point *pnt)
{
    for (int i=0;i<numFotogrametricPoints;i++)
        if (pnt==listPhotogrammetricPoints.at(i))
            return i;
    //qDebug("Nao achou o ponto %d na lista de pontos selecionados ",pnt->getId());
    return -1;//se nao achar retorna -1
}

void BundleAdjustment::updateMatAdjust()
{
    //qDebug("num images: %d",numImages);
    for (int i=1;i<=numImages;i++)
    {
        //qDebug("X+dx=%.3f+%.3f\nY+dy:%.3f+%.3f\nZ+dz=%.3f+%.3f\n",getXAdjus(i),getdXx1(i),getYAdjus(i),getdYx1(i),getZAdjus(i),getdZx1(i));
        matAdjust.set(i,1, getOmegaAdjus(i) + getdOmegax1(i) );//ajustando omega
        matAdjust.set(i,2, getPhiAdjus(i)   + getdPhix1(i)   );//ajustando phi
        matAdjust.set(i,3, getKappaAdjus(i) + getdKappax1(i) );//ajustando kappa
        matAdjust.set(i,4, getXAdjus(i)     + getdXx1(i)     );//ajustando X
        matAdjust.set(i,5, getYAdjus(i)     + getdYx1(i)     );//ajustando Y
        matAdjust.set(i,6, getZAdjus(i)     + getdZx1(i)     );//ajustando Z
    }
}


Matrix BundleAdjustment::getCoordColinearTerrain(double xsi, double eta, double z, int imageId)
{
    Matrix result(1,2);
    //imprime(matAdjust,"matAdjust");
    setRot(getOmegaAdjus(imageId),getPhiAdjus(imageId),getKappaAdjus(imageId));

    //qDebug("omega %f phi %f kappa %f",getOmegaAdjus(imageId),getPhiAdjus(imageId),getKappaAdjus(imageId));

    double x=getXAdjus(imageId)+(z-getZAdjus(imageId))*(r11*(xsi-xsi0)+r12*(eta-eta0)-r13*c)/(r31*(xsi-xsi0)+r32*(eta-eta0)-r33*c);
    double y=getYAdjus(imageId)+(z-getZAdjus(imageId))*(r21*(xsi-xsi0)+r22*(eta-eta0)-r23*c)/(r31*(xsi-xsi0)+r32*(eta-eta0)-r33*c);
    //qDebug("\nX: %f\tY: %f",x,y);

    result.set(1,1,x);
    result.set(1,2,y);
    return result;
}

/** omega, phi, kappa, X0, Y0, Z0)*/
void BundleAdjustment::calculateInicialsValues()
{
    if (numFotogrametricPoints!=0)
    {
        QTime init;
        init.start();
        Matrix L=createL();
        //int ltime=init.restart();
        //L.show('f',3,"L");

        Matrix M1=createM1();
        //int M1time=init.restart();
        //M1.show('f',3,"M1");

        Matrix M2=createM2();
        //int M2time=init.restart();
        //M2.show('f',3,"M2");

        Matrix m11=getM11(M1);
        //int m11time=init.restart();
        //m11.show('f',3,"M11");

        setInverseM11(m11);
        //int inverseM11time=init.restart();

        Matrix m12=getM12(M1,M2);
        //m12.show('f',3,"M12");
        //int m12time=init.restart();

        Matrix m22=getM22(M2);
        //m22.show('f',3,"M22");
        //int m22time=init.restart();

        Matrix m1=getm1(M1,L);
        //m1.show('f',3,"m1");
        //int m1time=init.restart();

        Matrix m2=getm2(M2,L);
        //m2.show('f',3,"m2");
        //int m2time=init.restart();

        Matrix xypf=getXYpf(m12,m22,m1,m2);
        //int xypftime=init.restart();

        Matrix paf=getPAf(m12,m1,xypf);
        //int paftime=init.restart();


#ifdef TIMES
        qDebug("Tempo para executar:");
        qDebug("Para criar L: %.3f",ltime/1000.0);
        qDebug("Para criar M1: %.3f",M1time/1000.0);
        qDebug("Para criar M2: %.3f",M2time/1000.0);
        qDebug("Para calcular M11: %.3f",m11time/1000.0);
        qDebug("Para calcular inverseM11: %.3f",inverseM11time/1000.0);
        qDebug("Para calcular M12: %.3f",m12time/1000.0);
        qDebug("Para calcular M22: %.3f",m22time/1000.0);
        qDebug("Para calcular m1: %.3f",m1time/1000.0);
        qDebug("Para calcular m2: %.3f",m2time/1000.0);
        qDebug("Para calcular paf: %.3f",paftime/1000.0);
        qDebug("Para calcular xypf: %.3f",xypftime/1000.0);
#endif
        //paf.show('f',4,"parameters OE");
        //xypf.show('f',4,"xypointFoto");
        updateCoordinatesAllPoints(xypf,getInicialZPhotogrammetricPoints());

        Matrix temp(numImages,6);
        matInitialValues=temp;
        for (int i=0;i<numImages;i++)
        {
            xsi0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
            eta0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getEta();
            c=listImages.at(i)->getSensor()->getFocalDistance();

            Matrix pta=getPTA(paf,i);

            matInitialValues.set(i+1,1,0);
            matInitialValues.set(i+1,2,0);
            //qDebug("Kappa0");
            matInitialValues.set(i+1,3,listImages.at(i)->getFlightDirection());
            matInitialValues.set(i+1,4,pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
            matInitialValues.set(i+1,5,pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
            //matInicialValues.set(i+1,6,c*getMediaScale(i));
            //qDebug("Z0 %i: %.4f",i,listImages.at(i)->getFlight()->getScaleDen()/1000);
            matInitialValues.set(i+1,6,c*listImages.at(i)->getFlight()->getScaleDen()/1000);
            //Image *img;
            //img->getFlight()->getScale()Scale();
        }
    }
    else
    {
        //qDebug("initial Values: Sem pontos Fotogrametricos");

        Matrix L=createL();
        Matrix M1=createM1();
        Matrix m11=getM11(M1);
        setInverseM11(m11);
        Matrix m1=getm1(M1,L);
        Matrix paf=inverseM11*m1;

        Matrix temp(numImages,6);
        matInitialValues=temp;
        for (int i=0;i<numImages;i++)
        {
            xsi0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
            eta0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getEta();
            c=listImages.at(i)->getSensor()->getFocalDistance();
            Matrix pta=getPTA(paf,i);
            matInitialValues.set(i+1,1,0);
            matInitialValues.set(i+1,2,0);
            //matInicialValues.set(i+1,3,getKappaZero(pta,i));
            matInitialValues.set(i+1,3,listImages.at(i)->getFlightDirection());
            matInitialValues.set(i+1,4,pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
            matInitialValues.set(i+1,5,pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
            //matInicialValues.set(i+1,6,c*getMediaScale(i));
            //qDebug("Z0 %i: %.4f",i,listImages.at(i)->getFlight()->getScaleDen()/1000);
            matInitialValues.set(i+1,6,c*listImages.at(i)->getFlight()->getScaleDen()/1000);
        }
    }
}

Matrix BundleAdjustment::createL()
{
    Matrix L(0,1);
    for (int i=0;i<numImages;i++)
    {
        int numpnts=listImages.at(i)->countPoints();
        for(int j=0;j<numpnts;j++)
        {
            if (whereInPoints(getPointFrom(i,j))!=-1)
            {
                Matrix temp(2,1);
                if(isControlPoint(i,j))
                {
                    temp.set(1,1,listImages.at(i)->getPointAt(j)->getObjectCoordinate().getX());
                    temp.set(2,1,listImages.at(i)->getPointAt(j)->getObjectCoordinate().getY());
                }
                L=L|temp;
            }
        }
    }
    //imprime(L,"Lobject");
    return L;
}

Matrix BundleAdjustment::createM1()
{
    Matrix result(0,0);
    for (int i=0;i<numImages;i++)
    {
        Matrix temp=imageToMatrixDetectorCoordenates(listImages.at(i));
        result.putMatrix(temp,result.getRows()+1,6*i+1);
    }
    //imprime(result,"M1object");
    return result;
}

Matrix BundleAdjustment::createM2()
{
    //Matrix result(0,3*numFotogrametricPoints);
    Matrix result(0,2*numFotogrametricPoints);
    Matrix B(2,2);
    B.set(1,1,-1);
    B.set(2,2,-1);
    int posCol=0;
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        //int rows=numberControlPoints(listImages.at(i))+numberPhotogrammetricPoints(listImages.at(i));
        // int posLin=1;
        //qDebug("rows %d:%d",i,rows);
        Matrix oneImage(0,2*numFotogrametricPoints);
        //	qDebug("Imagem %s",listImages.at(i)->getFilename().c_str());
        for (int j=0;j<pnts;j++)
        {
            if (whereInPoints(getPointFrom(i,j))!=-1)
            {
                posCol=whereInPhotogrammetricPoints(getPointFrom(i,j));
                Matrix auxPoint(2,2*numFotogrametricPoints);
                if(posCol!=-1)// && isPhotogrammetricPoint(i,j))
                {
                    posCol=2*posCol+1;

                    auxPoint.putMatrix(B,1,posCol);
                    //auxPoint.show('f',3,"AuxPontos");
                    //	qDebug("\tPoint %s na pos [%d,%d]",getPointFrom(i,j)->getPointId().c_str(),posLin,posCol);
                    //jf.show();
                }
                oneImage=oneImage|auxPoint;
                //if (whereInPoints(getPointFrom(i,j))!=-1)
                //posLin+=2;
            }
        }
        result=result|oneImage;
        //result=oneImage;
    }
    return result;
}

// Retorna uma matriz correspondente a uma imagem na matriz M
Matrix BundleAdjustment::imageToMatrixDetectorCoordenates(Image *img)
{
    int numpnts=img->countPoints();

    Matrix result(0,6);
    for (int i=0;i<numpnts;i++)
    {
        // configuraÃÂ§ao de cada matriz temp
        //                   |1	eta xi	0	 0		0 |
        //                   |0	 0	0	1	eta 	xi|

        if (whereInPoints(img->getPointAt(i))!=-1 && img->getPointAt(i)->getType() != Point::CHECKING)
        {
            Matrix temp(2,6);
            // conversao digital to analog sera feito pela classe do Rafael em breve
            double lin=img->getPointAt(i)->getImageCoordinate(img->getId()).getLin();
            double col=img->getPointAt(i)->getImageCoordinate(img->getId()).getCol();

            //qDebug("object col: %d\t lin: %d",col, lin);
            //Matrix coord = digitalToAnalog(img->getIO(),lin,col);

            RayTester ray(img);
            ray.setIOParameters(img->getIO()->getXa());
            DetectorSpaceCoordinate coordinates = ray.imageToDetector(col,lin);

            temp.set(1,1,1);
            temp.set(1,2,coordinates.getEta());
            temp.set(1,3,coordinates.getXi());
            temp.set(2,4,1);
            temp.set(2,5,coordinates.getEta());
            temp.set(2,6,coordinates.getXi());

            result=result|temp;
        }
    }
    // imprime(result,"image");
    return result;
}

Matrix BundleAdjustment::digitalToAnalog(InteriorOrientation *oi, double linha, double coluna)
{
    double a0, a1, a2, b0, b1, b2;

    a0 = oi->getXa().get(1,1);
    a1 = oi->getXa().get(2,1);
    a2 = oi->getXa().get(3,1);
    b0 = oi->getXa().get(4,1);
    b1 = oi->getXa().get(5,1);
    b2 = oi->getXa().get(6,1);

    //imprime(oi->getXa(),"Xa");

    Matrix result(1,2);
    result.set(1,1, a0 + a1*coluna + a2*linha);
    result.set(1,2, b0 + b1*coluna + b2*linha);
    return result;
}

Matrix BundleAdjustment::analogToDigital(InteriorOrientation *oi,double xsi, double eta)
{
    double a0, a1, a2, b0, b1, b2;
    a0 = oi->getXa().get(1,1);
    a1 = oi->getXa().get(2,1);
    a2 = oi->getXa().get(3,1);
    b0 = oi->getXa().get(4,1);
    b1 = oi->getXa().get(5,1);
    b2 = oi->getXa().get(6,1);
    //Ideia do Rafael Aguiar para contornar erro de cast (somar 0.1)
    Matrix result(1,2);
    result.set(1,1, ((b2*xsi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2) + 0.1 ) );
    result.set(1,2, ((a1*eta - a1*b0 - b1*xsi + b1*a0) / (a1*b2 - b1*a2) + 0.1 ) );
    return result;
}

int BundleAdjustment::numberControlPoints(Image *img)
{
    int cont=0;
    int points=img->countPoints();
    for (int i=0;i<points;i++)
        if(img->getPointAt(i)->getType() == Point::CONTROL && whereInPoints(img->getPointAt(i))!=-1)
            cont++;
    return cont;
}

int BundleAdjustment::numberPhotogrammetricPoints(Image *img)
{
    int cont=0;
    int points=img->countPoints();
    for (int i=0;i<points;i++)
        if(img->getPointAt(i)->getType() == Point::PHOTOGRAMMETRIC && whereInPoints(img->getPointAt(i))!=-1)
            cont++;
    return cont;
}

double BundleAdjustment::getInicialZPhotogrammetricPoints()
{
    double z=0;
    int n=0;
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        for(int j=0;j<pnts;j++)
            if(isControlPoint(i,j) && whereInPoints(getPointFrom(i,j))!=-1)
            {
                z+=getPointFrom(i,j)->getObjectCoordinate().getZ();
                n++;
            }
    }
    return z/=n;
}
// atualiza as coordenadas de todos os pontos
void BundleAdjustment::updateCoordinatesAllPoints(Matrix xypf, double zphotogrammetric)
{
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        int n=1;
        for(int j=0;j<pnts;j++)
            if(isPhotogrammetricPoint(i,j) && whereInPoints(getPointFrom(i,j))!=-1)
            {
                ObjectSpaceCoordinate coord = getPointFrom(i,j)->getObjectCoordinate();
                coord.setX(xypf.get(n++,1));
                coord.setY(xypf.get(n++,1));
                coord.setZ(zphotogrammetric);
                getPointFrom(i,j)->setObjectCoordinate(coord);
            }
    }
}
void BundleAdjustment::zeroingCoordinatesPhotogrammetrics()
{
    for (unsigned int i=0;i<listPhotogrammetricPoints.size();i++)
    {
        ObjectSpaceCoordinate coord = listPhotogrammetricPoints.at(i)->getObjectCoordinate();
        coord.setX(0);
        coord.setY(0);
        coord.setZ(0);
        listPhotogrammetricPoints.at(i)->setObjectCoordinate(coord);
    }
}

// Retorna uma matriz correspondente a uma imagem na Matriz A
Matrix BundleAdjustment::imageToMatrixJacobiana(int indexImage)
{
    Image *img=listImages.at(indexImage);
    int pnts=img->countPoints();
    Matrix result(0,6);
    for (int i=0;i<pnts;i++)
    {
        if(whereInPoints(img->getPointAt(i))!=-1 && !isCheckingPoint(indexImage,i))
        {
            ObjectSpaceCoordinate aux=img->getPointAt(i)->getObjectCoordinate();
            double x=aux.getX();
            double y=aux.getY();
            double z=aux.getZ();
            result=result|getJacobianaControl(x,y,z,indexImage+1);
        }
    }
    return result;
}

/* Retorna toda a matriz A1*/
void BundleAdjustment::createA1()
{
    Matrix result;
    int currentRows=0;
    for (int i=0;i<numImages;i++)
    {
        Matrix partial=imageToMatrixJacobiana(i);
        result.putMatrix(partial,currentRows+1,6*i+1);
        currentRows+=partial.getRows();
    }
    A1=result;
}

void BundleAdjustment::createA2()
{
    Matrix result(0,3*numFotogrametricPoints);
    int posCol=0;
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        //int rows=numberControlPoints(listImages.at(i))+numberPhotogrammetricPoints(listImages.at(i));
        //int posLin=1;
        //qDebug("rows %d:%d",i,rows);
        Matrix oneImage(0,3*numFotogrametricPoints);
        //	qDebug("Imagem %s",listImages.at(i)->getFilename().c_str());
        for (int j=0;j<pnts;j++)
        {
            if (whereInPoints(getPointFrom(i,j))!=-1)
            {
                posCol=whereInPhotogrammetricPoints(getPointFrom(i,j));
                Matrix auxPoint(2,3*numFotogrametricPoints);
                if(posCol!=-1)// && isPhotogrammetricPoint(i,j))
                {
                    ObjectSpaceCoordinate aux=getPointFrom(i,j)->getObjectCoordinate();
                    double x=aux.getX();
                    double y=aux.getY();
                    double z=aux.getZ();

                    //qDebug("\tPoint %s X:%.3f\tY:%.3f\tZ:%.3f",getPointFrom(i,j)->getPointId().c_str(),x,y,z);
                    Matrix jf=getJacobianaFotogrametric(x,y,z,i+1);
                    posCol=3*posCol+1;

                    auxPoint.putMatrix(jf,1,posCol);
                    //auxPoint.show('f',3,"AuxPontos");
                    //	qDebug("\tPoint %s na pos [%d,%d]",getPointFrom(i,j)->getPointId().c_str(),posLin,posCol);
                    //jf.show();
                }
                oneImage=oneImage|auxPoint;
                //if (whereInPoints(getPointFrom(i,j))!=-1)
                //posLin+=2;
            }
        }
        result=result|oneImage;
        //result=oneImage;
    }
    A2=result;//result;*/
}

void BundleAdjustment::createL0()
{
    Matrix result(0,1);
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        for (int j=0;j<pnts;j++)
        {
            if (whereInPoints(getPointFrom(i,j))!=-1 && !isCheckingPoint(i,j))
            {
                ObjectSpaceCoordinate aux=getPointFrom(i,j)->getObjectCoordinate();
                double x=aux.getX();
                double y=aux.getY();
                double z=aux.getZ();
                /** Pegar esse metodo na classe do Rafael, as coordenadas pela equaÃÂ§ao de colinearidade*/
                Matrix coord=getCoordinatesEqColin(x,y,z,i+1);
                result=result|(coord.transpose());
            }
        }
    }
    L0=result;
}

void BundleAdjustment::createLb()
{
    Matrix result(0,1);
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        for (int j=0;j<pnts;j++)
        {
            if (whereInPoints(getPointFrom(i,j))!=-1 && !isCheckingPoint(i,j))
            {
                DetectorSpaceCoordinate aux=getPointFrom(i,j)->getDetectorCoordinate(listImages.at(i)->getId());
                double xi=aux.getXi();
                double eta=aux.getEta();
                Matrix temp(2,1);
                temp.set(1,1,xi);
                temp.set(2,1,eta);
                result=result|temp;
            }
        }
    }
    Lb=result;
}

void BundleAdjustment::calculateResiduos()
{
    Matrix result(0,1);
    for (int i=0;i<numImages;i++)
    {
        int pnts=listImages.at(i)->countPoints();
        for (int j=0;j<pnts;j++)
        {
            if (isControlPoint(i,j) && whereInPoints(getPointFrom(i,j))!=-1)
            {
                double x=getPointFrom(i,j)->getObjectCoordinate().getX();
                double y=getPointFrom(i,j)->getObjectCoordinate().getY();
                double z=getPointFrom(i,j)->getObjectCoordinate().getZ();
                double xi=getPointFrom(i,j)->getDetectorCoordinate(listImages.at(i)->getId()).getXi();
                double eta=getPointFrom(i,j)->getDetectorCoordinate(listImages.at(i)->getId()).getEta();
                //qDebug("%f %f %f \t\t xi: %f eta: %f",x,y,z,xi,eta);

                /** Usar classe do Rafael para isso */
                Matrix coord=getCoordColinearTerrain(xi,eta,z,i+1);
                double resx=x-coord.get(1,1);
                double resy=y-coord.get(1,2);
                // reaproveitando a matrix que nÃÂ£o serÃÂ¡ mais utilizada nesse loop;
                //qDebug("Residuo x de %s[%d,%d]\nResx= %.9f\tResy=%.9f\n",getPointFrom(i,j)->getPointId().c_str(),i,j,resx,resy);
                coord=coord.transpose();
                coord.set(1,1,resx);
                coord.set(2,1,resy);
                result=result|coord;
            }
        }
    }
    matRes=result;
}

void BundleAdjustment::calculatePeso()
{
    int d=1;
    for (int i=0;i<numImages;i++)
    {
        int numPoints=listImages.at(i)->countPoints();
        for (int j=0;j<numPoints;j++)
        {
            if (isControlPoint(i,j) && whereInPoints(getPointFrom(i,j))!=-1)
            {
                double p=(numEquations-numUnknows)/(pow(getRx(listImages.at(i),j),2));
                P.set(d,d,p);
                d++;
                p=(numEquations-numUnknows)/((pow(getRy(listImages.at(i),j),2)));
                P.set(d,d,p);
                d++;
            }
            if (isPhotogrammetricPoint(i,j) && whereInPoints(getPointFrom(i,j))!=-1)
            {
                P.set(d,d,1);
                d++;
                P.set(d,d,1);
                d++;
            }
        }
    }
}
void BundleAdjustment::updateCoordFotog()
{
    int n=1;
    for (int i=0;i<numPoints;i++)
    {
        Point *pnt=listPoints.at(i);
        if(pnt->getType() == Point::PHOTOGRAMMETRIC  && whereInPoints(pnt)!=-1)
        {
            ObjectSpaceCoordinate coord = pnt->getObjectCoordinate();
            double x=coord.getX();
            double y=coord.getY();
            double z=coord.getZ();
            //printf("%f+%f\n",x, getdXx2(n));

            coord.setX(x+getdXx2(n));
            coord.setY(y+getdYx2(n));
            coord.setZ(z+getdZx2(n));

            pnt->setObjectCoordinate(coord);
            n++;
        }
    }
}

double BundleAdjustment::getRx(Image *img, int pointIndex)
{
    int base=1;
    int pos=whereInImages(img);
    for(int i=0;i<pos;i++)
        base+=2*numberControlPoints(listImages.at(i));
    return matRes.get(base+2*(pointIndex),1);
}

double BundleAdjustment::getRy(Image *img, int pointIndex)
{
    int base=1;
    int pos=whereInImages(img);
    for(int i=0;i<pos;i++)
        base+=2*numberControlPoints(listImages.at(i));
    return matRes.get(base+2*(pointIndex)+1,1);
}

int BundleAdjustment::whereInImages(Image *img)
{
    for (int i=0;i<numImages;i++)
    {
        if(listImages.at(i)==img)
            return i;
    }
    return -1;
}

/*Metodos para aumento de performance do calculo*/
/*
Matrix BundleAdjustment::getSparseN11()
{
        Matrix result;
        int currentRows=0;
        for (int i=0;i<numImages;i++)
        {
                Matrix partial=imageToMatrixJacobiana(i);
                //partial.transpose()*partial;
                result.putMatrix(partial,currentRows+1,6*i+1);
                currentRows+=partial.getRows();
        }
        A1=result;
        return A1;
}
*/
/* setters and getters*/
Matrix BundleAdjustment::getMatRes()
{
    //matRes.show('f',8,"MatRes");
    return matRes;
}

std::deque<double> BundleAdjustment::getListRMSE()
{
    return listRMSE;
}

bool BundleAdjustment::getConvergencyStatus()
{
    return converged;
}

int BundleAdjustment::getTotalIterations()
{
    return totalIterations;
}

void BundleAdjustment::setAFP()
{
    //Isso ÃÂ© feito apenas para:
    //ExibiÃÂ§ao dos angulos em Graus.
    /*
        Matrix result(matAdjust.getRows(),matAdjust.getCols());
        for (int i=1;i<=numImages;i++)
        {
                result.set(i,1,Dms::radianoToDegreeDecimal(matAdjust.get(i,1)));
                result.set(i,2,Dms::radianoToDegreeDecimal(matAdjust.get(i,2)));
                result.set(i,3,Dms::radianoToDegreeDecimal(matAdjust.get(i,3)));
                result.set(i,4,matAdjust.get(i,4));
                result.set(i,5,matAdjust.get(i,5));
                result.set(i,6,matAdjust.get(i,6));
        }
        afp=result;
        */
    afp=matAdjust;
}

Matrix BundleAdjustment::getAFP()
{
    return afp;
}

/* metodos auxiliares para teste*/
bool BundleAdjustment::isControlPoint(int imageIndex ,int pointIndex)
{
    return listImages.at(imageIndex)->getPointAt(pointIndex)->getType() == Point::CONTROL;
}

bool BundleAdjustment::isPhotogrammetricPoint(int imageIndex, int pointIndex)
{
    return listImages.at(imageIndex)->getPointAt(pointIndex)->getType() == Point::PHOTOGRAMMETRIC;
}

bool BundleAdjustment::isCheckingPoint(int imageIndex, int pointIndex)
{
    return listImages.at(imageIndex)->getPointAt(pointIndex)->getType() == Point::CHECKING;
}

bool BundleAdjustment::testResiduo()
{
    int rowsMatRes=matRes.getRows();
    for (int i=1;i<=rowsMatRes;i++)
    {
        if (fabs(double(matRes.get(i,1)>MAXRESIDUO)))
        {
            return false;
        }
    }
    return true;
}

// Se der infinito retorna -2, se der NAN retorna -1, se nao convergir retorna 0 e se convergir retorna 1
int BundleAdjustment::testConverged()
{
    /*
        int rowsX1=x1.getRows();
        int rowsX2=x2.getRows();
   // printf("testando X1");
        for (int i=1;i<=rowsX1;i++)
        {
                if (isinf(fabs(x1.get(i,1))))
                {
                        qDebug("numero e INFINITO");
                        return -2;
                }
                if (isnan(fabs(x1.get(i,1))))
                {
                        qDebug("numero e NAN");
                        return -1;
                }
                if (fabs(x1.get(i,1)>convergency))
                {
                        return 0;
                }
        }
        //printf("testando X2");
        for (int i=1;i<=rowsX2;i++)
        {
                if (isinf(fabs(x1.get(i,1))))
                {
                        qDebug("numero e INFINITO");
                        return -2;
                }
                if (isnan(fabs(x2.get(i,1))))
                {
                        qDebug("numero e NAN");
                        return -1;
                }
                if (fabs(x2.get(i,1)>convergency))
                {
                        return 0;
                }
        }
        return 1;
        */
    int rowsX1=x1.getRows();
    int rowsX2=x2.getRows();
    // printf("testando X1");
    for (int i=0;i<rowsX1;i++)
    {
        if (isinf(fabs(x1.get(i+1,1))))
        {
            qDebug("numero e INFINITO");
            return -2;
        }
        if (isnan(fabs(x1.get(i+1,1))))
        {
            qDebug("numero e NAN");
            return -1;
        }
        if (i%6<3)//Os trÃªs primeiros elementos sÃ£o Xo,Yo,Zo
        {
            if (fabs(double(x1.get(i+1,1)>metricConvergency)))
                return 0;
        }
        else // Os trÃªs ultimos elementos sÃ£o omega, phi, kappa
        {
            //printf("x1(%d)= %.5f\n",i+1,fabs(x1.get(i+1,1)));
            if (fabs(double(x1.get(i+1,1)>angularConvergency)))
            {
                //printf("angulo nao convergido x1(%d,1)= %.5f \tangular convergency: %.5f\n",i+1,fabs(x1.get(i+1,1)),angularConvergency);
                return 0;
            }
        }
    }
    //printf("testando X2");
    for (int i=1;i<=rowsX2;i++)
    {
        if (isinf(fabs(x2.get(i,1))))
        {
            qDebug("numero e INFINITO");
            return -2;
        }
        if (isnan(fabs(x2.get(i,1))))
        {
            qDebug("numero e NAN");
            return -1;
        }
        if (fabs(double(x2.get(i,1)>metricConvergency)))
        {
            return 0;
        }
    }
    return 1;
}

bool BundleAdjustment::isConverged()
{
    if(totalIterations<=maxIterations && testConverged()==1)
        return converged=true;
    else
        return converged=false;
}

void BundleAdjustment::setUserInitialValues(Matrix initialValues)
{
    matInitialValues=initialValues;
    userInitialValues= true;
}

std::string BundleAdjustment::printAll()
{
    std::string result="";
    int numImages=listImages.size();
    for (int i=0;i<numImages;i++)
    {
        Image *img=listImages.at(i);
        //printf("%s:\n",img->getFilename().c_str());
        result+=img->getFilename().c_str();
        result+="\n";
        int numPoints=img->countPoints();
        for (int j=0; j<numPoints;j++)
        {
            if (whereInPoints(getPointFrom(i,j))!=-1)
            {
                Point *pnt=img->getPointAt(j);
                double col=pnt->getImageCoordinate(img->getId()).getCol();
                double lin=pnt->getImageCoordinate(img->getId()).getLin();
                double X=pnt->getObjectCoordinate().getX();
                double Y=pnt->getObjectCoordinate().getY();
                double Z=pnt->getObjectCoordinate().getZ();
                result +="\t";
                result += pnt->getPointId().c_str();
                result += " col: ";
                result += Conversion::doubleToString(col);
                result += " lin: ";
                result += Conversion::doubleToString(lin);
                result += " X: ";
                result += Conversion::doubleToString(X);
                result += " Y: ";
                result += Conversion::doubleToString(Y);
                result += " Z: ";
                result += Conversion::doubleToString(Z);
                result += "\n";
            }
            //printf("\t%s: col: %d lin %d\n",pnt->getPointId().c_str(),col,lin);
        }
        result += "\n";
        //printf("\n");
    }
    return result;
}

Matrix BundleAdjustment::getMVC()
{
    //Isso ÃÂ© feito apenas para:
    //ExibiÃÂ§ao dos angulos em Graus.
    Matrix tempX1(numImages,6);
    for (int i=0;i<numImages;i++)
    {
        tempX1.set(i+1,1,Dms::radianoToDegreeDecimal(x1.get(6*i+4,1)));
        tempX1.set(i+1,2,Dms::radianoToDegreeDecimal(x1.get(6*i+5,1)));
        tempX1.set(i+1,3,Dms::radianoToDegreeDecimal(x1.get(6*i+6,1)));
        tempX1.set(i+1,4,x1.get(6*i+1,1));
        tempX1.set(i+1,5,x1.get(6*i+2,1));
        tempX1.set(i+1,6,x1.get(6*i+3,1));
    }
    return tempX1;
}

bool BundleAdjustment::isPossibleCalculate()
{
    if (numEquations<numUnknows)
        return false;
    else
        return true;
}

Matrix BundleAdjustment::getL0()
{
    return L0;
}

Matrix BundleAdjustment::getLb()
{
    return Lb;
}

void BundleAdjustment::setMetricConvergencyValue(double value)
{
    metricConvergency = value;
}

double BundleAdjustment::getMetricConvergencyValue()
{
    return metricConvergency;
}

void BundleAdjustment::setAngularConvergencyValue(double value)
{
    angularConvergency = value;
}

double BundleAdjustment::getAngularConvergencyValue()
{
    return angularConvergency;
}

void BundleAdjustment::setMaxNumberIterations(int value)
{
    maxIterations = value;
}

int BundleAdjustment::getMaxNumberIterations()
{
    return maxIterations;
}

Matrix BundleAdjustment::getMatrixInicialValues()
{
    return matInitialValues;
}

/** Metodos EIGEN */
/*
Matrix BundleAdjustment::convertEigenToMatrix(Eigen::MatrixXd eigen)
{
        int cols = eigen.cols();
        int rows = eigen.rows();
        Matrix mat(rows,cols);
        //qDebug("num cols: %f\tnumRows: %f",numCols,numRows);
        for(int i=0;i<rows;i++)
                for (int j=0;j<cols;j++)
                {
                        mat.set(i+1,j+1,eigen(i,j));
                }
        return mat;
}

Eigen::MatrixXd BundleAdjustment::convertMatrixToEigen(Matrix mat)
{
        int cols=mat.getCols();
        int rows=mat.getRows();
        Eigen::MatrixXd eig(rows,cols);
        for(int i=0;i<rows;i++)
                for (int j=0;j<cols;j++)
                {
                        eig(i,j)=mat.get(i+1,j+1);
                }
        return eig;
}

Matrix BundleAdjustment::convertVectorEigenToMatrix(Eigen::VectorXd eigen)
{
        //int cols = eigen.cols();
        int rows = eigen.rows();
        Matrix mat(rows,1);
        //qDebug("num cols: %f\tnumRows: %f",numCols,numRows);
        for(int i=0;i<rows;i++)
                mat.set(i+1,1,eigen(i,0));

        return mat;
}
*/

Matrix BundleAdjustment::getResiduo(Point *photogrammetricPoint)
{
    Matrix result(1,3);
    int posFoto=whereInPhotogrammetricPoints(photogrammetricPoint);

    if(posFoto>0)
    {
        result.set(1,1,getdXx2(posFoto));
        result.set(1,2,getdYx2(posFoto));
        result.set(1,3,getdZx2(posFoto));
    }

    return result;
}

std::deque<Point*> BundleAdjustment::getPhotogrammetricList()
{
    return listPhotogrammetricPoints;
}

bool BundleAdjustment::hasAllImagesInitialValues()
{
    for (unsigned int i=0;i<listImages.size();i++)
    {
        //qDebug("antes   initial value imagem %d",i);
        Image *img=listImages.at(i);
        if(!img->isInsAvailable())
            return false;
        //qDebug("INS AVAILABLE imagem %d",i);
        if(!img->isGnssAvailable())
            return false;
        //qDebug("GNSS AVAILABLE imagem %d",i);
        if(img->getInsType()!="Initial")
            return false;
        //qDebug("INS TYPE INITIAL imagem %d",i);
        if(img->getGnssType()!="Initial")
            return false;
        //qDebug("GNSS TYPE INITIAL imagem %d",i);
    }
    return true;
}

Matrix BundleAdjustment::convertToGeocentric(Matrix coordinates, GeoSystem sys, int hemi, int zona)
{
    int rows= coordinates.getRows();
    for (int i=1;i<=rows;i++)
    {
        double E = coordinates.get(i,4);
        double N = coordinates.get(i,5);
        double H = coordinates.get(i,6);
        Matrix temp=ConvertionsSystems::utmToGeocentrica(E,N,H,zona,hemi,sys);
        coordinates.set(i,4,temp.get(1,1));
        coordinates.set(i,5,temp.get(1,2));
        coordinates.set(i,6,temp.get(1,3));
    }
    return coordinates;
}

Matrix BundleAdjustment::convertToUTM(Matrix coordinates, GeoSystem sys)
{
    int rows= coordinates.getRows();
    for (int i=1;i<=rows;i++)
    {
        double X = coordinates.get(i,4);
        double Y = coordinates.get(i,5);
        double Z = coordinates.get(i,6);
        Matrix temp=ConvertionsSystems::GeocentricaToUtm(X,Y,Z,sys);
        coordinates.set(i,4,temp.get(1,1));
        coordinates.set(i,5,temp.get(1,2));
        coordinates.set(i,6,temp.get(1,3));
    }
    return coordinates;
}

void BundleAdjustment::convertToGeocentric(std::deque<Point*> points, GeoSystem sys, int hemi, int zona)
{
    int rows=points.size();
    for (int i=0;i<rows;i++)
    {
        Point *pnt=points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double E = temp.getX();
        double N = temp.getY();
        double H = temp.getZ();
        Matrix gc=ConvertionsSystems::utmToGeocentrica(E,N,H,zona,hemi,sys);
        temp.setX(gc.get(1,1));
        temp.setY(gc.get(1,2));
        temp.setZ(gc.get(1,3));
        pnt->setObjectCoordinate(temp);
    }
}

void BundleAdjustment::convertToUTM(std::deque<Point*> points, GeoSystem sys)
{
    int rows=points.size();
    for (int i=0;i<rows;i++)
    {
        Point *pnt=points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double E = temp.getX();
        double N = temp.getY();
        double H = temp.getZ();
        Matrix gc=ConvertionsSystems::GeocentricaToUtm(E,N,H,sys);
        temp.setX(gc.get(1,1));
        temp.setY(gc.get(1,2));
        temp.setZ(gc.get(1,3));
        pnt->setObjectCoordinate(temp);
    }
}

double BundleAdjustment::calculateRMSE()
{
    int numRes=matRes.getRows();
    double sum=0;
    for (int i=1;i<=numRes;i++)
        sum+=pow(matRes.get(i,1),2);
    return sqrt(sum/(numEquations-numUnknows));
}

// em teste

void BundleAdjustment::normalize(std::deque<Point *>points,double &range, double &offset)
{
    int rows=points.size();
    double minX, minY, minZ, maxX, maxY, maxZ,minT,maxT;

    Point *pnt=points.at(0);
    ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();

    minX=maxX=temp.getX();
    minY=maxY=temp.getY();
    minZ=maxZ=temp.getZ();

    for (int i=1;i<rows;i++)
    {
        Point *pnt=points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double X = temp.getX();
        double Y = temp.getY();
        double Z = temp.getZ();

        if(X<minX)
            minX=X;
        if(Y<minY)
            minY=Y;
        if(Z<minZ)
            minZ=Z;

        if(X>=maxX)
            maxX=X;
        if(Y>=maxY)
            maxY=Y;
        if(Z>=maxZ)
            maxZ=Z;
    }

    // Calculo do minimo e do maximo

    if(maxX>=maxY)// && maxY>maxZ)
        maxT=maxX;
    else
        maxT=maxY;

    if(maxT<maxZ)
        maxT=maxZ;

    if(minX<=minY)// && maxY>maxZ)
        minT=minX;
    else
        minT=minY;

    if(minT>minZ)
        minT=minZ;
    qDebug("maxT %.5f minT %.5f",maxT, minT);

    double myRange = maxT-minT;
    double myOffset = -(minT);

    for (int i=0;i<rows;i++)
    {
        Point *pnt=points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double X = temp.getX();
        double Y = temp.getY();
        double Z = temp.getZ();

        temp.setX((X+myOffset)/myRange);
        temp.setY((Y+myOffset)/myRange);
        temp.setZ((Z+myOffset)/myRange);
        pnt->setObjectCoordinate(temp);
    }

    for(unsigned int i=1; i <= matInitialValues.getRows();i++)
    {
        double X0 = matInitialValues.get(i,4);
        double Y0 = matInitialValues.get(i,5);
        double Z0 = matInitialValues.get(i,6);

        matInitialValues.set(i,4,(X0+myOffset)/myRange);
        matInitialValues.set(i,5,(Y0+myOffset)/myRange);
        matInitialValues.set(i,6,(Z0+myOffset)/myRange);
    }

    metricConvergency=(metricConvergency+myOffset)/myRange;

    range=myRange;
    offset=myOffset;

    //qDebug("range %.4f offset %.4f metricconvergency %.18f",range,offset,metricConvergency);

}

void BundleAdjustment::desnormalize(std::deque<Point*>points, double range, double offset)
{
    int rows=points.size();
    for (int i=0;i<rows;i++)
    {
        Point *pnt=points.at(i);
        ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
        double X = temp.getX();
        double Y = temp.getY();
        double Z = temp.getZ();
        temp.setX(X*range-offset);
        temp.setY(Y*range-offset);
        temp.setZ(Z*range-offset);
        pnt->setObjectCoordinate(temp);

    }

    for(unsigned int i=1;i<= matAdjust.getRows();i++)
    {
        double X0 = matAdjust.get(i,4);
        double Y0 = matAdjust.get(i,5);
        double Z0 = matAdjust.get(i,6);

        matAdjust.set(i,4,X0*range-offset);
        matAdjust.set(i,5,Y0*range-offset);
        matAdjust.set(i,6,Z0*range-offset);
    }

    metricConvergency=metricConvergency*range - offset;

}

/* Method into disuse:
 *
double BundleAdjustment::getKappaZero(Matrix pta,int imageId)
{
        //double kappa0=0.002;
        //mf[0]=MFX mf[1]=MFY

        //coordenadas das marcas fiduciais em milimetros
        double coluna=0.016;
        double linha=113.0;

        double mfX= pta.get(1,1) + pta.get(2,1)*coluna + pta.get(3,1)*linha;
        double mfY= pta.get(4,1) + pta.get(5,1)*coluna + pta.get(6,1)*linha;

        double Ox=(pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
        double Oy=(pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);

        double dX=mfX-Ox;
        double dY=mfY-Oy;

        double kappa0;
        if (fabs(dY)<1e-14 && mfX>Ox)
                kappa0= 0;
        else if (fabs(dY)<1e-14 && mfX<Ox)
                kappa0= M_PI;
        else if (fabs(dX)<1e-14 && mfY>Oy)
                kappa0=M_PI/2;
        else if (fabs(dX)<1e-14 && mfY<Oy)
                kappa0= 3*M_PI/2;
        else if (fabs(dX)>1e-14 && fabs(dY)>1e-14)
        {
                double alpha=atan(fabs(dY/dX));
                if (dY > 0 && dX > 0)
                        return alpha;
                if (dY > 0 && dX < 0)
                        return M_PI-alpha;
                if (dY < 0 && dX < 0)
                        return M_PI+alpha;
                if (dY < 0 && dX > 0)
                        return -alpha;
        }
        return kappa0;
}

double BundleAdjustment::getMediaScale(int imageIndex)
{
        *
        double media=0.0;
        double normaObjeto=0.0;
        double normaImagem=0.0;

        int pnts=listImages.at(imageIndex)->countPoints();

        double xFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getX();
        double yFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getY();
        double zFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getZ();
        double xiFirstPoint=getPointFrom(imageIndex,0)->getDetectorCoordinate(listImages.at(imageIndex)->getId()).getXi();
        double etaFirstPoint=getPointFrom(imageIndex,0)->getDetectorCoordinate(listImages.at(imageIndex)->getId()).getEta();

        for(int j=1;j<pnts;j++)
        {
                if (whereInPoints(getPointFrom(imageIndex,j))!=-1 && !isCheckingPoint(imageIndex,j))
                {
                        double xCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getX();
                        double yCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getY();
                        double zCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getZ();

                        double xiCurrentPoint=getPointFrom(imageIndex,j)->getDetectorCoordinate(listImages.at(imageIndex)->getId()).getXi();
                        double etaCurrentPoint=getPointFrom(imageIndex,j)->getDetectorCoordinate(listImages.at(imageIndex)->getId()).getEta();

                        normaObjeto =sqrt(pow(xFirstPoint-xCurrentPoint,2)+pow(yFirstPoint-yCurrentPoint,2)+pow(zFirstPoint-zCurrentPoint,2));
                        normaImagem =sqrt(pow(xiFirstPoint-xiCurrentPoint,2)+pow(etaFirstPoint-etaCurrentPoint,2));
                        //printf("Objeto sqrt(( %.3f - %.3f )^2) + ( %.3f - %.3f)^2 ))= %f\n",xiFirstPoint,xiCurrentPoint,etaFirstPoint,etaCurrentPoint,normaImagem);
                        media+=normaObjeto/normaImagem;
                }
        }
        int p=numberControlPoints(listImages.at(imageIndex))+numberPhotogrammetricPoints(listImages.at(imageIndex));
        printf("object media %f count: %d\n",media,p);
        /
        //return //media/(p-1);
}
*/

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br



