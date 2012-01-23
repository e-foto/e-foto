#include "TerrainForm.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

TerrainForm::TerrainForm(QWidget *parent):AbstractForm(parent)
{
	setupUi(this);
	latDmsEdit->setDmsMaximum(90,0,0,false);
	latDmsEdit->setDmsMinimum(0,0,0,true);
	longDmsEdit->setDmsMaximum(180,0,0,false);
	longDmsEdit->setDmsMinimum(0,0,0,true);

	connect(minAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt()));
	connect(maxAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt()));
	connect(meanAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt()));
}

void TerrainForm::fillLatDir(string str)
{
	if (str == "N")
		latDirCombo->setCurrentIndex(0);
	else
		latDirCombo->setCurrentIndex(1);
}

void TerrainForm::fillLongDir(string str)
{
	if (str == "E")
		longDirCombo->setCurrentIndex(1);
	else
		longDirCombo->setCurrentIndex(0);
}

string TerrainForm::getLatDir()
{
	if (latDirCombo->currentIndex() == 0)
		return "N";
	else
		return "S";
}

string TerrainForm::getLongDir()
{
	if (longDirCombo->currentIndex() == 1)
		return "E";
	else
		return "W";
}

void TerrainForm::fillvalues(string values)
{
	cleanForm();
	EDomElement ede(values);
	bool ok;//variavel 'dummy' para a função QString::toDouble(*bool)

	minAltSpinBox->setValue( (QString::fromUtf8(ede.elementByTagAtt("minAltitude","uom","#m").toString().c_str())).toDouble(&ok)
							);

	maxAltSpinBox->setValue( (QString::fromUtf8(ede.elementByTagAtt("maxAltitude","uom","#m").toString().c_str())).toDouble(&ok)
							);

	meanAltSpinBox->setValue( (QString::fromUtf8(ede.elementByTagAtt("meanAltitude","uom","#m").toString().c_str())).toDouble(&ok)
							 );

	grsComboBox->setCurrentIndex(0);
	grsComboBox->setCurrentIndex(grsComboBox->findText(QString::fromUtf8(ede.elementByTagName("GRS").toString().c_str())));

	cpsComboBox->setCurrentIndex(0);
	cpsComboBox->setCurrentIndex(cpsComboBox->findText(QString::fromUtf8(ede.elementByTagName("CPS").toString().c_str())));

	utmFuseSpin->setValue((QString::fromUtf8(ede.elementByTagName("utmFuse").toString().c_str())).toInt(&ok)
						  );

	//latitude
	EDomElement lat=ede.elementByTagName("Lat");

	latDirCombo->setCurrentIndex(latDirCombo->findText(lat.attribute("direction").c_str() ) ) ;
	fillLatDir(lat.attribute("direction"));
	latDmsEdit->getDmsValue()->setDegree((QString::fromUtf8(lat.elementByTagName("degrees").toString().c_str())).toInt(&ok));
	latDmsEdit->getDmsValue()->setMinute((QString::fromUtf8(lat.elementByTagName("minutes").toString().c_str())).toInt(&ok));
	latDmsEdit->getDmsValue()->setSeconds((QString::fromUtf8(lat.elementByTagName("seconds").toString().c_str())).toDouble(&ok));
	latDmsEdit->updateValue();

	//longitude
	EDomElement lon=ede.elementByTagName("Long");

	longDirCombo->setCurrentIndex(longDirCombo->findText(lon.attribute("direction").c_str()  ) ) ;
	fillLongDir(lon.attribute("direction"));

	longDmsEdit->getDmsValue()->setDegree((QString::fromUtf8(lon.elementByTagName("degrees").toString().c_str())).toInt(&ok));
	longDmsEdit->getDmsValue()->setMinute((QString::fromUtf8(lon.elementByTagName("minutes").toString().c_str())).toInt(&ok));
	longDmsEdit->getDmsValue()->setSeconds((QString::fromUtf8(lon.elementByTagName("seconds").toString().c_str())).toDouble(&ok));
	longDmsEdit->updateValue();
}

string TerrainForm::getvalues()
{
	string xmlString;
	stringstream auxStream;

	auxStream << "<terrain>\n";
	auxStream << "<meanAltitude uom=\"#m\">" << (meanAltSpinBox->value()) <<"</meanAltitude>\n";
	auxStream << "<minAltitude uom=\"#m\">"  << (minAltSpinBox->value()) <<"</minAltitude>\n";
	auxStream << "<maxAltitude uom=\"#m\">" << (maxAltSpinBox->value()) <<"</maxAltitude>\n";
	auxStream << "<GRS>" << grsComboBox->currentText().toUtf8().data() << "</GRS>\n";
	auxStream << "<CPS>" << cpsComboBox->currentText().toUtf8().data() << "</CPS>\n";
	auxStream << "<workAreaCenterCoordinates>\n";

	//auxStream << "<Lat direction=\"" << latDirCombo->currentText().toUtf8().data() <<"\">\n";
	auxStream << "<Lat direction=\"" << getLatDir() <<"\">\n";
	auxStream << "<degrees>" << Conversion::intToString(latDmsEdit->getDmsValue()->getDegree())<<"</degrees>\n";
	auxStream << "<minutes>" << Conversion::intToString(latDmsEdit->getDmsValue()->getMinute())<<"</minutes>\n";
	auxStream << "<seconds>" << Conversion::doubleToString(latDmsEdit->getDmsValue()->getSeconds())<<"</seconds>\n";
	auxStream << "</Lat>\n";

	//auxStream << "<Long direction=\"" << longDirCombo->currentText().toUtf8().data() <<"\">\n";
	auxStream << "<Long direction=\"" << getLongDir() <<"\">\n";
	auxStream << "<degrees>" << Conversion::intToString(longDmsEdit->getDmsValue()->getDegree())<<"</degrees>\n";
	auxStream << "<minutes>" << Conversion::intToString(longDmsEdit->getDmsValue()->getMinute())<<"</minutes>\n";
	auxStream << "<seconds>" << Conversion::doubleToString(longDmsEdit->getDmsValue()->getSeconds())<<"</seconds>\n";
	auxStream << "</Long>\n";

	auxStream << "<utmFuse>"<< utmFuseSpin->value()<<"</utmFuse>\n";
	auxStream << "</workAreaCenterCoordinates>\n";
	auxStream << "</terrain>";

	xmlString = auxStream.str();
	return xmlString;
}

void TerrainForm::setReadOnly(bool state)
{
	meanAltSpinBox->setReadOnly(state);
	minAltSpinBox->setReadOnly(state);
	maxAltSpinBox->setReadOnly(state);
	grsComboBox->setDisabled(state);
	cpsComboBox->setDisabled(state);

	//latitude
	latDirCombo->setDisabled(state);
	latDmsEdit->setReadOnly(state);

	//longitude
	longDirCombo->setDisabled(state);
	longDmsEdit->setReadOnly(state);

	utmFuseSpin->setReadOnly(state);
}

void TerrainForm::validatorAlt()
{
	maxAltSpinBox->setMinimum(minAltSpinBox->value());
	meanAltSpinBox->setMinimum(minAltSpinBox->value());
	meanAltSpinBox->setMaximum(maxAltSpinBox->value());
	minAltSpinBox->setMaximum(maxAltSpinBox->value());
}

void TerrainForm::cleanForm()
{
	maxAltSpinBox->clear();
	minAltSpinBox->clear();
	meanAltSpinBox->clear();
	grsComboBox->setCurrentIndex(0);
	cpsComboBox->setCurrentIndex(0);
	utmFuseSpin->clear();
	latDmsEdit->clear();
	latDirCombo->setCurrentIndex(0);
	longDmsEdit->clear();
	longDirCombo->setCurrentIndex(0);
}

void TerrainForm::setFormLocale(QLocale locale)
{
	maxAltSpinBox->setLocale(locale);
	minAltSpinBox->setLocale(locale);
	meanAltSpinBox->setLocale(locale);
	utmFuseSpin->setLocale(locale);
	latDmsEdit->setLocale(locale);
	longDmsEdit->setLocale(locale);
}

bool TerrainForm::isForm(string formName)
{
	return !formName.compare("TerrainForm");
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
