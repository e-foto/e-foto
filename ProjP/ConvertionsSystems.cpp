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
    double f=3.352811e-3;//066474e-3;
    double b=a*(1-f);
//clarke 1866
    double k0=0.9996;
    //double a=6378206.4;
    //double f=1/294.978698;
    //double a=6378388;
    //double f=1/297;

   // double b=a*(1-f);
    double e2=(a*a-b*b)/(a*a);

    Dms lamb0;
    Dms oneSecond(0,0,1);
    double sin1=sin(oneSecond.dmsToRadiano());
    double inteiro;
	modf(lamb0.radianoToDegreeDecimal(lambda)/6, &inteiro);

	//qDebug("%d",inteiro);
	int lambda0=(inteiro)*6+3;
	//int lambda0=(lamb0.radianoToDegreeDecimal(lambda)/6 )*6+3;

	double el2=e2/(1-e2);
    double v=a/(sqrt(1-e2*pow(sin(phi),2)));

    double A=1 + 3*e2/4 + 45*pow(e2,2)/64 + 175*pow(e2,3)/256 + 11025*pow(e2,4)/16384 + 43659*pow(e2,5)/65536;
    double B=   (3*e2/4 + 15*pow(e2,2)/16 + 525*pow(e2,3)/512 + 2205*pow(e2,4)/2048 + 72765*pow(e2,5)/65536)/2;
    double C=            (15*pow(e2,2)/64 + 105*pow(e2,3)/256 + 2205*pow(e2,4)/4096 + 10395*pow(e2,5)/16384)/4;
    double D=                               (35*pow(e2,3)/512 + 315*pow(e2,4)/2048 + 31185*pow(e2,5)/131072)/6;
    double E2=                                                  (315*pow(e2,4)/16384 + 3465*pow(e2,5)/65536)/8;
    double F=                                                                         (693*pow(e2,5)/131072)/10;

	double S=a*(1-e2)*(A*phi - B*sin(2*phi) + C*sin(4*phi) - D*sin(6*phi) + E2*sin(8*phi) - F*sin(10*phi) );

    double I=S*k0;// Ko=0.9996
    double II  = (v*sin(phi)*pow(sin1,2)*cos(phi)*k0*1e8)/2;
    double III = v*sin(phi)*pow(sin1,4)*pow(cos(phi),3)* (5 - pow(tan(phi),2) + 9*el2*pow(cos(phi),2) + 4*pow(el2,2)*pow(cos(phi),4) ) *k0*1e16/24;
    double IV  = v*cos(phi)*k0;//*1e4*sin1;
    double V   = (v*pow(cos(phi),3)/**pow(sin1,3)*//*(1 - pow(tan(phi),2) + el2*pow(cos(phi),2) )*k0)/6;

    //qDebug("v = %f",v);
/*
    double dlambda=(lambda-Dms::degreeDecimalToRadiano(lambda0));

    double p=0.0001*fabs(dlambda)*3600*180/(M_PI);

    //qDebug("p= %f",p);

    double A6=pow(sin1,6)*pow(p,6)*(v*sin(phi)*pow(cos(phi),5))*(61 - 58*pow(tan(phi),2) + pow(tan(phi),4) + 270*el2*pow(cos(phi),2) - 330*el2*pow(sin(phi),2) )*k0*1e24/720;
    double B5=pow(sin1,5)*pow(p,5)*(v*pow(cos(phi),5))*(5 - 18*pow(tan(phi),2) + pow(tan(phi),4) + 14*el2*pow(cos(phi),2) - 58*el2*pow(sin(phi),2) )*k0*1e20/120;

	double Nl=I+II*pow(p,2)+III*pow(p,4)+A6;
    double q=Dms::radianoToDegreeDecimal(dlambda)*M_PI/180;

    //qDebug("q = %f",q);
    //qDebug("IV = %f",IV);
    //qDebug("V = %f",V);

    double El=IV*q+V*pow(q,3);//+B5;-201821,819682498475+0,000042532615327860497751625

    double N;
    if (hemi=='s' || hemi =='S')
    {
        N=10000000 - Nl;
    }
    else
    {
        N=Nl;
    }

	double E;

    if(dlambda<0)
    {
        E = El + 500000;
    }
	else
    {
		E = El;
	}
*/
//teste com as equações da tabela
//geoElipToUtm(double phi, double lambda, double haltura, char hemi)
//wgs-84
	double a=6378137;
	double f=3.35281067183099e-3;
    double b=a*(1-f);
    //double e=sqrt(1-(b*b)/(a*a));
	double e2=(1-(b*b)/(a*a));	
	double el2=e2/(1-e2);
	double k0=0.9996;
    double n=(a-b)/(a+b);

    //em teste
    double A=a*(1 - n + (5*n*n/4)*(1-n)+(81*pow(n,4)/64)*(1-n));
    double B=(3*a*n/2)*(1-n-(7*n*n/8)*(1-n)+55*pow(n,4)/64);
    double C=(15*a*n*n/16)*(1-n+(3*n*n/4)*(1-n));
    double D=(35*a*pow(n,3)/48)*(1 - n + 11*n*n/16);
    double E2=(315*a*pow(n,4)/51)*(1-n);

    double S=A*phi - B*sin(2*phi) + C*sin(4*phi) - D*sin(6*phi) + E2*sin(8*phi);

//
	double inteiro;
	modf((lambda*180/M_PI)/6, &inteiro);

	//qDebug("%d",inteiro);
	int lambda0=(inteiro)*6+3;

	double dLambda=lambda-lambda0*M_PI/180;
/*
	double I=S*k0;// Ko=0.9996
	double II  = (v*sin(phi)*pow(sin1,2)*cos(phi)*k0*1e8)/2;
	double III = v*sin(phi)*pow(sin1,4)*pow(cos(phi),3)* (5 - pow(tan(phi),2) + 9*el2*pow(cos(phi),2) + 4*pow(el2,2)*pow(cos(phi),4) ) *k0*1e16/24;
	double IV  = v*cos(phi)*k0;//*1e4*sin1;
    double V   = (v*pow(cos(phi),3)/*(1 - pow(tan(phi),2) + el2*pow(cos(phi),2) )*k0)/6;
    */
    double nu=a/sqrt(1-e2*pow(sin(phi),2));
	
    double I   = S*k0;								// Ko=0.9996
    //double I=3374564.9755;
	double II  = (nu*sin(phi)*cos(phi)*k0)/2;
	double III = nu*sin(phi)*pow(cos(phi),3)*(5 - pow(tan(phi),2) + 9*el2*pow(cos(phi),2) + 4*pow(el2,2)*pow(cos(phi),4) ) *k0/24;
    double IV  = nu*cos(phi)*k0;
	double V   = (nu*pow(cos(phi),3)*(1 - pow(tan(phi),2) + el2*pow(cos(phi),2) )*k0)/6;

 //   qDebug("arc= %.12g \nnu= %.12g \np= %.12g",S,nu,dLambda);
 //   qDebug("I= %.12g \nII= %.12g \nIII= %.12g ",I,II,III);
//    double A6=pow(M_PI/(3600*180)*dLambda,6)*(nu*sin(phi)*pow(cos(phi),5))*(61 - 58*pow(tan(phi),2) + pow(tan(phi),4) + 270*el2*pow(cos(phi),2) - 330*el2*pow(sin(phi),2) )*k0*1e24/720;

    double N=I+II*pow(dLambda,2)+III*pow(dLambda,4);
    double E=500000+(IV*dLambda+V*pow(dLambda,3));

    Matrix xyh(1,4);

	E=round(E*1e4)/1e4;
	N=round(N*1e4)/1e4;
	//haltura=round(haltura*1e6)/1e6;

	Dms phiDms;
	phiDms.setSecondsPrecision(2);
	Dms lambdaDms;
	lambdaDms.setSecondsPrecision(2);

	xyh.set(1,1,E);
    xyh.set(1,2,N);
    xyh.set(1,3,haltura);
    xyh.set(1,4,lambda0);

	phiDms.radianoToDms(phi);
	lambdaDms.radianoToDms(lambda);
	//qDebug("phi: %.9f\tlambda: %.9f\thaltura: %f",phi,lambda,haltura);
	qDebug("phi: %s\tlambda: %s\thaltura: %f",phiDms.toString().toStdString().c_str(),lambdaDms.toString().toStdString().c_str(),haltura);
	qDebug("E: %.9f\tN: %.9f\tH: %f MC=%f\n",xyh.get(1,1),xyh.get(1,2),xyh.get(1,3),xyh.get(1,4));

    return xyh;
}

Matrix ConvertionsSystems::uTmToGeo(double E, double N, double haltura, double MC, char hemi)
{
/*
    double Nl;
    if (hemi=='s' || hemi =='S')
    {
        Nl=10000000 - N;
    }
    else
    {
        Nl=N;
    }
    double El=E-500000;

	//wgs-84
	/*
		double a=6378137;
		double f=3.35281066474e-3;
	*/
/*
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

    double q=1e-6*El;

    double A=1 + 3*e2/4 + 45*pow(e2,2)/64 + 175*pow(e2,3)/256 + 11025*pow(e2,4)/16384 + 43659*pow(e2,5)/65536;
    double B=   (3*e2/4 + 15*pow(e2,2)/16 + 525*pow(e2,3)/512 + 2205*pow(e2,4)/2048 + 72765*pow(e2,5)/65536)/2;
    double C=            (15*pow(e2,2)/64 + 105*pow(e2,3)/256 + 2205*pow(e2,4)/4096 + 10395*pow(e2,5)/16384)/4;
    double D=                               (35*pow(e2,3)/512 + 315*pow(e2,4)/2048 + 31185*pow(e2,5)/131072)/6;
    double E2=                                                  (315*pow(e2,4)/16384 + 3465*pow(e2,5)/65536)/8;
    double F=                                                                         (693*pow(e2,5)/131072)/10;

    double phil=Nl/(a*(1-e2)*A);

	double S=0;
	double oldPhil=0;
	double N1=Nl;

	//processo iterativo para aproximaÃ§Ã£o de phil
	while( (phil-oldPhil)>1e-20)
	{
		oldPhil=phil;
		S=a*(1-e2)*(A*oldPhil - B*sin(2*oldPhil) + C*sin(4*oldPhil) - D*sin(6*oldPhil) + E2*sin(8*oldPhil) - F*sin(10*oldPhil) );
		double N2=N1-(S*k0);
		phil=N2/N1*oldPhil+oldPhil;

	}

	double v=a/(sqrt(1-e2*pow(sin(phil),2)));

	//calculo das parcelas
	double VII=(tan(phil)*(1 + el2*pow(cos(phil),2))*1e12)/(2*pow(v,2)*sin1*pow(k0,2));
	double VIII=(tan(phil)*(5 + 3*pow(tan(phil),2) + 6*el2*pow(cos(phil),2) - 6*el2*pow(sin(phil),2) - 3*pow(el2,2)*pow(cos(phil),4) - 9*pow(el2,2)*pow(cos(phil)*sin(phil),2))*1e24)/(24*pow(v,4)*sin1*pow(k0,4));
	double IX=(1e6)/(v*sin1*k0*cos(phil));
	double X=((1 + 2*pow(tan(phil),2) + el2*pow(cos(phil),2))*1e18)/(6*pow(v,3)*sin1*pow(k0,3)*cos(phil));

	double E5=pow(q,5)*(5 + 28*pow(tan(phil),2) + 24*pow(tan(phil),4) + 6*el2*pow(cos(phil),2) + 8*el2*pow(sin(phil),2))*1e30/(120*pow(v,5)*sin1*pow(k0,5)*cos(phil));
      double D6=(pow(q,6)*tan(phil)*(61 + 90*pow(tan(phil),2) + 45*pow(tan(phil),4) + 107*el2*pow(cos(phil),2) - 162*el2*pow(sin(phil),2) - 45*el2*pow(tan(phil)*sin(phil),2))*1e36)/(720*pow(v,6)*sin1*pow(k0,6));

	//calculos finais

	double phi= phil + (-VII*pow(q,2) + VIII*pow(q,4) - D6)*M_PI/(180*3600);

	double dlambda = IX*q - X*pow(q,3) + E5;

	if (El>0)
		dlambda=-dlambda;

	double lambda=MC*M_PI/180+dlambda*M_PI/(180*3600);
*/
//teste com as equações da tabela
// uTmToGeo(double E, double N, double haltura, double MC, char hemi)

    //funcionando com a precisão dada

//wgs-84
	double a=6378137;
	double f=3.35281067183099e-3;
    double b=a*(1-f);
    //double e=sqrt(1-(b*b)/(a*a));
	double e2=(1-(b*b)/(a*a));	
	double el2=e2/(1-e2);
	double k0=0.9996;
	double n=(a-b)/(a+b);

    if (hemi=='s' || hemi=='S')
    {
		N=10000000-N;
    }
	double arc=N/k0;
    double mu=arc/(a*(1-e2/4-3*e2*e2/64-5*e2*e2*e2/256));

	double e1=(1-sqrt(1-e2))/(1+sqrt(1-e2));
    double c1=3*e1/2-27*pow(e1,3)/32;
    double c2=21*pow(e1,2)/16-55*pow(e1,4)/32;
    double c3=151*pow(e1,3)/96;
    double c4=1097*pow(e1,4)/512;

	double phi1=mu+c1*sin(2*mu)+c2*sin(4*mu)+c3*sin(6*mu)+c4*sin(8*mu);

	double El=500000-E;

	double q0=el2*pow(cos(phi1),2);
    double t0=pow(tan(phi1),2);
	double n0=a/sqrt(1-e2*pow(sin(phi1),2));
	double r0=a*(1-e2)/pow(1-e2*pow(sin(phi1),2),1.5);
	double d0=El/(n0*k0);

//    qDebug("arc= %.9g \nmu= %.9g \nphi1= %.9g \neisq= %.9g",arc,mu,phi1,el2);
//    qDebug("q0= %.9g \nt0= %.9g \nn0= %.9g \nr0= %.9g",q0,t0,n0,r0);

// coeficientes de latitude
	double fact1=n0*tan(phi1)/r0;
	double fact2=d0*d0/2;
	double fact3=(5+3*t0+10*q0-4*q0*q0-9*el2)*pow(d0,4)/24;
	double fact4=(61+90*t0+298*q0+45*t0*t0-252*el2-3*q0*q0)*pow(d0,6)/720;

	//0,592160605	0,000111165		1,2421e-8	1,48861e-12
//    qDebug("fact1= %.9g \nfact2= %.9g \nfact3= %.9g \nfact4= %.9g",fact1,fact2,fact3,fact4);


// coeficientes de longitude
	double lof1=d0;
	double lof2=(1+2*t0+q0)*pow(d0,3)/6;
	double lof3=(5-2*q0+28*t0-3*q0*q0+8*el2+24*t0*t0)*pow(d0,5)/120;
	
	//0,014910702	9,38912e-7		1,08451e-10
//    qDebug("lof1= %.9g \nlof2= %.9g \nlof3= %.9g",lof1,lof2,lof3);

	double dLong=((lof1-lof2+lof3)/cos(phi1))*180/M_PI;
	double lambda=(MC-dLong)*M_PI/180;  				//Longitude valor em radianos
	//double phi=180*(phi1-fact1*(fact2+fact3+fact4))/M_PI;
	double phi=phi1-fact1*(fact2+fact3+fact4);			//Latitude valor em radianos

	Matrix plh(1,3);

	Dms phiDms;
	Dms lambdaDms;
	phiDms.setSecondsPrecision(2);
	lambdaDms.setSecondsPrecision(2);

	phiDms.radianoToDms( phi );
	lambdaDms.radianoToDms(lambda);

	qDebug("E: %.9f\tN: %.9f\tH: %f",E,N,haltura);
	//qDebug("phi: %.9f\tlambda: %.9f\thaltura: %f\n",plh.get(1,1),plh.get(1,2),plh.get(1,3));
	qDebug("phi: %s\tlambda: %s\thaltura: %f\n",phiDms.toString(3).toStdString().c_str(),lambdaDms.toString(3).toStdString().c_str(),haltura);

	plh.set(1,1,phiDms.dmsToRadiano());
	plh.set(1,2,lambdaDms.dmsToRadiano());
	plh.set(1,3,haltura);

	return plh;
}
