#include "FlightForm.h"
#include <qlayout.h>

FlightForm::FlightForm(QWidget *parent) : AbstractForm(parent)
{
        setupUi(this);
}

void FlightForm::fillvalues(string values)
{
        cleanForm();
        EDomElement ede(values);
        bool ok;

        string edefrac;
        int index = (values.find("</mml:mn>")) + 9;
        edefrac = values.substr(index);
        EDomElement edomfrac(edefrac);

        flightIDLineEdit->setText(QString::fromUtf8(ede.elementByTagName("flightId").toString().c_str()));
		flightHeightSpinBox->setValue(QString::fromUtf8(ede.elementByTagAtt("flightHeight","uom","#m").toString().c_str()).toDouble(&ok));
		horizontalOverlapDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("overlap").elementByTagAtt("longitudinal","uom","#%").toString().c_str()).toDouble(&ok));
        verticalOverlapDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("overlap").elementByTagAtt("transversal","uom","#%").toString().c_str()).toDouble(&ok));
        descriptionTextEdit->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
        producerNameLineEdit->setText(QString::fromUtf8(ede.elementByTagName("producerName").toString().c_str()));
        flightDateEdit->setDate(QDate::fromString(ede.elementByTagName("execution").toString().c_str(),Qt::ISODate));
        nominalScaleSpinBox->setValue(QString::fromUtf8(edomfrac.elementByTagName("mml:mn").toString().c_str()).toInt(&ok,10));
}

string FlightForm::getvalues()
{
    stringstream auxStream;
    string xmlString;

    auxStream << "<flight key=\"1\" sensor_key=\"1\">\n";
	auxStream << "<flightId>"  << flightIDLineEdit->text().toUtf8().data() <<"</flightId>\n";
	auxStream << "<description>"  << descriptionTextEdit->toPlainText().toUtf8().data() <<"</description>\n";
	auxStream << "<execution>"  << flightDateEdit->date().toString(Qt::ISODate).toUtf8().data() <<"</execution>\n";
	auxStream << "<producerName>"  << producerNameLineEdit->text().toUtf8().data() <<"</producerName>\n";
	auxStream << "<nominalScale>\n";
	auxStream << "<mml:mfrac>\n";
	auxStream << "<mml:mn>1</mml:mn>\n";
        auxStream << "<mml:mn>" << Conversion::intToString(nominalScaleSpinBox->value()) <<"</mml:mn>\n";
	auxStream << "</mml:mfrac>\n";
	auxStream << "</nominalScale>\n";
        auxStream << "<flightHeight uom=\"#m\">" << Conversion::intToString(flightHeightSpinBox->value())<<"</flightHeight>\n";
	auxStream << "<overlap>\n";
        auxStream << "<longitudinal uom=\"#%\">" << Conversion::doubleToString(horizontalOverlapDoubleSpinBox->value())<<"</longitudinal>\n";
        auxStream << "<transversal uom=\"#%\">" << Conversion::doubleToString(verticalOverlapDoubleSpinBox->value())<<"</transversal>\n";
	auxStream << "</overlap>\n";
	auxStream << "</flight>\n";

    xmlString = auxStream.str();
    return xmlString.c_str();
}

void FlightForm::setReadOnly(bool state)
{
    flightIDLineEdit->setReadOnly(state);
    flightHeightSpinBox->setReadOnly(state);
    horizontalOverlapDoubleSpinBox->setReadOnly(state);
    verticalOverlapDoubleSpinBox->setReadOnly(state);
    descriptionTextEdit->setReadOnly(state);
    producerNameLineEdit->setReadOnly(state);
    nominalScaleSpinBox->setReadOnly(state);
    flightDateEdit->setReadOnly(state);
}

void FlightForm::cleanForm()
{
    flightIDLineEdit->clear();
    producerNameLineEdit->clear();
    descriptionTextEdit->clear();
    flightDateEdit->clear();
    flightHeightSpinBox->clear();
    nominalScaleSpinBox->clear();
    horizontalOverlapDoubleSpinBox->clear();
    verticalOverlapDoubleSpinBox->clear();
}

void FlightForm::setFormLocale(QLocale locale)
{
    flightIDLineEdit->setLocale(locale);
    producerNameLineEdit->setLocale(locale);
    descriptionTextEdit->setLocale(locale);
    flightDateEdit->setLocale(locale);
    flightHeightSpinBox->setLocale(locale);
    nominalScaleSpinBox->setLocale(locale);
    horizontalOverlapDoubleSpinBox->setLocale(locale);
    verticalOverlapDoubleSpinBox->setLocale(locale);
}

bool FlightForm::isForm(string formName)
{
        return !formName.compare("FlightForm");
}
