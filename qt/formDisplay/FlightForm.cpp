/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "FlightForm.h"
#include <qlayout.h>

#include "EDom.h"
#include <sstream>


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

FlightForm::FlightForm(QWidget *parent) : AbstractForm(parent)
{
	setupUi(this);
}

void FlightForm::fillvalues(std::string values)
{
	cleanForm();
	EDomElement ede(values);
	bool ok;

    std::string edefrac;
	int index = (values.find("</mml:mn>")) + 9;
	edefrac = values.substr(index);
	EDomElement edomfrac(edefrac);

    flightIDLineEdit->setText(QString::fromUtf8(ede.elementByTagName("flightId").toString().c_str()));
    descriptionTextEdit->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
    producerNameLineEdit->setText(QString::fromUtf8(ede.elementByTagName("producerName").toString().c_str()));

    flightHeightDoubleSpinBox->setValue(0);
    horizontalOverlapDoubleSpinBox->setValue(4);
    verticalOverlapDoubleSpinBox->setValue(4);
    flightDateEdit->setDate(QDate(1900,1,1));
    nominalScaleSpinBox->setValue(99);
    flightHeightDoubleSpinBox->setSpecialValueText(" ");
    horizontalOverlapDoubleSpinBox->setSpecialValueText(" ");
    verticalOverlapDoubleSpinBox->setSpecialValueText(" ");
    flightDateEdit->setSpecialValueText(" ");
    nominalScaleSpinBox->setSpecialValueText(" ");

    if (ede.elementByTagAtt("flightHeight","uom","#m").getContent() != "")
        flightHeightDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagAtt("flightHeight","uom","#m").toString().c_str()).toDouble(&ok));
    if (ede.elementByTagName("overlap").elementByTagAtt("longitudinal","uom","#%").getContent() != "")
        horizontalOverlapDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("overlap").elementByTagAtt("longitudinal","uom","#%").toString().c_str()).toDouble(&ok));
    if (ede.elementByTagName("overlap").elementByTagAtt("transversal","uom","#%").getContent() != "")
        verticalOverlapDoubleSpinBox->setValue(QString::fromUtf8(ede.elementByTagName("overlap").elementByTagAtt("transversal","uom","#%").toString().c_str()).toDouble(&ok));
    if (ede.elementByTagName("execution").getContent() != "")
        flightDateEdit->setDate(QDate::fromString(ede.elementByTagName("execution").toString().c_str(),Qt::ISODate));
    if (edomfrac.elementByTagName("mml:mn").getContent() != "")
        nominalScaleSpinBox->setValue(QString::fromUtf8(edomfrac.elementByTagName("mml:mn").toString().c_str()).toInt(&ok,10));
}

std::string FlightForm::getvalues()
{
    std::stringstream auxStream;
    std::string xmlString;

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
	auxStream << "<flightHeight uom=\"#m\">" << Conversion::intToString(flightHeightDoubleSpinBox->value())<<"</flightHeight>\n";
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
	flightHeightDoubleSpinBox->setReadOnly(state);
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
	flightHeightDoubleSpinBox->clear();
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
	flightHeightDoubleSpinBox->setLocale(locale);
	nominalScaleSpinBox->setLocale(locale);
	horizontalOverlapDoubleSpinBox->setLocale(locale);
	verticalOverlapDoubleSpinBox->setLocale(locale);
}

bool FlightForm::isForm(std::string formName)
{
	return !formName.compare("FlightForm");
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
