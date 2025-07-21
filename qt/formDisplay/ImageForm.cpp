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
#include "ImageForm.h"
#include <QFileDialog>
#include "HeaderForm.h"
#include "ProjectUserInterface_Qt.h"
#include "ImageViewers.h"
#include <filesystem>
#include <sstream>

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

void ImageForm::fillvalues(std::string values)
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

    std::string auxString;
    auxString = ede.elementByTagName("filePath").toString();

    if (auxString == "."){
        auxString = "";
    }

    QFileInfo fileInfo(QString::fromUtf8(auxString.c_str()));
    QString absolutePath = fileInfo.absoluteFilePath();
    filePathLine->setText(absolutePath);

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
        std::deque<double> aux = ede.elementByTagName("GNSS").elementByTagName("gml:pos").toGmlPos();
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

std::string ImageForm::getvalues()
{
    std::string xmlString;
    std::stringstream auxStream;

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
        std::string type = gnssTypeComboBox->currentIndex() == 0 ? "Initial": gnssTypeComboBox->currentIndex() == 1 ? "Fixed" : "Unused";
        auxStream << "<GNSS uom=\"#m\" type=\"" << type << "\">\n";
        auxStream << "<gml:pos>" << Conversion::doubleToString(eDoubleSpinBox_2->value()) << " " << Conversion::doubleToString(nDoubleSpinBox_2->value()) << " " << Conversion::doubleToString(hDoubleSpinBox_2->value()) <<"</gml:pos>\n";
        auxStream << gnssSigmaController->getValues();
        auxStream << "</GNSS>\n";
    }
    if (insGroup->isChecked())
    {
        std::string type = insTypeComboBox->currentIndex() == 0 ? "Initial": insTypeComboBox->currentIndex() == 1 ? "Fixed" : "Unused";
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

    if (!fileImage.isEmpty())
    {
      std::filesystem::path filePath = fileImage.toStdString();
      std::filesystem::path projectPath = proj->getSavedIn().toStdString();

      QString fileImageName = QString::fromStdString(filePath.filename().string());

      QString sugestionID = QString::fromStdString(filePath.stem().string());

      QString relativeFilePath;
      if (projectPath.empty())
      {
        relativeFilePath = QString::fromStdString(filePath.parent_path().string());
      }
      else
      {
        std::filesystem::path relativePath = std::filesystem::relative(filePath.parent_path(), projectPath);
        relativeFilePath = QString::fromStdString(relativePath.string());
      }

      imageIDLine->setText(sugestionID);
      fileNameLine->setText(fileImageName);
      filePathLine->setText(relativeFilePath.isEmpty() ? "." : relativeFilePath);
      lastPath = filePathLine->text();

      int w = 0, h = 0;
      bool success = false;
      for (int i = 0; i < 100 && !success; i++)
      {
        QImage img(fileImage);
        w = img.width();
        h = img.height();
        if (w != 0 && h != 0)
        {
          success = true;
        }
      }

              // Handle image loading failure
      if (!success)
      {
        QMessageBox msgBox;
        msgBox.setText("Error: The image loading process.");
        msgBox.exec();
      }

              // Update dimension lines
      heightLine->setText(QString::number(h) + " px");
      widthLine->setText(QString::number(w) + " px");

      return fileImage;
    }

    return fileNameLine->text();
}

void ImageForm::startSingleViewer()
{
    SingleViewer* sv = new SingleViewer(this);
    sv->blockOpen();
    sv->blockSave();
    sv->blockMark();
    sv->show();

        QString path = filePathLine->text();
        if (!path.isEmpty())
            path = path + "/";

        sv->loadImage(path + fileNameLine->text());

    /*
    SeparatedStereoViewer* v = new SeparatedStereoViewer(this);
    v->show();
    v->loadLeftImage(filePathLine->text() + "/" + fileNameLine->text());
    v->loadRightImage(filePathLine->text() + "/" + fileNameLine->text());

    StereoViewer* s = new StereoViewer(this);
    s->show();
    s->loadLeftImage(filePathLine->text() + "/" + fileNameLine->text());
    s->loadRightImage(filePathLine->text() + "/" + fileNameLine->text());
    s->blockOpen();
    s->blockSave();
    //s->blockMark();
    */
}

void ImageForm::metadataVisibleChanged(QString newText)
{
    if (newText.isEmpty())
        metadataGroup->setVisible(false);
    else
        metadataGroup->setVisible(true);
}

std::string ImageForm::getFileImageName()
{
  std::filesystem::path filePath = fileImageName.toStdString();
  return filePath.filename().string();
}

std::string ImageForm::getFileImagePath()
{
  std::filesystem::path filePath = fileImagePath.toStdString();
  return filePath.string();
}

bool ImageForm::isForm(std::string formName)
{
    return !formName.compare("ImageForm");
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
