#include "ConvertionsSystems.h"
#include <QDebug>
#include "Dms.h"

ConvertionsSystems::ConvertionsSystems()
{
}
/** Class to convertions
  */

Matrix ConvertionsSystems::GeoelipToGeocentricCartesian(double phi, double lambda, double haltura, GeoSystem system)
{
    double a;
    double f;
    if (system==CORREGOALEGRE)
     {
        //Corrego Alegre
        a=6378388;
        f=1/297;
        //double b=6356912;
    }
    else if (system==SAD69)
    {
        a=6378160;
        f=1/298.25;
    }
    else if (system==SIRGAS2000)
    {
        a=6378137;
        f=1/298.257222101;
    }


    double b=a*(1-f);

    qDebug("phi= %f\tlambda= %f\th= %f",phi,lambda,haltura);

    double e2=1-(b*b)/(a*a);
    double Ni=a/(sqrt(1-e2*sin(phi)*sin(phi)));

    double Xi=(Ni+haltura)*cos(phi)*cos(lambda);
    double Yi=(Ni+haltura)*cos(phi)*sin(lambda);
    double Zi=(Ni*(1-e2)+haltura)*sin(phi);

    Matrix xyz(1,3);
    xyz.set(1,1,Xi);
    xyz.set(1,2,Yi);
    xyz.set(1,3,Zi);

    qDebug("Xi: %f\tYi: %f\tZi: %f\n",xyz.get(1,1),xyz.get(1,2),xyz.get(1,3));
    return xyz;
}

Matrix ConvertionsSystems::GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi, GeoSystem system)
{
    double a;
    double f;
    if (system==CORREGOALEGRE)
     {
        //Corrego Alegre
        a=6378388;
        f=1/297;
        //double b=6356912;
    }
    else if (system==SAD69)
    {
        a=6378160;
        f=1/298.25;
    }
    else if (system==SIRGAS2000)
    {
        a=6378137;
        f=1/298.257222101;
    }
    double b=a*(1-f);

    double e2=1-(b*b)/(a*a);
    double el2=(a/b)*(a/b)-1;

        double Si=sqrt(Xi*Xi+Yi*Yi);
    double phil=atan((Zi*a)/(Si*b));

    double phi=atan( (Zi+el2*b*pow(sin(phil),3)) / (Si-e2*a*pow(cos(phil),3)) );
        double lambda=asin(Yi/Si);

    double Ni=a/(sqrt(1-e2*sin(phi)*sin(phi)));
        double haltura=Si/cos(phi)-Ni;

        Matrix plh(1,3);
        plh.set(1,1,phi);
        plh.set(1,2,lambda);
        plh.set(1,3,haltura);
        qDebug("Xi: %f\tYi: %f\tZi: %f",Xi,Yi,Zi);
        qDebug("Phi: %f\tLambda: %f\th: %f\n",plh.get(1,1),plh.get(1,2),plh.get(1,3));

        return plh;
}

Matrix ConvertionsSystems::getMatrixR(double phi, double lambda)
{
        Matrix R(3,3);
        R.set(1,1,-sin(lambda));
        R.set(1,2,-sin(phi)*cos(lambda));
        R.set(1,3,cos(phi)*cos(lambda));

        R.set(2,1,cos(lambda));
        R.set(2,2,-sin(phi)*sin(lambda));
        R.set(2,3,cos(phi)*sin(lambda));

        R.set(3,1,0);
        R.set(3,2,cos(phi));
        R.set(3,3,sin(phi));

        return R;
}

Matrix ConvertionsSystems::geoElipToUtm(double phi, double lambda, double lambda0 ,double haltura)
{
//wgs-84
    double a=6378137;
    double f=3.35281066474e-3;
    double b=a*(1-f);

    double n=(a-b)/(a+b);
    double alpha=(a+b)*(1+pow(n,2)/4+pow(n,4)/64)/2;
    double beta=-3*n/2+9*pow(n,3)/16-3*pow(n,5)/32;
    double gama=15*pow(n,2)/16-15*pow(n,4)/32;
    double delta=-35*pow(n,3)/48+105*pow(n,5)/256;
    double xsi=315*pow(n,4)/512;
/*
    qDebug("alpha: %f",alpha);
    qDebug("beta: %g",beta);
    qDebug("gama: %g",gama);
    qDebug("delta: %g",delta);
    qDebug("xsi: %g",xsi);
*/
 //wgs 84 ellipsoid
    /*
    double n=(a-b)/(a+b);
    double alpha=6367449.1458;
    double beta=-2.51882792e-3;
    double gama=2.64354e-6;
    double delta=-3.45e-9;
    double xsi=5e-12;
*/
    double el2=(a*a-b*b)/(b*b);
    double t=tan(phi);

    //Bessel ellipsoid
    /*
    double alpha=6366742.5203;
    double beta=-2.51127456e-3;
    double gama=2.62771e-6;
    double delta=-3.42e-9;
    double xsi=5e-12;
*/

    double eta2=el2*pow(cos(phi),2);
    double N=(a*a)/(b*sqrt(1+eta2));
    double l=lambda-lambda0;
    double B=alpha*(phi+beta*sin(2*phi)+gama*sin(4*phi)+delta*sin(6*phi)+xsi*sin(8*phi));
  //  qDebug("B: %f\n",B);

    double y= B+t*N*pow(cos(phi),2)*l*l/2+
             (t*N*pow(cos(phi),4)*(5-t*t+9*eta2+4*eta2*eta2)            *    pow(l,4)) / 24+
             (t*N*pow(cos(phi),6)*(61-58*t*t+pow(t,4)+270*eta2-330*t*t*eta2)*pow(l,6)) / 720+
             (t*N*pow(cos(phi),8)*(1385-3111*t*t+543*pow(t,4)-pow(t,6)) *    pow(l,8)) / 40320;

    double x= N*l*cos(phi) +N*pow(cos(phi),3)*(1-t*t+eta2)              *    pow(l,3)  / 6+
             (N*pow(cos(phi),5)*(5-18*t*t+pow(t,4)+14*eta2-58*t*t*eta2) *    pow(l,5)) / 120+
             (N*pow(cos(phi),7)*(61-479*t*t+179*pow(t,4)-pow(t,6))      *    pow(l,7)) / 5040;

    Matrix xyh(1,3);
    xyh.set(1,1,x*0.9996);
    xyh.set(1,2,y*0.9996);
    xyh.set(1,3,haltura);
    qDebug("phi: %f\tlambda: %f\thaltura: %f",phi,lambda,haltura);
    qDebug("x: %f\ty: %f\th: %f\n",xyh.get(1,1),xyh.get(1,2),xyh.get(1,3));

    return xyh;
}

Matrix ConvertionsSystems::uTmToGeo(double x, double y, double haltura, double lambda0)
{
    //wgs-84

        double a=6378137;
        double f=3.35281066474e-3;
        double b=a*(1-f);

    // wgs 84 ellipsoid
/*
        double alphab=6367449.1458;
        double betab=2.51882658e-3;
        double gamab=3.70095e-6;
        double deltab=7.45e-9;
        double xsib=17e-12;
        double yb=y/alphab;
*/
        //bessel ellipsoid
/*
        double alphab=6366742.5203;
        double betab=2.51127324e-3;
        double gamab=3.67879e-6;
        double deltab=7.38e-9;
        double xsib=17e-12;
        double yb=y/alphab;
*/
        double n=(a-b)/(a+b);
        double alphab=(a+b)*(1+n*n/4+pow(n,4)/64)/2;
        double betab=3*n/2-27*pow(n,3)/32+269*(pow(n,5))/512;
        double gamab=21*n*n/16-55*pow(n,4)/32;
        double deltab=151*pow(n,3)/96-417*pow(n,5)/128;
        double xsib=1097*pow(n,4)/512;
        double yb=y/alphab;

        double phif=yb+betab*sin(2*yb)+gamab*sin(4*yb)+deltab*sin(6*yb)+xsib*sin(8*yb);

        double elf2=(a*a-b*b)/(b*b);
        double tf=tan(phif);
       // qDebug("phif bessel: %f",phif);
        //Bessel ellipsoid
        /*
        double alpha=6366742.5203;
        double beta=2.51127324e-3;
        double gama=3.678279e-6;
        double delta=7.38e-9;
        double xsi=17e-12;
        */
        double etaf2=elf2*pow(cos(phif),2);
        double Nf=(a*a)/(b*sqrt(1+etaf2));

        double phi= phif+(tf*(-1-etaf2)*x*x)/(2*Nf*Nf)+
                    (tf*(5+3*tf*tf+6*etaf2-6*tf*tf*etaf2-3*etaf2*etaf2-9*tf*tf*etaf2*etaf2)*pow(x,4))/(24*pow(Nf,4))+
                    (tf*(-61-90*tf*tf-45*pow(tf,4)-107*etaf2+162*tf*tf*etaf2+45*pow(tf,4)*etaf2)*pow(x,6))/(720*pow(Nf,6))+
                    (tf*(1385+3633*tf*tf+4095*pow(tf,4)+1575*pow(tf,6))*pow(x,8))/(40320*pow(Nf,8));

        double lambda= lambda0+x/(Nf*cos(phif))+
                       (-1-2*tf*tf-etaf2)*pow(x,3)/(6*pow(Nf,3)*cos(phif))+
                       (5+28*tf*tf+24*pow(tf,4)+6*etaf2+8*tf*tf*etaf2)*pow(x,5)/(120*pow(Nf,5)*cos(phif))+
                       (-61-662*tf*tf-1320*pow(tf,4)-720*pow(tf,6))*pow(x,7)/(5040*pow(Nf,7)*cos(phif));

        Matrix plh(1,3);
        plh.set(1,1,phi/0.9996);
        plh.set(1,2,lambda);
        plh.set(1,3,haltura);

        qDebug("x: %f\ty: %f\th: %f\n",x,y,haltura);
        qDebug("phi: %f\tlambda: %f\thaltura: %f\n",plh.get(1,1),plh.get(1,2),plh.get(1,3));

        return plh;
}

/*Ex1	E=5.000.000,0;
                N=6.900.000,0;
        Meridiando central do fuso = -30°;

        Latidude = 21° 46' 32,6842" South
        Longitude =11° 13' 9,72626" West
        Convergencia meridiana = 17° 58' 47,65429"

  Ex2	E=8.000.000,0;
                N=2.000.000,0;
        Meridiando central do fuso = 30°;

                WGS84
        Latidude = 17° 34' 50,82536" South
        Longitude= 44° 01' 13,59099" East
        Convergencia meridiana = 4° 18' 52,91364"
        */


//}
