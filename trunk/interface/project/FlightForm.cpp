#include "FlightForm.h"
#include <qlayout.h>

FlightForm::FlightForm(QWidget *parent, string XmlString)
{
    QVBoxLayout* myLayout = new QVBoxLayout(this);
    pflightview = new FlightView(this);
	pflightview->fillvalues(XmlString);
    myLayout->addWidget(pflightview);
}

void FlightForm::fillvalues(string values)
{
    pflightview->fillvalues(values);
}

string FlightForm::getvalues()
{
    return pflightview->getvalues();
}

void FlightForm::setReadOnly(bool state)
{
    pflightview->setReadOnly(state);
}
FlightForm::~FlightForm()
{
    delete pflightview;
}

bool FlightForm::isForm(string formName)
{
}
