#include "ConvertionsSystems.h"
//#include <QDebug>
#include "Dms.h"
#include <math.h>

#include <stdlib.h>


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/** Class to convertions
  */

Matrix ConvertionsSystems::GeoelipToGeocentricCartesian(double phi, double lambda, double haltura, GeoSystem system)
{

	double a=system.getA();
	double f=system.getF();

	double b=a*(1-f);

		//qDebug("phi= %.16f\tlambda= %.16f\th= %.16f",phi,lambda,haltura);

	double e2=1-(b*b)/(a*a);
	double Ni=a/(sqrt(1-e2*sin(phi)*sin(phi)));

	double Xi=(Ni+haltura)*cos(phi)*cos(lambda);
	double Yi=(Ni+haltura)*cos(phi)*sin(lambda);
	double Zi=(Ni*(1-e2)+haltura)*sin(phi);

	Matrix xyz(1,3);
	xyz.set(1,1,Xi);
	xyz.set(1,2,Yi);
	xyz.set(1,3,Zi);

		//qDebug("Xi: %f\tYi: %f\tZi: %f\n",xyz.get(1,1),xyz.get(1,2),xyz.get(1,3));
	return xyz;
}

Matrix ConvertionsSystems::GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi, GeoSystem system)
{
	double a=system.getA();
	//double f=system.getF();
	double b=system.getB();
	double e1=system.getFirstEccentricity();
	double e2=system.getSecondEccentricity();

	double tanu = Zi*a/(b*sqrt(Xi*Xi+Yi*Yi));
	double cosu = 1/sqrt(1+tanu*tanu);
	double senu = tanu/sqrt(1+tanu*tanu);

	double Si = sqrt(Xi*Xi+Yi*Yi);

	double convergency = (1e-18)*M_PI/180;

	double L2=atan( (Zi+e2*e2*b*pow(senu,3)) / (Si-e1*e1*a*pow(cosu,3)) );
	int cont=0;
	double Ni=0.0;
	double L1=0.0;

	while (fabs(L2-L1)>convergency && cont<50)
	{
		L1=L2;
		Ni=a/(sqrt(1-e1*e1*sin(L1)*sin(L1)));
		L2=atan((Zi+Ni*e1*e1*sin(L1))/Si);
		cont++;
	}

	double phi=L2;
	double lambda = atan(Yi/Xi);
	double N2=a/(sqrt(1-e1*e1*sin(phi)*sin(phi)));
	double haltura = Si/cos(phi) - N2;

	Matrix plh(1,3);
	plh.set(1,1,phi);
	plh.set(1,2,lambda);
	plh.set(1,3,haltura);
	//qDebug("Xi: %f\tYi: %f\tZi: %f",Xi,Yi,Zi);
	//qDebug("Phi: %.16f\tLambda: %.16f\th: %.16f\n",plh.get(1,1),plh.get(1,2),plh.get(1,3));

	return plh;
}

Matrix ConvertionsSystems::geoToUtm(double phi, double lambda, GeoSystem system)//, char hemi, char side)
{
	//qDebug("phi %.15f \t lambda %.15f",phi*180/M_PI,lambda*180/M_PI);



	int hemisferio=1;
	if (phi<0)
		hemisferio=-1;
	double a=system.getA();
	//double f=system.getF();
	double b=system.getB();
	//double e=sqrt(1-(b*b)/(a*a));
	double e=system.getFirstEccentricity();
	double e1=e*a/b;
	double k0=0.9996;
	double n=(a-b)/(a+b);

	double v=a/sqrt(1-pow(e*sin(phi),2));


	//qDebug("e %.15f \t e1 %.15f \t v %.15f",e,e1,v);

	int zona;

	//Correção do Francisco 15/09/2011
	double lon=lambda*180/M_PI;
	if (lon>=180)
			lon=179;
	zona=(int)(fabs(31+lon/6)+0.1);//+0.1: Idéia do rafael para contornar erro de arredondamento.

	double A=a*(1 - n + (5*n*n/4)*(1-n)+(81*pow(n,4)/64)*(1-n));
	double B=(3*a*n/2)*(1-n+(7*n*n/8)*(1-n)+55*pow(n,4)/64);
	double C=(15*a*n*n/16)*(1-n+(3*n*n/4)*(1-n));
	double D=(35*a*pow(n,3)/48)*(1 - n + 11*n*n/16);
	double E2=(315*a*pow(n,4)/512)*(1-n);

	//qDebug("A= %.14f \nB= %.14f \nC= %.14f \nD= %.14f \nE2= %.14f",A,B,C,D,E2);


	double S=A*phi - B*sin(2*phi) + C*sin(4*phi) - D*sin(6*phi) + E2*sin(8*phi);


	//qDebug("%d",inteiro);
	int lambda0=6*zona-183;

	double dLambda=lambda-lambda0*M_PI/180;

	double T1 = S*k0;								// Ko=0.9996
	double T2 = v*k0*sin(phi)*cos(phi)/2;
	double T3 = v*k0*sin(phi)*pow(cos(phi),3)*(5 - pow(tan(phi),2) + 9*pow(e1*cos(phi),2) + 4*pow(e1*cos(phi),4) )/24;
	double T4 = v*k0*sin(phi)*pow(cos(phi),5)*(61 - 58*pow(tan(phi),2) + pow(tan(phi),4) + 270*pow(e1*cos(phi),2) - 330*pow(tan(phi)*e1*cos(phi),2) + 445*pow(e1*cos(phi),4) + 324*pow(e1*cos(phi),6) - 680*tan(phi)*tan(phi)*pow(e1*cos(phi),4) + 88*pow(e1*cos(phi),8) - 600*tan(phi)*tan(phi)*pow(e1*cos(phi),6) - 192*tan(phi)*tan(phi)*pow(e1*cos(phi),8) )/720;
	double T5 = v*k0*pow(cos(phi),7)*(1385 - 3111*pow(tan(phi),2) + 543*pow(tan(phi),4) - pow(tan(phi),6))/40320;
	double T6 = v*cos(phi)*k0;
	double T7 = (v*k0*pow(cos(phi),3)*(1 - pow(tan(phi),2) + pow(e1*cos(phi),2) ))/6;
	double T8 = v*k0*pow(cos(phi),5)*(5-18*pow(tan(phi),2) + pow(tan(phi),4) + 14*pow(e1*cos(phi),2) - 58*pow(tan(phi)*e1*cos(phi),2) + 13*pow(e1*cos(phi),4) + 4*pow(e1*cos(phi),6) - 64*tan(phi)*tan(phi)*pow(e1*cos(phi),4) - 24*tan(phi)*tan(phi)*pow(e1*cos(phi),6) )/120;
	double T9 = v*k0*pow(cos(phi),7)*(61 - 479*pow(tan(phi),2) + 179*pow(tan(phi),4) - pow(tan(phi),6))/5040;

  //qDebug("S= %.14f \nv= %.14f \np= %.14f",S,v,dLambda);
  //qDebug("T1= %.14f \nT2= %.14f \nT3= %.14f ",T1,T2,T3);
   // double A6=pow(M_PI/(3600*180)*dLambda,6)*(nu*sin(phi)*pow(cos(phi),5))*(61 - 58*pow(tan(phi),2) + pow(tan(phi),4) + 270*el2*pow(cos(phi),2) - 330*el2*pow(sin(phi),2) )*k0*1e24/720;

	double N=10000000 + T1 + T2*pow(dLambda,2) + T3*pow(dLambda,4) + T4*pow(dLambda,6) + T5*pow(dLambda,8);
	double E= 500000  + T6*dLambda + T7*pow(dLambda,3) + T8*pow(dLambda,5) + T9*pow(dLambda,7);
	//if (N<0)
	  //  N=10000000+N;

	Matrix xyh(1,4);

	xyh.set(1,1,E);
	xyh.set(1,2,N);
	xyh.setInt(1,3,zona);
	xyh.setInt(1,4,hemisferio);

	//qDebug("phi: %.9f\tlambda: %.9f\thaltura: %f",phi,lambda,haltura);
	//qDebug("phi: %s\tlambda: %s\thaltura: %f",oneSecond.radianoToDms(phi)->toString().toStdString().c_str(),oneSecond.radianoToDms(lambda)->toString().toStdString().c_str(),haltura);

   // qDebug("Phi: %.9f\tLambda: %.9f\t phi: %.9f lambda: %.9f\n",phi*180/M_PI,lambda*180/M_PI,phi,lambda);
   // qDebug("E: %.9f\tN: %.9f\t zona: %d hemi: %d\n",xyh.get(1,1),xyh.get(1,2),xyh.get(1,3),xyh.get(1,4));

   return xyh;
}

Matrix ConvertionsSystems::utmToGeo(double E, double N,int zona, int hemi, GeoSystem system)
{
   // qDebug("E: %.9f\tN: %.9f\t zona: %d hemi: %d\n",E,N,zona,hemi);
	//teste com as equações da tabela
	//funcionando com a precisão dada

	if (hemi < 0)
	{
		N=10000000-N;
		//qDebug("sul hemi %c\tN= %f",hemi,N);
	}
	int MC = 6*zona-183;

	//qDebug("zona %d \tMC %d",zona,MC);
	double E1=500000-E;

	double a=system.getA();
	double e=system.getFirstEccentricity();

	double k0=0.9996;

	double M=N/k0;
	double mu=M/(a*(1-pow(e,2)/4-3*pow(e,4)/64-5*pow(e,6)/256));

	double e1=(1-sqrt(1-e*e))/(1+sqrt(1-e*e));
	double J1=3*e1/2-27*pow(e1,3)/32;
	double J2=21*pow(e1,2)/16-55*pow(e1,4)/32;
	double J3=151*pow(e1,3)/96;
	double J4=1097*pow(e1,4)/512;
   // qDebug("e1= %.9f \n J1= %.9f \nJ2= %.9g \nJ3= %.9g \nJ4= %.9g",e1,J1,J2,J3,J4);

	//double convergency = (1e-10)*M_PI/180;

	double fp=mu+J1*sin(2*mu)+J2*sin(4*mu)+J3*sin(6*mu)+J4*sin(8*mu);

	double C1,T1,R1,N1,D,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Long,Lat;
	double dLong=1.0;

	double e12=e*e/(1-e*e);
	//while( fabs(dLong)>convergency && cont < 5000)
	//fp=fp2;
	C1=e12*pow(cos(fp),2);
	T1=pow(tan(fp),2);
	//T1=32.251831742;
	R1=a*(1-e*e)/pow(1-pow(e*sin(fp),2),1.5);
	N1=a/sqrt(1-pow(e*sin(fp),2));
	D=E1/(N1*k0);
	Q1=N1*tan(fp)/R1;
	Q2=D*D/2;
	Q3=(5+3*T1+10*C1-4*C1*C1-9*e12)*pow(D,4)/24;
	Q4=(61+90*T1+298*C1+45*T1*T1-3*C1*C1-252*e12)*pow(D,6)/720;
	Q5=D;
	Q6=(1+2*T1+C1)*pow(D,3)/6;
	Q7=(5-2*C1+28*T1-3*C1*C1+8*e12+24*T1*T1)*pow(D,5)/120;

	Lat=fp-Q1*(Q2-Q3+Q4);
	dLong=(Q5-Q6+Q7)/cos(fp);
	Long=MC*M_PI/180-dLong;  				//Longitude valor em radianos
	if (hemi < 0)
		Lat = -Lat;
	//qDebug("Lat[1,1]: %.10f Long[1,2]: %.10f",Lat*180/M_PI,Long*180/M_PI);

	Matrix plh(1,2);
	plh.set(1,1,Lat);
	plh.set(1,2,Long);

	//qDebug("E: %.9f\tN: %.9f\tH: %f",E,N,haltura);
	//qDebug("phi: %.9f\tlambda: %.9f\thaltura: %f\n",plh.get(1,1),plh.get(1,2),plh.get(1,3));
   // qDebug("\n\nM= %.9f \nmu= %.9f \nfp= %.9f \ne12= %.9g",M,mu,fp,e12);
   // qDebug("C1= %.9f \nT1= %.9f \nR1= %.9f \nN1= %.9f",C1,T1,R1,N1);

   // qDebug("Q1= %.9f \nQ2= %.9f \nQ3= %.9g \nQ4= %.9g",Q1,Q2,Q3,Q4);
   // qDebug("Q5= %.9f \nQ6= %.9g \nQ7= %.9g",Q5,Q6,Q7);


	//qDebug("Phi: %.9f\tLambda: %.9f\t phi: %.9f lambda: %.9f\n",Lat*180/M_PI,Long*180/M_PI,Lat,Long);


	return plh;

}

Matrix ConvertionsSystems::convertSystems3Parameters(GeoSystem oldSystem, GeoSystem newSystem, Matrix coordinates)
{
	Matrix aux=GeoelipToGeocentricCartesian(coordinates.get(1,1),coordinates.get(1,2),coordinates.get(1,3),oldSystem);
	if (oldSystem.getSystemName()=="SAD69")
	{
		if(newSystem.getSystemName()=="WGS84")
		{
			aux.set(1,1,aux.get(1,1)-66.87);
			aux.set(1,2,aux.get(1,2)+4.37);
			aux.set(1,3,aux.get(1,3)-38.52);
		}
		if(newSystem.getSystemName()=="CORREGO ALEGRE")
		{
			aux.set(1,1,aux.get(1,1)+138.70);
			aux.set(1,2,aux.get(1,2)-164.40);
			aux.set(1,3,aux.get(1,3)-34.40);
		}
		if(newSystem.getSystemName()=="SIRGAS2000")
		{
			aux.set(1,1,aux.get(1,1)-67.348);
			aux.set(1,2,aux.get(1,2)+3.879);
			aux.set(1,3,aux.get(1,3)-38.223);
		}
	}
	else if (oldSystem.getSystemName()=="SIRGAS2000")
	{
		if(newSystem.getSystemName()=="WGS84")
		{
			aux.set(1,1,aux.get(1,1)+0.478);
			aux.set(1,2,aux.get(1,2)+0.491);
			aux.set(1,3,aux.get(1,3)-0.297);
		}
		if(newSystem.getSystemName()=="CORREGO ALEGRE")
		{
			aux.set(1,1,aux.get(1,1)+206.048);
			aux.set(1,2,aux.get(1,2)-168.279);
			aux.set(1,3,aux.get(1,3)+3.823);
		}
		if(newSystem.getSystemName()=="SAD69")
		{
			aux.set(1,1,aux.get(1,1)+67.348);
			aux.set(1,2,aux.get(1,2)-3.879);
			aux.set(1,3,aux.get(1,3)+38.223);
		}
	}
	else if (oldSystem.getSystemName()=="CORREGO ALEGRE")
	{
		if(newSystem.getSystemName()=="WGS84")
		{
			aux.set(1,1,aux.get(1,1)-205.57);
			aux.set(1,2,aux.get(1,2)+168.77);
			aux.set(1,3,aux.get(1,3)-4.12);

		}
		if(newSystem.getSystemName()=="SIRGAS2000")
		{
			aux.set(1,1,aux.get(1,1)-206.048);
			aux.set(1,2,aux.get(1,2)+168.279);
			aux.set(1,3,aux.get(1,3)-3.823);
		}
		if(newSystem.getSystemName()=="SAD69")
		{
			aux.set(1,1,aux.get(1,1)-138.7);
			aux.set(1,2,aux.get(1,2)+164.40);
			aux.set(1,3,aux.get(1,3)-34.40);
		}
	}
	else if (oldSystem.getSystemName()=="WGS84")
	{
		if(newSystem.getSystemName()=="SIRGAS2000")
		{
			aux.set(1,1,aux.get(1,1)-0.478);
			aux.set(1,2,aux.get(1,2)-0.491);
			aux.set(1,3,aux.get(1,3)+0.297);
		}
		if(newSystem.getSystemName()=="CORREGO ALEGRE")
		{
			aux.set(1,1,aux.get(1,1)+205.57);
			aux.set(1,2,aux.get(1,2)-168.77);
			aux.set(1,3,aux.get(1,3)-72.623);
		}
		if(newSystem.getSystemName()=="SAD69")
		{
			aux.set(1,1,aux.get(1,1)+66.87);
			aux.set(1,2,aux.get(1,2)-4.37);
			aux.set(1,3,aux.get(1,3)+38.52);
		}
	}
	return GeocentricCartesianToGeoelip(aux.get(1,1),aux.get(1,2),aux.get(1,3),newSystem);
}

Matrix ConvertionsSystems::convertSystemsSimplifiedMolodensky(GeoSystem oldSystem, GeoSystem newSystem, double phi, double lambda, double haltura)
{
	//u sistema old; v sistema new
	Matrix parameters=transformParameters(oldSystem,newSystem);
	double dX=parameters.get(1,1);
	double dY=parameters.get(1,2);
	double dZ=parameters.get(1,3);

	/* wgs84 sad69*/
		double f0=oldSystem.getF();
		double f1=newSystem.getF();

		double a0=oldSystem.getA();
		double a1=newSystem.getA();

		double e=f0*(2-f0);                             //e = 0.006694379990141
		double df=f1-f0;                                //df= 0.000000081204000
		double da=a1-a0;                                //da= 23.000
		double M=a0*(1-e)/pow(1-e*pow(sin(phi),2),1.5); //M = 6351714.150933220000000
		double N=a0/sqrt(1-e*pow(sin(phi),2));          //N = 6383593.832647510000000

		// formulas simplificadas

	double p1=(a0*df+f0*da)*sin(2*phi);
	double p2=-(dX*sin(phi)*cos(lambda));
	double p3=-(dY*sin(phi)*sin(lambda));
	double p4=dZ*cos(phi);
	double dphi=(p1+p2+p3+p4)/M;                        //dphi=0.000008053694158

	double dlambda=(1/(N*cos(phi)))*(-dX*sin(lambda)+dY*cos(lambda)); //dlambda=0.000010203976210

	double n1= (a0*df+f0*da)*pow(sin(phi),2);
	double n2= -da+dX*cos(phi)*cos(lambda);
	double n3= dY*cos(phi)*sin(lambda)+dZ*sin(phi);
	double dN= n1+n2+n3;                                //dN=-10.828195622534700

	Matrix result(1,3);
	result.set(1,1,phi+dphi);
	result.set(1,2,lambda+dlambda);
	result.set(1,3,haltura+dN);

	//qDebug("phi=%.16f\t lambda=%.16f h=%.16f",phi,lambda,haltura); //
	//qDebug("dX= %.6f\tdY= %.6f\tdZ= %.6f",dX,dY,dZ);
   // qDebug("e= %.16f\ndf= %.16f\nda= %.16f\nM= %.16f\nN= %.16f",e,df,da,M,N);
   // qDebug("dphi= %.16f\ndlambda= %.16f\ndN= %.16f",dphi,dlambda,dN);
	//qDebug("converted\nphi=%.16f\t lambda=%.16f h=%.16f",result[0],result[1],result[2]);
	return result;
}

Matrix ConvertionsSystems::transformParameters(GeoSystem oldSystem, GeoSystem newSystem)
{
	Matrix para(1,3);

	if (oldSystem.getSystemName()=="SAD69")
	{
		if(newSystem.getSystemName()=="WGS84")
		{
			para.set(1,1,-66.87);
			para.set(1,2,4.37);
			para.set(1,3,-38.52);
		}
		if(newSystem.getSystemName()=="CORREGO ALEGRE")
		{
			para.set(1,1,138.70);
			para.set(1,2,-164.40);
			para.set(1,3,-34.40);
		}
		if(newSystem.getSystemName()=="SIRGAS2000")
		{
			para.set(1,1,-67.348);
			para.set(1,2,3.879);
			para.set(1,3,-38.223);
		}
	}
	else if (oldSystem.getSystemName()=="SIRGAS2000")
	{
		if(newSystem.getSystemName()=="WGS84")
		{
			para.set(1,1,0.478);
			para.set(1,2,0.491);
			para.set(1,3,-0.297);
		}
		if(newSystem.getSystemName()=="CORREGO ALEGRE")
		{
			para.set(1,1,206.048);
			para.set(1,2,-168.279);
			para.set(1,3,3.823);
		}
		if(newSystem.getSystemName()=="SAD69")
		{
			para.set(1,1,67.348);
			para.set(1,2,-3.879);
			para.set(1,3,38.223);
		}
	}
	else if (oldSystem.getSystemName()=="CORREGO ALEGRE")
	{
		if(newSystem.getSystemName()=="WGS84")
		{
			para.set(1,1,-205.57);
			para.set(1,2,168.77);
			para.set(1,3,-4.12);
		}
		if(newSystem.getSystemName()=="SIRGAS2000")
		{
			para.set(1,1,-206.048);
			para.set(1,2,168.279);
			para.set(1,3,-3.823);
		}
		if(newSystem.getSystemName()=="SAD69")
		{
			para.set(1,1,-138.7);
			para.set(1,2,164.40);
			para.set(1,3,34.40);
		}
	}
	else if (oldSystem.getSystemName()=="WGS84")
	{
		if(newSystem.getSystemName()=="SIRGAS2000")
		{
			para.set(1,1,-0.478);
			para.set(1,2,-0.491);
			para.set(1,3,0.297);
		}
		if(newSystem.getSystemName()=="CORREGO ALEGRE")
		{
			para.set(1,1,205.57);
			para.set(1,2,-168.77);
			para.set(1,3,4.12);
		}
		if(newSystem.getSystemName()=="SAD69")
		{
			para.set(1,1,66.87);
			para.set(1,2,-4.37);
			para.set(1,3,38.52);
		}
	}
	return para;
}

#include <stdlib.h>

Matrix ConvertionsSystems::utmToGeocentrica(double E, double N,double H, int zona, char hemi, GeoSystem system)
{

	E=truncate(E,3);
	N=truncate(N,3);

	//qDebug("interno E = %.10f N = %.10f",E,N);
	Matrix plh=utmToGeo(E,N,zona,hemi,system);
	//plh.show('f',4,"UTM --> Geodesica");

	double phi=plh.get(1,1);
	double lambda=plh.get(1,2);
	//double haltura= plh.get(1,3);
	Matrix result=GeoelipToGeocentricCartesian(phi,lambda,H,system);
	//result.show('f',4,"Geodesica --> Geocentrica");
	return result;
}

Matrix ConvertionsSystems::GeocentricaToUtm(double X, double Y, double Z, GeoSystem system)
{
	Matrix plh=GeocentricCartesianToGeoelip(X,Y,Z,system);
	double phi=plh.get(1,1);
	double lambda=plh.get(1,2);
	double altitude=plh.get(1,3);
	Matrix result(1,5);

	//plh.show('f',4,"GeoCentrica --> Geodesica");

	Matrix temp=geoToUtm(phi,lambda,system);
	result.set(1,1,truncate(temp.get(1,1),3));//E
	result.set(1,2,truncate(temp.get(1,2),3));//N

	//result.set(1,1,temp.get(1,1));//E
	//result.set(1,2,temp.get(1,2));//N

	result.set(1,3,altitude);    //H
	result.set(1,4,temp.get(1,3));//zona
	result.set(1,5,temp.get(1,4));//hemisferio

	//result.show('f',4,"Geodesica - > UTM");
	return result;
}

double ConvertionsSystems::truncate(double value, int decimal)
{
	//para resolver problemas de arrendondamento
	value+=pow(10,-decimal-3);

	double frac1 = (value-floor(value))*pow(10,decimal);
	frac1=(frac1-floor(frac1))/pow(10,decimal);
	return value-frac1;
}


Matrix ConvertionsSystems::utmToNunes(double E, double N, double H, int zona, int hemi, double phi0, double lambda0, GeoSystem system)
{
	//H+=-5.85;
	double R0=getNunesRaio(phi0,lambda0,system);
	Matrix temp=utmToGeo(E,N,zona,hemi,system);

	double phi = temp.get(1,1);
	double lambda = temp.get(1,2);

	double R=getNunesRaio(phi,lambda,system);
	double beta = (phi - phi0);///R;
	double alpha = (lambda - lambda0);///R;

	Matrix result(1,3);

	double X=(R+H)*cos(beta)*sin(alpha);
	double Y=(R+H)*sin(beta);
	double Z=(R+H)*cos(beta)*cos(alpha)-R;

	result.set(1,1,X);
	result.set(1,2,Y);
	result.set(1,3,Z);

	return result;
}

Matrix ConvertionsSystems::nunesToUtm(double X, double Y, double Z, double phi0, double lambda0, GeoSystem system)
{
	double R = getNunesRaio(phi0,lambda0,system);

	double alpha = atan( X/(R+Z) );
	double beta  = atan( Y*cos(alpha)/(R+Z) );
	/*
	double lambda = R*alpha + lambda0;
	double phi =  R*beta + phi0;
	*/
	double lambda = alpha + lambda0;
	double phi =  beta + phi0;
	double H = (R+Z)/(cos(alpha)*cos(beta)) - R;


	Matrix temp=geoToUtm(phi,lambda,system);
	Matrix result(1,3);

	result.set(1,1,temp.get(1,1));
	result.set(1,2,temp.get(1,2));
	result.set(1,3,H);//+5.85);

	return result;
}

//E o Lambda0 nao serve para nada??
double ConvertionsSystems::getNunesRaio(double phi0, double lambda0, GeoSystem sys)
{
	double a=sys.getA();
	double e=sys.getFirstEccentricity();

	double M=a*(1-e*e)/(pow(1-pow(e*sin(phi0),2),1.5));
	double N=a/(sqrt(1-pow(e*sin(phi0),2)));

	return sqrt(M*N);

}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
