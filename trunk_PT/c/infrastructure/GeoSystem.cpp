#include "GeoSystem.h"

GeoSystem::GeoSystem(GeoSystemEnum system)
{
    switch (system)
    {
        case(CORREGOALEGRE):
        setEixoMenor(6378388.0);
        setFoco(1.0/297);
        setEixoMaior(6378388.0*(1-1.0/297));
        setSystemName("CORREGO ALEGRE");
        break;

        case(SAD69):
        setEixoMenor(6378160.0);
        setFoco(1/298.25);
        setEixoMaior(6378160.0*(1-1/298.25));
        setSystemName("SAD69");
        break;

        case(SIRGAS2000):
        setEixoMenor(6378137.0);
        setFoco(1/298.257222101);
        setEixoMaior(6378137.0*(1-1/298.257222101));
        setSystemName("SIRGAS2000");
        break;

        case(WGS84):
        setEixoMenor(6378137.0);
        setFoco(1/298.257223563);
        setEixoMaior(6378137.0*(1-1/298.257223563));
        setSystemName("WGS84");
        break;
    }

}

GeoSystem::GeoSystem(double a, double f, string newName)
{
    setEixoMenor(a);
    setFoco(f);
    setEixoMaior(a*(1-f));
    setSystemName(newName);
}

void GeoSystem::setEixoMenor(double newA)
{
    a=(newA>0.0 ? newA: 0.0);
}

void GeoSystem::setEixoMaior(double newB)
{
    b=(newB>0.0 ? newB: 0.0);
}

void GeoSystem::setFoco(double newF)
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
    setEixoMenor(a);
    setFoco(f);
    setEixoMaior(a*(1-f));
    setSystemName(newName);
}

void GeoSystem::setSystem(GeoSystemEnum system)
{
    switch (system)
    {
    case(CORREGOALEGRE):
        setEixoMenor(6378388.0);
        setFoco(1.0/297);
        setEixoMaior(6378388.0*(1-1.0/297));
        setSystemName("CORREGO ALEGRE");
        break;

    case(SAD69):
        setEixoMenor(6378160.0);
        setFoco(1/298.25);
        setEixoMaior(6378160.0*(1-1/298.25));
        setSystemName("SAD69");
        break;

    case(SIRGAS2000):
        setEixoMenor(6378137.0);
        setFoco(1/298.257222101);
        setEixoMaior(6378137.0*(1-1/298.257222101));
        setSystemName("SIRGAS2000");
        break;

    case(WGS84):
        setEixoMenor(6378137.0);
        setFoco(1/298.257223563);
        setEixoMaior(6378137.0*(1-1/298.257223563));
        setSystemName("WGS84");
        break;
    }
}
