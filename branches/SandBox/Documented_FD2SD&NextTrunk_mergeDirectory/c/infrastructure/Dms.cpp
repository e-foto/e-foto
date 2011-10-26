#include "Dms.h"
#include "QtGui"
#include <QAbstractSpinBox>
#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
  *Class to manipulate rad degree and degree decimal
  */

Dms::Dms()
{
	Dms(0, 0, 0.0);
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
	setDegree(degree);
	setMinute(minute);
	setSeconds(second);
}

Dms::Dms(QString degree)
{
	Dms *newDegree = stringToDms(degree);
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
//    if (signalMin && signalMax)
		degree = (newDegrees<0||newDegrees>359)? abs(degree) : abs(newDegrees) ;
}

void Dms::setMinute(int newMinutes)
{
	min = (newMinutes<0||newMinutes>59)? min : newMinutes ;
}

void Dms::setSeconds(double newSeconds)
{
	sec = (newSeconds<0.0 || newSeconds>60.0)? sec : newSeconds;
	//double temp=sec;
	//temp= round(temp*pow(10,getSecondsPrecision()))/pow(10,getSecondsPrecision());
	//sec=temp;
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

	//qDebug("tostring: deg= %d  min = %d sec = %f",getDegree(),getMinute(),getSeconds());
	text+=QString::number(this->getDegree());
	text+=QString::fromUtf8("°");
	text+=QString::number(this->getMinute());
	text+="'";
	text.append(QLocale::system().toString(this->getSeconds(),'f',decimals));
	//text.append(QString::number(this->getSeconds(),'f',decimals));
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

//    qDebug("string: %s",dms.toStdString().c_str());
//    qDebug("stringToDMS: %d %d %.9f",degree,minute,second);

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

	//setSecondsPrecision(getSecondsPrecision());
	// qDebug()<<this->toString();
	return this;
}

void Dms::setSecondsPrecision(int precision)
{
	secondsPrecision = (precision>=1 ? precision : 2);
}

int  Dms::getSecondsPrecision()
{
	return secondsPrecision;
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

//	result->setSecondsPrecision(getSecondsPrecision());
	return result;
}

double Dms::dmsToDegreeDecimal()
{
  //  qDebug("dms to degree: %d %d %.9f",getDegree(),getMinute(),getSeconds());
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
		//	return (new Dms(degrees,minutes,seconds,true));
	}
	setDegree(degrees);
	setMinute(minutes);
	setSeconds(seconds);
	//qDebug("degreeDecimal %.9f",degreeDecimal);
	//qDebug("degreeDecimalToDMS: \tdeg= %d min= %d sec=%.9f",degrees,minutes,getSeconds());

	//qDebug("dg->dms: degree: %f dms: %s",degreeDecimal,this->toString(9).toStdString().c_str());

	return this;
}

double Dms::degreeDecimalToRadiano(double oldDegree)
{
	//qDebug("degree: %f radiano: %f",degree,(degree*M_PI)/180);
	double k=(oldDegree*M_PI)/180;
	return k;
}

Dms* Dms::radianoToDms(double radiano)
{
	//qDebug("radia= %f",radiano);
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
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
