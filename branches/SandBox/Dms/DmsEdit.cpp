#include "DmsEdit.h"
#include <QDebug>

DmsEdit::DmsEdit(QWidget *parent):
    QLineEdit(parent)
{
    degMinSecLine= new Dms("012 34 45.6789");
    setText(degMinSecLine->toString());

 //   validatorDegree = new QRegExpValidator(QRegExp("+?\\d{3}\\*\\d{2}\\'\\d{2}(?,\\d{3})\\\""),this);

    installEventFilter(this);
    setDecimals(3);

    connect(this,SIGNAL(textEdited(QString)), this, SLOT(validate(QString)));
    connect(this,SIGNAL(cursorPositionChanged(int,int)), this,SLOT(changedField(int,int)));
}

void DmsEdit::setDecimals(int newDecimals)
{
    decimals=( newDecimals >=0 ? newDecimals : 0);
}

int DmsEdit::getDecimals()
{
    return decimals;
}

void DmsEdit::validate(QString value)
{
    int posCursor= this->cursorPosition();
    Dms *temp=Dms::stringToDms(value);

    if(temp->isValid())
    {
        //qDebug()<<"Acceptable";
        this->setText(temp->toString());
    }
    else
    {
        //qDebug()<<"Unacceptable";
        this->setText(QString::fromUtf8("0°0'0.0\""));
    }

    this->setCursorPosition(posCursor);
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
    Dms *temp=Dms::stringToDms(text());
    if (positionValue()==DEGREES)
    {
        temp->setDegree(temp->getDegree()-1);
        setText(temp->toString());
    }
    else if (positionValue()==MINUTES)
    {
        temp->setMinute(temp->getMinute()-1);
        setText(temp->toString());
    }
    else if (positionValue()==SECONDS)
    {
        temp->setSeconds(temp->getSeconds()-1);
        setText(temp->toString());
    }

        // qDebug("stepDown foi chamado steps -1");
}

void DmsEdit::stepUp()
{

    Dms *temp=Dms::stringToDms(text());
    if (positionValue()==DEGREES)
    {
        temp->setDegree(temp->getDegree()+1);
        setText(temp->toString());
    }
    else if (positionValue()==MINUTES)
    {
        temp->setMinute(temp->getMinute()+1);
        setText(temp->toString());
    }
    else if (positionValue()==SECONDS)
    {
        temp->setSeconds(temp->getSeconds()+1);
        setText(temp->toString());
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
    PositionValue oldValue=positionValue(oldPos);
    PositionValue newValue=positionValue(newPos);

    if (oldValue==DEGREES && newValue==MINUTES)
    {
        direction=true;
        selectField(MINUTES);
    }else if (oldValue==DEGREES && newValue==SECONDS)
    {
        direction=true;
        selectField(SECONDS);
    }else if (oldValue==MINUTES && newValue==DEGREES)
    {
        direction=false;
        selectField(DEGREES);
    }else if (oldValue==MINUTES && newValue==SECONDS)
    {
        direction=true;
        selectField(SECONDS);
    }else if (oldValue==SECONDS && newValue==MINUTES)
    {
        direction=false;
        selectField(MINUTES);
    }else if (oldValue==SECONDS && newValue==DEGREES)
    {
        direction=false;
        selectField(DEGREES);
    }
    //setCursorPosition(newPos);
}

void DmsEdit::selectField(PositionValue pos)
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
    //setCursorPosition(oldPos);
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

void DmsEdit::focusInEvent(QFocusEvent *evento)
{
    if (evento->reason() & Qt::TabFocusReason)
    {/*
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
            QLineEdit::focusInEvent(evento);
        }*/
        selectField(DEGREES);
    }else if(evento->reason() & Qt::BacktabFocusReason)
    {/*
        if (positionValue()==MINUTES)
        {
            selectField(DEGREES);
        }
        else if (positionValue()==SECONDS)
        {
            selectField(MINUTES);
        }
        else if (positionValue()==SECONDS)
        {
            QLineEdit::focusInEvent(evento);
        }*/
        selectField(SECONDS);
    }


}
