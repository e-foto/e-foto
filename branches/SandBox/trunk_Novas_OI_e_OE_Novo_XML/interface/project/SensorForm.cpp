#include "SensorForm.h"
#include <QtGui>

SensorForm::SensorForm(QWidget *parent): AbstractForm(parent)
{
    setupUi(this);
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

    QRegExp regExp("[-]?\\d{1,4}([.]\\d{1,5})?([(e|E)][-]\\d{1,2}$)?");
    QRegExpValidator *validator= new QRegExpValidator(regExp,this);

	radialK0LineEdit->setTextValue("");
	radialK1LineEdit->setTextValue("");
	radialK2LineEdit->setTextValue("");

	xaA0LineEdit->setValidator(validator);
	xaA1LineEdit->setValidator(validator);
	xaA2LineEdit->setValidator(validator);
	xaB0LineEdit->setValidator(validator);
	xaB1LineEdit->setValidator(validator);
	xaB2LineEdit->setValidator(validator);

	//oldSensor
	//connect(detectorCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(detectorGroup(int)));
    connect(fiductialMarksCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(fiductialGroup(int)));

	//newSensor
	connect(calcModelComboBox, SIGNAL (currentIndexChanged(int)), this, SLOT(calculationMode(int)));

	connect(sensorColsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSensorDiagonal()));
	connect(sensorRowsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSensorDiagonal()));
	connect(pixelSizeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSensorDiagonal()));
	//calculationMode(0);//set inicial temporario
}

void SensorForm::fillvalues(string values)
{

    ede.setContent(values);
    bool ok;

    sensorIdLineEdit->setText(QString::fromUtf8(ede.elementByTagName("sensorId").toString().c_str()));

    geometryCombo->setCurrentIndex(geometryCombo->findText(QString::fromUtf8(ede.elementByTagName("geometry").toString().c_str())));
    platformCombo->setCurrentIndex(platformCombo->findText(QString::fromUtf8(ede.elementByTagName("platform").toString().c_str())));
    detectorCombo->setCurrentIndex(detectorCombo->findText(QString::fromUtf8(ede.elementByTagName("detector").toString().c_str())));
    energyCombo->setCurrentIndex(energyCombo->findText(QString::fromUtf8(ede.elementByTagName("energySource").toString().c_str())));

    if (ede.hasTagName("calculationModel"))
    {
        calcModelComboBox->setCurrentIndex(calcModelComboBox->findText(QString::fromUtf8(ede.elementByTagName("calculationModel").toString().c_str()))); //newSensor
        calculationMode(calcModelComboBox->currentIndex());
    }else
    {
        calcModelComboBox->setCurrentIndex(0); //newSensor
        calculationMode(calcModelComboBox->currentIndex());
    }

    descriptionTextEdit->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
    cameraNumberLineEdit->setText(QString::fromUtf8(ede.elementByTagName("number").toString().c_str()));

	cameraDispatchDateEdit->setDate(QDate::fromString(QString::fromUtf8(ede.elementByTagName("dispatch").toString().c_str()),Qt::ISODate));
	cameraExpirationDateEdit->setDate(QDate::fromString(QString::fromUtf8(ede.elementByTagName("expiration").toString().c_str()),Qt::ISODate));
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
		EDomElement fids = ede.elementByTagName("fiductialMarks");
		if (fids.children().size()==0)
		{
			fiductialMarksCombo->setCurrentIndex(0);
			fiductialGroup(fiductialMarksCombo->currentIndex());
		}
		else
		{
			fiductialMarksCombo->setCurrentIndex(fiductialMarksCombo->findText(QString::number(fids.children().size())+" marks"));
			fiductialGroup(fiductialMarksCombo->currentIndex());
		}
		
		EDomElement fid = ede.elementByTagAtt("fiductialMark","key","1");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi0doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiductialEta0doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid0SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiductialMark","key","2");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi1doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiductialEta1doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid1SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiductialMark","key","3");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi2doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiductialEta2doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid2SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiductialMark","key","4");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi3doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
			fiductialEta3doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
		}
		fid3SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		if (fiductialMarksCombo->currentIndex()==1){
			fiductialGroup(fiductialMarksCombo->currentIndex());
			fid =  ede.elementByTagAtt("fiductialMark","key","5");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi4doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiductialEta4doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid4SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiductialMark","key","6");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi5doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiductialEta5doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid5SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiductialMark","key","7");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi6doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiductialEta6doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid6SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiductialMark","key","8");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi7doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0).toDouble());
				fiductialEta7doubleSpin->setValue(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1).toDouble());
			}
			fid7SigmaController->fillValues(fid.elementByTagName("sigma").getContent());
		}

		xaA0LineEdit->setText("");
		xaA1LineEdit->setText("");
		xaA2LineEdit->setText("");
		xaB0LineEdit->setText("");
		xaB1LineEdit->setText("");
		xaB2LineEdit->setText("");
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

		xaA0LineEdit->setText("");
		xaA1LineEdit->setText("");
		xaA2LineEdit->setText("");
		xaB0LineEdit->setText("");
		xaB1LineEdit->setText("");
		xaB2LineEdit->setText("");
		fiductialXi0doubleSpin->setValue(0);
		fiductialEta0doubleSpin->setValue(0);
		fid0SigmaController->fillValues("");
		fiductialXi1doubleSpin->setValue(0);
		fiductialEta1doubleSpin->setValue(0);
		fid1SigmaController->fillValues("");
		fiductialXi2doubleSpin->setValue(0);
		fiductialEta2doubleSpin->setValue(0);
		fid2SigmaController->fillValues("");
		fiductialXi3doubleSpin->setValue(0);
		fiductialEta3doubleSpin->setValue(0);
		fid3SigmaController->fillValues("");
		fiductialXi4doubleSpin->setValue(0);
		fiductialEta4doubleSpin->setValue(0);
		fid4SigmaController->fillValues("");
		fiductialXi5doubleSpin->setValue(0);
		fiductialEta5doubleSpin->setValue(0);
		fid5SigmaController->fillValues("");
		fiductialXi6doubleSpin->setValue(0);
		fiductialEta6doubleSpin->setValue(0);
		fid6SigmaController->fillValues("");
		fiductialXi7doubleSpin->setValue(0);
		fiductialEta7doubleSpin->setValue(0);
		fid7SigmaController->fillValues("");
	}
	else if (calcModelComboBox->currentIndex() == 2)
	{
		EDomElement xa =  ede.elementByTagName("Xa");
		xaA0LineEdit->setText(QString::fromUtf8(xa.elementByTagName("a0").toString().c_str()));
		xaA1LineEdit->setText(QString::fromUtf8(xa.elementByTagName("a1").toString().c_str()));
		xaA2LineEdit->setText(QString::fromUtf8(xa.elementByTagName("a2").toString().c_str()));
		xaB0LineEdit->setText(QString::fromUtf8(xa.elementByTagName("b0").toString().c_str()));
		xaB1LineEdit->setText(QString::fromUtf8(xa.elementByTagName("b1").toString().c_str()));
		xaB2LineEdit->setText(QString::fromUtf8(xa.elementByTagName("b2").toString().c_str()));

		pixelSizeDoubleSpinBox->setValue(0);
		sensorRowsSpinBox->setValue(0);
		sensorColsSpinBox->setValue(0);
		updateSensorDiagonal();
		fiductialXi0doubleSpin->setValue(0);
		fiductialEta0doubleSpin->setValue(0);
		fid0SigmaController->fillValues("");
		fiductialXi1doubleSpin->setValue(0);
		fiductialEta1doubleSpin->setValue(0);
		fid1SigmaController->fillValues("");
		fiductialXi2doubleSpin->setValue(0);
		fiductialEta2doubleSpin->setValue(0);
		fid2SigmaController->fillValues("");
		fiductialXi3doubleSpin->setValue(0);
		fiductialEta3doubleSpin->setValue(0);
		fid3SigmaController->fillValues("");
		fiductialXi4doubleSpin->setValue(0);
		fiductialEta4doubleSpin->setValue(0);
		fid4SigmaController->fillValues("");
		fiductialXi5doubleSpin->setValue(0);
		fiductialEta5doubleSpin->setValue(0);
		fid5SigmaController->fillValues("");
		fiductialXi6doubleSpin->setValue(0);
		fiductialEta6doubleSpin->setValue(0);
		fid6SigmaController->fillValues("");
		fiductialXi7doubleSpin->setValue(0);
		fiductialEta7doubleSpin->setValue(0);
		fid7SigmaController->fillValues("");
	}
}

string SensorForm::getvalues()
{
    string xmlString;
    stringstream auxStream;

    auxStream << "<sensor key=\"" << ede.attribute("key").data() << "\">\n";
	auxStream << "\t<sensorId>" << sensorIdLineEdit->text().toUtf8().data() <<"</sensorId>\n";
    auxStream << "\t<type>\n" ;
    auxStream << "\t\t<geometry>" << geometryCombo->currentText().toUtf8().data() <<"</geometry>\n";
    auxStream << "\t\t<platform>" << platformCombo->currentText().toUtf8().data() <<"</platform>\n";
    auxStream << "\t\t<detector>" << detectorCombo->currentText().toUtf8().data() <<"</detector>\n";
    auxStream << "\t\t<energySource>"<< energyCombo->currentText().toUtf8().data() <<"</energySource>\n";
	auxStream << "\t\t<calculationModel>"<< calcModelComboBox->currentText().toUtf8().data() <<"</calculationModel>\n"; //new form sensor
	auxStream << "\t\t<spectralRanges uom=\"#um\"><!--#um equivale a dez elevado a menos seis metros-->\n";
    auxStream << "\t\t\t<spectralRange band=\"1\">\n"; // aqui tem um valor travado
    auxStream << "\t\t\t\t<inferiorLimit>0.4</inferiorLimit>\n"; // aqui tem um valor travado
    auxStream << "\t\t\t\t<superiorLimit>0.7</superiorLimit>\n"; // aqui tem um valor travado
    auxStream << "\t\t\t</spectralRange>\n";
    auxStream << "\t\t</spectralRanges>\n";
    auxStream << "\t</type>\n";
    auxStream << "\t<description>" << descriptionTextEdit->toPlainText().toUtf8().data() << "</description>\n";
    auxStream << "\t<calibrationCertificate>\n" ;
	auxStream << "\t\t<number>" << cameraNumberLineEdit->text().toUtf8().data() << "</number>\n";
	auxStream << "\t\t<dispatch>"<<cameraDispatchDateEdit->dateTime().toString(Qt::ISODate).toUtf8().data() << "</dispatch>\n";
	auxStream << "\t\t<expiration>"<<cameraExpirationDateEdit->dateTime().toString(Qt::ISODate).toUtf8().data() << "</expiration>\n";
    auxStream << "\t</calibrationCertificate>\n";
    auxStream << "\t<focalDistance uom=\"#mm\">\n";
    auxStream << "\t\t<value>" << calibratedFocalDistanceDoubleSpin->value() << "</value>\n";
	//auxStream << "\t\t<sigma>" << calibratedSigmaDoubleSpin->value() << "</sigma>\n";
	auxStream << calibratedSigmaController->getValues();
    auxStream << "\t</focalDistance>\n";
	auxStream << "\t<distortionCoefficients>\n";
		 //new form sensor
	if (radSymmetricGroupBox->isChecked())
	{
		if (radSymmetricModeComboBox->currentIndex()==0)
			auxStream << "\t\t<radialSymmetric considered=\"true\">\n";
		else
			auxStream << "\t\t<radialSymmetric considered=\"false\">\n";

		auxStream << "\t\t\t<k0>\n";
		auxStream << "\t\t\t\t<value>" <<radialK0LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK0LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "\t\t\t</k0>\n";
		auxStream << "\t\t\t<k1>\n";
		auxStream << "\t\t\t\t<value>" <<radialK1LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK1LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "\t\t\t</k1>\n";
		auxStream << "\t\t\t<k2>\n";
		auxStream << "\t\t\t\t<value>" <<radialK2LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK2LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "\t\t\t</k2>\n";
		auxStream << "\t\t\t<k3>\n";
		auxStream << "\t\t\t\t<value>" <<radialK3LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK3LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "\t\t\t</k3>\n";
		auxStream << radialSigmaController->getValues();
		auxStream << "\t\t</radialSymmetric>\n";
	}
	 //new form sensor
	if (decenteredGroupBox->isChecked())
	{
		if (decenteredModeComboBox->currentIndex()==0)
			auxStream << "\t\t<decentered considered=\"true\">\n";
		else
			auxStream << "\t\t<decentered considered=\"false\">\n";

		auxStream << "\t\t\t<P1>\n";
		auxStream << "\t\t\t\t<value>" << decenteredP1LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "\t\t\t\t<sigma>" << decenteredP1SigmaLineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "\t\t\t</P1>\n";
		auxStream << "\t\t\t<P2>\n";
		auxStream << "\t\t\t\t<value>" << decenteredP2LineEdit->textValue().toUtf8().data() << "</value>\n";
		//auxStream << "\t\t\t\t<sigma>" << decenteredP2SigmaLineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << "\t\t\t</P2>\n";
		auxStream << decenteredSigmaController->getValues();
		auxStream << "\t\t</decentered>\n";
	}
	auxStream << "\t</distortionCoefficients>\n";

	//Codigo em aberto // J´a tratado:
    auxStream << "\t<principalPointCoordinates uom=\"#mm\">\n";

	auxStream << "\t\t<gml:pos>"<<  principalX0doubleSpin->value() << " " << principalY0doubleSpin->value() << "</gml:pos>\n";
	//auxStream << "\t\t<gml:pos>"<< "-0.0063 -0.037" << "</gml:pos>\n"; // aqui tem um valor travado
	//auxStream << "\t\t<sigma>\n";
	//auxStream << "\t\t\t<mml:matrix>\n";

	//auxStream << "\t\t\t\t<mml:matrixrow>\n";
	//auxStream << "\t\t\t\t\t<mml:cn>" << "0.016" << "</mml:cn>\n";// aqui tem um valor travado
	//auxStream << "\t\t\t\t\t<mml:cn>" << "0" << "</mml:cn>\n";// aqui tem um valor travado
	//auxStream << "\t\t\t\t</mml:matrixrow>\n";

	//auxStream << "\t\t\t\t<mml:matrixrow>\n";
	//auxStream << "\t\t\t\t\t<mml:cn>" << "0" << "</mml:cn>\n"; // aqui tem um valor travado
	//auxStream << "\t\t\t\t\t<mml:cn>" << "0.017" << "</mml:cn>\n";// aqui tem um valor travado
	//auxStream << "\t\t\t\t</mml:matrixrow>\n";

	//auxStream << "\t\t\t</mml:matrix>\n";
	//auxStream << "\t\t</sigma>\n";
	auxStream << principalSigmaController->getValues();
    auxStream << "\t</principalPointCoordinates>\n";


	if (calcModelComboBox->currentIndex() == 0)
	{
		auxStream << "\t<fiductialMarks uom=\"#mm\">\n";
		auxStream << "\t\t<fiductialMark key=\"1\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi0doubleSpin->value() << " " << fiductialEta0doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma0doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid0SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		auxStream << "\t\t<fiductialMark key=\"2\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi1doubleSpin->value() << " " << fiductialEta1doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma1doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid2SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		auxStream << "\t\t<fiductialMark key=\"3\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi2doubleSpin->value() << " " << fiductialEta2doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma2doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid2SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		auxStream << "\t\t<fiductialMark key=\"4\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi3doubleSpin->value() << " " << fiductialEta3doubleSpin->value() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma3doubleSpin->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid3SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		if (fiductialMarksCombo->currentIndex()==1){
			auxStream << "\t\t<fiductialMark key=\"5\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi4doubleSpin->value() << " " << fiductialEta4doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma4doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid4SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";

			auxStream << "\t\t<fiductialMark key=\"6\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi5doubleSpin->value() << " " << fiductialEta5doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma5doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid5SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";

			auxStream << "\t\t<fiductialMark key=\"7\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi6doubleSpin->value() << " " << fiductialEta6doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma6doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid6SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";

			auxStream << "\t\t<fiductialMark key=\"8\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi7doubleSpin->value() << " " << fiductialEta7doubleSpin->value() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma7doubleSpin->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid7SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";
		}
		auxStream << "\t</fiductialMarks>\n";
	}
	else if (calcModelComboBox->currentIndex() == 1)
	{
		auxStream << "<sensorDimensions>\n";
		auxStream << "\t<pixelSize uom=\"#um\">"<< pixelSizeDoubleSpinBox->value()<< "</pixelSize>\n";
		auxStream << "\t<sensorSize>\n";
		auxStream << "\t\t<rows>"<< sensorRowsSpinBox->value()<<"</rows>\n";
		auxStream << "\t\t<columns>"<< sensorColsSpinBox->value()<<"</columns>\n";
		auxStream << "\t</sensorSize>\n";
		auxStream << "</sensorDimensions>\n";
	}
	else if (calcModelComboBox->currentIndex() == 2)
	{
		auxStream << "<Xa>\n";
		auxStream << "<a0>" << xaA0LineEdit->text().toUtf8().data() << "</a0>\n";
		auxStream << "<a1>" << xaA1LineEdit->text().toUtf8().data() << "</a1>\n";
		auxStream << "<a2>" << xaA2LineEdit->text().toUtf8().data() << "</a2>\n";
		auxStream << "<b0>" << xaB0LineEdit->text().toUtf8().data() << "</b0>\n";
		auxStream << "<b1>" << xaB1LineEdit->text().toUtf8().data() << "</b1>\n";
		auxStream << "<b2>" << xaB2LineEdit->text().toUtf8().data() << "</b2>\n";
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

    fiductialMarksCombo->setEnabled(!state);
	//fiductialSigmaCheckBox->setEnabled(!state);

	fiductialXi0doubleSpin->setReadOnly(state);
	fiductialEta0doubleSpin->setReadOnly(state);
	fid0SigmaController->setReadOnly(state);

	fiductialXi1doubleSpin->setReadOnly(state);
	fiductialEta1doubleSpin->setReadOnly(state);
	fid1SigmaController->setReadOnly(state);

	fiductialXi2doubleSpin->setReadOnly(state);
	fiductialEta2doubleSpin->setReadOnly(state);
	fid2SigmaController->setReadOnly(state);

	fiductialXi3doubleSpin->setReadOnly(state);
	fiductialEta3doubleSpin->setReadOnly(state);
	fid3SigmaController->setReadOnly(state);

	fiductialXi4doubleSpin->setReadOnly(state);
	fiductialEta4doubleSpin->setReadOnly(state);
	fid4SigmaController->setReadOnly(state);

	fiductialXi5doubleSpin->setReadOnly(state);
	fiductialEta5doubleSpin->setReadOnly(state);
	fid5SigmaController->setReadOnly(state);

	fiductialXi6doubleSpin->setReadOnly(state);
	fiductialEta6doubleSpin->setReadOnly(state);
	fid6SigmaController->setReadOnly(state);

	fiductialXi7doubleSpin->setReadOnly(state);
	fiductialEta7doubleSpin->setReadOnly(state);
	fid7SigmaController->setReadOnly(state);

    xaA0LineEdit->setReadOnly(state);
    xaA1LineEdit->setReadOnly(state);
    xaA2LineEdit->setReadOnly(state);
    xaB0LineEdit->setReadOnly(state);
    xaB1LineEdit->setReadOnly(state);
    xaB2LineEdit->setReadOnly(state);

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

// show 4 or 8 fiductialMarks
void SensorForm:: fiductialGroup(int index)
{
    if (index==0)
        showLast4Fiductial(false);
	else
	{
        showLast4Fiductial(true);
    }
}

void SensorForm::showLast4Fiductial(bool state)
{
	fiductialEta4doubleSpin->setVisible(state);
	fiductialXi4doubleSpin->setVisible(state);
	fid4SigmaContent->setVisible(state);

	fiductialEta5doubleSpin->setVisible(state);
	fiductialXi5doubleSpin->setVisible(state);
	fid5SigmaContent->setVisible(state);

	fiductialEta6doubleSpin->setVisible(state);
	fiductialXi6doubleSpin->setVisible(state);
	fid6SigmaContent->setVisible(state);

	fiductialEta7doubleSpin->setVisible(state);
	fiductialXi7doubleSpin->setVisible(state);
	fid7SigmaContent->setVisible(state);

    fiductialLabel_9->setVisible(state);
    fiductialLabel_10->setVisible(state);
    fiductialLabel_11->setVisible(state);
    fiductialLabel_12->setVisible(state);
    fiductialLabel_13->setVisible(state);
    fiductialLabel_14->setVisible(state);
    fiductialLabel_15->setVisible(state);
    fiductialLabel_16->setVisible(state);

}

void SensorForm::calculationMode (int modeIndex)
{
	if(modeIndex==0)
	{
		fiductialMarksGroupBox->setVisible(true);
		sensorDimensionsGroupBox->setVisible(false);
		xaGroupBox->setVisible(false);
	}
	else if (modeIndex==1)
	{
		fiductialMarksGroupBox->setVisible(false);
		sensorDimensionsGroupBox->setVisible(true);
		xaGroupBox->setVisible(false);
	}
	else
	{
		fiductialMarksGroupBox->setVisible(false);
		sensorDimensionsGroupBox->setVisible(false);
		xaGroupBox->setVisible(true);
	}
}

bool SensorForm::isForm(string formName)
{
	return !formName.compare("SensorForm");
}

void SensorForm::updateSensorDiagonal()
{
	double x=pixelSizeDoubleSpinBox->value()*sensorRowsSpinBox->value();
	double y=pixelSizeDoubleSpinBox->value()*sensorColsSpinBox->value();
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
