#include "DegreeLineEdit.h"
#include <QDebug>

DegreeLineEdit::DegreeLineEdit(QWidget *parent):
    QLineEdit(parent)
{
	degMinSecLine= new Dms("123 45 10.0");
    setText(degMinSecLine->toString());

    setInputMask("009*09'dd,DDDD\"");
    validatorDegree = new QRegExpValidator(QRegExp("+?\\d{3,0}\\*\\d{2,0}\\'\\d{2}(,\\d{3})?\\\""),this);
    //setValidator(validatorDegree);


    connect(this,SIGNAL(textEdited(QString)), this, SLOT(validate(QString)));
}

void DegreeLineEdit::validate(QString value)
{
    //QString value=text();
    int pos= value.length();

    if(validatorDegree->validate(value,pos)==QValidator::Acceptable)
    {
        qDebug()<<"acceptable";
    }
    if(validatorDegree->validate(value,pos)==QValidator::Intermediate)
    {
        qDebug()<<"intermadiate";
    }
    if(validatorDegree->validate(value,pos)==QValidator::Invalid)
    {
        qDebug()<<"invalid";
    }

}


