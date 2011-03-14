#include "Dms.h"
#include "QtGui"
#include <QAbstractSpinBox>
#include <math.h>

/**
  *Class to manipulate rad degree and degree decimal
  */

Dms::Dms()
{
	Dms(0, 0, 0.0);
}

Dms::Dms(int degree, int minute, double second, bool signal)
{
    setDegree(degree);
    setMinute(minute);
    setSeconds(second);
    setSignal(signal);
}

Dms::Dms(QString degree)
{
    Dms *newDegree = stringToDms(degree);

    this->setDegree(newDegree->getDegree());
    this->setMinute(newDegree->getMinute());
    this->setSeconds(newDegree->getSeconds());
    this->setSignal(newDegree->hasSignal());
}
Dms::Dms(double seconds)
{
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
	degree = (newDegrees<0||newDegrees>359)? abs(degree) : abs(newDegrees) ;
}

void Dms::setMinute(int newMinutes)
{
    min = (newMinutes<0||newMinutes>59)? min : newMinutes ;
}

void Dms::setSeconds(double newSeconds)
{
	sec = (newSeconds<0.0 || newSeconds>=60.0)? sec : newSeconds;
}

void Dms::setSignal(bool newSignal)
{
     signal=newSignal;
}

int Dms::getDegree()
{
    return degree;
}

int Dms::getMinute()
{
    return min;
}

double Dms::getSeconds()
{
    return sec;
}

bool Dms::isValid()
{
	if( (this->degree>=0 && this->degree<360) && (this->min>=0 && this->min<60) && (this->sec>=0.0 && this->sec<60.0) )
		return true;
	else
		return false;
}

bool Dms::hasSignal()
{
	return signal;
}

Dms* Dms::getDms()
{
    if (this->isValid())
        return this;
    else
        return 0;
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

QString Dms::toString(int decimals)
{
    QString text;
	if(this->hasSignal())
        text+="-";

    text+=QString::number(this->getDegree());
    text+=QString::fromUtf8("°");
    text+=QString::number(this->getMinute());
    text+="'";
	text.append(QLocale::system().toString(this->getSeconds(),'f',decimals));
    text+="\"";

    return text;
}

Dms* Dms::stringToDms(QString dms)
{
        dms.replace(QString::fromUtf8("°")," ");
        dms.replace("'"," ");
        dms.replace("\""," ");

	int degree=(dms.section(" ",0,0)).toInt();
	int minute=(dms.section(" ",1,1)).toInt();
	double second=(dms.section(" ",2,2)).toDouble();

	if(dms.startsWith('-'))
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

       // qDebug()<<this->toString();
        return this;
}

/**
* Convertions
*/
Dms * Dms::secondsToDms(double seconds)
{
	double value=fabs(seconds);

	int deg=(int)value/3600;
	double resto1=fmod(value,3600);
	int min=(int)resto1/60;
	double sec=fmod(resto1,60);

	Dms *result;
	if(seconds<0)
		result= new Dms(deg,min,sec,true);
	else
		result= new Dms(deg,min,sec);

	return result;
}

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
    int degrees;
    int minutes;
    double seconds;
    if(degreeDecimal>=0)
    {
        degrees = degreeDecimal;
        minutes = (degreeDecimal-degrees)*60.0;
        seconds = ((degreeDecimal-degrees)*60.0-minutes)*60;
        setSignal(false);
    }else
    {
        degreeDecimal=-degreeDecimal;
        degrees = degreeDecimal;
        minutes = (degreeDecimal-degrees)*60.0;
        seconds = ((degreeDecimal-degrees)*60.0-minutes)*60;
        setSignal(true);
        //	return (new Dms(degrees,minutes,seconds,true));
    }
    setDegree(degrees);
    setMinute(minutes);
    setSeconds(seconds);
    //qDebug("degree: %f dms: %s",degreeDecimal,this->toString().toStdString().c_str());

    return this;
}

double Dms::degreeDecimalToRadiano(double degree)
{
    //qDebug("degree: %f radiano: %f",degree,(degree*M_PI)/180);
    return (degree*M_PI)/180;
}

Dms* Dms::radianoToDms(double radiano)
{
    return degreeDecimalToDms(radianoToDegreeDecimal(radiano));
}

double Dms::radianoToDegreeDecimal(double radiano)
{
    //qDebug("radiano: %f degree: %f",radiano,(radiano/M_PI)*180);
    return (radiano/M_PI)*180;
}

void Dms::addDegMinSecs(Dms *degMinSec1)
{
    /*int newDegree= degMinSec1->getDegree()+degMinSec2->getDegree();
    int newMinute= degMinSec1->getMinute()+degMinSec2->getMinute();
    double newSeconds= degMinSec1->getSeconds()+degMinSec2->getSeconds();

    if(newSeconds>=60.0)
    {
        newSeconds-=60.0;
        newMinute+=1;
    }
    if(newMinute>=60)
    {
        newMinute-=60;
        newDegree+=1;
    }
    if(newDegree>=360)
        newDegree-=360;
    */
	double sum=this->dmsToDegreeDecimal()+degMinSec1->dmsToDegreeDecimal();

	Dms *temp=degreeDecimalToDms(sum);
	this->setDms(*temp);

}

void Dms::mulDegMinSecs(int factor)
{
	for (int i=1; i<factor;i++)
    {
		addDegMinSecs(this);
    }
}

/** If calling object is bigger than degMinSec then function returns 1
*   If calling object is smaller than degMinSec then function returns -1
*   If calling object is equall degMinSec then function returns 0
*/
int Dms::compareDegMinSecs(Dms *degMinSec)
{
    if (this->hasSignal() && !degMinSec->hasSignal())
    {
        return -1;
    }
    if (!this->hasSignal() && degMinSec->hasSignal())
    {
        return 1;
    }
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
}
