#include "BundleAdjustment.h"
#include "Dms.h"

#include <qdebug.h>

#define MAXITERATIONS 10
#define CONVERGENCY 0.001
#define MAXRESIDUO 0.0001

BundleAdjustment::BundleAdjustment()
{

}

BundleAdjustment::BundleAdjustment(deque<Image*>listSelectedImages, deque<Point*> listSelectedPoints, int flightDirection)
{
	listImages=listSelectedImages;
	listPoints=listSelectedPoints;
	fliDir=flightDirection;

	numImages=listImages.size();
	numPoints=listPoints.size();
	numControlPoints=0;
	numFotogrametricPoints=0;

	for (int i=0;i<numPoints;i++)
	{
		if(listPoints.at(i)->is("ControlPoint"))
			numControlPoints++;
		else if(listPoints.at(i)->is("PhotogrammetricPoint"))
			numFotogrametricPoints++;
	}

	fillAnalogCoordinates();
	numEquations=numberOfEquations();
	numUnknows=6*numImages+3*numFotogrametricPoints;

	/** ser´a eliminiado em breve*/
	c= listImages.at(0)->getSensor()->getFocalDistance();
	xsi0=listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getXi();
	eta0=listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getEta();

	userInitialValues= false;
	printAll();
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

void BundleAdjustment::fillAnalogCoordinates()
{
	for (int i=0; i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		for (int j=0;j<pnts;j++)
		{
			double col=getPointFrom(i,j)->getDigitalCoordinate(listImages.at(i)->getId()).getCol();
			double lin=getPointFrom(i,j)->getDigitalCoordinate(listImages.at(i)->getId()).getLin();
			Matrix coord=digitalToAnalog(listImages.at(i)->getIO(),lin,col);
			AnalogImageSpaceCoordinate aux(listImages.at(i)->getId());
			aux.setXi(coord.get(1,1));
			aux.setEta(coord.get(1,2));
			getPointFrom(i,j)->putAnalogCoordinate(aux);
		}
	}
}

//Professor Nunes mandou matar a iteraçao sobre os residuos para evitar os valores "NANs"
bool BundleAdjustment::calculate()
{
	if (!userInitialValues)
		getInicialsValues();


	matAdjust.show('f',5,"matAdjust Inicial Values");
	P.identity(numEquations);
	//bool resOk=false;
	int changePesos=0;
	//Matrix tempRes;
	totalIterations=0;

	if (numFotogrametricPoints!=0)
	{

		//while(!resOk)
		//{
			bool conv=false;
			while(totalIterations<MAXITERATIONS && !conv)
			{
				createA1();
				//A1.show('f',3,"A1");
				createA2();
				//A2.show('f',3,"A2");
				createL0();
				//L0.show('f',3,"L0");
				createLb();
				//Lb.show('f',3,"Lb");
				Matrix l=Lb-L0;
				Matrix n11=getN11();
				//n11.show('f',3,"N11");
				Matrix n12=getN12();
				//n12.show('f',3,"N12");
				Matrix n22=getN22();
				//n22.show('f',3,"N22");
				Matrix n1=getn1(l);
				//n1.show('f',3,"n1");
				Matrix n2=getn2(l);
				//n2.show('f',3,"n2");
				getx1(n11,n12,n22,n1,n2);
				getx2(n12,n22,n2);
				//x1.show('f',10,"x1");
				//x2.show('f',3,"x2");
				//matAdjust.show('f',5,"MatAdjus antes do update do loop");
				updateMatAdjust();
				//matAdjust.show('f',5,"MatAdjus depois do update do loop");
				updateCoordFotog();
				conv=testConverged();
				totalIterations++;

				//qDebug("iteration %d",iterations);

			}
			//qDebug("numero iterations %d=%d",changePesos,iterations);

			//matAdjust.show('f',5,"MatAdjus depois da iteracao");
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
	else
	{
		//qDebug("Sem pontos Fotogrametricos");
		/*while(!resOk)
		{*/
			totalIterations=0;
			bool conv=false;
			while(totalIterations<MAXITERATIONS && !conv)
			{
				createA1();
				createL0();
				createLb();
				Matrix l=Lb-L0;
				Matrix n11=getN11();
				Matrix n1=getn1(l);
				n11.show('f',3,"N11");
				x1=n11.inverse()*n1;
				//x1.show('f',3,"x1");
				//matAdjust.show('f',5,"MatAdjus antes do update do loop");
				updateMatAdjust();
				//matAdjust.show('f',5,"MatAdjus depois do update do loop");
				conv=testConverged();
				totalIterations++;
				//qDebug("iteration %d",iterations);
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
	setAFP();
	return true;
}


/* metodos de calculo*/
Matrix BundleAdjustment::getM11(Matrix M1)
{
	return M1.transpose()*M1;
}

Matrix BundleAdjustment::getM12(Matrix M1, Matrix M2)
{
	return M1.transpose()*M2;
}

Matrix BundleAdjustment::getM22(Matrix M2)
{
	return M2.transpose()*M2;
}

Matrix BundleAdjustment::getm1(Matrix M1, Matrix L1)
{
	return (M1.transpose())*L1;
}

Matrix BundleAdjustment::getm2(Matrix M2, Matrix L1)
{
	return (M2.transpose())*L1;
}

Matrix BundleAdjustment::getPAf(Matrix M11, Matrix M12, Matrix M22, Matrix m1, Matrix m2)
{
	return ( (M11-M12*M22.inverse()*M12.transpose()).inverse() )*( m1-M12*M22.transpose()*m2 );
}

// Aproximaçao inicial para as coordenadas planimetricas(XY) dos pontos fotogrametricos
Matrix BundleAdjustment::getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf)
{
	return M22.inverse()*m2-M22.inverse()*M12.transpose()*paf;
}

Matrix BundleAdjustment::getN11()
{
	return A1.transpose()*P*A1;
}

Matrix BundleAdjustment::getN12()
{
	return A1.transpose()*P*A2;
}

Matrix BundleAdjustment::getN22()
{
	return A2.transpose()*A2;
}

Matrix BundleAdjustment::getn1(Matrix L)
{
	return A1.transpose()*P*L;
}

Matrix BundleAdjustment::getn2(Matrix L)
{
	return A2.transpose()*L;
}

Matrix BundleAdjustment::getx1(Matrix N11, Matrix N12, Matrix N22, Matrix n1, Matrix n2)
{
	Matrix temp1=N12*N22.inverse();
	Matrix temp2=temp1*N12.transpose();

	Matrix temp3=temp1*n2;
	Matrix temp4=(N11-temp2).inverse();
	Matrix temp5=n1-temp3;

	//x1=(N11-N12*N22.inverse()*N12.transpose()).inverse()*(n1-N12*N22.inverse()*n2);
	x1=temp4*temp5;
	return x1;
}

Matrix BundleAdjustment::getx2(Matrix N12, Matrix N22, Matrix n2)//, Matrix x1)
{
	x2=N22.inverse()*n2-N22.inverse()*N12.transpose()*x1;
	return x2;
}
/*
void BundleAdjustment::setx1(Matrix X1)
{
	//x1=X1;
}

void BundleAdjustment::setx2(Matrix X2)
{
	//x2=X2;
}*/

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

void BundleAdjustment::imprime(Matrix mat, char *id)
{
	printf("%s\n",id);
	int rows=mat.getRows();
	int cols=mat.getCols();
	for (int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
			printf("%.6f\t",mat.get(i+1,j+1));
		printf("\n");
	}
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
void BundleAdjustment::getInicialsValues()
{
	if (numFotogrametricPoints!=0)
	{
		Matrix L=createL();
		//L.show('f',3,"L");
		Matrix M1=createM1();
		//M1.show('f',3,"M1");
		Matrix M2=createM2();
		//M2.show('f',3,"M2");
		Matrix m11=getM11(M1);
		Matrix m12=getM12(M1,M2);
		//m12.show('f',3,"M12");

		Matrix m22=getM22(M2);
		//m22.show('f',3,"M22");
		Matrix m1=getm1(M1,L);
		Matrix m2=getm2(M2,L);

		Matrix paf=getPAf(m11,m12,m22,m1,m2);
		Matrix xypf=getXYpf(m22,m2,m12,paf);

		//paf.show('f',4,"paf");
		//xypf.show('f',4,"xypf");
		updateCoordinatesAllPoints(xypf,getInicialZPhotogrammetricPoints());

		Matrix temp(numImages,6);
		matAdjust=temp;
		for (int i=0;i<numImages;i++)
		{
			xsi0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
			eta0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getEta();
			c=listImages.at(i)->getSensor()->getFocalDistance();

			Matrix pta=getPTA(paf,i);

			matAdjust.set(i+1,1,0);
			matAdjust.set(i+1,2,0);
			//qDebug("Kappa0");
			matAdjust.set(i+1,3,listImages.at(i)->getFlightDirection());
			matAdjust.set(i+1,4,pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
			matAdjust.set(i+1,5,pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
			matAdjust.set(i+1,6,c*getMediaScale(i));
		}
	}
	else
	{
		//qDebug("initial Values: Sem pontos Fotogrametricos");

		Matrix L=createL();
		Matrix M1=createM1();
		Matrix m11=getM11(M1);
		Matrix m1=getm1(M1,L);
		Matrix paf=m11.inverse()*m1;

		Matrix temp(numImages,6);
		matAdjust=temp;
		for (int i=0;i<numImages;i++)
		{
			xsi0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
			eta0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getEta();
			c=listImages.at(i)->getSensor()->getFocalDistance();
			Matrix pta=getPTA(paf,i);
			matAdjust.set(i+1,1,0);
			matAdjust.set(i+1,2,0);
			//matAdjust.set(i+1,3,getKappaZero(pta,i));
			matAdjust.set(i+1,3,listImages.at(i)->getFlightDirection());
			matAdjust.set(i+1,4,pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
			matAdjust.set(i+1,5,pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
			matAdjust.set(i+1,6,c*getMediaScale(i));
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
		Matrix temp=imageToMatrixAnalogicalCoordenates(listImages.at(i));
		result.putMatrix(temp,result.getRows()+1,6*i+1);
	}
	//imprime(result,"M1object");
	return result;
}

Matrix BundleAdjustment::createM2()
{
	Matrix result(0,2*numFotogrametricPoints);
	/* configuraçao de cada matriz B
		 |-1  0|
		 | 0 -1|
	*/
	Matrix B(2,2);
	B.set(1,1,-1);
	B.set(2,2,-1);
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		int rows=numberControlPoints(listImages.at(i))+numberPhotogrammetricPoints(listImages.at(i));
		int posLin=1;
		Matrix oneImage(2*rows,2*numFotogrametricPoints);
		for (int j=0;j<pnts;j++)
		{
			if(whereInPoints(getPointFrom(i,j))!=-1)
			{
				if(isPhotogrammetricPoint(i,j))
				{

					int posCol=whereInPoints(getPointFrom(i,j));
					posCol=2*(posCol-numControlPoints)+1;
					//qDebug("M2:Achou o ponto fotogrametrico %d",getPointFrom(i,j)->getId());
					//	qDebug("put in i=%d j=%d",2*j+1,2*(pos-numControlPoints)+1);//2*
					//oneImage.putMatrix(B,2*(posLin)+1,2*(posCol-numControlPoints)+1);
					oneImage.putMatrix(B,posLin,posCol);
				}
				posLin+=2;
			}
		}
		//oneImage.show('f',3,"OneImage");
		result=result|oneImage;
		//result.show('f',3,"Result");
	}

	//imprime(result,"M2object");
	return result;
}

// Retorna uma matriz correspondente a uma imagem na matriz M
Matrix BundleAdjustment::imageToMatrixAnalogicalCoordenates(Image *img)
{
	int numpnts=img->countPoints();

	Matrix result(0,6);
	for (int i=0;i<numpnts;i++)
	{
		/* configuraçao de cada matriz temp
				   |1	eta xi	0	 0		0 |
				   |0	 0	0	1	eta 	xi|
		*/
		if (whereInPoints(img->getPointAt(i))!=-1 && !img->getPointAt(i)->is("CheckingPoint"))
		{
			Matrix temp(2,6);
			/** conversao digital to analog sera feito pela classe do Rafael em breve */
			int lin=img->getPointAt(i)->getDigitalCoordinate(img->getId()).getLin();
			int col=img->getPointAt(i)->getDigitalCoordinate(img->getId()).getCol();

			//qDebug("object col: %d\t lin: %d",col, lin);
			Matrix coord=digitalToAnalog(img->getIO(),lin,col);

			temp.set(1,1,1);
			temp.set(1,2,coord.get(1,2));
			temp.set(1,3,coord.get(1,1));
			temp.set(2,4,1);
			temp.set(2,5,coord.get(1,2));
			temp.set(2,6,coord.get(1,1));

			result=result|temp;
		}
	}
	// imprime(result,"image");
	return result;
}

Matrix BundleAdjustment::digitalToAnalog(InteriorOrientation *oi,int linha, int coluna)
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
		result.setInt(1,1, (int)((b2*xsi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2) + 0.1 ) );
		result.setInt(1,2, (int)((a1*eta - a1*b0 - b1*xsi + b1*a0) / (a1*b2 - b1*a2) + 0.1 ) );
		return result;
}

int BundleAdjustment::numberControlPoints(Image *img)
{
	int cont=0;
	int points=img->countPoints();
	for (int i=0;i<points;i++)
		if(img->getPointAt(i)->is("ControlPoint") && whereInPoints(img->getPointAt(i))!=-1)
			cont++;
	return cont;
}

int BundleAdjustment::numberPhotogrammetricPoints(Image *img)
{
	int cont=0;
	int points=img->countPoints();
	for (int i=0;i<points;i++)
		if(img->getPointAt(i)->is("PhotogrammetricPoint") && whereInPoints(img->getPointAt(i))!=-1)
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

double BundleAdjustment::getMediaScale(int imageIndex)
{

	double media=0.0;
	double normaObjeto=0.0;
	double normaImagem=0.0;

	int pnts=listImages.at(imageIndex)->countPoints();

	double xFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getX();
	double yFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getY();
	double zFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getZ();
	double xiFirstPoint=getPointFrom(imageIndex,0)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getXi();
	double etaFirstPoint=getPointFrom(imageIndex,0)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getEta();

	for(int j=1;j<pnts;j++)
	{
		if (whereInPoints(getPointFrom(imageIndex,j))!=-1 && !isCheckingPoint(imageIndex,j))
		{
			double xCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getX();
			double yCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getY();
			double zCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getZ();

			double xiCurrentPoint=getPointFrom(imageIndex,j)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getXi();
			double etaCurrentPoint=getPointFrom(imageIndex,j)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getEta();

			normaObjeto =sqrt(pow(xFirstPoint-xCurrentPoint,2)+pow(yFirstPoint-yCurrentPoint,2)+pow(zFirstPoint-zCurrentPoint,2));
			normaImagem =sqrt(pow(xiFirstPoint-xiCurrentPoint,2)+pow(etaFirstPoint-etaCurrentPoint,2));
			//printf("Objeto sqrt(( %.3f - %.3f )^2) + ( %.3f - %.3f)^2 ))= %f\n",xiFirstPoint,xiCurrentPoint,etaFirstPoint,etaCurrentPoint,normaImagem);
			media+=normaObjeto/normaImagem;
		}
	}
	int p=numberControlPoints(listImages.at(imageIndex))+numberPhotogrammetricPoints(listImages.at(imageIndex));
	//printf("object media %f count: %d\n",media,p);
	return media/(p-1);
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
			result=result|getJacobianaControl(x,y,z,indexImage+1); // esse id pode gerar inconsistencia// soluçao seria setar os ids de todas as imagens e pontos no inicio logo apos o sort de pontos
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

	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		int rows=numberControlPoints(listImages.at(i))+numberPhotogrammetricPoints(listImages.at(i));
		int posLin=1;
		Matrix oneImage(2*rows,3*numFotogrametricPoints);
		for (int j=0;j<pnts;j++)
		{
			if(whereInPoints(getPointFrom(i,j))!=-1 && !isCheckingPoint(i,j))
			{
				if(isPhotogrammetricPoint(i,j))
				{
					ObjectSpaceCoordinate aux=getPointFrom(i,j)->getObjectCoordinate();
					double x=aux.getX();
					double y=aux.getY();
					double z=aux.getZ();
					Matrix jf=getJacobianaFotogrametric(x,y,z,i+1);
					int posCol=whereInPoints(listImages.at(i)->getPointAt(j));
					posCol=3*(posCol-numControlPoints)+1;

					oneImage.putMatrix(jf,posLin,posCol);
				}
				posLin+=2;
			}
		}
		result=result|oneImage;
	}
	A2=result;
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
				/** Pegar esse metodo na classe do Rafael, as coordenadas pela equaçao de colinearidade*/
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
				AnalogImageSpaceCoordinate aux=getPointFrom(i,j)->getAnalogCoordinate(listImages.at(i)->getId());
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
				double xi=getPointFrom(i,j)->getAnalogCoordinate(listImages.at(i)->getId()).getXi();
				double eta=getPointFrom(i,j)->getAnalogCoordinate(listImages.at(i)->getId()).getEta();
				//qDebug("%f %f %f \t\t xi: %f eta: %f",x,y,z,xi,eta);

				/** Usar classe do Rafael para isso */
				Matrix coord=getCoordColinearTerrain(xi,eta,z,i+1);
				double resx=x-coord.get(1,1);
				double resy=y-coord.get(1,2);
				// reaproveitando a matrix que não será mais utilizada nesse loop;
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
				double p=(numEquations-numUnknows)/(pow(getRx(listImages.at(i),j+1),2));
				P.set(d,d,p);
				d++;
				p=(numEquations-numUnknows)/((pow(getRy(listImages.at(i),j+1),2)));
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
		if(pnt->is("PhotogrammetricPoint")  && whereInPoints(pnt)!=-1)
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

double BundleAdjustment::getRx(Image *img, int pointId)
{
	int base=0;
	for(int i=1;i<img->getId();i++)
		base+=2*numberControlPoints(img);
	return matRes.get(base+2*(pointId-1)+1,1);
}

double BundleAdjustment::getRy(Image *img, int pointId)
{
	int base=0;
	for(int i=1;i<img->getId();i++)
		base+=2*numberControlPoints(img);
	return matRes.get(base+2*(pointId-1)+2,1);
}

/*Metodos para aumento de performance do calculo*/

Matrix BundleAdjustment::getSparseN11()
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
	return A1;
}

/* setters and getters*/
Matrix BundleAdjustment::getMatRes()
{
	//matRes.show('f',8,"MatRes");
	return matRes;
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
	//Isso é feito apenas para:
	//Exibiçao dos angulos em Graus.
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
	return listImages.at(imageIndex)->getPointAt(pointIndex)->is("ControlPoint");
}

bool BundleAdjustment::isPhotogrammetricPoint(int imageIndex, int pointIndex)
{
	return listImages.at(imageIndex)->getPointAt(pointIndex)->is("PhotogrammetricPoint");
}

bool BundleAdjustment::isCheckingPoint(int imageIndex, int pointIndex)
{
	return listImages.at(imageIndex)->getPointAt(pointIndex)->is("CheckingPoint");
}

bool BundleAdjustment::testResiduo()
{
	int rowsMatRes=matRes.getRows();
	for (int i=1;i<=rowsMatRes;i++)
	{
		if (fabs(matRes.get(i,1)>MAXRESIDUO))
		{
			return false;
		}
	}
	return true;
}

bool BundleAdjustment::testConverged()
{
	int rowsX1=x1.getRows();
	int rowsX2=x2.getRows();
   // printf("testando X1");
	for (int i=1;i<=rowsX1;i++)
	{
		if (fabs(x1.get(i,1)>CONVERGENCY))
		{
			return false;
		}
	}
	//printf("testando X2");
	for (int i=1;i<=rowsX2;i++)
	{
		if (fabs(x2.get(i,1)>CONVERGENCY))
		{
			return false;
		}
	}
	return true;
}

bool BundleAdjustment::isConverged()
{
	if(totalIterations<MAXITERATIONS)
		return converged=true;
	else
		return converged=false;
}

void BundleAdjustment::setUserInitialValues(Matrix initialValues)
{

	matAdjust=initialValues;
	userInitialValues= true;

}

void BundleAdjustment::printAll()
{
	int numImages=listImages.size();
	for (int i=0;i<numImages;i++)
	{
		Image *img=listImages.at(i);
		printf("%s:\n",img->getFilename().c_str());
		int numPoints=img->countPoints();
		for (int j=0; j<numPoints;j++)
		{
			Point *pnt=img->getPointAt(j);
			int col=pnt->getDigitalCoordinate(img->getId()).getCol();
			int lin=pnt->getDigitalCoordinate(img->getId()).getLin();
			printf("\t%s: col: %d lin %d\n",pnt->getPointId().c_str(),col,lin);
		}
		printf("\n");
	}
}


Matrix BundleAdjustment::getMVC()
{
	//Isso é feito apenas para:
	//Exibiçao dos angulos em Graus.
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
