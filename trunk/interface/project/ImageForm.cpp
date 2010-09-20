#include "ImageForm.h"
#include <QFileDialog>

ImageForm :: ImageForm(QWidget *parent):AbstractForm(parent)
{
    setupUi(this);

    connect(fileImageButton , SIGNAL ( clicked()  ) ,
			this    ,  SLOT  ( loadImageFile()  )
            );
	//Aqui o campo de Flight_ID esta sendo escondido
	label_6->setVisible(false);
	flightIDSpin->setVisible(false);
}

void ImageForm::fillvalues(string values)
{

    EDomElement ede(values);
	bool ok;

	tagXml = "<image key=\"";
	tagXml += ede.attribute("key");
	tagXml += "\" sensor_key=\"";
	tagXml += ede.attribute("sensor_key");
	tagXml += "\" flight_key=\"";
	tagXml += ede.attribute("flight_key");
	tagXml += "\">";

    imageIDLine->setText( (QString::fromUtf8(ede.elementByTagName("img_id").toString().c_str()) )
						  );

    widthLine->setText( (QString::fromUtf8(ede.elementByTagAtt("width","uom","#px").toString().c_str())+" px"  )
						);

    heightLine->setText( (QString::fromUtf8(ede.elementByTagAtt("height","uom","#px").toString().c_str())+" px" )
						 );

    fileNameLine->setText( QString::fromUtf8(ede.elementByTagName("fileName").toString().c_str() )
						   );

    string auxString;
    auxString = ede.elementByTagName("filePath").toString();

    if (auxString == "."){
        auxString = "";
    }


    filePathLine->setText( QString::fromUtf8(auxString.c_str() )
                       );

	//flightIDSpin->setValue( (QString::fromUtf8(ede.elementByTagName("flightId").toString().c_str())).toInt(&ok)
						   //);

	resolutionSpin->setValue( (QString::fromUtf8(ede.elementByTagAtt("resolution","uom","#dpi").toString().c_str())).toInt(&ok)
							  );

	if (ede.elementByTagName("GNSS").getContent() == "")
	{
		gnssGroup->setChecked(false);
	}
	else
	{
		gnssGroup->setChecked(true);
		deque<double> aux = ede.elementByTagName("GNSS").elementByTagName("gml:pos").toGmlPos();
		if (aux.size() == 3)
		{
			eDoubleSpinBox_2->setValue(aux.at(0));
			nDoubleSpinBox_2->setValue(aux.at(1));
			hDoubleSpinBox_2->setValue(aux.at(2));
		}
		QString suffix(ede.elementByTagName("GNSS").attribute("uom").c_str());
		eDoubleSpinBox_2->setSuffix(" "+suffix.right(1));
		nDoubleSpinBox_2->setSuffix(" "+suffix.right(1));
		hDoubleSpinBox_2->setSuffix(" "+suffix.right(1));
		lineEditSigma_2->setText( QString::fromUtf8(ede.elementByTagName("sigma").toString().c_str()));
	}

	if (ede.elementByTagName("INS").getContent() == "")
	{
		insGroup->setChecked(false);
	}
	else
	{
		insGroup->setChecked(true);
		omegaLineEdit->setText(QString::fromUtf8(ede.elementByTagName("INS").elementByTagName("omega").toString().c_str()) );
		phiLineEdit->setText(QString::fromUtf8(ede.elementByTagName("INS").elementByTagName("phi").toString().c_str()) );
		kappaLineEdit->setText(QString::fromUtf8(ede.elementByTagName("INS").elementByTagName("kappa").toString().c_str()) );
		lineEditSigma_3->setText( QString::fromUtf8(ede.elementByTagName("INS").elementByTagName("sigma").toString().c_str()));
	}
}

string ImageForm::getvalues()
{
    string xmlString;
    stringstream auxStream;

	auxStream << tagXml << "\n";
    auxStream << "\t<img_id>" << imageIDLine->text().toUtf8().data() << "</img_id>\n";
    auxStream << "\t<width uom=\"#px\">" << widthLine->text().left((widthLine->text().lastIndexOf(" "))).toUtf8().data() << "</width>\n" ;
    auxStream << "\t<height uom=\"#px\">" << heightLine->text().left((heightLine->text().lastIndexOf(" "))).toUtf8().data() << "</height>\n";
    auxStream << "\t<fileName>" << fileNameLine->text().toUtf8().data()<< "</fileName>\n";
    auxStream << "\t<filePath>" << filePathLine->text().toUtf8().data()<<"</filePath>\n";
	//auxStream << "\t<flightId>" << flightIDSpin->value() << "</flightId>\n";
    auxStream << "\t<resolution uom=\"#dpi\">"<< resolutionSpin->value() <<"</resolution>\n";
	if (gnssGroup->isChecked())
	{
		auxStream << "\t<GNSS uom=\"#m\">\n";
		auxStream << "\t\t<gml:pos>" << doubleToString(eDoubleSpinBox_2->value()) << " " << doubleToString(nDoubleSpinBox_2->value()) << " " << doubleToString(hDoubleSpinBox_2->value()) <<"</gml:pos>\n";
		auxStream << "\t\t<sigma>" << lineEditSigma_2->text().toUtf8().data() <<"</sigma>\n";
		auxStream << "\t</GNSS>\n";
	}
	if (insGroup->isChecked())
	{
		auxStream << "\t<INS uom=\"#m\">\n";
		auxStream << "\t\t<omega>" << omegaLineEdit->text().toUtf8().data() <<"</sigma>\n";
		auxStream << "\t\t<phi>" << phiLineEdit->text().toUtf8().data() <<"</sigma>\n";
		auxStream << "\t\t<kappa>" << kappaLineEdit->text().toUtf8().data() <<"</sigma>\n";
		auxStream << "\t\t<sigma>" << lineEditSigma_3->text().toUtf8().data() <<"</sigma>\n";
		auxStream << "\t</INS>\n";
	}
    auxStream << "</image>\n";
    xmlString = auxStream.str();
    return xmlString;
}

void ImageForm:: setReadOnly(bool state)
{
    imageIDLine->setReadOnly(state);

    flightIDSpin->setReadOnly(state);
    resolutionSpin->setReadOnly(state);

    fileImageButton->setDisabled(state);
    gnssGroup->setDisabled(state);
    insGroup->setDisabled(state);


}
QString ImageForm::loadImageFile()
{
	QString fileImage = QFileDialog::getOpenFileName(this, "Open File", ".", "*.tif *.png *.bmp *.jpg");

	//***************************************************************************************************
	// Este tratamento pode precisar de ajustes para cumprir o requisito do e-foto de ser CrossPlataform
	int i=fileImage.lastIndexOf("/");

    fileImageName = fileImage.right(fileImage.length()-i-1);
    fileImagePath = fileImage.left(i);

    fileNameLine->setText(fileImageName);

    filePathLine->setText(fileImagePath+"/");
	//***************************************************************************************************

    return fileImage;
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
