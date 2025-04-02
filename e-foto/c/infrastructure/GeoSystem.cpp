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

#include "GeoSystem.h"

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
    if (sysName=="WGS84")
	{
        setSystem(WGS84);
	}
	else if (sysName=="SIRGAS2000")
	{
        setSystem(SIRGAS2000);
    }
	else if (sysName=="SAD69")
	{
        setSystem(SAD69);
	}
    else
	{
        setSystem(CORREGOALEGRE);
	}
    systemName=sysName;
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
        setF(1/298.247167427);
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


int GeoSystem::getEPSG(int fuse, bool hemisphere)
{
    if (fuse > 60 || fuse < 1)
        return 0;
    if (systemName.compare("WGS84") == 0)
    {
        if (hemisphere)
            return 32600 + fuse;
        else
            return 32700 + fuse;
    }

    if (systemName.compare("SIRGAS2000") == 0)
    {
        if (hemisphere)
        {
            if (fuse < 11 || fuse > 22)
                return 0;
            return 32954 + fuse;
        }
        else
        {
            if (fuse < 17 || fuse > 25)
                return 0;
            return 32960 + fuse;
        }
    }

    if (systemName.compare("SAD69") == 0)
    {
        if (hemisphere)
        {
            if (fuse < 18 || fuse > 22)
                return 0;
            return 29100 + fuse;
        }
        else
        {
            if (fuse < 17 || fuse > 25)
                return 0;
            return 29160 + fuse;
        }
    }

    if (systemName.compare("CORREGO ALEGRE") == 0)
    {
        if (!hemisphere)
        {
            if (fuse < 21 || fuse > 24)
                return 0;
            return 5515 + fuse;
        }
        return 0;
    }

    /*
    // WGS84
    32601	+proj=utm +zone=1 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32602	+proj=utm +zone=2 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32603	+proj=utm +zone=3 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32604	+proj=utm +zone=4 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32605	+proj=utm +zone=5 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32606	+proj=utm +zone=6 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32607	+proj=utm +zone=7 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32608	+proj=utm +zone=8 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32609	+proj=utm +zone=9 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32610	+proj=utm +zone=10 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32611	+proj=utm +zone=11 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32612	+proj=utm +zone=12 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32613	+proj=utm +zone=13 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32614	+proj=utm +zone=14 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32615	+proj=utm +zone=15 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32616	+proj=utm +zone=16 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32617	+proj=utm +zone=17 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32618	+proj=utm +zone=18 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32619	+proj=utm +zone=19 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32620	+proj=utm +zone=20 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32621	+proj=utm +zone=21 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32622	+proj=utm +zone=22 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32623	+proj=utm +zone=23 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32624	+proj=utm +zone=24 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32625	+proj=utm +zone=25 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32626	+proj=utm +zone=26 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32627	+proj=utm +zone=27 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32628	+proj=utm +zone=28 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32629	+proj=utm +zone=29 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32630	+proj=utm +zone=30 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32631	+proj=utm +zone=31 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32632	+proj=utm +zone=32 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32633	+proj=utm +zone=33 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32634	+proj=utm +zone=34 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32635	+proj=utm +zone=35 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32636	+proj=utm +zone=36 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32637	+proj=utm +zone=37 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32638	+proj=utm +zone=38 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32639	+proj=utm +zone=39 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32640	+proj=utm +zone=40 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32641	+proj=utm +zone=41 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32642	+proj=utm +zone=42 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32643	+proj=utm +zone=43 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32644	+proj=utm +zone=44 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32645	+proj=utm +zone=45 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32646	+proj=utm +zone=46 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32647	+proj=utm +zone=47 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32648	+proj=utm +zone=48 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32649	+proj=utm +zone=49 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32650	+proj=utm +zone=50 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32651	+proj=utm +zone=51 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32652	+proj=utm +zone=52 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32653	+proj=utm +zone=53 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32654	+proj=utm +zone=54 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32655	+proj=utm +zone=55 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32656	+proj=utm +zone=56 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32657	+proj=utm +zone=57 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32658	+proj=utm +zone=58 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32659	+proj=utm +zone=59 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32660	+proj=utm +zone=60 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32701	+proj=utm +zone=1 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32702	+proj=utm +zone=2 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32703	+proj=utm +zone=3 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32704	+proj=utm +zone=4 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32705	+proj=utm +zone=5 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32706	+proj=utm +zone=6 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32707	+proj=utm +zone=7 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32708	+proj=utm +zone=8 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32709	+proj=utm +zone=9 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32710	+proj=utm +zone=10 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32711	+proj=utm +zone=11 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32712	+proj=utm +zone=12 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32713	+proj=utm +zone=13 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32714	+proj=utm +zone=14 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32715	+proj=utm +zone=15 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32716	+proj=utm +zone=16 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32717	+proj=utm +zone=17 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32718	+proj=utm +zone=18 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32719	+proj=utm +zone=19 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32720	+proj=utm +zone=20 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32721	+proj=utm +zone=21 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32722	+proj=utm +zone=22 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32723	+proj=utm +zone=23 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32724	+proj=utm +zone=24 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32725	+proj=utm +zone=25 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32726	+proj=utm +zone=26 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32727	+proj=utm +zone=27 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32728	+proj=utm +zone=28 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32729	+proj=utm +zone=29 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32730	+proj=utm +zone=30 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32731	+proj=utm +zone=31 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32732	+proj=utm +zone=32 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32733	+proj=utm +zone=33 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32734	+proj=utm +zone=34 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32735	+proj=utm +zone=35 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32736	+proj=utm +zone=36 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32737	+proj=utm +zone=37 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32738	+proj=utm +zone=38 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32739	+proj=utm +zone=39 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32740	+proj=utm +zone=40 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32741	+proj=utm +zone=41 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32742	+proj=utm +zone=42 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32743	+proj=utm +zone=43 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32744	+proj=utm +zone=44 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32745	+proj=utm +zone=45 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32746	+proj=utm +zone=46 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32747	+proj=utm +zone=47 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32748	+proj=utm +zone=48 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32749	+proj=utm +zone=49 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32750	+proj=utm +zone=50 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32751	+proj=utm +zone=51 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32752	+proj=utm +zone=52 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32753	+proj=utm +zone=53 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32754	+proj=utm +zone=54 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32755	+proj=utm +zone=55 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32756	+proj=utm +zone=56 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32757	+proj=utm +zone=57 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32758	+proj=utm +zone=58 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32759	+proj=utm +zone=59 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs
    32760	+proj=utm +zone=60 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs

    // SIRGAS 2000
    31965	+proj=utm +zone=11 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31966	+proj=utm +zone=12 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31967	+proj=utm +zone=13 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31968	+proj=utm +zone=14 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31969	+proj=utm +zone=15 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31970	+proj=utm +zone=16 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31971	+proj=utm +zone=17 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31972	+proj=utm +zone=18 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31973	+proj=utm +zone=19 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31974	+proj=utm +zone=20 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31975	+proj=utm +zone=21 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31976	+proj=utm +zone=22 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31977	+proj=utm +zone=17 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31978	+proj=utm +zone=18 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31979	+proj=utm +zone=19 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31980	+proj=utm +zone=20 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31981	+proj=utm +zone=21 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31982	+proj=utm +zone=22 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31983	+proj=utm +zone=23 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31984	+proj=utm +zone=24 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs
    31985	+proj=utm +zone=25 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs

    // SAD69
    29118	+proj=utm +zone=18 +ellps=GRS67 +units=m +no_defs
    29119	+proj=utm +zone=19 +ellps=GRS67 +units=m +no_defs
    29120	+proj=utm +zone=20 +ellps=GRS67 +units=m +no_defs
    29121	+proj=utm +zone=21 +ellps=GRS67 +units=m +no_defs
    29122	+proj=utm +zone=22 +ellps=GRS67 +units=m +no_defs
    29177	+proj=utm +zone=17 +south +ellps=GRS67 +units=m +no_defs
    29178	+proj=utm +zone=18 +south +ellps=GRS67 +units=m +no_defs
    29179	+proj=utm +zone=19 +south +ellps=GRS67 +units=m +no_defs
    29180	+proj=utm +zone=20 +south +ellps=GRS67 +units=m +no_defs
    29181	+proj=utm +zone=21 +south +ellps=GRS67 +units=m +no_defs
    29182	+proj=utm +zone=22 +south +ellps=GRS67 +units=m +no_defs
    29183	+proj=utm +zone=23 +south +ellps=GRS67 +units=m +no_defs
    29184	+proj=utm +zone=24 +south +ellps=GRS67 +units=m +no_defs
    29185	+proj=utm +zone=25 +south +ellps=GRS67 +units=m +no_defs

    // CORREGO ALEGRE
    5536    +proj=utm +zone=21 +south +ellps=INTL +unitsm +no_defs
    5537    +proj=utm +zone=22 +south +ellps=INTL +unitsm +no_defs
    5538    +proj=utm +zone=23 +south +ellps=INTL +unitsm +no_defs
    5539    +proj=utm +zone=24 +south +ellps=INTL +unitsm +no_defs

    22521   Corrego Alegre 1970-72 / UTM zone 21S
    22522   Corrego Alegre 1970-72 / UTM zone 22S
    22523   Corrego Alegre 1970-72 / UTM zone 23S
    22524   Corrego Alegre 1970-72 / UTM zone 24S
    22525   Corrego Alegre 1970-72 / UTM zone 25S
    */

    return 0;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
