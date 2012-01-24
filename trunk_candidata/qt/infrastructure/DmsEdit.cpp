#include "DmsEdit.h"
#include <QDebug>
#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DmsEdit::DmsEdit(QWidget *parent):
    QLineEdit(parent)
{
	//radValidator=new QDoubleValidator(-2*M_PI,2*M_PI,getDecimals(),this);
    //degValidator=new QDoubleValidator(-359.9999999,359.9999999,getDecimals(),this);

    //qDebug()<< "language:"<<locale().name();
    //qDebug()<<"simbolo decimal: "<<locale().decimalPoint();

    installEventFilter(this);
    degMinSecLine= new Dms("0 0 0");
    oldDegMinSecLine= new Dms("0 0 0");
    dmsMin= new Dms(180,0,0.0,true);
    dmsMax= new Dms(180,0,0.0,false);

    setDecimals(5);
    setText(dmsToQString(degMinSecLine));
    setDmsEditMode(DMS);
    connect(this,SIGNAL(cursorPositionChanged(int,int)), this,SLOT(changedField(int,int)));
    connect(this,SIGNAL(editingFinished()),this,SLOT(validate()));
    connect(this,SIGNAL(returnPressed()),this,SLOT(validate())); //Novo 11.3.08
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(updateValue(QString)));
}

void DmsEdit::setDecimals(int newDecimals)
{
    decimals=( newDecimals >=0 ? newDecimals : 0);
    //Em teste //Novo 11.3.08
    validate();
}

int DmsEdit::getDecimals()
{
    return decimals;
}
void DmsEdit::validate()
{
    bool ok;
    if (getDmsEditMode()==DMS)
    {
        degMinSecLine->stringToDms(this->text().toStdString());

        int compMin=degMinSecLine->compareDegMinSecs(getMinimumDms());
        int compMax=degMinSecLine->compareDegMinSecs(getMaximumDms());
        if (compMin>=0 && compMax<=0)
        {
        //    qDebug("validate");
          //  qDebug("%s\t<\t%s\t<\t%s\n",getMinimumDms()->getDms()->toString(3).toStdString().c_str(),degMinSecLine->toString(3).toStdString().c_str(),getMaximumDms()->toString(3).toStdString().c_str());
            setText(dmsToQString(degMinSecLine));
        }
        else
        {
            degMinSecLine->setDms(*oldDegMinSecLine);
            setText(dmsToQString(degMinSecLine));
        }
    }
    else if (getDmsEditMode()==RAD)
    {
        //		QString decimalPnt=QLocale::decimalPoint();
        QString radVal="(-)?[0-6]{1}?(\\.|,)\\d{";

	//	radVal.append();
        radVal.append(QString::number(getDecimals()));
        radVal.append("}");

        // qDebug()<<radVal;
        QRegExpValidator *radValidator1=new QRegExpValidator(QRegExp(radVal),this);
        radValidator1->setLocale(QLocale::system());
        //radValidator1->locale().setNumberOptions(QLocale::O);
        //qDebug()<<radValidator1->locale().name()<<"simbolo decimal: "<<radValidator1->locale().decimalPoint();

        this->setValidator(radValidator1);
        //setValidator(new QDoubleValidator(-2*M_PI,2*M_PI,5,this));
        //em teste //Novo 11.3.08
        setText(this->locale().toString(radValue,'f',getDecimals()));

    }
    else if (getDmsEditMode()==DEG)
    {

        QString degVal="(-)?[0-3]?\\d{0,2}(\\.|,)\\d{";
        degVal.append(QString::number(getDecimals()));
        degVal.append("}");

        //qDebug()<<degVal;
        QRegExpValidator *degValidator1=new QRegExpValidator(QRegExp(degVal),this);
        this->setValidator(degValidator1);
	//	setValidator(new QDoubleValidator(-2*M_PI,2*M_PI,5,this));
        //em teste //Novo 11.3.08
        setText(this->locale().toString(degValue,'f',getDecimals()));
    }
}

void DmsEdit::stepBy(int steps)
{
    int pos=cursorPosition();
    //this->blockSignals(true);
    if(steps<0)
    {
        for(int i=steps;i<0;i++)
            stepDown();
    }
    else
    {
        for (int i=0; i<steps; i++)
            stepUp();
    }
   // this->blockSignals(false);
    updateValue();
    setCursorPosition(pos);
}

/** Method to subtract one step in current Field
*/
void DmsEdit::stepDown()
{
    bool ok;
    if (getDmsEditMode()==DMS)
    {
        //degMinSecLine->stringToDms(text());
        if (positionValue()==DEGREES)
        {
            if(degMinSecLine->getDegree()==0)
                degMinSecLine->setSignal(true);

            if(degMinSecLine->hasSignal() )
                degMinSecLine->setDegree(degMinSecLine->getDegree()+1);
            else
                degMinSecLine->setDegree(degMinSecLine->getDegree()-1);
            //setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==MINUTES)
        {
            if(degMinSecLine->getMinute()==0)
            {
                degMinSecLine->setDegree(degMinSecLine->getDegree()-1);
                degMinSecLine->setMinute(59);
            }
            else
                degMinSecLine->setMinute(degMinSecLine->getMinute()-1);
            //setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==SECONDS)
        {
            if(degMinSecLine->getSeconds()<1.0)
            {
                if(degMinSecLine->getMinute()==0)
                {
                    degMinSecLine->setDegree(degMinSecLine->getDegree()-1);
                    degMinSecLine->setMinute(59);
                }else
                    degMinSecLine->setMinute(degMinSecLine->getMinute()-1);
                degMinSecLine->setSeconds(degMinSecLine->getSeconds()+59.0);
            }
            else
                degMinSecLine->setSeconds(degMinSecLine->getSeconds()-1);
            //setText(degMinSecLine->toString(getDecimals()));
        }
    }
    else
    {
		setText(locale().toString(text().toDouble(&ok)-1));
    }
}

/** Method to add one step in current Field
*/
void DmsEdit::stepUp()
{
    bool ok;
    if (getDmsEditMode()==DMS)
    {
        //degMinSecLine->stringToDms(text());
        if (positionValue()==DEGREES)
        {
            if(degMinSecLine->hasSignal())
                degMinSecLine->setDegree(degMinSecLine->getDegree()-1);
            else
                degMinSecLine->setDegree(degMinSecLine->getDegree()+1);

            if (degMinSecLine->getDegree()==0)
                degMinSecLine->setSignal(false);
            //setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==MINUTES)
        {
            if(degMinSecLine->getMinute()==59)
            {
                degMinSecLine->setDegree(degMinSecLine->getDegree()+1);
                degMinSecLine->setMinute(0);
            }
            else
                degMinSecLine->setMinute(degMinSecLine->getMinute()+1);
            //setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==SECONDS)
        {
            if(degMinSecLine->getSeconds()>=59.0)
            {
                if(degMinSecLine->getMinute()==59)
                {
                    degMinSecLine->setDegree(degMinSecLine->getDegree()+1);
                    degMinSecLine->setMinute(0);
                }
                else
                    degMinSecLine->setMinute(degMinSecLine->getMinute()+1);
                degMinSecLine->setSeconds(degMinSecLine->getSeconds()-59.0);
                //degMinSecLine->setSeconds(0);
            }
            else
                degMinSecLine->setSeconds(degMinSecLine->getSeconds()+1);
            //setText(degMinSecLine->toString(getDecimals()));
        }
    }
    else
    {
		setText(locale().toString(text().toDouble(&ok)+1));
    }
}

/** Method to determine where Field cursor is currently
*/
PositionValue DmsEdit::positionValue()
{
    if(cursorPosition()<=text().lastIndexOf(QString::fromUtf8("Â°")))
        return DEGREES;
    else if ( cursorPosition()<=text().lastIndexOf("'"))
        return MINUTES;
    else if ( cursorPosition()<=text().lastIndexOf("\""))
        return SECONDS;
    return NONE;
}

/** Overloaded method
  */
PositionValue DmsEdit::positionValue(int pos)
{
    if(pos<=text().lastIndexOf(QString::fromUtf8("Â°")))
        return DEGREES;
    else if ( pos<=text().lastIndexOf("'"))
        return MINUTES;
    else if ( pos<=text().lastIndexOf("\""))
        return SECONDS;
    return NONE;
}

/** Method to determine to from field to wich field cursor moved
  */
void DmsEdit::changedField(int oldPos, int newPos)
{
    if (getDmsEditMode()==DMS)
    {
        PositionValue oldValue=positionValue(oldPos);
        PositionValue newValue=positionValue(newPos);

        if (oldValue==DEGREES && newValue==MINUTES)
        {
            selectField(MINUTES);
        }else if (oldValue==DEGREES && newValue==SECONDS)
        {
            selectField(SECONDS);
        }else if (oldValue==MINUTES && newValue==DEGREES)
        {
            selectField(DEGREES);
        }else if (oldValue==MINUTES && newValue==SECONDS)
        {
            selectField(SECONDS);
        }else if (oldValue==SECONDS && newValue==MINUTES)
        {
            selectField(MINUTES);
        }else if (oldValue==SECONDS && newValue==DEGREES)
        {
            selectField(DEGREES);
        }
    }
}
/** Method to select field in @param pos
  */
void DmsEdit::selectField(PositionValue pos)
{
    if (getDmsEditMode()==DMS)
    {
        int posGrau = text().lastIndexOf(QString::fromUtf8("Â°"));
        int posMin  = text().lastIndexOf(QString::fromUtf8("'"));
        int posSec  = text().lastIndexOf(QString::fromUtf8("\""));

        if (pos == DEGREES)
        {
            setSelection(posGrau,-posGrau);
        }else if (pos == MINUTES)
        {
            setSelection(posMin,-(posMin-posGrau-1));
        }else if (pos == SECONDS)
        {
            setSelection(posSec,-(posSec-posMin-1));
        }
    }
}

/** Method overloaded from QLineEdit
  */
bool DmsEdit::eventFilter(QObject *objeto, QEvent *evento)
{

    if (evento->type()==QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evento);
        keyPressEvent(keyEvent);
        return true;
    }
    if (evento->type()==QEvent::FocusIn)
    {
        QFocusEvent *focusInEvento = static_cast<QFocusEvent *>(evento);
        focusInEvent(focusInEvento);
        return true;
    }
	if (evento->type()==QEvent::FocusOut)
	{
		QFocusEvent *focusOutEvento = static_cast<QFocusEvent *>(evento);
		focusOutEvent(focusOutEvento);
		return true;
	}
	else
	{
		QLineEdit::eventFilter(objeto,evento);
	}
}

/** Method overloaded from QLineEdit
  */
void DmsEdit::keyPressEvent(QKeyEvent *evento)
{
    if (getDmsEditMode()==DMS)
    {
        switch(evento->key())
        {
        case Qt::Key_Up:
            stepBy(1);
            break;

        case Qt::Key_Down:
            stepBy(-1);
            break;

        case Qt::Key_Tab:
            if (positionValue()==DEGREES)
            {
                selectField(MINUTES);
            }
            else if (positionValue()==MINUTES)
            {
                selectField(SECONDS);
            }
            else if (positionValue()==SECONDS)
            {
                QLineEdit::keyPressEvent(evento);
            }
            else if (positionValue()==NONE)
            {
                QLineEdit::keyPressEvent(evento);
            }
            break;
    case Qt::Key_Tab+1:
            if (positionValue()==SECONDS)
            {
                selectField(MINUTES);
            }
            else if (positionValue()==MINUTES)
            {
                selectField(DEGREES);
            }
            else if (positionValue()==DEGREES)
            {
                QLineEdit::keyPressEvent(evento);
            }
            break;
    default:
            QLineEdit::keyPressEvent(evento);
        }
    }
    else
    {
        switch(evento->key())
        {
        case Qt::Key_Up:
            stepBy(1);
            break;

        case Qt::Key_Down:
            stepBy(-1);
            break;
        default:
            QLineEdit::keyPressEvent(evento);
        }
    }
}

/** Method overloaded from QLineEdit
  */
void DmsEdit::focusInEvent(QFocusEvent *evento)
{
    if (getDmsEditMode()==DMS)
    {
        if (evento->reason() & Qt::TabFocusReason)
        {
            selectField(DEGREES);
        }else if(evento->reason() & Qt::BacktabFocusReason)
        {
            selectField(SECONDS);
        }
    }
    else
    {
        QLineEdit::focusInEvent(evento);
    }
}

/** Method overloaded from QLineEdit
  */
void DmsEdit::focusOutEvent(QFocusEvent *evento)
{
	validate();
	QLineEdit::focusOutEvent(evento);
}
/** Method to @return mode. Which may be RAD, DEG and DMS
  */
DmsEditMode DmsEdit::getDmsEditMode()
{
    return mode;
}

/** Method to set mode
  */
void DmsEdit::setDmsEditMode(DmsEditMode newMode)
{
    blockSignals(true);
    bool ok;
    if (getDmsEditMode()==DMS && newMode==RAD)
    {
    //	qDebug()<<"dms to rad";
        radValue=degMinSecLine->dmsToRadiano();
        degValue=degMinSecLine->dmsToDegreeDecimal();
        setText(locale().toString(radValue,'f',getDecimals()));
		//setText(QString::number(degMinSecLine->dmsToRadiano(),'f',getDecimals()));
    //	qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
    }
    else if(getDmsEditMode()==DMS && newMode==DEG)
    {
    //	qDebug()<<"dms to deg";
        degValue=degMinSecLine->dmsToDegreeDecimal();
        radValue=degMinSecLine->dmsToRadiano();
		setText(locale().toString(degValue,'f',getDecimals()));
		//setText(QString::number(degMinSecLine->dmsToDegreeDecimal(),'f',getDecimals()));
    //	qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
    }
    else if(getDmsEditMode()==DEG && newMode==DMS)
    {
    //	qDebug()<<"deg to dms";
        setText(dmsToQString(degMinSecLine->degreeDecimalToDms(degValue)));
		//setText(degMinSecLine->degreeDecimalToDms(text().toDouble(&ok))->toString(getDecimals()));
        //qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
    }
    else if(getDmsEditMode()==DEG && newMode==RAD)
    {
    //	qDebug()<<"deg to rad";
    //	qDebug("antes: radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
        radValue=Dms::degreeDecimalToRadiano(degValue);
        //qDebug("depois: radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5));
        degMinSecLine->degreeDecimalToDms(degValue);
		setText(locale().toString(radValue,'f',getDecimals()));
        //setText(QString::number(Dms::degreeDecimalToRadiano(text().toDouble),'f',getDecimals()));

    }
    else if(getDmsEditMode()==RAD && newMode==DMS)
    {
        setText(dmsToQString(degMinSecLine->radianoToDms(radValue)));
        //setText(degMinSecLine->radianoToDms(text().toDouble(&ok))->toString(getDecimals()));
        //	qDebug()<<"rad to dms";
        //	qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
    }
    else if(getDmsEditMode()==RAD && newMode==DEG)
    {
        degValue=Dms::radianoToDegreeDecimal(radValue);
        degMinSecLine->radianoToDms(radValue);
        setText(locale().toString(degValue,'f',getDecimals()));
        //setText(QString::number(Dms::radianoToDegreeDecimal(text().toDouble(&ok)),'f',getDecimals()));
        //	qDebug()<<"rad to dms";
        //	qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
    }
    mode=newMode;
    validate();
    blockSignals(false);
}

void DmsEdit::updateValue(QString newValue)
{
    int pos=cursorPosition();
    DmsEditMode mode=getDmsEditMode();
    //Dms old(degMinSecLine->getDegree(),degMinSecLine->getMinute(),degMinSecLine->getSeconds(),degMinSecLine->hasSignal());

    if (newValue=="")
    {
        //qDebug("update value chamado");
        if (mode==DMS)
        {
            int compMin=degMinSecLine->compareDegMinSecs(getMinimumDms());
            int compMax=degMinSecLine->compareDegMinSecs(getMaximumDms());
            if (compMin>=0 && compMax<=0)
            {
                //qDebug("%s < %s < %s",getMinimumDms()->toString(getDecimals()).toStdString().c_str(),degMinSecLine->toString(getDecimals()).toStdString().c_str(),getMaximumDms()->toString(getDecimals()).toStdString().c_str());
                oldDegMinSecLine->setDms(*degMinSecLine);
                setText(dmsToQString(degMinSecLine));
                // degValue=degMinSecLine->dmsToDegreeDecimal();
                //radValue=degMinSecLine->dmsToRadiano();
            }
            else
            {
                degMinSecLine->setDms(*oldDegMinSecLine);
                //qDebug("%s não esta entre ",oldDegMinSecLine->toString(getDecimals()).toStdString().c_str());
                setText(dmsToQString(degMinSecLine));
            }
            //setText(degMinSecLine->toString(getDecimals()));
            return;
        }
        else if (mode==DEG)
        {
            //setText(QString::number(degValue,'f',5));
            degMinSecLine->degreeDecimalToDms(degValue);
            radValue=degMinSecLine->dmsToRadiano();
        }
        else if (mode==RAD)
        {
            //setText(QString::number(radValue,'f',5));
            degMinSecLine->radianoToDms(radValue);
            degValue=degMinSecLine->dmsToDegreeDecimal();
        }
        // qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
        return;
    }
    bool ok;
    if (mode==DMS)
    {
        //qDebug("\nString não vazia\n");

        /**   sdsdf
        //degMinSecLine->stringToDms(newValue.toStdString());*/
        qStringToDms(newValue);

        int compMin=degMinSecLine->compareDegMinSecs(getMinimumDms());
        int compMax=degMinSecLine->compareDegMinSecs(getMaximumDms());
        if (compMin>=0 && compMax<=0)
        {
            radValue=degMinSecLine->dmsToRadiano();
            degValue=degMinSecLine->dmsToDegreeDecimal();
            setText(dmsToQString(degMinSecLine));
            oldDegMinSecLine->setDms(*degMinSecLine);
          //  qDebug("%s < %s < %s",getMinimumDms()->toString(getDecimals()).toStdString().c_str(),degMinSecLine->toString(getDecimals()).toStdString().c_str(),getMaximumDms()->toString(getDecimals()).toStdString().c_str());
            // degValue=degMinSecLine->dmsToDegreeDecimal();
            //radValue=degMinSecLine->dmsToRadiano();
        }
        else
        {
            degMinSecLine->setDms(*oldDegMinSecLine);
            //qDebug("%s não esta entre ",oldDegMinSecLine->toString(getDecimals()).toStdString().c_str());
            setText(dmsToQString(degMinSecLine));
        }
       // degMinSecLine->stringToDms(newValue);
        //radValue=degMinSecLine->dmsToRadiano();
        //degValue=degMinSecLine->dmsToDegreeDecimal();
    }
    else if(mode==RAD)
    {
        radValue=newValue.toDouble(&ok);
        degMinSecLine->radianoToDms(radValue);
        degValue=degMinSecLine->dmsToDegreeDecimal();
    }
    else if(mode==DEG)
    {
        degValue=newValue.toDouble(&ok);
        degMinSecLine->degreeDecimalToDms(degValue);
        radValue=Dms::degreeDecimalToRadiano(degValue);
    }
    setCursorPosition(pos);
    // qDebug("rad value = %f",radValue);
}

double DmsEdit::getRadianValue()
{
	return radValue;
}

double DmsEdit::getDegreeValue()
{
	return degValue;
}

Dms* DmsEdit::getDmsValue()
{
    return degMinSecLine;
}

double DmsEdit::getMaximumDeg()
{
    return degMax;
}

double DmsEdit::getMinimumDeg()
{
    return degMin;
}

double DmsEdit::getMaximumRad()
{
    return radMax;
}

double DmsEdit::getMinimumRad()
{
    return radMin;
}

Dms* DmsEdit::getMaximumDms()
{
    return dmsMax;
}

Dms* DmsEdit::getMinimumDms()
{
    return dmsMin;
}

void DmsEdit::setDmsMaximum(int degree, int minutes, double seconds, bool signal)
{
    dmsMax->setDms(degree,minutes,seconds,signal);
    radMax=dmsMax->dmsToRadiano();
    degMax=dmsMax->dmsToDegreeDecimal();
}

void DmsEdit::setDmsMinimum(int degree, int minutes, double seconds, bool signal)
{
    dmsMin->setDms(degree,minutes,seconds,signal);
    radMin=dmsMin->dmsToRadiano();
    degMin=dmsMin->dmsToDegreeDecimal();
}

QString DmsEdit::dmsToQString(Dms *dms)
{
    QString dmsQString;

    if (dms->hasSignal())
        dmsQString+='-';

    dmsQString+=QString::number(dms->getDegree());
    //dmsQString+=QString::fromUtf8("°");
    dmsQString+="°";
    dmsQString+=QString::number(dms->getMinute());
    dmsQString+="\'";
    dmsQString+=QString::number(dms->getSeconds(),'f',getDecimals());
    dmsQString+="\"";
    return dmsQString;
}

void DmsEdit::qStringToDms(QString dmsQString)
{
    dmsQString.replace("°"," ");
    dmsQString.replace("'"," ");
    dmsQString.replace("\""," ");

    degMinSecLine->stringToDms(dmsQString.toStdString());
   // Dms * newDms= new Dms(dmsQString);
    //return newDms;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
