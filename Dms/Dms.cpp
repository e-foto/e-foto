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

void Dms ::setDegree(int newDegrees)
{
    degree = (0>newDegrees||newDegrees>359)? 0 : newDegrees ;
}

void Dms ::setMinute(int newMinutes)
{
	min = (newMinutes<0||newMinutes>59)? 0 : newMinutes ;
}

void Dms ::setSeconds(double newSeconds)
{
	sec = (newSeconds<0.0 || newSeconds>=60.0)? 0.00 : newSeconds;
}

void Dms :: setSignal(bool newSignal)
{
	this->signal=newSignal;
}

int Dms ::getDegree()
{
    return this->degree;
}

int Dms::getMinute()
{
    return this->min;
}

double Dms::getSeconds()
{
    return this->sec;
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

QString Dms::toString()
{
    QString text;

    if(this->hasSignal())
		text+="-";

    text+=QString::number(this->getDegree());
	text+="°";
    text+=QString::number(this->getMinute());
    text+="'";
    text.append(QString::number(this->getSeconds(),'f',5));
    text+="\"";

    return text;
}

Dms* Dms::stringToDms(QString dms)
{
	int degree=(dms.section(" ",0,0)).toInt();
	int minute=(dms.section(" ",1,1)).toInt();
	double second=(dms.section(" ",2,2)).toDouble();

	if(dms.startsWith('-'))
	{
		Dms *converted= new Dms(degree,minute,second,true);
		return converted;
	}
	else
	{
		Dms *converted= new Dms(degree,minute,second,false);
        return converted;
	}
}

/**
* Convertions
*/
double Dms::dmsToDegreeDecimal()
{
    double minPartial=this->getMinute()+this->getSeconds()/60;
    double result=this->getDegree()+minPartial/60;
    return (this->hasSignal() ? -result:result );
}

double Dms::dmsToRadiano()
{
	double parcial=dmsToDegreeDecimal();
    return degreeDecimalToRadiano(parcial);
}


Dms* Dms::degreeDecimalToDms(double degreeDecimal)
{
    if(degreeDecimal>=0)
    {
        int degrees = degreeDecimal;
        int minutes = (degreeDecimal-degrees)*60.0;
        double seconds = ((degreeDecimal-degrees)*60.0-minutes)*60;
		return (new Dms(degrees,minutes,seconds));
    }else
    {
        degreeDecimal=-degreeDecimal;
        int degrees = degreeDecimal;
        int minutes = (degreeDecimal-degrees)*60.0;
        double seconds = ((degreeDecimal-degrees)*60.0-minutes)*60;
		return (new Dms(degrees,minutes,seconds,true));
    }
}

double Dms::degreeDecimalToRadiano(double degree)
{
    return (degree*M_PI)/180;
}

Dms* Dms::radianoToDms(double radiano)
{
	return degreeDecimalToDms(radianoToDegreeDecimal(radiano));
}

double Dms::radianoToDegreeDecimal(double radiano)
{
	//qDebug("radiano : %f \tem degree= %f",radiano,(radiano/3.141592654)*180);
    return (radiano/M_PI)*180;
}

Dms* Dms::addDegMinSecs(Dms *degMinSec1, Dms *degMinSec2)
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
	double sum=degMinSec1->dmsToDegreeDecimal()+degMinSec2->dmsToDegreeDecimal();

	return (new Dms())->degreeDecimalToDms(sum);
}

Dms* Dms::mulDegMinSecs(int factor, Dms *degMinSec1)
{
	Dms *result=degMinSec1;

    for (int i=1; i<factor;i++)
    {
		result=Dms::addDegMinSecs(result,degMinSec1);
    }
    return result;
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

/**
*   Class derivated from QSpinBox for format ddÂºmm'ss,sss"
*
*/
/*
DegreeRadSpin::DegreeRadSpin(QWidget *parent)
    :QSpinBox(parent)
{
  /*
	degreeMinSecThis= new Dms(150,45,30.50);

	double degreeDecimal=degreeMinSecThis->dmsToDegreeDecimal();
    double radiano=DegreeMinSec::degreeDecimalToRadiano(degreeDecimal);
    degreeMinSecThis->degreeDecimalToDegreeMinSec(degreeDecimal);
    DegreeMinSec::radianoToDegreeDecimal(radiano);
*/
/*
    DegreeMinSec *teste1myType= new DegreeMinSec(40,50,30.20);
    DegreeMinSec *teste2myType= new DegreeMinSec(100,30,50.50);
	DegreeMinSec *teste3myType= new DegreeMinSec(45,30,0.0,true);
	DegreeMinSec *teste4myType= new DegreeMinSec("-0 0 1.5");

    //teste1myType->compareDegMinSecs(new DegreeMinSec(40,50,30.21))
/*
    qDebug("comparando %d",teste1myType->compareDegMinSecs(new DegreeMinSec(40,50,30.21)));
    qDebug("comparando %d",teste1myType->compareDegMinSecs(new DegreeMinSec(40,50,20.19)));
    qDebug("comparando %d\n",teste1myType->compareDegMinSecs(new DegreeMinSec(40,52,20.21)));

    qDebug("comparando %d",teste1myType->compareDegMinSecs(new DegreeMinSec(42,52,30.21)));
    qDebug("comparando %d\n",teste1myType->compareDegMinSecs(new DegreeMinSec(42,50,10.21)));

    qDebug("comparando %d",teste1myType->compareDegMinSecs(new DegreeMinSec(38,50,30.21)));
    qDebug("comparando %d\n",teste1myType->compareDegMinSecs(new DegreeMinSec(38,52,0.21)));
    qDebug("comparando %d",teste1myType->compareDegMinSecs(new DegreeMinSec(40,50,30.20)));
*/
/*
    DegreeMinSec *somaTest1Test2= DegreeMinSec::addDegMinSecs(teste1myType,teste2myType);
    qDebug("soma de teste1 e teste2:\t%s ",somaTest1Test2->toString().toStdString().c_str());

	DegreeMinSec *subTest1Test2= DegreeMinSec::subDegMinSecs(teste1myType,teste2myType); //-59Âº40'20,30" = -(teste2-teste1)
    qDebug("subtracao de teste1 e teste2:\t%s ",subTest1Test2->toString().toStdString().c_str());

    DegreeMinSec *mulTest1= DegreeMinSec::mulDegMinSecs(2,teste1myType);
    qDebug("multiplicacao de teste1 *2:\t%s ",mulTest1->toString().toStdString().c_str());

    DegreeMinSec *mulTest2= DegreeMinSec::mulDegMinSecs(3,teste1myType);
    qDebug("multiplicacao de teste1 *3:\t%s ",mulTest2->toString().toStdString().c_str());

/*
    qDebug("\n\nteste 1\n");
	double teste1dec = teste1myType->degreeMinSecToDegreeDecimal(); //myType to degree decimal
	double teste1rad =DegreeMinSec::degreeDecimalToRadiano(teste1myType->degreeMinSecToDegreeDecimal()); // myType to radiano
    qDebug("%s to degree decimal: %f",teste1myType->toString().toStdString().c_str(),teste1dec);
    qDebug("%s to radiano: %f rad\n",teste1myType->toString().toStdString().c_str(),teste1rad);

    qDebug("%f rad to xxÂºxx'xx: %s",teste1rad,teste1myType->radianoToDegreeMinSec(teste1rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste1rad, DegreeMinSec::radianoToDegreeDecimal(teste1rad) );

    qDebug("%f degree to xxÂºxx'xx: %s",teste1dec,teste1myType->degreeDecimalToDegreeMinSec(teste1dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste1dec, DegreeMinSec::degreeDecimalToRadiano(teste1dec) );

	qDebug("\n\nteste 2\n");
	double teste2dec = teste2myType->degreeMinSecToDegreeDecimal(); //myType to degree decimal
	double teste2rad =DegreeMinSec::degreeDecimalToRadiano(teste2myType->degreeMinSecToDegreeDecimal()); // myType to radiano

    qDebug("%s to degree decimal: %f",teste2myType->toString().toStdString().c_str(),teste2dec);
    qDebug("%s to radiano: %f rad\n",teste2myType->toString().toStdString().c_str(),teste2rad);

    qDebug("%f rad to xxÂºxx'xx: %s",teste2rad,teste2myType->radianoToDegreeMinSec(teste2rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste2rad, DegreeMinSec::radianoToDegreeDecimal(teste2rad) );

    qDebug("%f degree to xxÂºxx'xx: %s",teste2dec,teste2myType->degreeDecimalToDegreeMinSec(teste2dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste2dec, DegreeMinSec::degreeDecimalToRadiano(teste2dec) );

	qDebug("\n\nteste 3\n");
	double teste3dec = teste3myType->degreeMinSecToDegreeDecimal(); //myType to degree decimal
	double teste3rad =DegreeMinSec::degreeDecimalToRadiano(teste3myType->degreeMinSecToDegreeDecimal()); // myType to radiano

    qDebug("%s to degree decimal: %f",teste3myType->toString().toStdString().c_str(),teste3dec);
    qDebug("%s to radiano: %f rad\n",teste3myType->toString().toStdString().c_str(),teste3rad);

    qDebug("%f rad to xxÂºxx'xx: %s",teste3rad,teste3myType->radianoToDegreeMinSec(teste3rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste3rad, DegreeMinSec::radianoToDegreeDecimal(teste3rad) );

    qDebug("%f degree to xxÂºxx'xx: %s",teste3dec,teste3myType->degreeDecimalToDegreeMinSec(teste3dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste3dec, DegreeMinSec::degreeDecimalToRadiano(teste3dec) );


	qDebug("\n\nteste 4\n");
	double teste4dec = teste4myType->degreeMinSecToDegreeDecimal(); //myType to degree decimal
	double teste4rad =DegreeMinSec::degreeDecimalToRadiano(teste4myType->degreeMinSecToDegreeDecimal()); // myType to radiano

    qDebug("%s to degree decimal: %f",teste4myType->toString().toStdString().c_str(),teste4dec);
    qDebug("%s to radiano: %f rad\n",teste4myType->toString().toStdString().c_str(),teste4rad);

    qDebug("%f rad to xxÂºxx'xx: %s",teste4rad,teste4myType->radianoToDegreeMinSec(teste4rad)->toString().toStdString().c_str() );
	qDebug("%f rad to degree decimal: %f\n",teste4rad, DegreeMinSec::radianoToDegreeDecimal(teste4rad) );

    qDebug("%f degree to xxÂºxx'xx: %s",teste4dec,teste4myType->degreeDecimalToDegreeMinSec(teste4dec)->toString().toStdString().c_str() );
	qDebug("%f degree to degree decimal: %f",teste4dec, DegreeMinSec::degreeDecimalToRadiano(teste4dec) );

*/
/*
    this->setLocale(QLocale("ENGLISH"));
    texto = new QLineEdit();

    setLineEdit(texto);
    //texto->setInputMask("009*09'09.009\"");
    //texto->setText(degreeMinSecThis->toString());

    //validator = new QRegExpValidator(QRegExp("[a-zA-Z0-9]{0,50}"),this);
    validator = new QRegExpValidator(QRegExp("[0-5]?\\d(*[0-5]?\\d)?(\'[0-5]?\\d)?(\"\\d\\d)?([.]\\d{1,3})?"),this);

    texto->setValidator(validator);

    connect(this,SIGNAL(editingFinished()), this, SLOT(showTexto()));
    this->setWrapping(true);
}

void DegreeRadSpin::showTexto()
{
    validateAndInterprete();
}

void DegreeRadSpin::stepBy(int steps)
{
    int oldPosition = texto->cursorPosition();
    int oldLength = texto->text().length();

    if(steps<0){
        for(int i=steps;i<0;i++)
            this->stepDown();
        //qDebug("stepBy foi chamado steps: %d",steps);
    }
    else{
        for (int i=0; i<steps; i++)
            this->stepUp();
        //qDebug("stepBy foi chamado steps: %d",steps);
    }

    texto->setText(degreeMinSecThis->toString());
    correctCursorPosition(oldPosition,oldLength);
}

void DegreeRadSpin::stepDown()
{
    if (positionValue()==DEGREES)
        degreeMinSecThis->setDegree(degreeMinSecThis->getDegree()-1);
    else if (positionValue()==MINUTES)
        degreeMinSecThis->setMinute(degreeMinSecThis->getMinute()-1);
    else if (positionValue()==SECONDS)
        degreeMinSecThis->setSeconds(degreeMinSecThis->getSeconds()-1);
        // qDebug("stepDown foi chamado steps -1");
}

void DegreeRadSpin::stepUp()
{
    if(positionValue()==DEGREES)
        degreeMinSecThis->setDegree(degreeMinSecThis->getDegree()+1);
    else if (positionValue()==MINUTES)
        degreeMinSecThis->setMinute(degreeMinSecThis->getMinute()+1);
    else if (positionValue()==SECONDS)
        degreeMinSecThis->setSeconds(degreeMinSecThis->getSeconds()+1);
    //qDebug("stepUp foi chamado steps +1");
}

void DegreeRadSpin::validateAndInterprete()
{
    bool ok;
    DegreeMinSec *newDegree =new DegreeMinSec(texto->text());
    QString txt=texto->text();
	//qDebug("validateAndInterprete");
	//qDebug("txt: %s",txt.toStdString().c_str());

    if (txt.endsWith("rad"))//,Qt::CaseInsensitive))
    {
        double rad=(txt.remove("rad")).toDouble(&ok);
        QString degree= (degreeMinSecThis->radianoToDegreeMinSec(rad))->toString();
	   // qDebug("rad: \n%f degminsec: %s",rad,degree.toStdString().c_str());
        texto->setText((degreeMinSecThis->radianoToDegreeMinSec(rad))->toString());
    }
    texto->setText(newDegree->toString());
}

QString DegreeRadSpin::textFromValue(int val) const
{
    //qDebug("textFromValue");//: %s val:%d",degreeMinSecThis->toString().toStdString().c_str(),val);

    return degreeMinSecThis->toString();
}

int DegreeRadSpin::valueFromText(const QString &text) const
{
    DegreeMinSec newDegree(text);
    //qDebug("degree: %d minute: %d second: %f",newDegree.getDegree(),newDegree.getMinute(),newDegree.getSeconds());
    if(newDegree.isValid())
    {
        //qDebug("valueFromText: Ã© valido");
        degreeMinSecThis->setDegree(newDegree.getDegree());
        degreeMinSecThis->setMinute(newDegree.getMinute());
        degreeMinSecThis->setSeconds(newDegree.getSeconds());
		degreeMinSecThis->setSignal(newDegree.hasSignal());
    }
    //texto->setCursorPosition(texto->cursorPosition());
    qDebug("valueFromText");// Ã© invalido");
    return 6;
}

QValidator::State DegreeRadSpin::validate(QString &input, int &pos) const
{
  DegreeMinSec newDegree(input);
   // qDebug("validate: %s\tpos: %d degree: %d minute: %d second: %f",input.toStdString().c_str(),pos,newDegree.getDegree(),newDegree.getMinute(),newDegree.getSeconds());
    qDebug("validate valor: %s",newDegree.toString().toStdString().c_str());

    if(newDegree.isValid())
    {
        //qDebug("validate old antes valor: %s",degreeMinSecThis->toString().toStdString().c_str());
       // qDebug("validate new antes valor: %s",newDegree.toString().toStdString().c_str());
        degreeMinSecThis->setDegree(newDegree.getDegree());
        degreeMinSecThis->setMinute(newDegree.getMinute());
        degreeMinSecThis->setSeconds(newDegree.getSeconds());
		degreeMinSecThis->setSignal(newDegree.hasSignal());
	   // qDebug("validate old depois valor: %s",DmsThis->toString().toStdString().c_str());
        //qDebug("validate new depois valor: %s",newDegree.toString().toStdString().c_str());
        return validator->validate(input,pos);
    }
    qDebug("validate Ã© invalido");
    return validator->validate(input,pos);
}

PositionValue DegreeRadSpin::positionValue()
{
    if(texto->cursorPosition()<=texto->text().lastIndexOf("*") )
        return DEGREES;
    else if ( texto->cursorPosition()<=texto->text().lastIndexOf("'"))
        return MINUTES;
    else if ( texto->cursorPosition()<=texto->text().lastIndexOf("\""))
        return SECONDS;
    return NONE;
}

void DegreeRadSpin::correctCursorPosition(int oldPosition, int oldLength)
{
    if(texto->text().length()>oldLength)
        texto->setCursorPosition(oldPosition+(texto->text().length()-oldLength));
    else if(texto->text().length()<oldLength)
        texto->setCursorPosition(oldPosition+(texto->text().length()-oldLength));
    else
        texto->setCursorPosition(oldPosition);
}

void DegreeRadSpin::setMinimum(int min){}
void DegreeRadSpin::setMaximum(int max){}
int DegreeRadSpin::minimum(){return -359;}
int DegreeRadSpin::maximum(){return 40;}
*/

/** Class to convertions
  */

Matrix TransformationsCoordinates::GeoelipToGeocentricCartesian(double phi, double lambda, double haltura, GeoSystem system)
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

Matrix TransformationsCoordinates::GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi, GeoSystem system)
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

Matrix TransformationsCoordinates::getMatrixR(double phi, double lambda)
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

Matrix GeoelipToUtm(double phi, double lambda, double lambda0 ,double haltura, GeoSystem system)
{
    double a=6378388;
    double f=1/297;

    double b=a*(1-f);

    double n=(a-b)/(a+b);
    double alpha=(a+b)*(1+pow(n,2)/4+pow(n,4)/64)/2;
    double beta=-3*n/2+9*pow(n,3)/16-3*pow(n,5)/32;
    double gama=15*pow(n,2)/16-15*pow(n,4)/32;
    double delta=-35*pow(n,3)/48+105*pow(n,5)/256;
    double xsi=315*pow(n,4)/512;

   /*
    double n=(a-b)/(a+b);
    double alpha=6366742.5203;
    double beta=-2.51127456e-3;
    double gama=2.62771e-6;
    double delta=-3.42e-9;
    double xsi=5e-12;
*/
    double el2=(a*a-b*b)/(b*b);
    double t=tan(phi);

    double eta2=el2*pow(cos(phi),2);
    double N=(a*a)/(b*sqrt(1+n*n));
    double l=lambda-lambda0;
    double B=alpha*(phi+beta*sin(2*phi)+gama*sin(4*phi)+delta*sin(6*phi)+xsi*sin(8*phi));

    double y= B+t*N*pow(cos(phi),2)*l*l/2+
             (t*N*pow(cos(phi),4)*(5-t*t+9*eta2+4*eta2*eta2)            *    pow(l,4)) / 24+
             (t*N*pow(cos(phi),6)*(61-58*t*t+pow(t,4)+270*eta2-330*t*t*eta2)*pow(l,6)) / 720+
             (t*N*pow(cos(phi),8)*(1385-3111*t*t+543*pow(t,4)-pow(t,6)) *    pow(l,8)) / 40320;

    double x= N*l*cos(phi) +N*pow(cos(phi),3)*(1-t*t+eta2)              *    pow(l,3)  / 6+
             (N*pow(cos(phi),5)*(5-18*t*t+pow(t,4)+14*eta2-58*t*t*eta2) *    pow(l,5)) / 120+
             (N*pow(cos(phi),7)*(61-479*t*t+179*pow(t,4)-pow(t,6))      *    pow(l,7)) / 5040;


    Matrix xyh(1,3);
    xyh.set(1,1,x);
    xyh.set(1,2,y);
    xyh.set(1,3,haltura);
    qDebug("Xi: %f\tYi: %f\tZi: %f",phi,lambda,haltura);
    qDebug("Phi: %f\tLambda: %f\th: %f\n",xyh.get(1,1),xyh.get(1,2),xyh.get(1,3));

    return xyh;

}

/*Ex1	E=5.000.000,0;
		N=6.900.000,0;
        Meridiando central do fuso = -30°;

        Latidude = 21° 46' 32,6842" South
        Longitude =11° 13' 9,72626" West
        Convergencia meridiana = 17° 58' 47,65429"

  Ex2	E=8.000.000,0;
		N=2.000.000,0;
        Meridiando central do fuso = 30°;

		WGS84
        Latidude = 17° 34' 50,82536" South
        Longitude= 44° 01' 13,59099" East
        Convergencia meridiana = 4° 18' 52,91364"
	*/


//}
