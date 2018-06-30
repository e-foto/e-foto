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

#include "SensorForm.h"

#include <QtGui>
#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SensorForm::SensorForm(QWidget *parent): AbstractForm(parent)
{
	setupUi(this);
	setFormLocale(QLocale(QLocale::system()));
	cameraDispatchDateEdit->setDate(QDate().currentDate());

	calibratedSigmaController = new SigmaFormController("Not Available",1);
	calibratedSigmaSelector->setSigmaFormController(calibratedSigmaController);
	calibratedSigmaSelector->blockCovarianceMatrixOption();
	calibratedSigmaContent->setSigmaFormController(calibratedSigmaController);

	fid0SigmaController = new SigmaFormController("Not Available",2);
	fid0SigmaContent->setSigmaFormController(fid0SigmaController);

	fid1SigmaController = new SigmaFormController("Not Available",2);
	fid1SigmaContent->setSigmaFormController(fid1SigmaController);

	fid2SigmaController = new SigmaFormController("Not Available",2);
	fid2SigmaContent->setSigmaFormController(fid2SigmaController);

	fid3SigmaController = new SigmaFormController("Not Available",2);
	fid3SigmaContent->setSigmaFormController(fid3SigmaController);

	fid4SigmaController = new SigmaFormController("Not Available",2);
	fid4SigmaContent->setSigmaFormController(fid4SigmaController);

	fid5SigmaController = new SigmaFormController("Not Available",2);
	fid5SigmaContent->setSigmaFormController(fid5SigmaController);

	fid6SigmaController = new SigmaFormController("Not Available",2);
	fid6SigmaContent->setSigmaFormController(fid6SigmaController);

	fid7SigmaController = new SigmaFormController("Not Available",2);
	fid7SigmaContent->setSigmaFormController(fid7SigmaController);

	fidSigmaSelector->setSigmaFormController(fid0SigmaController);
	fidSigmaSelector->setSigmaFormController(fid1SigmaController);
	fidSigmaSelector->setSigmaFormController(fid2SigmaController);
	fidSigmaSelector->setSigmaFormController(fid3SigmaController);
	fidSigmaSelector->setSigmaFormController(fid4SigmaController);
	fidSigmaSelector->setSigmaFormController(fid5SigmaController);
	fidSigmaSelector->setSigmaFormController(fid6SigmaController);
	fidSigmaSelector->setSigmaFormController(fid7SigmaController);
	fidSigmaSelector->blockCovarianceMatrixOption();

	radialSigmaController = new SigmaFormController("Not Available",4);
	radialSigmaSelector->setSigmaFormController(radialSigmaController);
	radialSigmaSelector->blockCovarianceMatrixOption();
	radialSigmaContent->setSigmaFormController(radialSigmaController);

	decenteredSigmaController = new SigmaFormController("Not Available",2);
	decenteredSigmaSelector->setSigmaFormController(decenteredSigmaController);
	decenteredSigmaSelector->blockCovarianceMatrixOption();
	decenteredSigmaContent->setSigmaFormController(decenteredSigmaController);

	principalSigmaController = new SigmaFormController("Not Available",2);
	principalSigmaSelector->setSigmaFormController(principalSigmaController);
	principalSigmaSelector->blockCovarianceMatrixOption();
	principalSigmaContent->setSigmaFormController(principalSigmaController);

	//    QRegExp regExp("[-]?\\d{1,4}([.]\\d{1,5})?([(e|E)][-]\\d{1,2}$)?");
	//    QRegExpValidator *validator= new QRegExpValidator(regExp,this);

	radialK0LineEdit->setTextValue("");
	radialK1LineEdit->setTextValue("");
	radialK2LineEdit->setTextValue("");

	//oldSensor
	//connect(detectorCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(detectorGroup(int)));
	connect(fiducialMarksCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(fiducialGroup(int)));

	//newSensor
	connect(calcModelComboBox, SIGNAL (currentIndexChanged(int)), this, SLOT(calculationMode(int)));
	connect(sensorColsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSensorDiagonal()));
	connect(sensorRowsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSensorDiagonal()));
	connect(pixelSizeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSensorDiagonal()));

}

void SensorForm::fillvalues(std::string values)
{

	clearForm();
	ede.setContent(values);
	bool ok;

	sensorIdLineEdit->setText(QString::fromUtf8(ede.elementByTagName("sensorId").toString().c_str()));

	geometryCombo->setCurrentIndex(geometryCombo->findText(QString::fromUtf8(ede.elementByTagName("geometry").toString().c_str())));
	platformCombo->setCurrentIndex(platformCombo->findText(QString::fromUtf8(ede.elementByTagName("platform").toString().c_str())));
	detectorCombo->setCurrentIndex(detectorCombo->findText(QString::fromUtf8(ede.elementByTagName("detector").toString().c_str())));
	energyCombo->setCurrentIndex(energyCombo->findText(QString::fromUtf8(ede.elementByTagName("energySource").toString().c_str())));

	if (ede.hasTagName("calculationMode"))
	{
		calcModelComboBox->setCurrentIndex(calcModelComboBox->findText(QString::fromUtf8(ede.elementByTagName("calculationMode").toString().c_str()))); //newSensor
		calculationMode(calcModelComboBox->currentIndex());
	}else
	{
		calcModelComboBox->setCurrentIndex(0); //newSensor
		calculationMode(calcModelComboBox->currentIndex());
	}

	descriptionTextEdit->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
	cameraNumberLineEdit->setText(QString::fromUtf8(ede.elementByTagName("number").toString().c_str()));

    cameraDispatchDateEdit->setDate(QDate(1900,1,1));
    cameraExpirationDateEdit->setDate(QDate(1900,1,1));
    calibratedFocalDistanceDoubleSpin->setValue(0);
    cameraDispatchDateEdit->setSpecialValueText(" ");
    cameraExpirationDateEdit->setSpecialValueText(" ");
    calibratedFocalDistanceDoubleSpin->setSpecialValueText(" ");

    if (ede.elementByTagName("dispatch").getContent() != "")
        cameraDispatchDateEdit->setDate(QDate::fromString(QString::fromUtf8(ede.elementByTagName("dispatch").toString().c_str()),Qt::ISODate));
    if (ede.elementByTagName("expiration").getContent() != "")
        cameraExpirationDateEdit->setDate(QDate::fromString(QString::fromUtf8(ede.elementByTagName("expiration").toString().c_str()),Qt::ISODate));
    if (ede.elementByTagName("value").getContent() != "")
        calibratedFocalDistanceDoubleSpin->setValue(QString::fromUtf8(ede.elementByTagName("value").toString().c_str()).toDouble(&ok));
	calibratedSigmaController->fillValues(ede.elementByTagName("sigma").getContent());

	EDomElement k;
	EDomElement cons;

	if(ede.hasTagName("radialSymmetric"))
	{
		radialAvailable=true;
		cons=ede.elementByTagName("radialSymmetric");
		if (cons.attribute("considered")=="true")
			radSymmetricModeComboBox->setCurrentIndex(0);
		else
			radSymmetricModeComboBox->setCurrentIndex(1);

		k=ede.elementByTagName("k0");
		radialK0LineEdit->setTextValue(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));

		k=ede.elementByTagName("k1");
		radialK1LineEdit->setTextValue(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));

		k=ede.elementByTagName("k2");
		radialK2LineEdit->setTextValue(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));

		k=ede.elementByTagName("k3");
		radialK3LineEdit->setTextValue(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));

		radialSigmaController->fillValues(ede.elementByTagName("radialSymmetric").elementByTagName("sigma").getContent());
	}
	else
	{
		radialAvailable=false;
	}

	if(ede.hasTagName("decentered"))
	{
		decenteredAvailable= true;
		cons=ede.elementByTagName("decentered");
		if (cons.attribute("considered")=="true")
			decenteredModeComboBox->setCurrentIndex(0);
		else
			decenteredModeComboBox->setCurrentIndex(1);

		k=ede.elementByTagName("P1");
		decenteredP1LineEdit->setTextValue(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));

		k=ede.elementByTagName("P2");
		decenteredP2LineEdit->setTextValue(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));

		decenteredSigmaController->fillValues(ede.elementByTagName("decentered").elementByTagName("sigma").getContent());
	}
	else
	{
		decenteredAvailable=false;
	}

	//new Sensor



	/* ainda sem implementação dos campos de tag <principalPointCoordinate>
//   tags interiores como mesmo nome "mml:cn"

	EDomElement pri=ede.elementByTagName("principalPointCoordinate");
	//string minha = pri.getContent();
*/ //Feito a seguir:

	EDomElement pri =  ede.elementByTagName("principalPointCoordinates");
	if (pri.elementByTagName("gml:pos").toString() != "")
	{
		principalX0doubleSpin->setValue(QString::fromUtf8(pri.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
		principalY0doubleSpin->setValue(QString::fromUtf8(pri.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
	}
	else
	{
        principalX0doubleSpin->setValue(0);
		principalY0doubleSpin->setValue(0);
	}
	principalSigmaController->fillValues(pri.elementByTagName("sigma").getContent());

	if (calcModelComboBox->currentIndex() == 0)
	{
		EDomElement fids = ede.elementByTagName("fiducialMarks");
		if (fids.children().size()==0)
		{
			fiducialMarksCombo->setCurrentIndex(0);
			fiducialGroup(fiducialMarksCombo->currentIndex());
		}
		else
		{
			fiducialMarksCombo->setCurrentIndex(fiducialMarksCombo->findText(QString::number(fids.children().size())+" marks"));
			fiducialGroup(fiducialMarksCombo->currentIndex());
		}

		EDomElement fid = ede.elementByTagAtt("fiducialMark","key","1");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiducialXi0doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiducialEta0doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid0SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiducialMark","key","2");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiducialXi1doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiducialEta1doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid1SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiducialMark","key","3");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiducialXi2doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiducialEta2doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid2SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiducialMark","key","4");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiducialXi3doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiducialEta3doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid3SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		if (fiducialMarksCombo->currentIndex()==1){
			fiducialGroup(fiducialMarksCombo->currentIndex());
			fid =  ede.elementByTagAtt("fiducialMark","key","5");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiducialXi4doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiducialEta4doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid4SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiducialMark","key","6");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiducialXi5doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiducialEta5doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid5SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiducialMark","key","7");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiducialXi6doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiducialEta6doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid6SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiducialMark","key","8");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiducialXi7doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiducialEta7doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid7SigmaController->fillValues(fid.elementByTagName("sigma").getContent());
		}

		xaA0SpinBox->setValue(0);
		xaA1SpinBox->setValue(0);
		xaA2SpinBox->setValue(0);
		xaB0SpinBox->setValue(0);
		xaB1SpinBox->setValue(0);
		xaB2SpinBox->setValue(0);
		pixelSizeDoubleSpinBox->setValue(0);
		sensorRowsSpinBox->setValue(0);
		sensorColsSpinBox->setValue(0);
		updateSensorDiagonal();

	}
	else if (calcModelComboBox->currentIndex() == 1)
	{
		EDomElement sensorDimensions= ede.elementByTagName("sensorDimensions");
		pixelSizeDoubleSpinBox->setValue(QString::fromUtf8(sensorDimensions.elementByTagName("pixelSize").toString().c_str()).toDouble());
		sensorRowsSpinBox->setValue(QString::fromUtf8(sensorDimensions.elementByTagName("rows").toString().c_str()).toDouble());
		sensorColsSpinBox->setValue(QString::fromUtf8(sensorDimensions.elementByTagName("columns").toString().c_str()).toDouble());

		xaA0SpinBox->setValue(0);
		xaA1SpinBox->setValue(0);
		xaA2SpinBox->setValue(0);
		xaB0SpinBox->setValue(0);
		xaB1SpinBox->setValue(0);
		xaB2SpinBox->setValue(0);
		fiducialXi0doubleSpin->setValue(0);
		fiducialEta0doubleSpin->setValue(0);
		fid0SigmaController->fillValues("");
		fiducialXi1doubleSpin->setValue(0);
		fiducialEta1doubleSpin->setValue(0);
		fid1SigmaController->fillValues("");
		fiducialXi2doubleSpin->setValue(0);
		fiducialEta2doubleSpin->setValue(0);
		fid2SigmaController->fillValues("");
		fiducialXi3doubleSpin->setValue(0);
		fiducialEta3doubleSpin->setValue(0);
		fid3SigmaController->fillValues("");
		fiducialXi4doubleSpin->setValue(0);
		fiducialEta4doubleSpin->setValue(0);
		fid4SigmaController->fillValues("");
		fiducialXi5doubleSpin->setValue(0);
		fiducialEta5doubleSpin->setValue(0);
		fid5SigmaController->fillValues("");
		fiducialXi6doubleSpin->setValue(0);
		fiducialEta6doubleSpin->setValue(0);
		fid6SigmaController->fillValues("");
		fiducialXi7doubleSpin->setValue(0);
		fiducialEta7doubleSpin->setValue(0);
		fid7SigmaController->fillValues("");
	}
	else if (calcModelComboBox->currentIndex() == 2)
	{
		EDomElement xa =  ede.elementByTagName("Xa");
		xaA0SpinBox->setValue(xa.elementByTagName("a0").toDouble());
		xaA1SpinBox->setValue(xa.elementByTagName("a1").toDouble());
		xaA2SpinBox->setValue(xa.elementByTagName("a2").toDouble());
		xaB0SpinBox->setValue(xa.elementByTagName("b0").toDouble());
		xaB1SpinBox->setValue(xa.elementByTagName("b1").toDouble());
		xaB2SpinBox->setValue(xa.elementByTagName("b2").toDouble());

		pixelSizeDoubleSpinBox->setValue(0);
		sensorRowsSpinBox->setValue(0);
		sensorColsSpinBox->setValue(0);
		updateSensorDiagonal();
		fiducialXi0doubleSpin->setValue(0);
		fiducialEta0doubleSpin->setValue(0);
		fid0SigmaController->fillValues("");
		fiducialXi1doubleSpin->setValue(0);
		fiducialEta1doubleSpin->setValue(0);
		fid1SigmaController->fillValues("");
		fiducialXi2doubleSpin->setValue(0);
		fiducialEta2doubleSpin->setValue(0);
		fid2SigmaController->fillValues("");
		fiducialXi3doubleSpin->setValue(0);
		fiducialEta3doubleSpin->setValue(0);
		fid3SigmaController->fillValues("");
		fiducialXi4doubleSpin->setValue(0);
		fiducialEta4doubleSpin->setValue(0);
		fid4SigmaController->fillValues("");
		fiducialXi5doubleSpin->setValue(0);
		fiducialEta5doubleSpin->setValue(0);
		fid5SigmaController->fillValues("");
		fiducialXi6doubleSpin->setValue(0);
		fiducialEta6doubleSpin->setValue(0);
		fid6SigmaController->fillValues("");
		fiducialXi7doubleSpin->setValue(0);
		fiducialEta7doubleSpin->setValue(0);
		fid7SigmaController->fillValues("");
	}

}

std::string SensorForm::getvalues()
{
    std::string xmlString;
    std::stringstream auxStream;

	auxStream << "<sensor key=\"" << ede.attribute("key").data() << "\">\n";
	auxStream << "<sensorId>" << sensorIdLineEdit->text().toUtf8().data() <<"</sensorId>\n";
	auxStream << "<type>\n" ;
	auxStream << "<geometry>" << geometryCombo->currentText().toUtf8().data() <<"</geometry>\n";
	auxStream << "<platform>" << platformCombo->currentText().toUtf8().data() <<"</platform>\n";
	auxStream << "<detector>" << detectorCombo->currentText().toUtf8().data() <<"</detector>\n";
	auxStream << "<energySource>"<< energyCombo->currentText().toUtf8().data() <<"</energySource>\n";
	auxStream << "<calculationMode>"<< calcModelComboBox->currentText().toUtf8().data() <<"</calculationMode>\n"; //new form sensor
	auxStream << "<spectralRanges uom=\"#um\"><!--#um equivale a dez elevado a menos seis metros-->\n";
	auxStream << "<spectralRange band=\"1\">\n"; // aqui tem um valor travado
	auxStream << "<inferiorLimit>0.4</inferiorLimit>\n"; // aqui tem um valor travado
	auxStream << "<superiorLimit>0.7</superiorLimit>\n"; // aqui tem um valor travado
	auxStream << "</spectralRange>\n";
	auxStream << "</spectralRanges>\n";
	auxStream << "</type>\n";
	auxStream << "<description>" << descriptionTextEdit->toPlainText().toUtf8().data() << "</description>\n";
	auxStream << "<calibrationCertificate>\n" ;
	auxStream << "<number>" << cameraNumberLineEdit->text().toUtf8().data() << "</number>\n";
	auxStream << "<dispatch>"<<cameraDispatchDateEdit->dateTime().toString(Qt::ISODate).toUtf8().data() << "</dispatch>\n";
	auxStream << "<expiration>"<<cameraExpirationDateEdit->dateTime().toString(Qt::ISODate).toUtf8().data() << "</expiration>\n";
	auxStream << "</calibrationCertificate>\n";
	auxStream << "<focalDistance uom=\"#mm\">\n";
	auxStream << "<value>" << calibratedFocalDistanceDoubleSpin->value() << "</value>\n";
	//auxStream << "<sigma>" << calibratedSigmaDoubleSpin->value() << "</sigma>\n";
	auxStream << calibratedSigmaController->getValues();
	auxStream << "</focalDistance>\n";
	auxStream << "<distortionCoefficients>\n";
	//new form sensor
	if (radSymmetricGroupBox->isChecked())
	{
		if (radSymmetricModeComboBox->currentIndex()==0)
			auxStream << "<radialSymmetric considered=\"true\">\n";
		else
			auxStream << "<radialSymmetric considered=\"false\">\n";

		auxStream << "<k0>\n";
		auxStream << "<value>" <<radialK0LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "<sigma>" <<radialSigmaK0LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "</k0>\n";
		auxStream << "<k1>\n";
		auxStream << "<value>" <<radialK1LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "<sigma>" <<radialSigmaK1LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "</k1>\n";
		auxStream << "<k2>\n";
		auxStream << "<value>" <<radialK2LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "<sigma>" <<radialSigmaK2LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "</k2>\n";
		auxStream << "<k3>\n";
		auxStream << "<value>" <<radialK3LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "<sigma>" <<radialSigmaK3LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "</k3>\n";
		auxStream << radialSigmaController->getValues();
		auxStream << "</radialSymmetric>\n";
	}
	//new form sensor
	if (decenteredGroupBox->isChecked())
	{
		if (decenteredModeComboBox->currentIndex()==0)
			auxStream << "<decentered considered=\"true\">\n";
		else
			auxStream << "<decentered considered=\"false\">\n";

		auxStream << "<P1>\n";
		auxStream << "<value>" << decenteredP1LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "<sigma>" << decenteredP1SigmaLineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "</P1>\n";
		auxStream << "<P2>\n";
		auxStream << "<value>" << decenteredP2LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "<sigma>" << decenteredP2SigmaLineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "</P2>\n";
		auxStream << decenteredSigmaController->getValues();
		auxStream << "</decentered>\n";
	}
	auxStream << "</distortionCoefficients>\n";

	//Codigo em aberto // J´a tratado:
	auxStream << "\t<principalPointCoordinates uom=\"#mm\">\n";

	auxStream << "<gml:pos>"<<  principalX0doubleSpin->value() << " " << principalY0doubleSpin->value() << "</gml:pos>\n";
	//auxStream << "<gml:pos>"<< "-0.0063 -0.037" << "</gml:pos>\n"; // aqui tem um valor travado
	//auxStream << "<sigma>\n";
	//auxStream << "<mml:matrix>\n";

	//auxStream << "<mml:matrixrow>\n";
	//auxStream << "<mml:cn>" << "0.016" << "</mml:cn>\n";// aqui tem um valor travado
	//auxStream << "<mml:cn>" << "0" << "</mml:cn>\n";// aqui tem um valor travado
	//auxStream << "</mml:matrixrow>\n";

	//auxStream << "<mml:matrixrow>\n";
	//auxStream << "<mml:cn>" << "0" << "</mml:cn>\n"; // aqui tem um valor travado
	//auxStream << "<mml:cn>" << "0.017" << "</mml:cn>\n";// aqui tem um valor travado
	//auxStream << "</mml:matrixrow>\n";

	//auxStream << "</mml:matrix>\n";
	//auxStream << "</sigma>\n";
	auxStream << principalSigmaController->getValues();
	auxStream << "</principalPointCoordinates>\n";


	if (calcModelComboBox->currentIndex() == 0)
	{
		auxStream << "<fiducialMarks uom=\"#mm\">\n";
		auxStream << "<fiducialMark key=\"1\">\n";
		auxStream << "<gml:pos>" << fiducialXi0doubleSpin->value() << " " << fiducialEta0doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "<sigma>" << fiducialSigma0doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid0SigmaController->getValues();
		auxStream << "</fiducialMark>\n";

		auxStream << "<fiducialMark key=\"2\">\n";
		auxStream << "<gml:pos>" << fiducialXi1doubleSpin->value() << " " << fiducialEta1doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "<sigma>" << fiducialSigma1doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid2SigmaController->getValues();
		auxStream << "</fiducialMark>\n";

		auxStream << "<fiducialMark key=\"3\">\n";
		auxStream << "<gml:pos>" << fiducialXi2doubleSpin->value() << " " << fiducialEta2doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "<sigma>" << fiducialSigma2doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid2SigmaController->getValues();
		auxStream << "</fiducialMark>\n";

		auxStream << "<fiducialMark key=\"4\">\n";
		auxStream << "<gml:pos>" << fiducialXi3doubleSpin->value() << " " << fiducialEta3doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "<sigma>" << fiducialSigma3doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid3SigmaController->getValues();
		auxStream << "</fiducialMark>\n";

		if (fiducialMarksCombo->currentIndex()==1){
			auxStream << "<fiducialMark key=\"5\">\n";
			auxStream << "<gml:pos>" << fiducialXi4doubleSpin->value() << " " << fiducialEta4doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "<sigma>" << fiducialSigma4doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid4SigmaController->getValues();
			auxStream << "</fiducialMark>\n";

			auxStream << "<fiducialMark key=\"6\">\n";
			auxStream << "<gml:pos>" << fiducialXi5doubleSpin->value() << " " << fiducialEta5doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "<sigma>" << fiducialSigma5doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid5SigmaController->getValues();
			auxStream << "</fiducialMark>\n";

			auxStream << "<fiducialMark key=\"7\">\n";
			auxStream << "<gml:pos>" << fiducialXi6doubleSpin->value() << " " << fiducialEta6doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "<sigma>" << fiducialSigma6doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid6SigmaController->getValues();
			auxStream << "</fiducialMark>\n";

			auxStream << "<fiducialMark key=\"8\">\n";
			auxStream << "<gml:pos>" << fiducialXi7doubleSpin->value() << " " << fiducialEta7doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "<sigma>" << fiducialSigma7doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid7SigmaController->getValues();
			auxStream << "</fiducialMark>\n";
		}
		auxStream << "</fiducialMarks>\n";
	}
	else if (calcModelComboBox->currentIndex() == 1)
	{
		auxStream << "<sensorDimensions>\n";
		auxStream << "<pixelSize uom=\"#um\">"<< pixelSizeDoubleSpinBox->value()<< "</pixelSize>\n";
		auxStream << "<sensorSize>\n";
		auxStream << "<rows>"<< sensorRowsSpinBox->value()<<"</rows>\n";
		auxStream << "<columns>"<< sensorColsSpinBox->value()<<"</columns>\n";
		auxStream << "</sensorSize>\n";
		auxStream << "</sensorDimensions>\n";
	}
	else if (calcModelComboBox->currentIndex() == 2)
	{
		auxStream << "<Xa>\n";
		auxStream << "<a0>" << xaA0SpinBox->value() << "</a0>\n";
		auxStream << "<a1>" << xaA1SpinBox->value() << "</a1>\n";
		auxStream << "<a2>" << xaA2SpinBox->value() << "</a2>\n";
		auxStream << "<b0>" << xaB0SpinBox->value() << "</b0>\n";
		auxStream << "<b1>" << xaB1SpinBox->value() << "</b1>\n";
		auxStream << "<b2>" << xaB2SpinBox->value() << "</b2>\n";
		auxStream << "</Xa>\n";
	}

	auxStream << "</sensor>\n";

	xmlString = auxStream.str();

	return xmlString;
}

void SensorForm::setReadOnly(bool state)
{
	sensorIdLineEdit->setReadOnly(state);
	geometryCombo->setEnabled(!state);
	platformCombo->setEnabled(!state);
	detectorCombo->setEnabled(!state);
	energyCombo->setEnabled(!state);
	descriptionTextEdit->setReadOnly(state);
	cameraNumberLineEdit->setReadOnly(state);

	cameraDispatchDateEdit->setReadOnly(state);
	cameraExpirationDateEdit->setReadOnly(state);
	calibratedFocalDistanceDoubleSpin->setReadOnly(state);
	calibratedSigmaController->setReadOnly(state);

	radialK0LineEdit->setReadOnly(state);
	//radialSigmaK0LineEdit->setReadOnly(state);
	radialK1LineEdit->setReadOnly(state);
	//radialSigmaK1LineEdit->setReadOnly(state);
	radialK2LineEdit->setReadOnly(state);
	//radialSigmaK2LineEdit->setReadOnly(state);
	radialK3LineEdit->setReadOnly(state);
	//radialSigmaK3LineEdit->setReadOnly(state);
	radialSigmaController->setReadOnly(state);

	decenteredP1LineEdit->setReadOnly(state);
	//decenteredP1SigmaLineEdit->setReadOnly(state);
	decenteredP2LineEdit->setReadOnly(state);
	//decenteredP2SigmaLineEdit->setReadOnly(state);
	decenteredSigmaController->setReadOnly(state);

	principalX0doubleSpin->setReadOnly(state);
	principalY0doubleSpin->setReadOnly(state);
	//principalSigmaX0doubleSpin->setReadOnly(state);
	//principalSigmaY0doubleSpin->setReadOnly(state);
	principalSigmaController->setReadOnly(state);

	fiducialMarksCombo->setEnabled(!state);
	//fiducialSigmaCheckBox->setEnabled(!state);

	fiducialXi0doubleSpin->setReadOnly(state);
	fiducialEta0doubleSpin->setReadOnly(state);
	fid0SigmaController->setReadOnly(state);

	fiducialXi1doubleSpin->setReadOnly(state);
	fiducialEta1doubleSpin->setReadOnly(state);
	fid1SigmaController->setReadOnly(state);

	fiducialXi2doubleSpin->setReadOnly(state);
	fiducialEta2doubleSpin->setReadOnly(state);
	fid2SigmaController->setReadOnly(state);

	fiducialXi3doubleSpin->setReadOnly(state);
	fiducialEta3doubleSpin->setReadOnly(state);
	fid3SigmaController->setReadOnly(state);

	fiducialXi4doubleSpin->setReadOnly(state);
	fiducialEta4doubleSpin->setReadOnly(state);
	fid4SigmaController->setReadOnly(state);

	fiducialXi5doubleSpin->setReadOnly(state);
	fiducialEta5doubleSpin->setReadOnly(state);
	fid5SigmaController->setReadOnly(state);

	fiducialXi6doubleSpin->setReadOnly(state);
	fiducialEta6doubleSpin->setReadOnly(state);
	fid6SigmaController->setReadOnly(state);

	fiducialXi7doubleSpin->setReadOnly(state);
	fiducialEta7doubleSpin->setReadOnly(state);
	fid7SigmaController->setReadOnly(state);

	xaA0SpinBox->setReadOnly(state);
	xaA1SpinBox->setReadOnly(state);
	xaA2SpinBox->setReadOnly(state);
	xaB0SpinBox->setReadOnly(state);
	xaB1SpinBox->setReadOnly(state);
	xaB2SpinBox->setReadOnly(state);

	//new Sensor
	calcModelComboBox->setEnabled(!state);
	radSymmetricModeComboBox->setEnabled(!state);
	decenteredModeComboBox->setEnabled(!state);
	pixelSizeDoubleSpinBox->setReadOnly(state);
	sensorRowsSpinBox->setReadOnly(state);
	sensorColsSpinBox->setReadOnly(state);
	decenteredGroupBox->setCheckable(!state);
	decenteredGroupBox->setChecked(decenteredAvailable);
	radSymmetricGroupBox->setCheckable(!state);
	radSymmetricGroupBox->setChecked(radialAvailable);
}

// show 4 or 8 fiducialMarks
void SensorForm:: fiducialGroup(int index)
{
	if (index==0)
		showLast4Fiducial(false);
	else
	{
		showLast4Fiducial(true);
	}
}

void SensorForm::showLast4Fiducial(bool state)
{
	fiducialEta4doubleSpin->setVisible(state);
	fiducialXi4doubleSpin->setVisible(state);
	fid4SigmaContent->setVisible(state);

	fiducialEta5doubleSpin->setVisible(state);
	fiducialXi5doubleSpin->setVisible(state);
	fid5SigmaContent->setVisible(state);

	fiducialEta6doubleSpin->setVisible(state);
	fiducialXi6doubleSpin->setVisible(state);
	fid6SigmaContent->setVisible(state);

	fiducialEta7doubleSpin->setVisible(state);
	fiducialXi7doubleSpin->setVisible(state);
	fid7SigmaContent->setVisible(state);

	fiducialLabel_9->setVisible(state);
	fiducialLabel_10->setVisible(state);
	fiducialLabel_11->setVisible(state);
	fiducialLabel_12->setVisible(state);
	fiducialLabel_13->setVisible(state);
	fiducialLabel_14->setVisible(state);
	fiducialLabel_15->setVisible(state);
	fiducialLabel_16->setVisible(state);

}

void SensorForm::calculationMode (int modeIndex)
{
	if(modeIndex==0)
	{
		fiducialMarksGroupBox->setVisible(true);
		sensorDimensionsGroupBox->setVisible(false);
		xaGroupBox->setVisible(false);
	}
	else if (modeIndex==1)
	{
		fiducialMarksGroupBox->setVisible(false);
		sensorDimensionsGroupBox->setVisible(true);
		xaGroupBox->setVisible(false);
	}
	else
	{
		fiducialMarksGroupBox->setVisible(false);
		sensorDimensionsGroupBox->setVisible(false);
		xaGroupBox->setVisible(true);
	}
}

void SensorForm::updateSensorDiagonal()
{
	double x=pixelSizeDoubleSpinBox->value()*sensorColsSpinBox->value();
	double y=pixelSizeDoubleSpinBox->value()*sensorRowsSpinBox->value();
	double result=sqrt(x*x+y*y);

	QString res;
	res+=QString::number(x/1000);
	res+=" x ";
	res+=QString::number(y/1000);
	res+=" (";
	res+=QString::number(result/1000);
	res+=" Diagonal)";
	sensorDimensionsLabel->setText(res);

	//sensorSizeDiagonaldoubleSpin->setText(QString::number(result));
}

void SensorForm::clearForm()
{
	sensorIdLineEdit->clear();
	descriptionTextEdit->clear();
	//type
	platformCombo->setCurrentIndex(0);
	calcModelComboBox->setCurrentIndex(0);
	calculationMode(calcModelComboBox->currentIndex());
	geometryCombo->setCurrentIndex(1);
	detectorCombo->setCurrentIndex(0);
	energyCombo->setCurrentIndex(0);

	//camera calibration certificate
	cameraDispatchDateEdit->clear();
	cameraExpirationDateEdit->clear();
	cameraNumberLineEdit->clear();

	//sensor parameters
	calibratedFocalDistanceDoubleSpin->clear();
	calibratedSigmaController->fillValues("Not Available");

	//radial Symmetric
	radialSigmaController->fillValues("Not Available");
	radialK0LineEdit->clear();
	radialK1LineEdit->clear();
	radialK2LineEdit->clear();
	radialK3LineEdit->clear();

	//decentered
	decenteredSigmaController->fillValues("Not Available");
	decenteredP1LineEdit->clear();
	decenteredP2LineEdit->clear();

	//coordinates of principal Point
	principalSigmaController->fillValues("Not Available");
	principalX0doubleSpin->clear();
	principalY0doubleSpin->clear();

	//fiducialMarks
	fid0SigmaController->fillValues("Not Available");
	fid1SigmaController->fillValues("Not Available");
	fid2SigmaController->fillValues("Not Available");
	fid3SigmaController->fillValues("Not Available");
	fid4SigmaController->fillValues("Not Available");
	fid5SigmaController->fillValues("Not Available");
	fid6SigmaController->fillValues("Not Available");

	fiducialXi0doubleSpin->clear();
	fiducialXi1doubleSpin->clear();
	fiducialXi2doubleSpin->clear();
	fiducialXi3doubleSpin->clear();
	fiducialXi4doubleSpin->clear();
	fiducialXi5doubleSpin->clear();
	fiducialXi6doubleSpin->clear();
	fiducialXi7doubleSpin->clear();
	fiducialEta0doubleSpin->clear();
	fiducialEta1doubleSpin->clear();
	fiducialEta2doubleSpin->clear();
	fiducialEta3doubleSpin->clear();
	fiducialEta4doubleSpin->clear();
	fiducialEta5doubleSpin->clear();
	fiducialEta6doubleSpin->clear();
	fiducialEta7doubleSpin->clear();

	fiducialMarksCombo->setCurrentIndex(0);
	fiducialGroup(fiducialMarksCombo->currentIndex());

	// xa
	xaA0SpinBox->clear();
	xaA1SpinBox->clear();
	xaA2SpinBox->clear();
	xaB0SpinBox->clear();
	xaB1SpinBox->clear();
	xaB2SpinBox->clear();

	//sensor dimensions

	pixelSizeDoubleSpinBox->clear();
	sensorRowsSpinBox->clear();
	sensorColsSpinBox->clear();
	sensorDimensionsLabel->clear();
	updateSensorDiagonal();
}

void SensorForm::setFormLocale(QLocale locale)
{
	sensorIdLineEdit->setLocale(locale);
	descriptionTextEdit->setLocale(locale);

	//camera calibration certificate
	cameraDispatchDateEdit->setLocale(locale);
	cameraExpirationDateEdit->setLocale(locale);
	cameraNumberLineEdit->setLocale(locale);

	//sensor parameters
	calibratedFocalDistanceDoubleSpin->setLocale(locale);

	//radial Symmetric
	radialK0LineEdit->setLocale(locale);
	radialK1LineEdit->setLocale(locale);
	radialK2LineEdit->setLocale(locale);
	radialK3LineEdit->setLocale(locale);

	//decentered
	decenteredP1LineEdit->setLocale(locale);
	decenteredP2LineEdit->setLocale(locale);

	//coordinates of principal Point
	principalX0doubleSpin->setLocale(locale);
	principalY0doubleSpin->setLocale(locale);

	//fiducialMarks
	fiducialXi0doubleSpin->setLocale(locale);
	fiducialXi1doubleSpin->setLocale(locale);
	fiducialXi2doubleSpin->setLocale(locale);
	fiducialXi3doubleSpin->setLocale(locale);
	fiducialXi4doubleSpin->setLocale(locale);
	fiducialXi5doubleSpin->setLocale(locale);
	fiducialXi6doubleSpin->setLocale(locale);
	fiducialXi7doubleSpin->setLocale(locale);
	fiducialEta0doubleSpin->setLocale(locale);
	fiducialEta1doubleSpin->setLocale(locale);
	fiducialEta2doubleSpin->setLocale(locale);
	fiducialEta3doubleSpin->setLocale(locale);
	fiducialEta4doubleSpin->setLocale(locale);
	fiducialEta5doubleSpin->setLocale(locale);
	fiducialEta6doubleSpin->setLocale(locale);
	fiducialEta7doubleSpin->setLocale(locale);

	// xa
	xaA0SpinBox->setLocale(locale);
	xaA1SpinBox->setLocale(locale);
	xaA2SpinBox->setLocale(locale);
	xaB0SpinBox->setLocale(locale);
	xaB1SpinBox->setLocale(locale);
	xaB2SpinBox->setLocale(locale);

	//sensor dimensions

	pixelSizeDoubleSpinBox->setLocale(locale);
	sensorRowsSpinBox->setLocale(locale);
	sensorColsSpinBox->setLocale(locale);
	sensorDimensionsLabel->setLocale(locale);


    // Sigma content
    calibratedSigmaContent->setLocale(locale);
    fid0SigmaContent->setLocale(locale);
    fid1SigmaContent->setLocale(locale);
    fid2SigmaContent->setLocale(locale);
    fid3SigmaContent->setLocale(locale);
    fid4SigmaContent->setLocale(locale);
    fid5SigmaContent->setLocale(locale);
    fid6SigmaContent->setLocale(locale);
    fid7SigmaContent->setLocale(locale);
    radialSigmaContent->setLocale(locale);
    decenteredSigmaContent->setLocale(locale);
    principalSigmaContent->setLocale(locale);

}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
