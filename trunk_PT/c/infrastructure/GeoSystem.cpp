#include "GeoSystem.h"
#include <math.h>

GeoSystem::GeoSystem(GeoSystemEnum system)
{
    switch (system)
    {
        case(CORREGOALEGRE):
		setEixoMaior(6378388.0);
		setF(1.0/297);
		setEixoMenor(6378388.0*(1-1.0/297));
        setSystemName("CORREGO ALEGRE");
        break;

        case(SAD69):
		setEixoMaior(6378160.0);
		setF(1/298.25);
		setEixoMenor(6378160.0*(1-1/298.25));
        setSystemName("SAD69");
        break;

        case(SIRGAS2000):
		setEixoMaior(6378137.0);
		setF(1/298.257222101);
		setEixoMenor(6378137.0*(1-1/298.257222101));
        setSystemName("SIRGAS2000");
        break;

        case(WGS84):
		setEixoMaior(6378137.0);
		setF(1/298.257223563);
		setEixoMenor(6378137.0*(1-1/298.257223563));
        setSystemName("WGS84");
        break;
    }
}

GeoSystem::GeoSystem(string sysName)
{
	if (sysName=="CORREGOALEGRE")
	{
		setEixoMaior(6378388.0);
		setF(1.0/297);
		setEixoMenor(6378388.0*(1-1.0/297));
		setSystemName("CORREGO ALEGRE");
	}
	else if (sysName=="SAD69")
	{
		setEixoMaior(6378160.0);
		setF(1/298.25);
		setEixoMenor(6378160.0*(1-1/298.25));
		setSystemName("SAD69");
	}
	else if (sysName=="SIRGAS2000")
	{
		setEixoMaior(6378137.0);
		setF(1/298.257222101);
		setEixoMenor(6378137.0*(1-1/298.257222101));
		setSystemName("SIRGAS2000");
	}
	else if (sysName=="WGS84")
	{
		setEixoMaior(6378137.0);
		setF(1/298.257223563);
		setEixoMenor(6378137.0*(1-1/298.257223563));
		setSystemName("WGS84");
	}
	else
	{
		setEixoMaior(6378137.0);
		setF(1/298.257223563);
		setEixoMenor(6378137.0*(1-1/298.257223563));
		setSystemName("WGS84");
	}
}

GeoSystem::GeoSystem(double a, double f, string newName)
{
	setEixoMaior(a);
	setF(f);
	setEixoMenor(a*(1-f));
    setSystemName(newName);
}

void GeoSystem::setEixoMaior(double newA)
{
	a=(newA>0.0 ? newA: 0.0);
}

void GeoSystem::setEixoMenor(double newB)
{
	b=(newB>0.0 ? newB: 0.0);
}

void GeoSystem::setF(double newF)
{
    f=(newF>0.0 ? newF: 0.0);
}

void GeoSystem::setSystemName(string name)
{
    systemName=name;
}

double GeoSystem::getA()
{
    return a;
}

double GeoSystem::getB()
{
    return b;
}

double GeoSystem::getF()
{
    return f;
}

double GeoSystem::getFirstEccentricity()
{
    return sqrt(f*(2-f));
}

double GeoSystem::getSecondEccentricity()
{
    double e1=getFirstEccentricity();
    return e1*e1/(1-e1*e1);
}

string GeoSystem::getSystemName()
{
    return systemName;
}

string GeoSystem::toString()
{
    stringstream aux;
    aux << systemName <<":\ta = " << a << "\tb = "<< b << "\tf = "<< f <<"\n";
    return aux.str().c_str();
}

void GeoSystem::setSystem(double a, double f, string newName)
{
	setEixoMaior(a);
	setF(f);
	setEixoMenor(a*(1-f));
    setSystemName(newName);
}

void GeoSystem::setSystem(GeoSystemEnum system)
{
    switch (system)
    {
    case(CORREGOALEGRE):
		setEixoMaior(6378388.0);
		setF(1.0/297);
		setEixoMenor(6378388.0*(1-1.0/297));
        setSystemName("CORREGO ALEGRE");
        break;

    case(SAD69):
		setEixoMaior(6378160.0);
		setF(1/298.25);
		setEixoMenor(6378160.0*(1-1/298.25));
        setSystemName("SAD69");
        break;

    case(SIRGAS2000):
		setEixoMaior(6378137.0);
		setF(1/298.257222101);
		setEixoMenor(6378137.0*(1-1/298.257222101));
        setSystemName("SIRGAS2000");
        break;

    case(WGS84):
		setEixoMaior(6378137.0);
		setF(1/298.257223563);
		setEixoMenor(6378137.0*(1-1/298.257223563));
        setSystemName("WGS84");
        break;
    }
}
