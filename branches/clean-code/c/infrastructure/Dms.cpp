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

#include "Dms.h"

#include "EDom.h"

#include <stdlib.h>

#include <QDebug>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

Dms::Dms():Dms(0, 0, 0.0)
{
}

Dms::Dms(int degree, int minute, double second, bool signal)
{
    setSignal(signal);
    if (degree<0)
    {
        setSignal(true);
        degree=-degree;
    }
    setSecondsPrecision();
    //setDegree(degree);
    degree_ = (degree<0||degree>359)? abs(degree_) : abs(degree) ;
    setMinute(minute);
    setSeconds(second);
}

Dms::Dms(std::string dms)
{
    Dms *newDegree = stringToDms(dms);
    setSecondsPrecision();
    this->setDegree(newDegree->getDegree());
    this->setMinute(newDegree->getMinute());
    this->setSeconds(newDegree->getSeconds());
    this->setSignal(newDegree->hasSignal());
}

Dms::Dms(double seconds)
{
    setSecondsPrecision();
    double value=fabs(seconds);

    int deg=(int)value/3600;
    double resto1=fmod(value,3600);
    int min=(int)resto1/60;
    double sec=fmod(resto1,60);

    if (seconds<0)
        setDms(deg,min,sec,true);
    else
        setDms(deg,min,sec);

}

void Dms::setDegree(int newDegrees)
{
        degree_ = (newDegrees<0||newDegrees>359)? abs(degree_) : abs(newDegrees) ;
}

void Dms::setMinute(int newMinutes)
{
    min_ = (newMinutes<0||newMinutes>59)? min_ : newMinutes ;
}

void Dms::setSeconds(double newSeconds)
{
    sec_ = (newSeconds<0.0 || newSeconds>60.0)? sec_ : newSeconds;
}

void Dms::setSignal(bool newSignal)
{
     signal_=newSignal;
}

int Dms::getDegree() const
{
    return degree_;
}

int Dms::getMinute() const
{
    return min_;
}

double Dms::getSeconds() const
{
    return sec_;
}

bool Dms::isValid() const
{
    if( (this->degree_>=0 && this->degree_<360) && (this->min_>=0 && this->min_<60) && (this->sec_>=0.0 && this->sec_<60.0) )
        return true;
    else
        return false;
}

bool Dms::hasSignal() const
{
    return signal_;
}

void Dms::setDms(Dms newDms)
{
    this->setDegree(newDms.getDegree());
    this->setMinute(newDms.getMinute());
    this->setSeconds(newDms.getSeconds());
    this->setSignal(newDms.hasSignal());
}

void Dms::setDms(int degree, int minute, double seconds,bool signal)
{
    this->setDegree(degree);
    this->setMinute(minute);
    this->setSeconds(seconds);
    this->setSignal(signal);
}

std::string Dms::toString(int decimals)
{
    std::string text;

    if(this->hasSignal())
        text+="-";

    text+=Conversion::intToString(this->getDegree());
    text+=" ";
    text+=Conversion::intToString(this->getMinute());
    text+=" ";
    text+=Conversion::doubleToString(this->getSeconds(),decimals);
    return text;
}

Dms* Dms::stringToDms(std::string dms)
{
    std::string aux=dms;

    int pos=aux.find_first_of(" ");
    int degree = Conversion::stringToInt(aux.substr(0,pos));
    aux=aux.substr(pos+1);// dms without Degrees

    pos = aux.find_first_of(" ");
    int minute = Conversion::stringToInt(aux.substr(0,pos));
    aux=aux.substr(pos+1);// dms without Degrees and Minutes

    pos = aux.find_first_of(" ");
    double second= Conversion::stringToDouble(aux.substr(0,pos));

    if(dms[0]=='-')
    {
        degree=-degree;
        setSignal(true);
    }
    else
    {
        setSignal(false);
    }

    setDegree(degree);
    setMinute(minute);
    setSeconds(second);

    return this;
}


void Dms::setSecondsPrecision(int precision)
{
    secondsPrecision_ = (precision>=1 ? precision : 2);
}

/**
* Convertions
*/
double Dms::dmsToDegreeDecimal()
{
    double minPartial=this->getMinute()+this->getSeconds()/60.0;
    double result=this->getDegree()+minPartial/60.0;

    return (this->hasSignal() ? -result:result );
}

double Dms::dmsToRadiano()
{
    double parcial=dmsToDegreeDecimal();
    return degreeDecimalToRadiano(parcial);
}

Dms* Dms::degreeDecimalToDms(double degreeDecimal)
{
    int degrees=0;
    int minutes=0;
    double seconds=0.0;

    if(degreeDecimal>=0)
    {
        degrees = degreeDecimal;
        minutes = (degreeDecimal-degrees)*60.0;
        seconds = (double)((degreeDecimal-degrees)*60.0-minutes)*60.0;
        setSignal(false);
    }else
    {
        degreeDecimal=-degreeDecimal;
        degrees = degreeDecimal;
        minutes = (degreeDecimal-degrees)*60.0;
        seconds = (double)((degreeDecimal-degrees)*60.0-minutes)*60.0;
        setSignal(true);
    }
    setDegree(degrees);
    setMinute(minutes);
    setSeconds(seconds);

    return this;
}

double Dms::degreeDecimalToRadiano(double oldDegree)
{
    double k=(oldDegree*M_PI)/180;
    return k;
}

Dms* Dms::radianoToDms(double radiano)
{
    return degreeDecimalToDms(radianoToDegreeDecimal(radiano));
}

double Dms::radianoToDegreeDecimal(double radiano)
{
    return (radiano/M_PI)*180;
}

/**
 *  If calling object is bigger than degMinSec then function returns 1
 *  If calling object is smaller than degMinSec then function returns -1
 *  If calling object is equall degMinSec then function returns 0
 */
int Dms::compareDegMinSecs(Dms *degMinSec)
{
    if (this->hasSignal() && !degMinSec->hasSignal())
    {
        return -1;
    }else if (!this->hasSignal() && degMinSec->hasSignal())
    {
        return 1;
    }else if (!this->hasSignal() && !degMinSec->hasSignal())
    {
        if(this->getDegree()>degMinSec->getDegree())
        {
            return 1;
        }
        else if (this->getDegree()<degMinSec->getDegree())
        {
            return -1;
        }
        else
        {
            if(this->getMinute()>degMinSec->getMinute())
            {
                return 1;
            }
            else if(this->getMinute()<degMinSec->getMinute())
            {
                return -1;
            }
            else
            {
                if(this->getSeconds()>degMinSec->getSeconds())
                {
                    return 1;
                }
                else if (this->getSeconds()<degMinSec->getSeconds())
                {
                    return -1;
                }
                else
                    return 0;
            }
        }
    }else if(this->hasSignal() && degMinSec->hasSignal())
    {
        if(this->getDegree()>degMinSec->getDegree())
        {
            return -1;
        }
        else if (this->getDegree()<degMinSec->getDegree())
        {
            return 1;
        }

        else
        {
            if(this->getMinute()>degMinSec->getMinute())
            {
                return -1;
            }
            else if(this->getMinute()<degMinSec->getMinute())
            {
                return 1;
            }
            else
            {
                if(this->getSeconds()>degMinSec->getSeconds())
                {
                    return -1;
                }
                else if (this->getSeconds()<degMinSec->getSeconds())
                {
                    return +1;
                }
                else
                    return 0;
            }
        }
    }
    return 0;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
