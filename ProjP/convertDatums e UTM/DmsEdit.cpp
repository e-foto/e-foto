#include "DmsEdit.h"
#include <QDebug>
#include <math.h>

DmsEdit::DmsEdit(QWidget *parent):
    QLineEdit(parent)
{
	//radValidator=new QDoubleValidator(-2*M_PI,2*M_PI,getDecimals(),this);
    //degValidator=new QDoubleValidator(-359.9999999,359.9999999,getDecimals(),this);

    //qDebug()<< "language:"<<locale().name();
    //qDebug()<<"simbolo decimal: "<<locale().decimalPoint();

	installEventFilter(this);
	degMinSecLine= new Dms("0 0'0\"");
	setDecimals(5);
    setText(degMinSecLine->toString(getDecimals()));
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
        degMinSecLine->stringToDms(this->text());

        if(degMinSecLine->isValid())
        {
           setText(degMinSecLine->toString(getDecimals()));
        }
        else
        {
            //qDebug("caiu no else");
           setText(QString::fromUtf8("0°0'0.0\""));
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
		setText(locale().toString(radValue,'f',getDecimals()));

    }
    else if (getDmsEditMode()==DEG)
    {

        QString degVal="(-)?[0-3]?\\d{2}(\\.|,)\\d{";
        degVal.append(QString::number(getDecimals()));
        degVal.append("}");

//        qDebug()<<degVal;
        QRegExpValidator *degValidator1=new QRegExpValidator(QRegExp(degVal),this);
		this->setValidator(degValidator1);
	//	setValidator(new QDoubleValidator(-2*M_PI,2*M_PI,5,this));
        //em teste //Novo 11.3.08
		setText(locale().toString(degValue,'f',getDecimals()));
    }
}


void DmsEdit::stepBy(int steps)
{

    int pos=cursorPosition();
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
    setCursorPosition(pos);

}

void DmsEdit::stepDown()
{
    bool ok;
    if (getDmsEditMode()==DMS)
    {
        degMinSecLine->stringToDms(text());
        if (positionValue()==DEGREES)
        {
            degMinSecLine->setDegree(degMinSecLine->getDegree()-1);
			setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==MINUTES)
        {
            degMinSecLine->setMinute(degMinSecLine->getMinute()-1);
			setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==SECONDS)
        {
            degMinSecLine->setSeconds(degMinSecLine->getSeconds()-1);
			setText(degMinSecLine->toString(getDecimals()));
        }
    }
    else
    {
		setText(locale().toString(text().toDouble(&ok)-1));
    }
}

void DmsEdit::stepUp()
{
    bool ok;
    if (getDmsEditMode()==DMS)
    {
        degMinSecLine->stringToDms(text());
        if (positionValue()==DEGREES)
        {
            degMinSecLine->setDegree(degMinSecLine->getDegree()+1);
			setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==MINUTES)
        {
            degMinSecLine->setMinute(degMinSecLine->getMinute()+1);
			setText(degMinSecLine->toString(getDecimals()));
        }
        else if (positionValue()==SECONDS)
        {
            degMinSecLine->setSeconds(degMinSecLine->getSeconds()+1);
			setText(degMinSecLine->toString(getDecimals()));
        }
    }
    else
    {
		setText(locale().toString(text().toDouble(&ok)+1));
    }
}

PositionValue DmsEdit::positionValue()
{
    if(cursorPosition()<=text().lastIndexOf(QString::fromUtf8("°")))
        return DEGREES;
    else if ( cursorPosition()<=text().lastIndexOf("'"))
        return MINUTES;
    else if ( cursorPosition()<=text().lastIndexOf("\""))
        return SECONDS;
    return NONE;
}

PositionValue DmsEdit::positionValue(int pos)
{
    if(pos<=text().lastIndexOf(QString::fromUtf8("°")))
        return DEGREES;
    else if ( pos<=text().lastIndexOf("'"))
        return MINUTES;
    else if ( pos<=text().lastIndexOf("\""))
        return SECONDS;
    return NONE;
}

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

void DmsEdit::selectField(PositionValue pos)
{
    if (getDmsEditMode()==DMS)
    {
        int posGrau = text().lastIndexOf(QString::fromUtf8("°"));
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

void DmsEdit::focusOutEvent(QFocusEvent *evento)
{
	validate();
	QLineEdit::focusOutEvent(evento);
}

DmsEditMode DmsEdit::getDmsEditMode()
{
    return mode;
}

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
		setText(degMinSecLine->degreeDecimalToDms(degValue)->toString(getDecimals()));
		//setText(degMinSecLine->degreeDecimalToDms(text().toDouble(&ok))->toString(getDecimals()));
        //qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
    }
    else if(getDmsEditMode()==DEG && newMode==RAD)
    {
    //	qDebug()<<"deg to rad";
    //	qDebug("antes: radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
        radValue=Dms::degreeDecimalToRadiano(degValue);
		qDebug("depois: radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
		degMinSecLine->degreeDecimalToDms(degValue);
		setText(locale().toString(radValue,'f',getDecimals()));
        //setText(QString::number(Dms::degreeDecimalToRadiano(text().toDouble),'f',getDecimals()));

    }
    else if(getDmsEditMode()==RAD && newMode==DMS)
    {
        setText(degMinSecLine->radianoToDms(radValue)->toString(getDecimals()));
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
	DmsEditMode mode=getDmsEditMode();

	if (newValue=="")
	{
        //qDebug("update value chamado");
		if (mode==DMS)
        {
			setText(degMinSecLine->toString(getDecimals()));
            degValue=degMinSecLine->dmsToDegreeDecimal();
            radValue=degMinSecLine->dmsToRadiano();
        }
		else if (mode==DEG)
        {
			setText(QString::number(degValue,'f',5));
            degMinSecLine->degreeDecimalToDms(degValue);
            radValue=degMinSecLine->dmsToRadiano();
        }
		else if (mode==RAD)
        {
			setText(QString::number(radValue,'f',5));
            degMinSecLine->radianoToDms(radValue);
            degValue=degMinSecLine->dmsToDegreeDecimal();
        }
       // qDebug("radvalue =%f \tdegvalue= %f\tdms=%s",radValue,degValue,degMinSecLine->toString(5).toStdString().c_str());
        return;
	}
	bool ok;
	if (mode==DMS)
    {
        degMinSecLine->stringToDms(newValue);
		radValue=degMinSecLine->dmsToRadiano();
		degValue=degMinSecLine->dmsToDegreeDecimal();
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
