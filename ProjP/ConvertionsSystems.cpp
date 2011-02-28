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

Matrix ConvertionsSystems::geoElipToUtm(double phi, double lambda, double haltura, char hemi)//, char side)
{
    /*
//wgs-84
    double a=6378137;
    double f=3.35281066474e-3;
    double b=a*(1-f);
/*
 /*   double n=(a-b)/(a+b);
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

 //wgs 84 ellipsoid

    double alpha=6367449.1458;
    double beta=-2.51882792e-3;
    double gama=2.64354e-6;
    double delta=-3.45e-9;
    double xsi=5e-12;

    double el2=(a*a-b*b)/(b*b);
    double t=tan(phi);

    //Bessel ellipsoid

    double alpha=6366742.5203;
    double beta=-2.51127456e-3;
    double gama=2.62771e-6;
    double delta=-3.42e-9;
    double xsi=5e-12;

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
*/

//sad-69
    double k0=0.9996;
    double a=6378206.4;
    double f=1/294.978698;
    //double a=6378388;
    //double f=1/297;

    double b=a*(1-f);
    double e2=(a*a-b*b)/(a*a);

    Dms lamb0;
    Dms oneSecond(0,0,1);
    double sin1=sin(oneSecond.dmsToRadiano());
    double inteiro;
    modf(lamb0.radianoToDegreeDecimal(lambda)/6, &inteiro);
    int lambda0=(inteiro)*6+3;

    double el2=e2/(1-e2);
    double v=a/(sqrt(1-e2*pow(sin(phi),2)));
    qDebug("v= %.12f",v);

    double A=1 + 3*e2/4 + 45*pow(e2,2)/64 + 175*pow(e2,3)/256 + 11025*pow(e2,4)/16384 + 43659*pow(e2,5)/65536;
    double B=   (3*e2/4 + 15*pow(e2,2)/16 + 525*pow(e2,3)/512 + 2205*pow(e2,4)/2048 + 72765*pow(e2,5)/65536)/2;
    double C=            (15*pow(e2,2)/64 + 105*pow(e2,3)/256 + 2205*pow(e2,4)/4096 + 10395*pow(e2,5)/16384)/4;
    double D=                               (35*pow(e2,3)/512 + 315*pow(e2,4)/2048 + 31185*pow(e2,5)/131072)/6;
    double E=                                                  (315*pow(e2,4)/16384 + 3465*pow(e2,5)/65536)/8;
    double F=                                                                         (693*pow(e2,5)/131072)/10;

    double S=a*(1-e2)*(A*phi - B*sin(2*phi) + C*sin(4*phi) - D*sin(6*phi) + E*sin(8*phi) - F*sin(10*phi) );

    double I=S*k0;// Ko=0.9996
    double II  = (v*sin(phi)*pow(sin1,2)*cos(phi)*k0*1e8)/2;
    double III = v*sin(phi)*pow(sin1,4)*pow(cos(phi),3)* (5 - pow(tan(phi),2) + 9*el2*pow(cos(phi),2) + 4*pow(el2,2)*pow(cos(phi),4) ) *k0*1e16/24;
    double IV  = v*cos(phi)*k0*1e4*sin1;
    double V   = v*pow(cos(phi),3)*pow(sin1,3)*(1 - pow(tan(phi),2) + el2*pow(cos(phi),2) )*k0*1e12/6;
    double dlambda=(lambda-Dms::degreeDecimalToRadiano(lambda0));
    double p=0.0001*fabs(dlambda)*3600*180/(M_PI);

    //qDebug(" lambda0dms = %s",lambda0dms.toString(6).toStdString().c_str());
    //qDebug(" lambdadms = %s",lambdaDms.toString(6).toStdString().c_str());
   // qDebug(" dlambda = %s",oneSecond.radianoToDms(dlambda)->toString().toStdString().c_str());

 //   qDebug("p= %.12f",p);

    double A6=pow(sin1,6)*pow(p,6)*(v*sin(phi)*pow(cos(phi),5))*(61 - 58*pow(tan(phi),2) + pow(tan(phi),4) + 270*el2*pow(cos(phi),2) - 330*el2*pow(sin(phi),2) )*k0*1e24/720;
    double B5=pow(sin1,5)*pow(p,5)*(v*pow(cos(phi),5))*(5 - 18*pow(tan(phi),2) + pow(tan(phi),4) + 14*el2*pow(cos(phi),2) - 58*el2*pow(sin(phi),2) )*k0*1e20/120;
/*
    qDebug("\nlambda0= %d",lambda0);
    qDebug("S= %.12f",S);
    qDebug("\nI= %.12f",I);
    qDebug("II= %.12f",II);
    qDebug("III= %.12f",III);
    qDebug("IV= %.12f",IV);
    qDebug("V= %.12f",V);
    qDebug("A6= %.12f",A6);
    qDebug("B5= %.12f",B5);
*/
    double Nl=I+II*pow(p,2)+III*pow(p,4)+A6;
    double El=IV*p+V*pow(p,3)+B5;

    double N;
    if (hemi=='s' || hemi =='S')
    {
        N=10000000 - Nl;
    }
    else
    {
        N=Nl;
    }

    if(p>0)
    {
        E = El + 500000;
    }
    else
    {
        E = El - 500000;
    }

    Matrix xyh(1,4);
    xyh.set(1,1,E);
    xyh.set(1,2,N);
    xyh.set(1,3,haltura);
    xyh.set(1,4,lambda0);
    qDebug("phi: %.9f\tlambda: %.9f\thaltura: %f",phi,lambda,haltura);
    qDebug("E: %.9f\tN: %.9f\tH: %f\n",xyh.get(1,1),xyh.get(1,2),xyh.get(1,3));

    return xyh;
}

Matrix ConvertionsSystems::uTmToGeo(double E, double N, double haltura, double MC, char hemi)
{
    qDebug("x: %.9f\ty: %.9f\th: %f\n",E,N,haltura);
    //wgs-84
    /*
        double a=6378137;
        double f=3.35281066474e-3;
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
/*


*/  double Nl;
    if (hemi=='s' || hemi =='S')
    {
        Nl=10000000 - N;
    }
    else
    {
        Nl=N;
    }

    double El=E-500000;

    double k0=0.9996;
    double a=6378206.4;
    double f=1/294.978698;
    //double a=6378388;
    //double f=1/297;

    double b=a*(1-f);
    double e2=(a*a-b*b)/(a*a);
    Dms oneSecond(0,0,1);
    double sin1=sin(oneSecond.dmsToRadiano());
    double el2=e2/(1-e2);

    double q=1e-6*(El);//q=0,272075812415666

    double A=1 + 3*e2/4 + 45*pow(e2,2)/64 + 175*pow(e2,3)/256 + 11025*pow(e2,4)/16384 + 43659*pow(e2,5)/65536;

    double phil=Nl/(a*(1-e2)*A);

    qDebug("phil=%.12f",phil);


    double v=a/(sqrt(1-e2*pow(sin(phil),2)));
    qDebug("v= %.12f",v);



    double VII=(tan(phil)*(1 + el2*pow(cos(phil),2))*1e12)/(2*pow(v,2)*sin1*pow(k0,2));
    double VIII=(tan(phil)*(5 + 3*pow(tan(phil),2) +6*el2*pow(cos(phil),2) - 6*el2*pow(sin(phil),2) - 3*pow(el2,2)*pow(cos(phil),4) - 9*pow(el2,2)*pow(cos(phil),2)*pow(sin(phil),2))*1e24)/(24*pow(v,4)*sin1*pow(k0,4));
    double IX=(1/cos(phil)*1e6)/(v*sin1*k0);
    double X=(1/cos(phil)*(1 + 2*pow(tan(phil),2) + el2*pow(cos(phil),2))*1e18)/(6*pow(v,3)*sin1*pow(k0,3));

    double E5=pow(q,5)*(1/cos(phi))*(5 + 28*pow(tan(phil),2) + 24*pow(tan(phil),4) + 6*el2*pow(cos(phil),2) + 8*el2*pow(sin(phil),2))*1e30/(120*pow(v,5)*sin1*pow(k0,5));
    double D6=(pow(q,6)*tan(phil)*(61 + 90*pow(tan(phil),2) + 45*pow(tan(phil),4) + 107*el2*pow(cos(phil),2) - 162*el2*pow(sin(phil),2) - 45*el2*pow(tan(phil)*sin(phil),2))*1e36)/(720*pow(v,6)*sin1*pow(k0,6));

    double phi= phil - VII*pow(q,2) + VIII*pow(q,4) - D6;
    double dlambda = IX*q - X*pow(q,3) + E5;


    double lambda;
    //if (El<0)
	lambda=MC+dlambda;

    Matrix plh(1,3);
    plh.set(1,1,phi);
    plh.set(1,2,lambda);
    plh.set(1,3,haltura);

	double value=10636.842;

	int degPhi=(int)value/3600;

	double resto1=fmod(value,3600);

	int minPhi=(int)resto1/60;

	double secPhi=fmod(resto1,60);

	//qDebug("temp %d\ndegPhi= %d\nresto1= %f",temp,degPhi,resto1);


    double resto2=0;
    double secLamb=0;
    double degLamb=modf(lambda/3600,&resto2);
    double minLamb=modf(resto2/60,&secLamb);

	qDebug("phi: %d %d %f \tlambda: %d %d %f",degPhi,minPhi,secPhi,degLamb,minLamb,secLamb);



   // Dms phiDms((int)degPhi,(int)minPhi,secPhi);
    //Dms lambDms((int)degLamb,(int) minLamb, secLamb);

    //qDebug("phi %s\tlamb %s",phiDms.toString(),lambDms.toString());

//  qDebug("phi: %.9f\tlambda: %.9f\thaltura: %f\n",plh.get(1,1),plh.get(1,2),plh.get(1,3));
    return plh;
}
