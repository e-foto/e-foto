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
	setDecimals(2);
    setText(degMinSecLine->toString());
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
		degMinSecLine=Dms::stringToDms(this->text());

        if(degMinSecLine->isValid())
        {
           setText(degMinSecLine->toString(getDecimals()));
        }
        else
        {
           setText(QString::fromUtf8("0°0'0.0\""));
        }
    }
    else if (getDmsEditMode()==RAD)
    {
//		QString decimalPnt=QLocale::decimalPoint();
		QString radVal="(-)?[0-6]{1}(\\.|,)\\d{";

	//	radVal.append();
		radVal.append(QString::number(getDecimals()));
        radVal.append("}");

	   // qDebug()<<radVal;
        QRegExpValidator *radValidator1=new QRegExpValidator(QRegExp(radVal),this);
		radValidator1->setLocale(QLocale::system());
		//radValidator1->locale().setNumberOptions(QLocale::O);
		//qDebug()<<radValidator1->locale().name()<<"simbolo decimal: "<<radValidator1->locale().decimalPoint();

        this->setValidator(radValidator1);
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
		degMinSecLine=Dms::stringToDms(text());
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
		degMinSecLine=Dms::stringToDms(text());
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


DmsEditMode DmsEdit::getDmsEditMode()
{
    return mode;
}

void DmsEdit::setDmsEditMode(DmsEditMode newMode)
{
    bool ok;
    if (getDmsEditMode()==DMS && newMode==RAD)
    {
        radValue=degMinSecLine->dmsToRadiano();
		setText(locale().toString(radValue,'f',getDecimals()));
        //setText(QString::number(degMinSecLine->dmsToRadiano(),'f',getDecimals()));
        //qDebug()<<"dms to rad";
    }
    else if(getDmsEditMode()==DMS && newMode==DEG)
    {
        degValue=degMinSecLine->dmsToDegreeDecimal();
		setText(locale().toString(degValue,'f',getDecimals()));
        //setText(QString::number(degMinSecLine->dmsToDegreeDecimal(),'f',getDecimals()));
        //qDebug()<<"dms to deg";
    }
    else if(getDmsEditMode()==DEG && newMode==DMS)
    {
		degMinSecLine->degreeDecimalToDms(degValue);
		setText(degMinSecLine->toString(getDecimals()));
        //setText(degMinSecLine->degreeDecimalToDms(text().toDouble(&ok))->toString(getDecimals()));
        //qDebug()<<"deg to dms";
    }
    else if(getDmsEditMode()==DEG && newMode==RAD)
    {

        radValue=Dms::degreeDecimalToRadiano(degValue);
		setText(locale().toString(radValue,'f',getDecimals()));
        //setText(QString::number(Dms::degreeDecimalToRadiano(text().toDouble),'f',getDecimals()));
        //qDebug()<<"deg to rad";
    }
    else if(getDmsEditMode()==RAD && newMode==DMS)
    {
		degMinSecLine->radianoToDms(radValue);
		setText(degMinSecLine->toString(getDecimals()));
        //setText(degMinSecLine->radianoToDms(text().toDouble(&ok))->toString(getDecimals()));
       // qDebug()<<"rad to dms";
    }
    else if(getDmsEditMode()==RAD && newMode==DEG)
    {
        degValue=Dms::radianoToDegreeDecimal(radValue);
		setText(locale().toString(degValue,'f',getDecimals()));
        //setText(QString::number(Dms::radianoToDegreeDecimal(text().toDouble(&ok)),'f',getDecimals()));
        //qDebug()<<"rad to dms";
    }
    mode=newMode;
    validate();
}

void DmsEdit::updateValue(QString newValue)
{
   // qDebug("update value chamado");
    bool ok;
    if (getDmsEditMode()==DMS)
    {
		degMinSecLine=Dms::stringToDms(newValue);
    }
    else if(getDmsEditMode()==RAD)
    {
        radValue=newValue.toDouble(&ok);
    }
    else if(getDmsEditMode()==DEG)
    {
        degValue=newValue.toDouble(&ok);
    }
}

double DmsEdit::getRadianValue()
{
	return degMinSecLine->dmsToRadiano();
}

Dms* DmsEdit::getDmsValue()
{
	return degMinSecLine;
}

void DmsEdit::setNewDms(Dms *newDegMinSec)
{
	if(newDegMinSec->isValid())
		degMinSecLine=newDegMinSec;
}
