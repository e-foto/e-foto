#include "TerrainForm.h"

TerrainForm::TerrainForm(QWidget *parent):AbstractForm(parent)
{
    setupUi(this);
    connect(latDegreeSpin,SIGNAL(editingFinished() ),this,SLOT( validatorLat()));
    connect(longDegreeSpin,SIGNAL(editingFinished() ),this,SLOT( validatorLong()));
    connect(minAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt(double)));
    connect(maxAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt(double)));
    connect(meanAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt(double)));
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

	//latDirCombo->setCurrentIndex(latDirCombo->findText(lat.attribute("direction").c_str() ) ) ;
	fillLatDir(lat.attribute("direction"));
	latDegreeSpin->setValue( (QString::fromUtf8(lat.elementByTagName("degrees").toString().c_str())).toInt(&ok)
                             );

	latMinutesSpin->setValue( (QString::fromUtf8(lat.elementByTagName("minutes").toString().c_str())).toInt(&ok)
                             );

	latSecondSpin->setValue( (QString::fromUtf8(lat.elementByTagName("seconds").toString().c_str())).toDouble(&ok)
                             );

    //longitude
    EDomElement lon=ede.elementByTagName("Long");

	//longDirCombo->setCurrentIndex(longDirCombo->findText(lon.attribute("direction").c_str()  ) ) ;
	fillLongDir(lon.attribute("direction"));
	longDegreeSpin->setValue( (QString::fromUtf8(lon.elementByTagName("degrees").toString().c_str())).toInt(&ok)
            );
	longMinutesSpin->setValue( (QString::fromUtf8(lon.elementByTagName("minutes").toString().c_str())).toInt(&ok)
            );
	longSecondSpin->setValue((QString::fromUtf8(lon.elementByTagName("seconds").toString().c_str())).toDouble(&ok)
                             );

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
	auxStream << "<degrees>" << latDegreeSpin->value()<<"</degrees>\n";
	auxStream << "<minutes>" << latMinutesSpin->value()<<"</minutes>\n";
	auxStream << "<seconds>" << latSecondSpin->value()<<"</seconds>\n";
	auxStream << "</Lat>\n";

	//auxStream << "<Long direction=\"" << longDirCombo->currentText().toUtf8().data() <<"\">\n";
	auxStream << "<Long direction=\"" << getLongDir() <<"\">\n";
	auxStream << "<degrees>" << longDegreeSpin->value()<<"</degrees>\n";
	auxStream << "<minutes>" << longMinutesSpin->value()<<"</minutes>\n";
	auxStream << "<seconds>" << longSecondSpin->value()<<"</seconds>\n";
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
    latDegreeSpin->setReadOnly(state);
    latMinutesSpin->setReadOnly(state);
    latSecondSpin->setReadOnly(state);

    //longitude
    longDirCombo->setDisabled(state);
    longDegreeSpin->setReadOnly(state);
    longMinutesSpin->setReadOnly(state);
    longSecondSpin->setReadOnly(state);

    utmFuseSpin->setReadOnly(state);
}

void TerrainForm::validatorLat()
{
    if(latDegreeSpin->value()==90){
        latMinutesSpin->setMaximum(0);
        latSecondSpin->setMaximum(0);
    }
    else{
        latMinutesSpin->setMaximum(59);
        latSecondSpin->setMaximum(59.9999);
    }
}

void TerrainForm::validatorLong()
{
    if(longDegreeSpin->value()==180){
        longMinutesSpin->setMaximum(0);
        longSecondSpin->setMaximum(0);
    }
    else{
        longMinutesSpin->setMaximum(59);
        longSecondSpin->setMaximum(59.9999);
    }
}

void TerrainForm::validatorAlt(double)
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
    latDegreeSpin->clear();
    latMinutesSpin->clear();
    latSecondSpin->clear();
    latDirCombo->setCurrentIndex(0);
    longDegreeSpin->clear();
    longMinutesSpin->clear();
    longSecondSpin->clear();
    longDirCombo->setCurrentIndex(0);
}

void TerrainForm::setFormLocale(QLocale locale)
{
    maxAltSpinBox->setLocale(locale);
    minAltSpinBox->setLocale(locale);
    meanAltSpinBox->setLocale(locale);
    utmFuseSpin->setLocale(locale);
    latDegreeSpin->setLocale(locale);
    latMinutesSpin->setLocale(locale);
    latSecondSpin->setLocale(locale);
    longDegreeSpin->setLocale(locale);
    longMinutesSpin->setLocale(locale);
    longSecondSpin->setLocale(locale);
}

bool TerrainForm::isForm(string formName)
{
	return !formName.compare("TerrainForm");
}
