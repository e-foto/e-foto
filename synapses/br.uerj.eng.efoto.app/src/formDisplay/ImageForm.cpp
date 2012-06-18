#include "ImageForm.h"
#include <QFileDialog>
#include <QDir>
#include "HeaderForm.h"

#ifdef INTEGRATED_EFOTO
#include "ImageViewers.h"
#include "ProjectUserInterface_Qt.h" // Rever! o uso desta classe para substituição deste include por um da classe Project
#endif //INTEGRATED_EFOTO REVER!
#ifdef SYNAPSE_EFOTO
#include "ICortex.h"
using namespace cortex;
	// Aqui vai entrar uma referência para a Project na sinapse engine
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ImageForm::ImageForm(QWidget *parent):AbstractForm(parent)
{
	setupUi(this);
	gnssSigmaController = new SigmaFormController("Not Available",3);
	gnssSigmaSelector->setSigmaFormController(gnssSigmaController);
	gnssSigmaSelector->blockCovarianceMatrixOption();
	gnssSigmaContent->setSigmaFormController(gnssSigmaController);
	gnssSigmaDialogButton->setSigmaFormController(gnssSigmaController);
	gnssSigmaDialogButton->setVisible(false);
	insSigmaController = new SigmaFormController("Not Available",3);
	insSigmaSelector->setSigmaFormController(insSigmaController);
	insSigmaSelector->blockCovarianceMatrixOption();
	insSigmaContent->setSigmaFormController(insSigmaController);
	insSigmaDialogButton->setSigmaFormController(insSigmaController);
	insSigmaDialogButton->setVisible(false);
	metadataGroup->setVisible(false);
	lastPath = ".";

	connect(fileImageButton,SIGNAL(clicked()),this,SLOT(loadImageFile()));
	connect(viewButton,SIGNAL(clicked()),this,SLOT(startSingleViewer()));
	connect(fileNameLine,SIGNAL(textChanged(QString)),this,SLOT(metadataVisibleChanged(QString)));

	//Aqui o campo de Flight_ID esta sendo escondido
	label_6->setVisible(false);
	flightIdComboBox->setVisible(false);

	omegaDmsEdit->setDmsEditMode(DEG);
	phiDmsEdit->setDmsEditMode(DEG);
	kappaDmsEdit->setDmsEditMode(DEG);

	groupBox->setVisible(false);
	groupBox_2->setVisible(false);
}

void ImageForm::fillvalues(string values)
{
	this->cleanForm();
	EDomElement ede(values);
	bool ok;

	tagXml = "<image key=\"";
	tagXml += ede.attribute("key");
	tagXml += "\" sensor_key=\"";
	tagXml += ede.attribute("sensor_key");
	tagXml += "\" flight_key=\"";
	tagXml += ede.attribute("flight_key");
	tagXml += "\">";

	imageIDLine->setText( (QString::fromUtf8(ede.elementByTagName("imageId").toString().c_str()) ));

	widthLine->setText( (QString::fromUtf8(ede.elementByTagAtt("width","uom","#px").toString().c_str())+" px"  ));

	heightLine->setText( (QString::fromUtf8(ede.elementByTagAtt("height","uom","#px").toString().c_str())+" px" ));

	fileNameLine->setText( QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str() ));

	string auxString;
	auxString = ede.elementByTagName("filePath").toString();

	if (auxString == "."){
		auxString = "";
	}


	filePathLine->setText( QString::fromUtf8(auxString.c_str() )
						  );

	//flightIDSpin->setValue( (QString::fromUtf8(ede.elementByTagName("flightId").toString().c_str())).toInt(&ok)
	//);
	resolutionSpin->setValue(0);
	resolutionSpin->setSpecialValueText(" ");
	if (ede.elementByTagAtt("resolution","uom","#dpi").getContent() != "" )
		resolutionSpin->setValue( (QString::fromUtf8(ede.elementByTagAtt("resolution","uom","#dpi").toString().c_str())).toInt(&ok) );

	flightDirection = ede.elementByTagName("flightDirection").toDouble();

	if (ede.elementByTagName("GNSS").getContent() == "")
	{
		gnssTypeComboBox->setCurrentIndex(0);
		eDoubleSpinBox_2->setValue(0);
		nDoubleSpinBox_2->setValue(0);
		hDoubleSpinBox_2->setValue(0);
		eDoubleSpinBox_2->setSuffix(" m");
		nDoubleSpinBox_2->setSuffix(" m");
		hDoubleSpinBox_2->setSuffix(" m");
		gnssSigmaController->fillValues("Not Available");
		activeGNSS = false;
	}
	else
	{
		deque<double> aux = ede.elementByTagName("GNSS").elementByTagName("gml:pos").toGmlPos();
		if (aux.size() == 3)
		{
			eDoubleSpinBox_2->setValue(aux.at(0));
			nDoubleSpinBox_2->setValue(aux.at(1));
			hDoubleSpinBox_2->setValue(aux.at(2));
		}
		if (ede.elementByTagName("GNSS").attribute("type") == "Initial")
			gnssTypeComboBox->setCurrentIndex(0);
		else if (ede.elementByTagName("GNSS").attribute("type") == "Fixed")
			gnssTypeComboBox->setCurrentIndex(1);
		else
			gnssTypeComboBox->setCurrentIndex(2);
		QString suffix(ede.elementByTagName("GNSS").attribute("uom").c_str());
		eDoubleSpinBox_2->setSuffix(" "+suffix.right(1));
		nDoubleSpinBox_2->setSuffix(" "+suffix.right(1));
		hDoubleSpinBox_2->setSuffix(" "+suffix.right(1));
		gnssSigmaController->fillValues(ede.elementByTagName("GNSS").elementByTagName("sigma").getContent());
		activeGNSS = true;
	}

	if (ede.elementByTagName("INS").getContent() == "")
	{
		insTypeComboBox->setCurrentIndex(0);

		omegaDmsEdit->getDmsValue()->stringToDms("0 0 0");
		phiDmsEdit->getDmsValue()->stringToDms("0 0 0");
		kappaDmsEdit->getDmsValue()->stringToDms("0 0 0");
		omegaDmsEdit->updateValue();
		phiDmsEdit->updateValue();
		kappaDmsEdit->updateValue();
		insSigmaController->fillValues("Not Available");
		activeINS = false;
	}
	else
	{
		if (ede.elementByTagName("INS").attribute("type") == "Initial")
			insTypeComboBox->setCurrentIndex(0);
		else if (ede.elementByTagName("INS").attribute("type") == "Fixed")
			insTypeComboBox->setCurrentIndex(1);
		else
			insTypeComboBox->setCurrentIndex(2);
		omegaDmsEdit->setText(locale().toString(Dms::radianoToDegreeDecimal(ede.elementByTagName("INS").elementByTagName("omega").toDouble())));
		phiDmsEdit->setText(locale().toString(Dms::radianoToDegreeDecimal(ede.elementByTagName("INS").elementByTagName("phi").toDouble())));
		kappaDmsEdit->setText(locale().toString(Dms::radianoToDegreeDecimal(ede.elementByTagName("INS").elementByTagName("kappa").toDouble())));
		omegaDmsEdit->updateValue();
		phiDmsEdit->updateValue();
		kappaDmsEdit->updateValue();


		insSigmaController->fillValues(ede.elementByTagName("INS").elementByTagName("sigma").getContent());
		activeINS = true;
	}




}

string ImageForm::getvalues()
{
	string xmlString;
	stringstream auxStream;

	auxStream << tagXml << "\n";
	auxStream << "<imageId>" << imageIDLine->text().toUtf8().data() << "</imageId>\n";
	auxStream << "<width uom=\"#px\">" << widthLine->text().left((widthLine->text().lastIndexOf(" "))).toUtf8().data() << "</width>\n" ;
	auxStream << "<height uom=\"#px\">" << heightLine->text().left((heightLine->text().lastIndexOf(" "))).toUtf8().data() << "</height>\n";
	auxStream << "<fileName>" << fileNameLine->text().toUtf8().data()<< "</fileName>\n";
	auxStream << "<filePath>" << filePathLine->text().toUtf8().data()<<"</filePath>\n";
	//auxStream << "<flightId>" << flightIDSpin->value() << "</flightId>\n";
	auxStream << "<resolution uom=\"#dpi\">"<< resolutionSpin->value() <<"</resolution>\n";
	auxStream << "<flightDirection>" << Conversion::doubleToString(flightDirection) << "</flightDirection>\n";
	if (gnssGroup->isChecked())
	{
		string type = gnssTypeComboBox->currentIndex() == 0 ? "Initial": gnssTypeComboBox->currentIndex() == 1 ? "Fixed" : "Unused";
		auxStream << "<GNSS uom=\"#m\" type=\"" << type << "\">\n";
		auxStream << "<gml:pos>" << Conversion::doubleToString(eDoubleSpinBox_2->value()) << " " << Conversion::doubleToString(nDoubleSpinBox_2->value()) << " " << Conversion::doubleToString(hDoubleSpinBox_2->value()) <<"</gml:pos>\n";
		auxStream << gnssSigmaController->getValues();
		auxStream << "</GNSS>\n";
	}
	if (insGroup->isChecked())
	{
		string type = insTypeComboBox->currentIndex() == 0 ? "Initial": insTypeComboBox->currentIndex() == 1 ? "Fixed" : "Unused";
		auxStream << "<INS uom=\"#rad\" type=\"" << type << "\">\n";
		auxStream << "<omega>" << Conversion::doubleToString(omegaDmsEdit->getDmsValue()->dmsToRadiano()) <<"</omega>\n";
		auxStream << "<phi>" << Conversion::doubleToString(phiDmsEdit->getDmsValue()->dmsToRadiano()) <<"</phi>\n";
		auxStream << "<kappa>" << Conversion::doubleToString(kappaDmsEdit->getDmsValue()->dmsToRadiano()) <<"</kappa>\n";
		auxStream << insSigmaController->getValues();
		auxStream << "</INS>\n";
	}
	auxStream << "</image>\n";
	xmlString = auxStream.str();
	this->cleanForm();
	return xmlString;
}

void ImageForm:: setReadOnly(bool state)
{
	imageIDLine->setReadOnly(state);
	flightIdComboBox->setDisabled(state);
	resolutionSpin->setReadOnly(state);
	fileImageButton->setDisabled(state);
	gnssSigmaController->setReadOnly(state);
	insSigmaController->setReadOnly(state);
	gnssGroup->setCheckable(!state);
	insGroup->setCheckable(!state);
	gnssTypeComboBox->setDisabled(state);
	insTypeComboBox->setDisabled(state);
	eDoubleSpinBox_2->setReadOnly(state);
	nDoubleSpinBox_2->setReadOnly(state);
	hDoubleSpinBox_2->setReadOnly(state);
	omegaDmsEdit->setReadOnly(state);
	phiDmsEdit->setReadOnly(state);
	kappaDmsEdit->setReadOnly(state);
	if (!state)
	{
		gnssGroup->setChecked(activeGNSS);
		insGroup->setChecked(activeINS);
	}
}

QString ImageForm::loadImageFile()
{
	QString fileImage = QFileDialog::getOpenFileName(this, "Open File", lastPath, "*.tif *.png *.bmp *.jpg");
	setIOAvailable(false);
	setEOAvailable(false);

#ifdef INTEGRATED_EFOTO
	QDir absolutePath (proj->getSavedIn());
#endif //INTEGRATED_EFOTO REVER!

	// Rever! esse ponto, pois o método acessado no objeto proj que hoje é da classe ProjectUserInterface_Qt passará a ser utilizado,
	// com um objeto da classe Project acessível somente por intermédio da comunicação com a sinapse engine.
#ifdef SYNAPSE_EFOTO
	QDir absolutePath;
#endif //SYNAPSE_EFOTO

	if (fileImage !="")
	{
		//***************************************************************************************************
		// Este tratamento pode precisar de ajustes para cumprir o requisito do e-foto de ser CrossPlataform
		int i=fileImage.lastIndexOf("/");
		int j=absolutePath.relativeFilePath(fileImage).lastIndexOf(('/'));
		fileImageName = fileImage.right(fileImage.length()-i-1);
		fileImagePath = fileImage.left(i);

		QString sugestionID=fileImageName;
		sugestionID.chop(4);
		imageIDLine->setText(sugestionID);

		fileNameLine->setText(fileImageName);
		if (j<0)
			filePathLine->setText(".");
		else
			filePathLine->setText(absolutePath.relativeFilePath(fileImage).left(j));
		lastPath = filePathLine->text();
		//***************************************************************************************************
		int w, h;
		bool success = false;
		for (int i = 0; i < 100 && !success; i++)
		{
			QImage img(fileImage);
			w = img.width(); h = img.height();
			if (w != 0 && h != 0)
			{
				success = true;
			}
		}
		if (!success)
		{
			QMessageBox* msgBox = new QMessageBox();
			msgBox->setText("Error: The image loading process.");
			msgBox->exec();
		}
		heightLine->setText(QString::number(h)+" px");
		widthLine->setText(QString::number(w)+" px");
		return fileImage;

	}else
		return fileNameLine->text();
}

void ImageForm::startSingleViewer()
{
#ifdef INTEGRATED_EFOTO
	SingleViewer* sv = new SingleViewer(this);
	sv->hideOpen(true);
	sv->hideSave(true);
	sv->hideMark(true);
	sv->show();
	sv->loadImage(filePathLine->text() + "/" + fileNameLine->text());
#endif //INTEGRATED_EFOTO REVER!

#ifdef SYNAPSE_EFOTO
    if (true)//viewerService.isNull())
    {
        viewerService = ICortex::getInstance()->getSynapse<viewer::IViewerService>();
    }
    singleViewer = viewerService->instanceSingleViewer();
    singleViewer->hideOpen(true);
    singleViewer->hideSave(true);
    singleViewer->hideMark(true);
    singleViewer->show();
    singleViewer->loadImage(filePathLine->text() + "/" + fileNameLine->text());
#endif //SYNAPSE_EFOTO
}

void ImageForm::metadataVisibleChanged(QString newText)
{
	if (newText.isEmpty())
		metadataGroup->setVisible(false);
	else
		metadataGroup->setVisible(true);
}

string ImageForm :: getFileImageName()
{
	return fileImageName.toStdString().c_str();
}

string ImageForm :: getFileImagePath()
{
	return fileImagePath.toStdString().c_str();
}

bool ImageForm::isForm(string formName)
{
	return !formName.compare("ImageForm");
}

void ImageForm::setIOAvailable(bool state)
{
	if (state)
	;
		//labelIO->setText("Interior orientation is available");
	//else
		//labelIO->setText("Interior orientation is not available");
}

void ImageForm::setEOAvailable(bool state)
{
	if (state)
	;
		//labelEO->setText("Spatial resection is available");
	//else
		//labelEO->setText("Spatial resection is not available");
}

void ImageForm::cleanForm()
{
	imageIDLine->clear();
	resolutionSpin->setSpecialValueText("N/A");
	flightIdComboBox->setCurrentIndex(0);
	//ground coordinates
	eDoubleSpinBox_2->clear();
	nDoubleSpinBox_2->clear();
	hDoubleSpinBox_2->clear();
	gnssSigmaController->fillValues("Not Available");
	//inertial navigation
	insSigmaController->fillValues("Not Available");
	omegaDmsEdit->setText(locale().toString(locale().toDouble("0.0"),'f',4));
	kappaDmsEdit->setText(locale().toString(locale().toDouble("0.0"),'f',4));
	phiDmsEdit->setText(locale().toString(locale().toDouble("0.0"),'f',4));
	//metadata
	fileNameLine->clear();
	filePathLine->clear();
	heightLine->clear();
	widthLine->clear();

	phiLabel->clear();
	omegaLabel->clear();
	kappaLabel->clear();
	x0Label->clear();
	y0Label->clear();
	z0Label->clear();
	a0Label->clear();
	a1Label->clear();
	a2Label->clear();
	b0Label->clear();
	b1Label->clear();
	b2Label->clear();

	gnssGroup->setChecked(false);
	insGroup->setChecked(false);
	flightDirection = 0.0;
}

void ImageForm::setFormLocale(QLocale locale)
{
	imageIDLine->setLocale(locale);
	resolutionSpin->setLocale(locale);

	//ground coordinates
	eDoubleSpinBox_2->setLocale(locale);
	nDoubleSpinBox_2->setLocale(locale);
	hDoubleSpinBox_2->setLocale(locale);

	//inertial navigation

	omegaDmsEdit->setLocale(locale);
	kappaDmsEdit->setLocale(locale);
	phiDmsEdit->setLocale(locale);
	//metadata
	fileNameLine->setLocale(locale);
	filePathLine->setLocale(locale);
	heightLine->setLocale(locale);
	widthLine->setLocale(locale);

}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
