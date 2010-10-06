#include "TerrainForm.h"

TerrainForm::TerrainForm(QWidget *parent):AbstractForm(parent)
{
    setupUi(this);
    connect(latDegreeSpin,SIGNAL(editingFinished() ),
                this,       SLOT( validatorLat()   )
            );
    connect(longDegreeSpin,SIGNAL(editingFinished() ),
                this,       SLOT( validatorLong()   )
			);
	connect(minAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt(double)));
	connect(maxAltSpinBox, SIGNAL(valueChanged(double)),this,SLOT(validatorAlt(double)));
}

void TerrainForm::fillvalues(string values)
{
    EDomElement ede(values);
	bool ok;//variavel 'dummy' para a função QString::toDouble(*bool)
	meanAltSpinBox->setValue( (QString::fromUtf8(ede.elementByTagAtt("meanAltitude","uom","#m").toString().c_str())).toDouble(&ok)
                             );

	minAltSpinBox->setValue( (QString::fromUtf8(ede.elementByTagAtt("minAltitude","uom","#m").toString().c_str())).toDouble(&ok)
                             );

	maxAltSpinBox->setValue( (QString::fromUtf8(ede.elementByTagAtt("maxAltitude","uom","#m").toString().c_str())).toDouble(&ok)
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
	latDegreeSpin->setValue( (QString::fromUtf8(lat.elementByTagName("degrees").toString().c_str())).toInt(&ok)
                             );

	latMinutesSpin->setValue( (QString::fromUtf8(lat.elementByTagName("minutes").toString().c_str())).toInt(&ok)
                             );

	latSecondSpin->setValue( (QString::fromUtf8(lat.elementByTagName("seconds").toString().c_str())).toDouble(&ok)
                             );

    //longitude
    EDomElement lon=ede.elementByTagName("Long");

    longDirCombo->setCurrentIndex(longDirCombo->findText(lon.attribute("direction").c_str()  ) ) ;
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
    auxStream << "\t<meanAltitude uom=\"#m\">" << (meanAltSpinBox->value()) <<"</meanAltitude>\n";
    auxStream << "\t<minAltitude uom=\"#m\">"  << (minAltSpinBox->value()) <<"</minAltitude>\n";
    auxStream << "\t<maxAltitude uom=\"#m\">" << (maxAltSpinBox->value()) <<"</maxAltitude>\n";
    auxStream << "\t<GRS>" << grsComboBox->currentText().toUtf8().data() << "</GRS>\n";
    auxStream << "\t<CPS>" << cpsComboBox->currentText().toUtf8().data() << "</CPS>\n";
    auxStream << "\t<workAreaCenterCoordinates>\n";

    auxStream << "\t\t<Lat direction=\"" << latDirCombo->currentText().toUtf8().data() <<"\">\n";
    auxStream << "\t\t\t<degrees>" << latDegreeSpin->value()<<"</degrees>\n";
    auxStream << "\t\t\t<minutes>" << latMinutesSpin->value()<<"</minutes>\n";
    auxStream << "\t\t\t<seconds>" << latSecondSpin->value()<<"</seconds>\n";
    auxStream << "\t\t</Lat>\n";

    auxStream << "\t\t<Long direction=\"" << longDirCombo->currentText().toUtf8().data() <<"\">\n";
    auxStream << "\t\t\t<degrees>" << longDegreeSpin->value()<<"</degrees>\n";
    auxStream << "\t\t\t<minutes>" << longMinutesSpin->value()<<"</minutes>\n";
    auxStream << "\t\t\t<seconds>" << longSecondSpin->value()<<"</seconds>\n";
    auxStream << "\t\t</Long>\n";

    auxStream << "\t\t<utmFuse>"<< utmFuseSpin->value()<<"</utmFuse>\n";
    auxStream << "\t</workAreaCenterCoordinates>\n";
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

bool TerrainForm::isForm(string formName)
{
	return !formName.compare("TerrainForm");
}
