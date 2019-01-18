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

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "GeoSystem.h"
#include <cmath>

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

GeoSystem::GeoSystem(DatumType system)
{
    setSystem(system);
}

GeoSystem::GeoSystem(std::string sysName)
{
	if (sysName=="CORREGOALEGRE")
	{
        setSystem(CORREGOALEGRE);
	}
	else if (sysName=="SAD69")
	{
        setSystem(SAD69);
	}
	else if (sysName=="SIRGAS2000")
	{
        setSystem(SIRGAS2000);
	}
    else
	{
        setSystem(WGS84);
	}
}

/*GeoSystem::GeoSystem(double a, double f, std::string newName)
{
	setEixoMaior(a);
	setF(f);
	setEixoMenor(a*(1-f));
	setSystemName(newName);
}
*/
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

void GeoSystem::setSystemName(std::string name)
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

std::string GeoSystem::getSystemName()
{
	return systemName;
}

std::string GeoSystem::toString()
{
    std::stringstream aux;
	aux << systemName <<":\ta = " << a << "\tb = "<< b << "\tf = "<< f <<"\n";
	return aux.str().c_str();
}

void GeoSystem::setSystem(double a, double f, std::string newName)
{
	setEixoMaior(a);
	setF(f);
	setEixoMenor(a*(1-f));
	setSystemName(newName);
}

void GeoSystem::setSystem(DatumType system)
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

    default:
		setEixoMaior(6378137.0);
		setF(1/298.257223563);
		setEixoMenor(6378137.0*(1-1/298.257223563));
		setSystemName("WGS84");
	}
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
