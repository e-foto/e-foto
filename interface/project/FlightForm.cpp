#include "FlightForm.h"
#include <qlayout.h>

FlightForm::FlightForm(QWidget *parent) : AbstractForm(parent)
{
        setupUi(this);
}

void FlightForm::fillvalues(string values)
{
        EDomElement ede(values);
        bool ok;

        flightKeySpinBox->setValue(QString::fromUtf8(ede.attribute("key").c_str()).toDouble(&ok));
        sensorKeySpinBox->setValue(QString::fromUtf8(ede.attribute("sensor_key").c_str()).toDouble(&ok));
        flightIDLineEdit->setText(QString::fromUtf8(ede.elementByTagName("flightId").toString().c_str()));
        flightHeightSpinBox->setValue(QString::fromUtf8(ede.elementByTagAtt("flightHeight","uom","#m").toString().c_str()).toDouble(&ok));
        horizontalOverlapDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("overlap").elementByTagAtt("longitudinal","uom","#%").toString().c_str()).toDouble(&ok));
        verticalOverlapDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("overlap").elementByTagAtt("transversal","uom","#%").toString().c_str()).toDouble(&ok));
        descriptionTextEdit->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
        producerNameLineEdit->setText(QString::fromUtf8(ede.elementByTagName("producerName").toString().c_str()));
        flightDateEdit->setDate(QDate::fromString(ede.elementByTagName("execution").toString().c_str(),Qt::ISODate));
        //Descobrir como se pega o denominador com o EDom
        nominalScaleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("nominalScale").elementByTagName("mml:frac").elementByTagName("mml:mn").toString().c_str()).toInt(&ok,10));

}

string FlightForm::getvalues()
{
    string xmlString;
    stringstream auxStream;

    auxStream << "<flights>\n";
    auxStream << "\t<flight key=\"" << (flightKeySpinBox->value()) <<"\" sensor_key=\""<<(sensorKeySpinBox->value())<<"\">\n";
    auxStream << "\t\t<flightId>"  << (flightIDLineEdit->text().toUtf8().data()) <<"</flightId>\n";
    auxStream << "\t\t<description>"  << (descriptionTextEdit->toPlainText().toUtf8().data()) <<"</description>\n";
    auxStream << "\t\t<execution>"  << (flightDateEdit->date().toString(Qt::ISODate).toUtf8().data()) <<"</execution>\n";
    auxStream << "\t\t<producerName>"  << (producerNameLineEdit->text().toUtf8().data()) <<"</producerName>\n";
    auxStream << "\t\t<nominalScale>\n";
    auxStream << "\t\t\t<mml:frac>\n";
    auxStream << "\t\t\t\t<mml:mn>1</mml:mn>\n";
    auxStream << "\t\t\t\t<mml:mn>" << nominalScaleSpinBox->value()<<"</mml:mn>\n";
    auxStream << "\t\t\t</mml:frac>\n";
    auxStream << "\t\t</nominalScale>\n";
    auxStream << "\t\t<flightHeight uom=\"#m\">" << flightHeightSpinBox->value()<<"</flightHeight>\n";
    auxStream << "\t\t<overlap>\n";
    auxStream << "\t\t\t<longitudinal uom=\"#%\">" << horizontalOverlapDoubleSpinBox->value()<<"</longitudinal>\n";
    auxStream << "\t\t\t<transversal uom=\"#%\">" << verticalOverlapDoubleSpinBox->value()<<"</transversal>\n";
    auxStream << "\t\t</overlap>\n";
    auxStream << "\t</flight>\n";
    auxStream << "</flights>";

    xmlString = auxStream.str();
    return xmlString;
}

void FlightForm::setReadOnly(bool state)
{
    flightKeySpinBox->setReadOnly(state);
    sensorKeySpinBox->setReadOnly(state);
    flightIDLineEdit->setReadOnly(state);
    flightHeightSpinBox->setReadOnly(state);
    horizontalOverlapDoubleSpinBox->setReadOnly(state);
    verticalOverlapDoubleSpinBox->setReadOnly(state);
    descriptionTextEdit->setReadOnly(state);
    producerNameLineEdit->setReadOnly(state);
    nominalScaleSpinBox->setReadOnly(state);
    flightDateEdit->setReadOnly(state);
}

bool FlightForm::isForm(string formName)
{
        return !formName.compare("FlightForm");
}
