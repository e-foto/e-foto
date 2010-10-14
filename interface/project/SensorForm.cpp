#include "SensorForm.h"
#include <QtGui>

SensorForm::SensorForm(QWidget *parent): AbstractForm(parent)
{
    setupUi(this);

	calibratedSigmaController = new SigmaFormController("Not Available",1);
	calibratedSigmaSelector->setSigmaFormController(calibratedSigmaController);
	calibratedSigmaSelector->blockCovarianceMatrixOption();
	calibratedSigmaContent->setSigmaFormController(calibratedSigmaController);

	fid0SigmaController = new SigmaFormController("Not Available",2);
	//fid0SigmaController->setDirection("horizontal");
	//fid0SigmaSelector->setSigmaFormController(fid0SigmaController);
	//fid0SigmaSelector->blockCovarianceMatrixOption();
	fid0SigmaContent->setSigmaFormController(fid0SigmaController);

	fid1SigmaController = new SigmaFormController("Not Available",2);
	//fid1SigmaController->setDirection("horizontal");
	//fid1SigmaSelector->setSigmaFormController(fid1SigmaController);
	//fid1SigmaSelector->blockCovarianceMatrixOption();
	fid1SigmaContent->setSigmaFormController(fid1SigmaController);

	fid2SigmaController = new SigmaFormController("Not Available",2);
	//fid2SigmaController->setDirection("horizontal");
	//fid2SigmaSelector->setSigmaFormController(fid2SigmaController);
	//fid2SigmaSelector->blockCovarianceMatrixOption();
	fid2SigmaContent->setSigmaFormController(fid2SigmaController);

	fid3SigmaController = new SigmaFormController("Not Available",2);
	//fid3SigmaController->setDirection("horizontal");
	//fid3SigmaSelector->setSigmaFormController(fid3SigmaController);
	//fid3SigmaSelector->blockCovarianceMatrixOption();
	fid3SigmaContent->setSigmaFormController(fid3SigmaController);

	fid4SigmaController = new SigmaFormController("Not Available",2);
	//fid4SigmaController->setDirection("horizontal");
	//fid4SigmaSelector->setSigmaFormController(fid4SigmaController);
	//fid4SigmaSelector->blockCovarianceMatrixOption();
	fid4SigmaContent->setSigmaFormController(fid4SigmaController);

	fid5SigmaController = new SigmaFormController("Not Available",2);
	//fid5SigmaController->setDirection("horizontal");
	//fid5SigmaSelector->setSigmaFormController(fid5SigmaController);
	//fid5SigmaSelector->blockCovarianceMatrixOption();
	fid5SigmaContent->setSigmaFormController(fid5SigmaController);

	fid6SigmaController = new SigmaFormController("Not Available",2);
	//fid6SigmaController->setDirection("horizontal");
	//fid6SigmaSelector->setSigmaFormController(fid6SigmaController);
	//fid6SigmaSelector->blockCovarianceMatrixOption();
	fid6SigmaContent->setSigmaFormController(fid6SigmaController);

	fid7SigmaController = new SigmaFormController("Not Available",2);
	//fid7SigmaController->setDirection("horizontal");
	//fid7SigmaSelector->setSigmaFormController(fid7SigmaController);
	//fid7SigmaSelector->blockCovarianceMatrixOption();
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
	//principalSigmaController->setDirection("horizontal");
	principalSigmaSelector->setSigmaFormController(principalSigmaController);
	principalSigmaSelector->blockCovarianceMatrixOption();
	principalSigmaContent->setSigmaFormController(principalSigmaController);

    QRegExp regExp("[-]?\\d{1,4}([.]\\d{1,5})?([(e|E)][-]\\d{1,2}$)?");
    QRegExpValidator *validator= new QRegExpValidator(regExp,this);

    radialK0LineEdit->setValidator(validator);
    radialK1LineEdit->setValidator(validator);
    radialK2LineEdit->setValidator(validator);
    radialK3LineEdit->setValidator(validator);

    principalX0LineEdit->setValidator(validator);
    principalY0LineEdit->setValidator(validator);

    fiductialEta0LineEdit->setValidator(validator);
    fiductialEta1LineEdit->setValidator(validator);
    fiductialEta2LineEdit->setValidator(validator);
    fiductialEta3LineEdit->setValidator(validator);
    fiductialEta4LineEdit->setValidator(validator);
    fiductialEta5LineEdit->setValidator(validator);
    fiductialEta6LineEdit->setValidator(validator);
    fiductialEta7LineEdit->setValidator(validator);

    fiductialXi0LineEdit->setValidator(validator);
    fiductialXi1LineEdit->setValidator(validator);
    fiductialXi2LineEdit->setValidator(validator);
    fiductialXi3LineEdit->setValidator(validator);
    fiductialXi4LineEdit->setValidator(validator);
    fiductialXi5LineEdit->setValidator(validator);
    fiductialXi6LineEdit->setValidator(validator);
    fiductialXi7LineEdit->setValidator(validator);


	xaA0LineEdit->setValidator(validator);
	xaA1LineEdit->setValidator(validator);
	xaA2LineEdit->setValidator(validator);
	xaB0LineEdit->setValidator(validator);
	xaB1LineEdit->setValidator(validator);
	xaB2LineEdit->setValidator(validator);

	//principalSigmaX0LineEdit->setValidator(validator);
	//principalSigmaY0LineEdit->setValidator(validator);
    principalX0LineEdit->setValidator(validator);
    principalY0LineEdit->setValidator(validator);

    decenteredP1LineEdit->setValidator(validator);
	//decenteredP1SigmaLineEdit->setValidator(validator);
    decenteredP2LineEdit->setValidator(validator);
	//decenteredP2SigmaLineEdit->setValidator(validator);

    connect(detectorCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(detectorGroup(int)));
    connect(fiductialMarksCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(fiductialGroup(int)));
	//connect(fiductialSigmaCheckBox,SIGNAL(toggled(bool) ), this, SLOT(setSigmasVisible(bool)));


}

void SensorForm::fillvalues(string values)
{

    ede.setContent(values);
    bool ok;

	sensorIdLineEdit->setText(QString::fromUtf8(ede.elementByTagName("sensorId").toString().c_str()));

    geometryCombo->setCurrentIndex(geometryCombo->findText(QString::fromUtf8(ede.elementByTagName("geometry").toString().c_str())));
    platformCombo->setCurrentIndex(platformCombo->findText(QString::fromUtf8(ede.elementByTagName("platform").toString().c_str())));
    detectorCombo->setCurrentIndex(detectorCombo->findText(QString::fromUtf8(ede.elementByTagName("detector").toString().c_str())));
    detectorGroup(detectorCombo->currentIndex());
    energyCombo->setCurrentIndex(energyCombo->findText(QString::fromUtf8(ede.elementByTagName("energySource").toString().c_str())));

    descriptionTextEdit->setPlainText(QString::fromUtf8(ede.elementByTagName("description").toString().c_str()));
    cameraNumberLineEdit->setText(QString::fromUtf8(ede.elementByTagName("number").toString().c_str()));

    cameraDispatchDateEdit->setDate(QDate::fromString(QString::fromUtf8(ede.elementByTagName("dispatch").toString().c_str())));
    cameraExpirationDateEdit->setDate(QDate::fromString(QString::fromUtf8(ede.elementByTagName("expiration").toString().c_str())));
    calibratedFocalDistanceDoubleSpin->setValue(QString::fromUtf8(ede.elementByTagName("value").toString().c_str()).toDouble(&ok));
	//calibratedSigmaDoubleSpin->setValue(QString::fromUtf8(ede.elementByTagName("sigma").toString().c_str()).toDouble(&ok));
	calibratedSigmaController->fillValues(ede.elementByTagName("sigma").getContent());

    EDomElement k= ede.elementByTagName("k0");
	radialK0LineEdit->setText(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));
	//radialSigmaK0LineEdit->setText(QString::fromUtf8(k.elementByTagName("sigma").toString().c_str()));

    k=ede.elementByTagName("k1");
    radialK1LineEdit->setText(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));
	//radialSigmaK1LineEdit->setText(QString::fromUtf8(k.elementByTagName("sigma").toString().c_str()));

    k=ede.elementByTagName("k2");
    radialK2LineEdit->setText(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));
	//radialSigmaK2LineEdit->setText(QString::fromUtf8(k.elementByTagName("sigma").toString().c_str()));

    k=ede.elementByTagName("k3");
    radialK3LineEdit->setText(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));
	//radialSigmaK3LineEdit->setText(QString::fromUtf8(k.elementByTagName("sigma").toString().c_str()));

	radialSigmaController->fillValues(ede.elementByTagName("radialSymmetric").elementByTagName("sigma").getContent());

    k=ede.elementByTagName("P1");
    decenteredP1LineEdit->setText(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));
	//decenteredP1SigmaLineEdit->setText(QString::fromUtf8(k.elementByTagName("sigma").toString().c_str()));

    k=ede.elementByTagName("P2");
    decenteredP2LineEdit->setText(QString::fromUtf8(k.elementByTagName("value").toString().c_str()));
	//decenteredP2SigmaLineEdit->setText(QString::fromUtf8(k.elementByTagName("sigma").toString().c_str()));

	decenteredSigmaController->fillValues(ede.elementByTagName("decentered").elementByTagName("sigma").getContent());

/* ainda sem implementação dos campos de tag <principalPointCoordinate>
//   tags interiores como mesmo nome "mml:cn"

    EDomElement pri=ede.elementByTagName("principalPointCoordinate");
    //string minha = pri.getContent();
*/ //Feito a seguir:

	EDomElement pri =  ede.elementByTagName("principalPointCoordinates");
	if (pri.elementByTagName("gml:pos").toString() != "")
	{
		principalX0LineEdit->setText(QString::fromUtf8(pri.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
		principalY0LineEdit->setText(QString::fromUtf8(pri.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
	}
	else
	{
		principalX0LineEdit->setText("");
		principalY0LineEdit->setText("");
	}
	//fiductialSigma0LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
	principalSigmaController->fillValues(pri.elementByTagName("sigma").getContent());


	if (detectorCombo->currentText() == "film")
	{

		EDomElement fids =  ede.elementByTagName("fiductialMarks");
		fiductialMarksCombo->setCurrentIndex(fiductialMarksCombo->findText(QString::number(fids.children().size())+" marks"));
		//fiductialSigmaCheckBox->setChecked(true);
		showLast4Fiductial(false);

		EDomElement fid =  ede.elementByTagAtt("fiductialMark","key","1");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi0LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
			fiductialEta0LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
		}
		//fiductialSigma0LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
		fid0SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiductialMark","key","2");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi1LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
			fiductialEta1LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
		}
		//fiductialSigma1LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
		fid1SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiductialMark","key","3");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi2LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
			fiductialEta2LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
		}
		//fiductialSigma2LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
		fid2SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		fid =  ede.elementByTagAtt("fiductialMark","key","4");
		if (fid.elementByTagName("gml:pos").toString() != "")
		{
			fiductialXi3LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
			fiductialEta3LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
		}
		//fiductialSigma3LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
		fid3SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		if (fiductialMarksCombo->currentIndex()==1){
			fiductialGroup(fiductialMarksCombo->currentIndex());
			fid =  ede.elementByTagAtt("fiductialMark","key","5");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi4LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
				fiductialEta4LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
			}
			//fiductialSigma4LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
			fid4SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiductialMark","key","6");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi5LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
				fiductialEta5LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
			}
			//fiductialSigma5LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
			fid5SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiductialMark","key","7");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi6LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
				fiductialEta6LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
			}
			//fiductialSigma6LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
			fid6SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

			fid =  ede.elementByTagAtt("fiductialMark","key","8");
			if (fid.elementByTagName("gml:pos").toString() != "")
			{
				fiductialXi7LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(0));
				fiductialEta7LineEdit->setText(QString::fromUtf8(fid.elementByTagName("gml:pos").toString().c_str()).split(" ").at(1));
			}
			//fiductialSigma7LineEdit->setText(QString::fromUtf8(fid.elementByTagName("sigma").toString().c_str()));
			fid7SigmaController->fillValues(fid.elementByTagName("sigma").getContent());

		}

		xaA0LineEdit->setText("");
		xaA1LineEdit->setText("");
		xaA2LineEdit->setText("");
		xaB0LineEdit->setText("");
		xaB1LineEdit->setText("");
		xaB2LineEdit->setText("");
    }
	else if (detectorCombo->currentText() == "ccd")
	{
		EDomElement xa =  ede.elementByTagName("Xa");
		xaA0LineEdit->setText(QString::fromUtf8(xa.elementByTagName("a0").toString().c_str()));
		xaA1LineEdit->setText(QString::fromUtf8(xa.elementByTagName("a1").toString().c_str()));
		xaA2LineEdit->setText(QString::fromUtf8(xa.elementByTagName("a2").toString().c_str()));
		xaB0LineEdit->setText(QString::fromUtf8(xa.elementByTagName("b0").toString().c_str()));
		xaB1LineEdit->setText(QString::fromUtf8(xa.elementByTagName("b1").toString().c_str()));
		xaB2LineEdit->setText(QString::fromUtf8(xa.elementByTagName("b2").toString().c_str()));

		fiductialXi0LineEdit->setText("");
		fiductialEta0LineEdit->setText("");
		fid0SigmaController->fillValues("");
		fiductialXi1LineEdit->setText("");
		fiductialEta1LineEdit->setText("");
		fid1SigmaController->fillValues("");
		fiductialXi2LineEdit->setText("");
		fiductialEta2LineEdit->setText("");
		fid2SigmaController->fillValues("");
		fiductialXi3LineEdit->setText("");
		fiductialEta3LineEdit->setText("");
		fid3SigmaController->fillValues("");
		fiductialXi4LineEdit->setText("");
		fiductialEta4LineEdit->setText("");
		fid4SigmaController->fillValues("");
		fiductialXi5LineEdit->setText("");
		fiductialEta5LineEdit->setText("");
		fid5SigmaController->fillValues("");
		fiductialXi6LineEdit->setText("");
		fiductialEta6LineEdit->setText("");
		fid6SigmaController->fillValues("");
		fiductialXi7LineEdit->setText("");
		fiductialEta7LineEdit->setText("");
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
    auxStream << "\t\t<dispatch>"<<cameraDispatchDateEdit->text().toUtf8().data() << "</dispatch>\n";
    auxStream << "\t\t<expiration>"<<cameraExpirationDateEdit->text().toUtf8().data() << "</expiration>\n";
    auxStream << "\t</calibrationCertificate>\n";
    auxStream << "\t<focalDistance uom=\"#mm\">\n";
    auxStream << "\t\t<value>" << calibratedFocalDistanceDoubleSpin->value() << "</value>\n";
	//auxStream << "\t\t<sigma>" << calibratedSigmaDoubleSpin->value() << "</sigma>\n";
	auxStream << calibratedSigmaController->getValues();
    auxStream << "\t</focalDistance>\n";
    auxStream << "\t<distortionCoefficients>\n";
    auxStream << "\t\t<radialSymmetric>\n";
    auxStream << "\t\t\t<k0>\n";
    auxStream << "\t\t\t\t<value>" <<radialK0LineEdit->text().toUtf8().data() << "</value>\n";
	//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK0LineEdit->text().toUtf8().data() << "</sigma>\n";
    auxStream << "\t\t\t</k0>\n";
    auxStream << "\t\t\t<k1>\n";
    auxStream << "\t\t\t\t<value>" <<radialK1LineEdit->text().toUtf8().data() << "</value>\n";
	//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK1LineEdit->text().toUtf8().data() << "</sigma>\n";
    auxStream << "\t\t\t</k1>\n";
    auxStream << "\t\t\t<k2>\n";
    auxStream << "\t\t\t\t<value>" <<radialK2LineEdit->text().toUtf8().data() << "</value>\n";
	//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK2LineEdit->text().toUtf8().data() << "</sigma>\n";
    auxStream << "\t\t\t</k2>\n";
    auxStream << "\t\t\t<k3>\n";
    auxStream << "\t\t\t\t<value>" <<radialK3LineEdit->text().toUtf8().data() << "</value>\n";
	//auxStream << "\t\t\t\t<sigma>" <<radialSigmaK3LineEdit->text().toUtf8().data() << "</sigma>\n";
    auxStream << "\t\t\t</k3>\n";
	auxStream << radialSigmaController->getValues();
    auxStream << "\t\t</radialSymmetric>\n";
    auxStream << "\t\t<decentered>\n";
    auxStream << "\t\t\t<P1>\n";
    auxStream << "\t\t\t\t<value>" << decenteredP1LineEdit->text().toUtf8().data() << "</value>\n";
	//auxStream << "\t\t\t\t<sigma>" << decenteredP1SigmaLineEdit->text().toUtf8().data() << "</sigma>\n";
    auxStream << "\t\t\t</P1>\n";
    auxStream << "\t\t\t<P2>\n";
    auxStream << "\t\t\t\t<value>" << decenteredP2LineEdit->text().toUtf8().data() << "</value>\n";
	//auxStream << "\t\t\t\t<sigma>" << decenteredP2SigmaLineEdit->text().toUtf8().data() << "</sigma>\n";
    auxStream << "\t\t\t</P2>\n";
	auxStream << decenteredSigmaController->getValues();
    auxStream << "\t\t</decentered>\n";
    auxStream << "\t</distortionCoefficients>\n";

	//Codigo em aberto // J´a tratado:
    auxStream << "\t<principalPointCoordinates uom=\"#mm\">\n";

	auxStream << "\t\t<gml:pos>"<<  principalX0LineEdit->text().toUtf8().data() << " " << principalY0LineEdit->text().toUtf8().data() << "</gml:pos>\n";
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


	if (detectorCombo->currentText() == "film")
	{
		auxStream << "\t<fiductialMarks uom=\"#mm\">\n";
		auxStream << "\t\t<fiductialMark key=\"1\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi0LineEdit->text().toUtf8().data() << " " << fiductialEta0LineEdit->text().toUtf8().data() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma0LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid0SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		auxStream << "\t\t<fiductialMark key=\"2\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi1LineEdit->text().toUtf8().data() << " " << fiductialEta1LineEdit->text().toUtf8().data() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma1LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid2SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		auxStream << "\t\t<fiductialMark key=\"3\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi2LineEdit->text().toUtf8().data() << " " << fiductialEta2LineEdit->text().toUtf8().data() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma2LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid2SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		auxStream << "\t\t<fiductialMark key=\"4\">\n";
		auxStream << "\t\t\t<gml:pos>" << fiductialXi3LineEdit->text().toUtf8().data() << " " << fiductialEta3LineEdit->text().toUtf8().data() << "</gml:pos>\n";
		//auxStream << "\t\t\t<sigma>" << fiductialSigma3LineEdit->text().toUtf8().data() << "</sigma>\n";
		auxStream << fid3SigmaController->getValues();
		auxStream << "\t\t</fiductialMark>\n";

		if (fiductialMarksCombo->currentIndex()==1){
			auxStream << "\t\t<fiductialMark key=\"5\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi4LineEdit->text().toUtf8().data() << " " << fiductialEta4LineEdit->text().toUtf8().data() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma4LineEdit->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid4SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";

			auxStream << "\t\t<fiductialMark key=\"6\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi5LineEdit->text().toUtf8().data() << " " << fiductialEta5LineEdit->text().toUtf8().data() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma5LineEdit->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid5SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";

			auxStream << "\t\t<fiductialMark key=\"7\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi6LineEdit->text().toUtf8().data() << " " << fiductialEta6LineEdit->text().toUtf8().data() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma6LineEdit->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid6SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";

			auxStream << "\t\t<fiductialMark key=\"8\">\n";
			auxStream << "\t\t\t<gml:pos>" << fiductialXi7LineEdit->text().toUtf8().data() << " " << fiductialEta7LineEdit->text().toUtf8().data() << "</gml:pos>\n";
			//auxStream << "\t\t\t<sigma>" << fiductialSigma7LineEdit->text().toUtf8().data() << "</sigma>\n";
			auxStream << fid7SigmaController->getValues();
			auxStream << "\t\t</fiductialMark>\n";
		}
		auxStream << "\t</fiductialMarks>\n";
	}
	else if (detectorCombo->currentText() == "ccd")
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

//  cout << xmlString.c_str() << "\ndentrodo getvalues" <<endl;
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

    principalX0LineEdit->setReadOnly(state);
    principalY0LineEdit->setReadOnly(state);
	//principalSigmaX0LineEdit->setReadOnly(state);
	//principalSigmaY0LineEdit->setReadOnly(state);
	principalSigmaController->setReadOnly(state);

    fiductialMarksCombo->setEnabled(!state);
	//fiductialSigmaCheckBox->setEnabled(!state);

    fiductialXi0LineEdit->setReadOnly(state);
    fiductialEta0LineEdit->setReadOnly(state);
	//fiductialSigma0LineEdit->setReadOnly(state);
	fid0SigmaController->setReadOnly(state);

    fiductialXi1LineEdit->setReadOnly(state);
    fiductialEta1LineEdit->setReadOnly(state);
	//fiductialSigma1LineEdit->setReadOnly(state);
	fid1SigmaController->setReadOnly(state);

    fiductialXi2LineEdit->setReadOnly(state);
    fiductialEta2LineEdit->setReadOnly(state);
	//fiductialSigma2LineEdit->setReadOnly(state);
	fid2SigmaController->setReadOnly(state);

    fiductialXi3LineEdit->setReadOnly(state);
    fiductialEta3LineEdit->setReadOnly(state);
	//fiductialSigma3LineEdit->setReadOnly(state);
	fid3SigmaController->setReadOnly(state);

    fiductialXi4LineEdit->setReadOnly(state);
    fiductialEta4LineEdit->setReadOnly(state);
	//fiductialSigma4LineEdit->setReadOnly(state);
	fid4SigmaController->setReadOnly(state);

    fiductialXi5LineEdit->setReadOnly(state);
    fiductialEta5LineEdit->setReadOnly(state);
	//fiductialSigma5LineEdit->setReadOnly(state);
	fid5SigmaController->setReadOnly(state);

    fiductialXi6LineEdit->setReadOnly(state);
    fiductialEta6LineEdit->setReadOnly(state);
	//fiductialSigma6LineEdit->setReadOnly(state);
	fid6SigmaController->setReadOnly(state);

    fiductialXi7LineEdit->setReadOnly(state);
    fiductialEta7LineEdit->setReadOnly(state);
	//fiductialSigma7LineEdit->setReadOnly(state);
	fid7SigmaController->setReadOnly(state);

    xaA0LineEdit->setReadOnly(state);
    xaA1LineEdit->setReadOnly(state);
    xaA2LineEdit->setReadOnly(state);
    xaB0LineEdit->setReadOnly(state);
    xaB1LineEdit->setReadOnly(state);
    xaB2LineEdit->setReadOnly(state);
}

// hide/show xaGroupBox or fiductialMarksGroupBox
void SensorForm :: detectorGroup(int index)
{
    if(index==0){
        xaGroupBox->setVisible(false);
        fiductialMarksGroupBox->setVisible(true);
    }
    else{
        xaGroupBox->setVisible(true);
        fiductialMarksGroupBox->setVisible(false);
    }
}

// show 4 or 8 fiductialMarks
void SensorForm:: fiductialGroup(int index)
{
    if (index==0)
        showLast4Fiductial(false);
    else{
        showLast4Fiductial(true);
		//if (!fiductialSigmaCheckBox->isChecked())
			//setSigmasVisible(false);
    }
}
/*
void SensorForm::setSigmasVisible(bool visible)
{
    fiductialSigma0LineEdit->setVisible(visible);
    fiductialSigma1LineEdit->setVisible(visible);
    fiductialSigma2LineEdit->setVisible(visible);
    fiductialSigma3LineEdit->setVisible(visible);

    sigmaLabel->setVisible(visible);
    sigmaLabel_2->setVisible(visible);
    sigmaLabel_3->setVisible(visible);
    sigmaLabel_4->setVisible(visible);
    if (fiductialMarksCombo->currentIndex()==1){
        sigmaLabel_5->setVisible(visible);
        sigmaLabel_6->setVisible(visible);
        sigmaLabel_7->setVisible(visible);
        sigmaLabel_8->setVisible(visible);
        fiductialSigma4LineEdit->setVisible(visible);
        fiductialSigma5LineEdit->setVisible(visible);
        fiductialSigma6LineEdit->setVisible(visible);
        fiductialSigma7LineEdit->setVisible(visible);
    }
}
*/
void SensorForm::showLast4Fiductial(bool state)
{
    fiductialEta4LineEdit->setVisible(state);
    fiductialXi4LineEdit->setVisible(state);
	fid4SigmaContent->setVisible(state);
	//fid4SigmaSelector->setVisible(state);
	//fiductialSigma4LineEdit->setVisible(state);

    fiductialEta5LineEdit->setVisible(state);
    fiductialXi5LineEdit->setVisible(state);
	fid5SigmaContent->setVisible(state);
	//fid5SigmaSelector->setVisible(state);
	//fiductialSigma5LineEdit->setVisible(state);

    fiductialEta6LineEdit->setVisible(state);
    fiductialXi6LineEdit->setVisible(state);
	fid6SigmaContent->setVisible(state);
	//fid6SigmaSelector->setVisible(state);
	//fiductialSigma6LineEdit->setVisible(state);

    fiductialEta7LineEdit->setVisible(state);
    fiductialXi7LineEdit->setVisible(state);
	fid7SigmaContent->setVisible(state);
	//fid7SigmaSelector->setVisible(state);
	//fiductialSigma7LineEdit->setVisible(state);

    fiductialLabel_9->setVisible(state);
    fiductialLabel_10->setVisible(state);
    fiductialLabel_11->setVisible(state);
    fiductialLabel_12->setVisible(state);
    fiductialLabel_13->setVisible(state);
    fiductialLabel_14->setVisible(state);
    fiductialLabel_15->setVisible(state);
    fiductialLabel_16->setVisible(state);
	//sigmaLabel_5->setVisible(state);
	//sigmaLabel_6->setVisible(state);
	//sigmaLabel_7->setVisible(state);
	//sigmaLabel_8->setVisible(state);
}

bool SensorForm::isForm(string formName)
{
    return !formName.compare("SensorForm");
}
