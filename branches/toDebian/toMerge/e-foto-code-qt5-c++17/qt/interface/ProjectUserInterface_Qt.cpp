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

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "ProjectUserInterface_Qt.h"

#include "ProjectManager.h"

#include "AboutForm.h"
#include "LoadingScreen.h"
#include "ETreeModel.h"
#include "EDomValidator.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QShortcut>
#include <QScrollArea>
#include <QSettings>
#include <iostream>
#include <sstream>
#include <ConvertionsSystems.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectUserInterface_Qt::ProjectUserInterface_Qt(ProjectManager* manager, QWidget* parent, Qt::WindowFlags fl)
    : QMainWindow(parent, fl)
{
    setupUi(this);

    //  Inicia variaveis
    this->manager = manager;
    this->currentForm = NULL;
    this->currentItemId = 0;
    this->editState = false;
    this->addNewState = false;
    this->changeModule = false;
    this->treeItems.clear();
    this->enableExecuteMenu(false);
    actionSave_file->setEnabled(false);
    actionSave_file_as->setEnabled(false);

    setWindowTitle(tr("efoto[Project Manager]"));
    imageForm.proj = this;

    // Realiza as conexões necessárias
    this->connect(actionNew, SIGNAL(triggered()), this, SLOT(newProject()));
    this->connect(actionLoad_file, SIGNAL(triggered()), this, SLOT(loadFile()));
    this->connect(actionLoad_last_project, SIGNAL(triggered()), this, SLOT(loadLastProject()));
    this->connect(actionSave_file, SIGNAL(triggered()), this, SLOT(saveFile()));
    this->connect(actionSave_file_as, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    this->connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    this->connect(actionInterior_Orientation, SIGNAL(triggered()), this, SLOT(executeIO()));
    this->connect(actionSpatial_resection, SIGNAL(triggered()), this, SLOT(executeSR()));
    this->connect(actionExport_Stereo, SIGNAL(triggered()), this, SLOT(exportSPFile()));
    this->connect(actionDEMExtraction, SIGNAL(triggered()), this, SLOT(executeDEM()));
    this->connect(actionOrtho_rectification, SIGNAL(triggered()), this, SLOT(executeOrtho()));
    this->connect(actionFoto_Tri, SIGNAL(triggered()), this, SLOT(executeFT()));
    this->connect(actionStereo, SIGNAL(triggered()), this, SLOT(executeSP()));
    this->connect(actionPTReport, SIGNAL(triggered()), this, SLOT(executePTReport()));
    this->connect(actionReport, SIGNAL(triggered()), this, SLOT(executeReport()));
    this->connect(actionAbout,SIGNAL(triggered()), this, SLOT(showAbout()));
    this->connect(treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(processTreeClick(QModelIndex)));
    this->connect(&imagesForm, SIGNAL(clicked(int)), this, SLOT(selectImage(int)));
    this->connect(&pointsForm, SIGNAL(clicked(int)), this, SLOT(selectPoint(int)));
    this->connect(imageForm.imageIDLine, SIGNAL(editingFinished()), this , SLOT( validatingImage()) );
    this->connect(imageForm.resolutionSpin, SIGNAL(editingFinished()), this , SLOT( validatingImage()) );
    this->connect(imageForm.fileNameLine, SIGNAL(textChanged(QString)),this , SLOT( validatingImage()) );
    this->connect(pointForm.lineEdit_gcp_id, SIGNAL(editingFinished()), this, SLOT( validatingPoint()) );
    this->connect(pointForm.sigmaController, SIGNAL(validateChanged()), this, SLOT( validatingPoint()) );
    this->connect(imageForm.gnssSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingImage()) );
    this->connect(imageForm.insSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingImage()) );
    this->connect(flightForm.flightIDLineEdit, SIGNAL(editingFinished()), this , SLOT( validatingFlight()) );
    this->connect(sensorForm.sensorIdLineEdit, SIGNAL(editingFinished()), this , SLOT( validatingSensor()) );
    this->connect(sensorForm.fid0SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid1SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid2SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid3SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid4SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid5SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid6SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.fid7SigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.calibratedSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.radialSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.decenteredSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );
    this->connect(sensorForm.principalSigmaController, SIGNAL(validateChanged()), this, SLOT(validatingSensor()) );

    connect(pointsForm.importButton, SIGNAL(clicked()), this, SLOT( importPointsFromTxt() ) );
    connect(pointsForm.exportToTxtButton, SIGNAL(clicked()), this, SLOT(exportPointsToTxt()) );
    connect(imagesForm.importButton, SIGNAL(clicked()), this, SLOT( importImagesBatch() ) );
    //imagesForm.importButton->setEnabled(false);

    //setGeometry(qApp->desktop()->availableGeometry());
    setWindowState(this->windowState() | Qt::WindowMaximized);
    qApp->processEvents();
    // Bloqueia alguns dos dipositivos
    this->removeDockWidget(debuggerDockWidget);
    this->removeDockWidget(projectDockWidget);

    this->show();

    // Coloca s forms na QStackedWidget
    QScrollArea *headerArea = new QScrollArea;
    headerArea->setWidget(&headerForm);
    headerArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(headerArea);
    QScrollArea *terrainArea = new QScrollArea;
    terrainArea->setWidget(&terrainForm);
    terrainArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(terrainArea);
    QScrollArea *sensorArea = new QScrollArea;
    sensorArea->setWidget(&sensorForm);
    sensorArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(sensorArea);
    QScrollArea *flightArea = new QScrollArea;
    flightArea->setWidget(&flightForm);
    flightArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(flightArea);
    QScrollArea *imagesArea = new QScrollArea;
    imagesArea->setWidget(&imagesForm);
    imagesArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(imagesArea);
    QScrollArea *imageArea = new QScrollArea;
    imageArea->setWidget(&imageForm);
    imageArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(imageArea);
    QScrollArea *pointsArea = new QScrollArea;
    pointsArea->setWidget(&pointsForm);
    pointsArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(pointsArea);
    QScrollArea *pointArea = new QScrollArea;
    pointArea->setWidget(&pointForm);
    pointArea->setAlignment(Qt::AlignHCenter);
    centerArea.addWidget(pointArea);

    offset.setVisible(false);
    centerArea.setVisible(false);
    controlButtons.setVisible(false);
    centralWidget()->layout()->addWidget(&offset);
    centralWidget()->layout()->addWidget(&centerArea);
    centralWidget()->layout()->addWidget(&controlButtons);

    // Adiciona um atalho para os desenvolvedores observarem as mudanÃ§as no XML durante o runtime
    QShortcut* shortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+D", "Debug")),this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(toggleDebug()));

    // Inserido pelo Paulo 05/09/2011
    // Adiciona um atalho para os desenvolvedores dar upload das coordenadas digitais do export do LPS
    //QShortcut* shortcut2 = new QShortcut(QKeySequence(tr("Ctrl+Shift+P", "Import")),this);
    //connect(shortcut2, SIGNAL(activated()), this, SLOT(importPointsFromTxt2()));

    //QShortcut* shortcut3 = new QShortcut(QKeySequence(tr("Ctrl+Shift+I", "Import")),this);
    //connect(shortcut3, SIGNAL(activated()), this, SLOT(importImagesBatch()));


    //QShortcut* shortcut4 = new QShortcut(QKeySequence(tr("Ctrl+Shift+O", "Import")),this);
    //connect(shortcut4, SIGNAL(activated()), this, SLOT(importOIDigitalMarks()));


    refreshMenu();
    actionLoad_last_project->setEnabled(availableLP());

}

ProjectUserInterface_Qt::~ProjectUserInterface_Qt()
{
    // no need to delete child widgets, Qt does it all for us
}

void ProjectUserInterface_Qt::closeEvent(QCloseEvent *event)
{
    //qDebug() << changeModule;
    if (changeModule || confirmToClose())
        event->accept();
    else
        event->ignore();

    refreshMenu();
}

void ProjectUserInterface_Qt::newProject()
{
    if (!confirmToClose())
        return;

    if (!saveFileAs(true))
        return;

    addDockWidget(Qt::LeftDockWidgetArea,projectDockWidget);
    addDockWidget(Qt::BottomDockWidgetArea,debuggerDockWidget);
    setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
    projectDockWidget->setVisible(true);
    introWidget->setVisible(false);
    centerArea.setVisible(true);
    controlButtons.setVisible(true);
    offset.setVisible(true);

    manager->newProject(manager->savedIn);
    //savedIn = "";
    actionSave_file->setEnabled(true);
    actionSave_file_as->setEnabled(true);
    //viewHeader();
    newTree();

    //Os comandos a seguir so serao uteis enquanto o projeto ficar restrito a apenas um sensor e um flight

    std::string text = "";

    text += "<sensor key=\"1\">\n";
    text +=	"<sensorId></sensorId>\n";
    text +=	"<type>\n";
    text +=	"<geometry>frame</geometry>\n";
    text +=	"<platform>aerial</platform>\n";
    text +=	"<detector>film</detector>\n";
    text +=	"<energySource>natural</energySource>\n";
    text +=	"<calculationMode>With Fiducial Marks</calculationMode>";
    text +=	"<spectralRanges uom=\"#um\">\n";
    //text +=	"<spectralRange band=\"\">\n";
    //text +=	"<inferiorLimit></inferiorLimit>\n";
    //text +=	"<superiorLimit></superiorLimit>\n";
    //text +=	"</spectralRange>\n";
    text +=	"</spectralRanges>\n";
    text +=	"</type>\n";
    text +=	"<description></description>\n";
    text +=	"<calibrationCertificate>\n";
    text +=	"<number></number>\n";
    text +=	"<dispatch></dispatch>\n";
    text +=	"<expiration></expiration>\n";
    text +=	"</calibrationCertificate>\n";
    text +=	"<focalDistance uom=\"#mm\">\n";
    text +=	"<value></value>\n";
    text +=	"<sigma></sigma>\n";
    text +=	"</focalDistance>\n";
    text +=	"<distortionCoefficients>\n";
    text +=	"<radialSymmetric considered=\"false\">\n";
    text +=	"<k0>\n";
    text +=	"<value></value>\n";
    text += "<sigma></sigma>\n";
    text +=	"</k0>\n";
    text +=	"<k1>\n";
    text +=	"<value></value>\n";
    text += "<sigma></sigma>\n";
    text +=	"</k1>\n";
    text +=	"<k2>\n";
    text +=	"<value></value>\n";
    text += "<sigma></sigma>\n";
    text +=	"</k2>\n";
    text +=	"<k3>\n";
    text +=	"<value></value>\n";
    text += "<sigma></sigma>\n";
    text +=	"</k3>\n";
    text +=	"</radialSymmetric>\n";
    text +=	"<decentered considered=\"false\">\n";
    text +=	"<P1>\n";
    text +=	"<value></value>\n";
    text += "<sigma></sigma>\n";
    text +=	"</P1>\n";
    text +=	"<P2>\n";
    text +=	"<value></value>\n";
    text += "<sigma></sigma>\n";
    text +=	"</P2>\n";
    text +=	"</decentered>\n";
    text +=	"</distortionCoefficients>\n";
    text +=	"<principalPointCoordinates uom=\"#mm\">\n";
    text +=	"<gml:pos></gml:pos>\n";
    text +=	"<sigma>\n";
    //text +=	"<mml:matrix>\n";
    //text +=	"<mml:matrixrow>\n";
    //text +=	"<mml:cn></mml:cn>\n";
    //text +=	"<mml:cn></mml:cn>\n";
    //text +=	"</mml:matrixrow>\n";
    //text +=	"<mml:matrixrow>\n";
    //text +=	"<mml:cn></mml:cn>\n";
    //text +=	"<mml:cn></mml:cn>\n";
    //text +=	"\t\t\t\t</mml:matrixrow>\n";
    //text +=	"\t\t\t</mml:matrix>\n";
    text +=	"</sigma>\n";
    text +=	"</principalPointCoordinates>\n";
    text +=	"<fiducialMarks uom=\"#mm\">\n";
    text +=	"<fiducialMark key=\"1\">\n";
    text +=	"<gml:pos></gml:pos>\n";
    text +=	"<sigma></sigma>\n";
    text +=	"</fiducialMark>\n";
    text +=	"<fiducialMark key=\"2\">\n";
    text +=	"<gml:pos></gml:pos>\n";
    text +=	"<sigma></sigma>\n";
    text +=	"</fiducialMark>\n";
    text +=	"<fiducialMark key=\"3\">\n";
    text +=	"<gml:pos></gml:pos>\n";
    text +=	"<sigma></sigma>\n";
    text +=	"</fiducialMark>\n";
    text +=	"<fiducialMark key=\"4\">\n";
    text +=	"<gml:pos></gml:pos>\n";
    text +=	"<sigma></sigma>\n";
    text +=	"</fiducialMark>\n";
    text +=	"</fiducialMarks>\n";
    text += "</sensor>";

    manager->addComponent(text, "sensors");

    text = "";

    text += "<flight key=\"1\" sensor_key=\"1\">\n";
    text += "<flightId></flightId>\n";
    text += "<description></description>\n";
    text += "<execution></execution>\n";
    text += "<producerName></producerName>\n";
    text += "<nominalScale>\n";
    text += "<mml:mfrac>\n";
    text += "<mml:mn></mml:mn>\n";
    text += "<mml:mn></mml:mn>\n";
    text += "</mml:mfrac>\n";
    text += "</nominalScale>\n";
    text += "<flightHeight uom=\"#m\"></flightHeight>\n";
    text += "<overlap>\n";
    text += "<longitudinal uom=\"#%\"></longitudinal>\n";
    text += "<transversal uom=\"#%\"></transversal>\n";
    text += "</overlap>\n";
    text += "</flight>";

    manager->addComponent(text, "flights");

    // Estas linhas fazem parte realmente deste cÃÂ³digo e nÃÂ£o sÃÂ£o parte da gambiarra a cima
    saveFile();

    refreshMenu();
}

void ProjectUserInterface_Qt::loadFile(std::string filenameAtStart)
{
    if (!confirmToClose())
        return;

    QString filename;

    if (filenameAtStart == "")
    {
        filename = QFileDialog::getOpenFileName(this, "Open File", ".", "*.epp");
    }
    else
    {
        //filename = QString(filenameAtStart.c_str());
        //const char* name = filenameAtStart.c_str();
        filename = QString::fromLocal8Bit(filenameAtStart.c_str());
    }

    if (filename == "")
        return;
    else
    {
        if (manager->loadFile(filename.toLocal8Bit().constData()))
        {
            EDomElement imagesXml(manager->getXml("images").c_str());
            std::deque<EDomElement> imagesEdom=imagesXml.elementsByTagName("image");

            QDir dirImage(filename.left(filename.lastIndexOf('/')));
            QFileInfo imageFileInfo;

            QString imagesMissing="";
            int contMissings=0;
            for (int i=0 ;i < (int)imagesEdom.size() ;i++)
            {
                QString imagesName(imagesEdom.at(i).elementByTagName("filePath").toString().append("/").c_str());
                dirImage.setCurrent(imagesName);
                imagesName.append(imagesEdom.at(i).elementByTagName("fileName").toString().c_str());
                imageFileInfo.setFile(dirImage.absoluteFilePath(imagesName));
                if (!imageFileInfo.exists())
                {
                    imagesMissing.append(contMissings%4==3 ? imageFileInfo.fileName().append("  \n") : imageFileInfo.fileName().append(" , "));
                    contMissings++;
                }
            }
            imagesMissing.chop(3);
            if(imagesMissing.compare("")!=0)
            {
                QMessageBox* alertImages= new QMessageBox(QMessageBox::Warning,"Images missing",imagesMissing.prepend("Those images are missing:\n").append("."));
                alertImages->show();
            }

            addDockWidget(Qt::LeftDockWidgetArea,projectDockWidget);
            addDockWidget(Qt::BottomDockWidgetArea,debuggerDockWidget);
            setCorner(Qt::TopLeftCorner,Qt::LeftDockWidgetArea);
            setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
            projectDockWidget->setVisible(true);
            introWidget->setVisible(false);
            centerArea.setVisible(true);
            controlButtons.setVisible(true);
            offset.setVisible(true);


            manager->savedIn = filename.toLocal8Bit().constData();
            //manager->savedIn = filename.toStdString();
            //qDebug("load savedIn: %s",savedIn.c_str());

            actionSave_file->setEnabled(false);
            actionSave_file_as->setEnabled(true);

            //***************************************************************************************************
            // Este tratamento pode precisar de ajustes para cumprir o requisito do e-foto de ser CrossPlataform
            int i=filename.lastIndexOf("/");

            QString fileName = "<fileName>"+filename.right(filename.length()-i-1)+"</fileName>";
            QString filePath = "<filePath>"+filename.left(i)+"</filePath>";

            EDomElement node(manager->getXml("projectHeader"));

            node.replaceChildByTagName("fileName",fileName.toLocal8Bit().constData());
            node.replaceChildByTagName("filePath",filePath.toLocal8Bit().constData());

            manager->editComponent("Header", node.getContent());
            //***************************************************************************************************

            QDir dir(filename.left(i));
            dir.setCurrent(dir.absolutePath());


            newTree();
        }
        else
        {
            QMessageBox* alert = NULL;
            switch (manager->informFileVersionError())
            {
            case 0:
                alert = new QMessageBox(QMessageBox::Warning,"Unable to open file","The e-foto software was unable to open the selected file.\nThis may be due to:\n\n - Unsupported characters in the file's name or path (maybe accented characters or whitespace);\n - The file does not exist;\n - A bug in the program.\n\nTry changing the file's name or path and try again.");
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                alert = new QMessageBox(QMessageBox::Warning,"Unable to load file","The e-foto software was unable to load the selected file.\nThis may be due to:\n\n - Unsupported file version;\n - The file is not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug in the program.\n\nTry changing the file or version of the software and try again.");
                break;
            default:
                alert = new QMessageBox(QMessageBox::Warning,"Unable to open or load file","The e-foto software was unable to open or load the selected file.\nThis may be due to:\n\n - Unsupported characters in the file's name or path (maybe accented characters or whitespace);\n - Unsupported file version;\n - The file is not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug in the program.\n\nTry changing the file's name or path or changing the file or version of the software and try again.");
                break;
            }
            alert->show();
        }
    }
    updateLabelFileName();

    refreshMenu();
}

void ProjectUserInterface_Qt::saveFile()
{
    if (!manager->savedIn.empty())
    {
        QDateTime dateTimeBackup = headerForm.dateTimeEditModificationDate->dateTime();
        //headerForm.dateTimeEditModificationDate->setTime(QTime::currentTime());
        headerForm.dateTimeEditModificationDate->setDateTime(QDateTime::currentDateTime());
        manager->editComponent("Header", headerForm.getvalues());
        if (manager->saveFile(manager->savedIn))
        {
            actionSave_file->setEnabled(false);
        }
        else
        {
            headerForm.dateTimeEditModificationDate->setDateTime(dateTimeBackup);
            manager->editComponent("Header", headerForm.getvalues());
            QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to save file", "The e-foto software was unable to save the file.\nThis may be due to:\n\n - The disk does not have enought free space;\n - You do not have the needed permissions;\n - A bug in the program.\n\nCheck your disk space and permissions and try again.");
            alert->show();
        }
        if (centerArea.currentIndex() == 0)
            viewHeader();
        updateTree();
    }
    else
        saveFileAs(); // Isso jÃÂ¡ nÃÂ£o executa nunca mais. Eu acho!
}

bool ProjectUserInterface_Qt::saveFileAs(bool onNewProject)
{
    QString filename;
    QString caption;
    if (onNewProject)
        caption = tr("Save A New File");
    else
        caption = tr("Save File As");

    QFileDialog salvar(this,caption,".","*.epp");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("epp");
    if(salvar.exec())
    {
        filename = salvar.selectedFiles()[0];
        if (filename.isEmpty())
            return false;
        else
        {
            //***************************************************************************************************
            // Este tratamento pode precisar de ajustes para cumprir o requisito do e-foto de ser CrossPlataform
            QString extension = filename.right(4);
            if (extension.toLower() != ".epp")
                filename.append(".epp");
            int i=filename.lastIndexOf("/");

            QString fileName = filename.right(filename.length()-i-1);
            QString filePath = filename.left(i);

            QString fileNameBackup = headerForm.lineEditFileName->text();
            QString filePathBackup = headerForm.lineEditFilePath->text();
            QDateTime dateTimeCreatBackup = headerForm.dateTimeEditCreationDate->dateTime();
            QDateTime dateTimeModifBackup = headerForm.dateTimeEditModificationDate->dateTime();

            headerForm.lineEditFilePath->setText(filePath);
            headerForm.lineEditFileName->setText(fileName);
            headerForm.dateTimeEditCreationDate->setDateTime(QDateTime::currentDateTime());
            headerForm.dateTimeEditModificationDate->setDateTime(QDateTime::currentDateTime());

            manager->editComponent("Header", headerForm.getvalues());
            //***************************************************************************************************

            if (manager->saveFile(filename.toLocal8Bit().constData()))
            {
                manager->savedIn = filename.toLocal8Bit().constData();
                actionSave_file->setEnabled(false);
            }
            else
            {
                headerForm.lineEditFilePath->setText(filePathBackup);
                headerForm.lineEditFileName->setText(fileNameBackup);
                headerForm.dateTimeEditCreationDate->setDateTime(dateTimeCreatBackup);
                headerForm.dateTimeEditModificationDate->setDateTime(dateTimeModifBackup);
                manager->editComponent("Header", headerForm.getvalues());

                QMessageBox* alert = new QMessageBox(QMessageBox::Warning,"Unable to save file", "The e-foto software was unable to save the file.\nThis may be due to:\n\n - The disk does not have enought free space;\n - You do not have the needed permissions;\n - The file's name or path is invalid (maybe accented characters or whitespace);\n - A bug in the program.\n\nCheck your disk space and permissions and try again.");
                alert->show();
                return false;
            }
            if (centerArea.currentIndex() == 0)
                viewHeader();
            updateTree();
        }
        return true;
    }
    return false;
}

void ProjectUserInterface_Qt::executeIO()
{
    bool ok;
    QStringList items;
    std::deque<std::string> strItems = manager->listImages();
    for (unsigned int i = 0; i < strItems.size(); i++)
        items << strItems.at(i).c_str();
    QString chosen = QInputDialog::getItem(this, tr("Select your image!"), tr("Image name:"), items, 0, false, &ok);
    if (ok)
    {
        int value = manager->getImageId(chosen.toLocal8Bit().constData());
        if (value != -1)
        {
            changeModule = true;
            confirmToClose();
            LoadingScreen::instance().show();
            qApp->processEvents();
            this->close();
            manager->startModule("InteriorOrientation", value);
            changeModule = false;
        }
    }
}

void ProjectUserInterface_Qt::executeSR()
{
    bool ok;
    QStringList items;
    std::deque<std::string> strItems = manager->listImages();
    for (unsigned int i = 0; i < strItems.size(); i++)
        items << strItems.at(i).c_str();
    QString chosen = QInputDialog::getItem(this, tr("Select your image!"), tr("Image name:"), items, 0, false, &ok);
    if (ok)
    {
        int value = manager->getImageId(chosen.toLocal8Bit().constData());
        if (value != -1)
        {
            changeModule = true;
            confirmToClose();
            LoadingScreen::instance().show();
            qApp->processEvents();
            this->close();
            manager->startModule("SpatialRessection", value);
            changeModule = false;
        }
    }
}

void ProjectUserInterface_Qt::executeDEM()
{
    changeModule = true;
    confirmToClose();
    LoadingScreen::instance().show();
    qApp->processEvents();
    this->close();
    manager->startModule("DEM-Extraction",0);
    changeModule = false;
}

void ProjectUserInterface_Qt::executeSP()
{
    changeModule = true;
    confirmToClose();
    LoadingScreen::instance().show();
    qApp->processEvents();
    this->close();
    manager->startModule("StereoPlotter",0);
    changeModule = false;
}

void ProjectUserInterface_Qt::executeReport()
{
    changeModule = true;
    confirmToClose();
    LoadingScreen::instance().show();
    qApp->processEvents();
    this->close();
    manager->startModule("Report",0);
    changeModule = false;
}

void ProjectUserInterface_Qt::executePTReport()
{
    changeModule = true;
    confirmToClose();
    LoadingScreen::instance().show();
    qApp->processEvents();
    this->close();
    manager->startModule("PT-Report",0);
    changeModule = false;
}

void ProjectUserInterface_Qt::executeFT()
{
    changeModule = true;
    confirmToClose();
    LoadingScreen::instance().show();
    qApp->processEvents();
    this->close();
    manager->startModule("FotoTriangulation", 0);
    changeModule = false;

}

void ProjectUserInterface_Qt::executeOrtho()
{
    changeModule = true;
    confirmToClose();
    LoadingScreen::instance().show();
    qApp->processEvents();
    this->close();
    manager->startModule("Ortho-rectification",0);
    changeModule = false;
}

void ProjectUserInterface_Qt::processTreeClick(QModelIndex index)
{
    if ((editState && index!=currentIndex)||addNewState)
    {
        if (controlButtons.saveButton->isEnabled())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
                                          "Do you want to keep all changes?",
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                controlButtons.saveButton->click();
            }
            else if (reply == QMessageBox::No)
            {
                controlButtons.cancelButton->click();
            }
            else
            {
                treeWidget->setCurrentIndex(currentIndex);
                return;
            }
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
                                          "Registration data is not complete and will be lost. Continue?",
                                          QMessageBox::Yes | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                controlButtons.cancelButton->click();
            }
            else
            {
                //if (addNewState)
                //  treeWidget->setCurrentIndex(0);
                //else
                treeWidget->setCurrentIndex(currentIndex);
                return;
            }
        }
    }
    currentIndex = index;
    ETreeModel* etm = manager->getTreeModel();
    if (index.parent() == QModelIndex())
    {
        if (etm->dataAt(index.row()) == "projectHeader")
        {
            viewHeader();
        }
        else if (etm->dataAt(index.row()) == "terrain")
        {
            viewTerrain();
        }
        else if (etm->dataAt(index.row()) == "sensors")
        {
            //viewSensors(); mudanÃÂ§a temporaria
            viewSensor(1);
        }
        else if (etm->dataAt(index.row()) == "flights")
        {
            //viewFlights(); mudanÃÂ§a temporaria
            viewFlight(1);
        }
        else if (etm->dataAt(index.row()) == "images")
        {
            viewImages();
        }
        else if (etm->dataAt(index.row()) == "points")
        {
            viewPoints();
        }
    }
    else
    {
        if (etm->dataAt(index.parent().row()) == "sensors")
        {
            //viewSensor(etm->idAt(index.parent().row(), index.row())); mudanÃÂ§a temporaria
            viewSensor(1);
        }
        else if (etm->dataAt(index.parent().row()) == "flights")
        {
            //viewFlight(etm->idAt(index.parent().row(), index.row())); mudanÃÂ§a temporaria
            viewFlight(1);
        }
        else if (etm->dataAt(index.parent().row()) == "images")
        {
            viewImage(etm->idAt(index.parent().row(), index.row()));
        }
        else if (etm->dataAt(index.parent().row()) == "points")
        {
            viewPoint(etm->idAt(index.parent().row(), index.row()));
        }
    }
}

void ProjectUserInterface_Qt::exportSPFile()
{
    bool ok;
    QStringList items;
    std::deque<std::string> strItems = manager->listImages();
    for (unsigned int i = 0; i < strItems.size(); i++)
        items << strItems.at(i).c_str();

    QString chosen1 = QInputDialog::getItem(this, tr("Select left image"), tr("Image name:"), items, 0, false, &ok);
    if (!ok) return;

    items.removeOne(chosen1);

    QString chosen2 = QInputDialog::getItem(this, tr("Select right image!"), tr("Image name:"), items, 0, false, &ok);
    if (!ok) return;

    QFileDialog salvar(this, "Save File", ".", "*.txt");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString filename = salvar.selectedFiles()[0];
        if (filename.isEmpty())
            return;

        int image1 = manager->getImageId(chosen1.toLocal8Bit().constData());
        int image2 = manager->getImageId(chosen2.toLocal8Bit().constData());
        if (image1 != -1 && image2 != -1)
        {
            bool result = manager->makeSPFile(filename.toLocal8Bit().constData(), image1, image2);
            if (result == false)
            {
                QMessageBox msgBox;
                msgBox.setText("Error: invalid input parameters.");
                msgBox.exec();
            }
        }
    }
}

//Metodo que sera reescrito depois que trabalharmos esta interface grafica com o QLinguist
void ProjectUserInterface_Qt::languageChange()
{
    //retranslateUi(this);
}

void ProjectUserInterface_Qt::newTree()
{
    treeWidget->clear();
    treeItems.clear();
    //this->treeWidget->setHeaderHidden(false);
    if (manager->savedIn == "")
    {
        //treeWidget->setHeaderLabel(tr("New Project"));
        projectDockWidget->setWindowTitle(tr("Open Project: *Unsaved"));
    }
    else
    {
        //treeWidget->setHeaderLabel(headerForm.lineEditFileName->text());
        if (actionSave_file->isEnabled())
            projectDockWidget->setWindowTitle(QString(tr("Open Project: *")) + headerForm.lineEditFileName->text());
        else
            projectDockWidget->setWindowTitle(QString(tr("Open Project: ")) + headerForm.lineEditFileName->text());
    }
    if (headerForm.lineEditName->text() != "")
        setWindowTitle(headerForm.lineEditName->text() + tr(" - efoto[Project Manager]"));
    else
        setWindowTitle(tr("efoto[Project Manager]"));

    ETreeModel* etm = manager->getTreeModel();

    for (unsigned int i = 0; i < etm->countChildren(); i++)
    {
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
        treeItems.push_back(rootItem);
        if (etm->dataAt(i) == "projectHeader")
            rootItem->setText(0, tr("Project Header"));
        else if (etm->dataAt(i) == "terrain")
            rootItem->setText(0, tr("Terrain"));
        else if (etm->dataAt(i) == "flights")
            rootItem->setText(0, tr("Flight"));
        else if (etm->dataAt(i) == "sensors")
            rootItem->setText(0, tr("Sensor"));
        else if (etm->dataAt(i) == "images")
            rootItem->setText(0, tr("Images"));
        else if (etm->dataAt(i) == "points")
            rootItem->setText(0, tr("Points"));
        if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" || etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights"))
        {
            for (unsigned int j = 0; j < etm->countGrandchildren(i); j++)
            {
                QTreeWidgetItem* leafItem = new QTreeWidgetItem(rootItem);
                leafItem->setText(0, tr(etm->dataAt(i,j).c_str()));
            }
        }
    }
    treeWidget->setCurrentItem(treeItems.at(0));
    processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::updateTree()
{
    if (manager->savedIn == "")
    {
        projectDockWidget->setWindowTitle(tr("Open Project: *Unsaved"));
    }
    else
    {
        updateLabelFileName();
    }
    if (headerForm.lineEditName->text() != "")
        setWindowTitle(headerForm.lineEditName->text() + tr(" - efoto[Project Manager]"));
    else
        setWindowTitle(tr("efoto[Project Manager]"));

    ETreeModel* etm = manager->getTreeModel();

    for (int i = 0; i < (int)etm->countChildren(); i++)
    {
        if (i < treeItems.size())
        {
            if (etm->dataAt(i) == "projectHeader")
                treeItems.at(i)->setText(0, tr("Project Header"));
            else if (etm->dataAt(i) == "terrain")
                treeItems.at(i)->setText(0, tr("Terrain"));
            else if (etm->dataAt(i) == "flights")
                treeItems.at(i)->setText(0, tr("Flight"));
            else if (etm->dataAt(i) == "sensors")
                treeItems.at(i)->setText(0, tr("Sensor"));
            else if (etm->dataAt(i) == "images")
                treeItems.at(i)->setText(0, tr("Images"));
            else if (etm->dataAt(i) == "points")
                treeItems.at(i)->setText(0, tr("Points"));
            if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" || etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights"))
                for (int j = 0; j < (int)etm->countGrandchildren(i); j++)
                {
                    if (j < treeItems.at(i)->childCount())
                    {
                        treeItems.at(i)->child(j)->setText(0, tr(etm->dataAt(i,j).c_str()));
                    }
                    else
                    {
                        QTreeWidgetItem* leafItem = new QTreeWidgetItem(treeItems.at(i));
                        leafItem->setText(0, tr(etm->dataAt(i,j).c_str()));
                    }
                }
        }
        else
        {
            QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
            treeItems.push_back(rootItem);
            if (etm->dataAt(i) == "projectHeader")
                rootItem->setText(0, tr("Project Header"));
            else if (etm->dataAt(i) == "terrain")
                rootItem->setText(0, tr("Terrain"));
            else if (etm->dataAt(i) == "flights")
                rootItem->setText(0, tr("Flight"));
            else if (etm->dataAt(i) == "sensors")
                rootItem->setText(0, tr("Sensor"));
            else if (etm->dataAt(i) == "images")
                rootItem->setText(0, tr("Images"));
            else if (etm->dataAt(i) == "points")
                rootItem->setText(0, tr("Points"));
            if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" || etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights"))
            {
                for (unsigned int j = 0; j < etm->countGrandchildren(i); j++)
                {
                    QTreeWidgetItem* leafItem = new QTreeWidgetItem(rootItem);
                    leafItem->setText(0, tr(etm->dataAt(i,j).c_str()));
                }
            }
        }
    }
}

bool ProjectUserInterface_Qt::exec()
{
    actionSave_file->setEnabled(!manager->getSavedState());

    refreshMenu();

    //PAULO -> codigo para dar um refresh no formulario
    updateCurrentForm();
    this->show();
    LoadingScreen::instance().close();
    qApp->processEvents();
    changeModule = false;
    updateLabelFileName();
    //if (qApp->exec())
    //return false;
    return true;
}

void ProjectUserInterface_Qt::selectImage(int pos)
{
    treeWidget->setCurrentItem(treeItems.at(4)->child(pos));
    processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::selectPoint(int pos)
{
    treeWidget->setCurrentItem(treeItems.at(5)->child(pos));
    processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::viewHeader()
{
    controlButtons.disconnectAll();
    controlButtons.singleConnect();
    connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveHeader()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelHeader()));

    EDomElement node(manager->getXml("projectHeader"));

    headerForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
    centerArea.setCurrentIndex(0);
    currentForm = &headerForm;
    currentForm->setReadOnly(true);

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
    controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::viewTerrain()
{
    controlButtons.disconnectAll();
    controlButtons.singleConnect();
    connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveTerrain()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelTerrain()));

    EDomElement node(manager->getXml("terrain"));

    terrainForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
    centerArea.setCurrentIndex(1);
    currentForm = &terrainForm;
    currentForm->setReadOnly(true);

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
    controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::viewSensors()
{
    // Por enquanto este metodo esta fora de uso.
    /*
controlButtons.disconnectAll();
controlButtons.multiConnect();
connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newSensor()));
connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewSensor()));
connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelSensors()));

EDomElement node(manager->getXml("sensors"));

sensorsForm.fillvalues(node.getContent());
centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
centerArea.setCurrentIndex(X); // falta definir este indice
currentForm = &sensorsForm;
currentForm->setReadOnly(true);

debuggerTextEdit->clear();
debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
debuggerTextEdit->setReadOnly(true);

menuProject->setEnabled(true);
menuExecute->setEnabled(true);
*/
}

void ProjectUserInterface_Qt::viewSensor(int id)
{
    currentItemId = id;
    controlButtons.disconnectAll();
    controlButtons.singleConnect();
    connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveSensor()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelSensor()));
    connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deleteSensor()));
    EDomElement node(manager->getXml("sensor", "key", Conversion::intToString(id)));
    sensorForm.clearForm();
    sensorForm.setFormLocale(QLocale(QLocale::system()));
    sensorForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
    centerArea.setCurrentIndex(2);
    currentForm = &sensorForm;
    currentForm->setReadOnly(true);
    validatingSensor();

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewFlights()
{
    // Por enquanto este metodo esta fora de uso.
    /*
controlButtons.disconnectAll();
controlButtons.multiConnect();
connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newFlight()));
connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewFlight()));
connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelFlights()));

EDomElement node(manager->getXml("flights"));

flightsForm.fillvalues(node.getContent());
centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
centerArea.setCurrentIndex(X); // falta definir este indice
currentForm = &flightsForm;
currentForm->setReadOnly(true);

debuggerTextEdit->clear();
debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
debuggerTextEdit->setReadOnly(true);

menuProject->setEnabled(true);
menuExecute->setEnabled(true);
*/
}

void ProjectUserInterface_Qt::viewFlight(int id)
{
    currentItemId = id;
    controlButtons.disconnectAll();
    controlButtons.singleConnect();
    connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveFlight()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelFlight()));
    connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deleteFlight()));

    EDomElement node(manager->getXml("flight", "key", Conversion::intToString(id)));

    flightForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
    centerArea.setCurrentIndex(3);
    currentForm = &flightForm;
    currentForm->setReadOnly(true);
    validatingFlight();

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewImages()
{
    controlButtons.disconnectAll();
    controlButtons.multiConnect();
    connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newImage()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewImage()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelImages()));

    EDomElement imgs(manager->getXml("images"));

    imagesForm.fillvalues(imgs.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget {border: 0px}");
    centerArea.setCurrentIndex(4);
    currentForm = &imagesForm;
    currentForm->setReadOnly(true);

    EDomElement ios(manager->getXml("interiorOrientation"));
    if (ios.getContent()!="")
    {
        imagesForm.setIOsAvailable(ios.getContent());
    }

    EDomElement eos(manager->getXml("exteriorOrientation"));
    EDomElement srs(manager->getXml("spatialResections"));
    EDomElement ptri(manager->getXml("phototriangulation"));
    std::string eoContent = eos.getContent();
    eoContent.append(srs.getContent());
    eoContent.append(ptri.getContent());
    if (eos.getContent()!="")
    {
        imagesForm.setEOsAvailable(eoContent);
    }
    int height=(imagesForm.imagesTable->rowCount()+2)*imagesForm.imagesTable->rowHeight(0)+imagesForm.label->height()+imagesForm.importButton->height();
    imagesForm.setMinimumSize(imagesForm.imagesTable->width(),height);
    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(imgs.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);

}

void ProjectUserInterface_Qt::viewImage(int id)
{
    currentItemId = id;
    controlButtons.disconnectAll();
    controlButtons.singleConnect();
    connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelImage()));
    connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deleteImage()));

    EDomElement node(manager->getXml("image", "key", Conversion::intToString(id)));

    imageForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
    centerArea.setCurrentIndex(5);
    currentForm = &imageForm;
    currentForm->setReadOnly(true);
    validatingImage();

    EDomElement node2(manager->getXml("imageIO", "image_key", Conversion::intToString(id)));
    if (node2.getContent()!="")
    {
        imageForm.groupBox->setVisible(true);
        imageForm.a0Label->setText("a0: " + QString::number(node2.elementByTagName("a0").toDouble(),'f', 4));
        imageForm.a1Label->setText("a1: " + QString::number(node2.elementByTagName("a1").toDouble(),'f',4));
        imageForm.a2Label->setText("a2: " + QString::number(node2.elementByTagName("a2").toDouble(),'f',4));
        imageForm.b0Label->setText("b0: " + QString::number(node2.elementByTagName("b0").toDouble(),'f',4));
        imageForm.b1Label->setText("b1: " + QString::number(node2.elementByTagName("b1").toDouble(),'f',4));
        imageForm.b2Label->setText("b2: " + QString::number(node2.elementByTagName("b2").toDouble(),'f',4));
    }
    else
    {
        imageForm.groupBox->setVisible(false);
    }

    EDomElement node3(manager->getXml("imageEO", "image_key", Conversion::intToString(id)));
    if (node3.getContent()!="")
    {
        imageForm.groupBox_2->setVisible(true);

        if (node3.attribute("type")=="spatialResection")
            imageForm.groupBox_2->setTitle("EO Parameters (Spatial Resection)");
        else if (node3.attribute("type")=="user")
            imageForm.groupBox_2->setTitle("EO Parameters (3D Direct Georeferencing)");
        else
            imageForm.groupBox_2->setTitle("EO Parameters (Phototriangulation)");

        imageForm.x0Label->setText("X0: " + QString::number(node3.elementByTagName("X0").toDouble(),'f',4) +" m");
        imageForm.y0Label->setText("Y0: " + QString::number(node3.elementByTagName("Y0").toDouble(),'f',4) +" m");
        imageForm.z0Label->setText("Z0: " + QString::number(node3.elementByTagName("Z0").toDouble(),'f',4) +" m");
        imageForm.omegaLabel->setText(QString::fromUtf8( " ω: ") + QString::number(node3.elementByTagName("omega").toDouble()*180/M_PI,'f',4) + QString::fromUtf8("°"));
        imageForm.phiLabel->setText(QString::fromUtf8( " φ: ") + QString::number(node3.elementByTagName("phi").toDouble()*180/M_PI,'f',4) + QString::fromUtf8("°"));
        imageForm.kappaLabel->setText(QString::fromUtf8( " κ: ") + QString::number(node3.elementByTagName("kappa").toDouble()*180/M_PI,'f',4) + QString::fromUtf8("°"));
    }
    else
    {
        imageForm.groupBox_2->setVisible(false);
    }

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
    //	actionInterior_Orientation->setEnabled(availabeOI());
}

void ProjectUserInterface_Qt::viewPoints()
{
    controlButtons.disconnectAll();
    controlButtons.multiConnect();
    connect((&controlButtons)->newButton, SIGNAL(clicked()), this, SLOT(newPoint()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(saveNewPoint()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPoints()));

    EDomElement node(manager->getXml("points"));

    pointsForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget {border: 0px}");
    centerArea.setCurrentIndex(6);
    currentForm = &pointsForm;
    currentForm->setReadOnly(true);
    int height=(pointsForm.pointsTable->rowCount()+2)*pointsForm.pointsTable->rowHeight(0)+pointsForm.importButton->height()+pointsForm.label->height();
    pointsForm.setMinimumSize(pointsForm.pointsTable->width(),height);
    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewPoint(int id)
{
    currentItemId = id;
    controlButtons.disconnectAll();
    controlButtons.singleConnect();
    connect((&controlButtons)->editButton, SIGNAL(clicked()), this, SLOT(enableForm()));
    connect((&controlButtons)->saveButton, SIGNAL(clicked()), this, SLOT(savePoint()));
    connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this, SLOT(cancelPoint()));
    connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this, SLOT(deletePoint()));

    EDomElement node(manager->getXml("point", "key", Conversion::intToString(id)));

    pointForm.setImageList(manager->listImageKeys(), manager->listImages());
    pointForm.fillvalues(node.getContent());
    centerArea.setStyleSheet("QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget {border: 0px}");
    centerArea.setCurrentIndex(7);
    currentForm = &pointForm;
    currentForm->setReadOnly(true);
    validatingPoint();

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(node.indent('\t').getContent().c_str()));
    debuggerTextEdit->setReadOnly(true);

    menuProject->setEnabled(true);
    enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::enableExecuteMenu(bool status)
{
    menuExecute->setEnabled(status);

    refreshMenu();
    //actionInterior_Orientation->setEnabled(status);
    //actionSpatial_resection->setEnabled(status);
    //actionFoto_Tri->setEnabled(status);
    //actionStereo->setEnabled(status);
    //actionDEMExtraction->setEnabled(status);
    //actionOrtho_rectification->setEnabled(status);
    //actionPTReport->setEnabled(status);
    //actionReport->setEnabled(status);
}

// Habilitando cada view...
void ProjectUserInterface_Qt::enableForm()
{
    currentForm->setReadOnly(false);
    debuggerTextEdit->setReadOnly(false);
    centerArea.setStyleSheet(this->styleSheet());
    currentForm->setStyleSheet(this->styleSheet());
    menuProject->setEnabled(false);
    enableExecuteMenu(false);
    editState = true;
    currentForm->setFocus();
}

// Salvando cada view...
void ProjectUserInterface_Qt::saveHeader()
{
    editState = false;
    manager->editComponent("Header", headerForm.getvalues());
    viewHeader();
    actionSave_file->setEnabled(true);
    updateTree();
}

void ProjectUserInterface_Qt::saveTerrain()
{
    editState = false;
    manager->editComponent("Terrain", terrainForm.getvalues());
    viewTerrain();
    actionSave_file->setEnabled(true);
    updateTree();
}

void ProjectUserInterface_Qt::saveSensor()
{
    editState = false;
    manager->editComponent("Sensor", currentItemId, sensorForm.getvalues());
    viewSensor(currentItemId);
    actionSave_file->setEnabled(true);
    updateTree();
}

void ProjectUserInterface_Qt::saveFlight()
{
    editState = false;
    manager->editComponent("Flight", currentItemId, flightForm.getvalues());
    viewFlight(currentItemId);
    actionSave_file->setEnabled(true);
    updateTree();
}

void ProjectUserInterface_Qt::saveImage()
{
    std::string xmlString="";
    std::stringstream eoXML;
    bool existOE, userOE=false;

    editState = false;
    xmlString = imageForm.getvalues();
    manager->editComponent("Image", currentItemId, xmlString);
    EDomElement imageUserEO(xmlString);
    if (imageUserEO.hasTagName("GNSS") && imageUserEO.hasTagName("INS"))
    {
        existOE = !(manager->getXml("imageEO","image_key",Conversion::intToString(currentItemId))=="");
        if (existOE)
        {
            EDomElement oeXMLObj(manager->getXml("imageEO","image_key",Conversion::intToString(currentItemId)));
            userOE = oeXMLObj.attribute("type")=="user";
            if (userOE)
                manager->removeComponent( "EO", currentItemId);
        }
        if (!existOE || userOE)
        {
            EDomElement xyzXml(imageUserEO.elementByTagName("gml:pos"));
            EDomElement omegaXML(imageUserEO.elementByTagName("omega"));
            EDomElement phiXML(imageUserEO.elementByTagName("phi"));
            EDomElement kappaXML(imageUserEO.elementByTagName("kappa"));
            std::deque<double> XA;
            XA= xyzXml.toGmlPos();
            eoXML << "\t<imageEO type=\"user\" image_key=\"" << Conversion::intToString(currentItemId) << "\">\n";
            eoXML << "\t\t\t<Xa>\n";
            eoXML << "\t\t\t\t<X0 uom=\"#m\">"<< Conversion::doubleToString(XA.at(0)) << "</X0>\n";
            eoXML << "\t\t\t\t<Y0 uom=\"#m\">"<< Conversion::doubleToString(XA.at(1)) << "</Y0>\n";
            eoXML << "\t\t\t\t<Z0 uom=\"#m\">"<< Conversion::doubleToString(XA.at(2)) << "</Z0>\n";
            eoXML << "\t\t\t\t<omega uom=\"#rad\">"<< omegaXML.toString() << "</omega>\n";
            eoXML << "\t\t\t\t<phi uom=\"#rad\">"<< phiXML.toString() << "</phi>\n";
            eoXML << "\t\t\t\t<kappa uom=\"#rad\">"<< kappaXML.toString() << "</kappa>\n";
            eoXML << "\t\t\t</Xa>\n";
            eoXML << "</imageEO>\n";
            manager->addComponent( eoXML.str(), "exteriorOrientation");
            actionStereo->setEnabled(availableStereoPlotter());
            actionDEMExtraction->setEnabled(availableDemExtraction());
            actionOrtho_rectification->setEnabled(availableOrthoImage());
        }
    }
    viewImage(currentItemId);
    actionSave_file->setEnabled(true);
    updateTree();
}

void ProjectUserInterface_Qt::savePoint()
{
    editState = false;
    manager->editComponent("Point", currentItemId, pointForm.getvalues());
    viewPoint(currentItemId);
    actionSave_file->setEnabled(true);
    updateTree();
}

// Cancelando cada view...
void ProjectUserInterface_Qt::cancelHeader()
{
    editState = false;
    viewHeader();
}

void ProjectUserInterface_Qt::cancelTerrain()
{
    editState = false;
    viewTerrain();
}

void ProjectUserInterface_Qt::cancelSensor()
{
    editState = false;
    viewSensor(currentItemId);
}

void ProjectUserInterface_Qt::cancelFlight()
{
    editState = false;
    viewFlight(currentItemId);
}

void ProjectUserInterface_Qt::cancelImage()
{
    editState = false;
    viewImage(currentItemId);
}

void ProjectUserInterface_Qt::cancelPoint()
{
    editState = false;
    viewPoint(currentItemId);
}

void ProjectUserInterface_Qt::cancelSensors()
{
    addNewState = false;
    viewSensors();
}

void ProjectUserInterface_Qt::cancelFlights()
{
    addNewState = false;
    viewFlights();
}

void ProjectUserInterface_Qt::cancelImages()
{
    addNewState = false;
    viewImages();
}

void ProjectUserInterface_Qt::cancelPoints()
{
    addNewState = false;
    viewPoints();
}

// Criando um vazio de cada view...

void ProjectUserInterface_Qt::newSensor()
{
    // Por enquanto este metodo esta fora de uso.
    /*
std::string text = "";

text += "<sensor key=\"\">\n";
text +=	"<sensorId></sensorId>\n";
text +=	"<type>\n";
text +=	"<geometry></geometry>\n";
text +=	"<platform></platform>\n";
text +=	"<detector></detector>\n";
text +=	"<energySource></energySource>\n";
text +=	"<spectralRanges uom=\"#um\">\n";
text +=	"<spectralRange band=\"\">\n";
text +=	"<inferiorLimit></inferiorLimit>\n";
text +=	"<superiorLimit></superiorLimit>\n";
text +=	"</spectralRange>\n";
text +=	"</spectralRanges>\n";
text +=	"</type>\n";
text +=	"<description></description>\n";
text +=	"<calibrationCertificate>\n";
text +=	"<number></number>\n";
text +=	"<dispatch></dispatch>\n";
text +=	"<expiration></expiration>\n";
text +=	"</calibrationCertificate>\n";
text +=	"<focalDistance uom=\"#mm\">\n";
text +=	"<value></value>\n";
text +=	"<sigma></sigma>\n";
text +=	"</focalDistance>\n";
text +=	"<distortionCoefficients>\n";
text +=	"<radialSymmetric>\n";
text +=	"<k0>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k0>\n";
text +=	"<k1>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k1>\n";
text +=	"<k2>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k2>\n";
text +=	"<k3>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</k3>\n";
text +=	"</radialSymmetric>\n";
text +=	"<decentered>\n";
text +=	"<P1>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</P1>\n";
text +=	"<P2>\n";
text +=	"<value></value>\n";
text += "<sigma></sigma>\n";
text +=	"</P2>\n";
text +=	"</decentered>\n";
text +=	"</distortionCoefficients>\n";
text +=	"<principalPointCoordinates uom=\"#mm\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma>\n";
text +=	"<mml:matrix>\n";
text +=	"<mml:matrixrow>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"</mml:matrixrow>\n";
text +=	"<mml:matrixrow>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"<mml:cn></mml:cn>\n";
text +=	"</mml:matrixrow>\n";
text +=	"</mml:matrix>\n";
text +=	"</sigma>\n";
text +=	"</principalPointCoordinates>\n";
text +=	"<fiducialMarks uom=\"#mm\">\n";
text +=	"<fiducialMark key=\"1\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiducialMark>\n";
text +=	"<fiducialMark key=\"2\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiducialMark>\n";
text +=	"<fiducialMark key=\"3\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiducialMark>\n";
text +=	"<fiducialMark key=\"4\">\n";
text +=	"<gml:pos></gml:pos>\n";
text +=	"<sigma></sigma>\n";
text +=	"</fiducialMark>\n";
text +=	"</fiducialMarks>\n";
text += "</sensor>";
*/
}

void ProjectUserInterface_Qt::newFlight()
{
    // Por enquanto este metodo esta fora de uso.
    /*
std::string text = "";

text += "<flight key=\"\" sensor_key=\"\">\n";
text += "<flightId></flightId>\n";
text += "<description></description>\n";
text += "<execution></execution>\n";
text += "<producerName></producerName>\n";
text += "<nominalScale>\n";
text += "<mml:mfrac>\n";
text += "<mml:mn></mml:mn>\n";
text += "<mml:mn></mml:mn>\n";
text += "</mml:mfrac>\n";
text += "</nominalScale>\n";
text += "<flightHeight uom=\"#m\"></flightHeight>\n";
text += "<overlap>\n";
text += "<longitudinal uom=\"#%\"></longitudinal>\n";
text += "<transversal uom=\"#%\"></transversal>\n";
text += "</overlap>\n";
text += "</flight>";
*/
}

void ProjectUserInterface_Qt::newImage()
{
    addNewState = true;
    currentItemId = manager->getFreeImageId();

    std::string text = "";
    text += "<image key=\"" + Conversion::intToString(currentItemId) + "\" sensor_key=\"1\" flight_key=\"1\">\n";
    text += "<imageId></imageId>\n";
    text += "<width uom=\"#px\"></width>\n";
    text += "<height uom=\"#px\"></height>\n";
    text += "<fileName></fileName>\n";
    text += "<filePath>.</filePath>\n";
    text += "<flightId></flightId>\n";
    text += "<resolution uom=\"#dpi\">N/A</resolution>\n";
    text += "</image>";

    imageForm.fillvalues(text);
    centerArea.setCurrentIndex(5);
    currentForm = &imageForm;
    currentForm->setReadOnly(false);
    centerArea.setStyleSheet(this->styleSheet());
    validatingImage();

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(text.c_str()));
    debuggerTextEdit->setReadOnly(true);
}

void ProjectUserInterface_Qt::newPoint()
{
    addNewState = true;
    //updateView(new QLabel("Futuro view de point"));
    std::string text = "";

    currentItemId = manager->getFreePointId();
    text += "<point key=\"" + Conversion::intToString(currentItemId) + "\" type=\"\">\n";
    text += "<pointId></pointId>\n";
    text += "<description></description>\n";
    text += "<spatialCoordinates uom=\"#m\">\n";
    text += "<gml:pos></gml:pos>\n";
    text += "<sigma>Not Available</sigma>\n";
    text += "</spatialCoordinates>\n";
    text += "</point>";

    pointForm.setImageList(manager->listImageKeys(), manager->listImages());
    pointForm.fillvalues(text);
    centerArea.setCurrentIndex(7);
    currentForm = &pointForm;
    currentForm->setReadOnly(false);
    centerArea.setStyleSheet(this->styleSheet());
    validatingPoint();

    debuggerTextEdit->clear();
    debuggerTextEdit->setText(QString::fromUtf8(text.c_str()));
    debuggerTextEdit->setReadOnly(false);
}

// Salvando um novo de cada view...

void ProjectUserInterface_Qt::saveNewSensor()
{
    /*
std::string currentData = debuggerTextEdit->toPlainText().toStdString();
if (EDomValidator::validateSensor(currentData))
{
manager->addComponent(currentData, "sensors");
newTree();
viewSensors();
}
else
{
QMessageBox msgBox;
msgBox.setText("Erro: Xml passado ÃÂ© invÃÂ¡lido.");
msgBox.exec();
emit viewButtons->editButton->click();
}
*/
}

void ProjectUserInterface_Qt::saveNewFlight()
{
    /*
std::string currentData = debuggerTextEdit->toPlainText().toStdString();
if (EDomValidator::validateFlight(currentData))
{
manager->addComponent(currentData, "flights");
newTree();
viewFlights();
}
else
{
QMessageBox msgBox;
msgBox.setText("Erro: Xml passado ÃÂ© invÃÂ¡lido.");
msgBox.exec();
emit viewButtons->editButton->click();
}
*/
}

void ProjectUserInterface_Qt::saveNewImage()
{
    addNewState = false;
    manager->addComponent(imageForm.getvalues(),"images");
    viewImages();
    actionSave_file->setEnabled(true);
    updateTree();
}

void ProjectUserInterface_Qt::saveNewPoint()
{
    addNewState = false;
    manager->addComponent(pointForm.getvalues(),"points");
    viewPoints();
    actionSave_file->setEnabled(true);
    updateTree();
}

// Apagando cada view...

void ProjectUserInterface_Qt::deleteFlight()
{
}

void ProjectUserInterface_Qt::deleteSensor()
{
}

void ProjectUserInterface_Qt::deletePoint()
{
}

void ProjectUserInterface_Qt::deleteImage()
{
}

void ProjectUserInterface_Qt::toggleDebug()
{
    if (debuggerDockWidget->isVisible())
        debuggerDockWidget->setVisible(false);
    else
        debuggerDockWidget->setVisible(true);
}

void ProjectUserInterface_Qt::showAbout()
{
    AboutForm* about = new AboutForm();
    about->show();
}

// CÃÂ³digos das classes extras.

//#include <QStringList>

/*TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
 parentItem = parent;
 itemData = data;
}

TreeItem::~TreeItem()
{
 qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
 childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
 return childItems.value(row);
}

int TreeItem::childCount() const
{
 return childItems.count();
}

int TreeItem::columnCount() const
{
 return itemData.count();
}

QVariant TreeItem::data(int column) const
{
 return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
 return parentItem;
}

int TreeItem::row() const
{
 if (parentItem)
  return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

 return 0;
}

TreeModel::TreeModel(const QString &data, QObject *parent)
 : QAbstractItemModel(parent)
{
 QList<QVariant> rootData;
 rootData << "";
 rootItem = new TreeItem(rootData);
 setupModelData(data.split(QString("\n")), rootItem);
}

TreeModel::~TreeModel()
{
 delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
 if (parent.isValid())
  return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
 else
  return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
 if (!index.isValid())
  return QVariant();

 if (role != Qt::DisplayRole)
  return QVariant();

 TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

 return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
 if (!index.isValid())
  return 0;

 return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                  int role) const
{
 if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  return rootItem->data(section);

 return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
  const
{
 if (!hasIndex(row, column, parent))
  return QModelIndex();

 TreeItem *parentItem;

 if (!parent.isValid())
  parentItem = rootItem;
 else
  parentItem = static_cast<TreeItem*>(parent.internalPointer());

 TreeItem *childItem = parentItem->child(row);
 if (childItem)
  return createIndex(row, column, childItem);
 else
  return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
 if (!index.isValid())
  return QModelIndex();

 TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
 TreeItem *parentItem = childItem->parent();

 if (parentItem == rootItem)
  return QModelIndex();

 return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
 TreeItem *parentItem;
 if (parent.column() > 0)
  return 0;

 if (!parent.isValid())
  parentItem = rootItem;
 else
  parentItem = static_cast<TreeItem*>(parent.internalPointer());

 return parentItem->childCount();
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
 QList<TreeItem*> parents;
 QList<int> indentations;
 parents << parent;
 indentations << 0;

 int number = 0;

 while (number < lines.count()) {
  int position = 0;
  while (position < lines[number].length()) {
   if (lines[number].mid(position, 1) != " ")
        break;
   position++;
  }

  QString lineData = lines[number].mid(position).trimmed();

  if (!lineData.isEmpty()) {
   // Read the column data from the rest of the line.
   QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
   QList<QVariant> columnData;
   for (int column = 0; column < columnStrings.count(); ++column)
        columnData << columnStrings[column];

   if (position > indentations.last()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if (parents.last()->childCount() > 0) {
         parents << parents.last()->child(parents.last()->childCount()-1);
         indentations << position;
        }
   } else {
        while (position < indentations.last() && parents.count() > 0) {
         parents.pop_back();
         indentations.pop_back();
        }
   }

   // Append a new item to the current parent's list of children.
   parents.last()->appendChild(new TreeItem(columnData, parents.last()));
  }

  number++;
 }
}*/

void ProjectUserInterface_Qt::validatingSensor()
{
    if ((sensorForm.sensorIdLineEdit->text() == "") ||
            (!sensorForm.fid0SigmaController->getValidate()) ||
            (!sensorForm.fid1SigmaController->getValidate()) ||
            (!sensorForm.fid2SigmaController->getValidate()) ||
            (!sensorForm.fid3SigmaController->getValidate()) ||
            ((!sensorForm.fid4SigmaController->getValidate())&&(sensorForm.fiducialMarksCombo->currentIndex()==1)) ||
            ((!sensorForm.fid5SigmaController->getValidate())&&(sensorForm.fiducialMarksCombo->currentIndex()==1)) ||
            ((!sensorForm.fid6SigmaController->getValidate())&&(sensorForm.fiducialMarksCombo->currentIndex()==1)) ||
            ((!sensorForm.fid7SigmaController->getValidate())&&(sensorForm.fiducialMarksCombo->currentIndex()==1)) ||
            (!sensorForm.principalSigmaController->getValidate()) ||
            (!sensorForm.radialSigmaController->getValidate()) ||
            (!sensorForm.decenteredSigmaController->getValidate()) ||
            (!sensorForm.calibratedSigmaController->getValidate())){
        controlButtons.saveButton->setEnabled(false);
    }
    else
        controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingFlight()
{
    if ((flightForm.flightIDLineEdit->text() == "")){
        controlButtons.saveButton->setEnabled(false);
    }
    else
        controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingImage()
{
    if ((imageForm.imageIDLine->text() == "") || (imageForm.resolutionSpin->text() == " ") || (imageForm.fileNameLine->text() == "") || (!imageForm.gnssSigmaController->getValidate()) || (!imageForm.insSigmaController->getValidate())){
        controlButtons.saveButton->setEnabled(false);
    }
    else
        controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingPoint()
{
    if ((pointForm.lineEdit_gcp_id->text() == "") || (!pointForm.sigmaController->getValidate()))
        controlButtons.saveButton->setEnabled(false);
    else
        controlButtons.saveButton->setEnabled(true);
}

bool ProjectUserInterface_Qt::confirmToClose()
{
    if (editState || addNewState)
    {
        if (controlButtons.saveButton->isEnabled())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
                                          "Do you want to keep all changes?",
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                controlButtons.saveButton->click();
            }
            else if (reply == QMessageBox::No)
            {
                controlButtons.cancelButton->click();
            }
            else
            {
                return false;
            }
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr(" Warning: leaving form in edit mode"),
                                          "Registration data is not complete and will be lost. Continue?",
                                          QMessageBox::Yes | QMessageBox::Cancel);
            if (reply == QMessageBox::Yes)
            {
                controlButtons.cancelButton->click();
            }
            else
            {
                return false;
            }
        }
    }
    //qDebug() << actionSave_file->isEnabled();
    if (actionSave_file->isEnabled())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr(" Warning: You have unsaved data"),
                                      tr("Do you want to save all changes?"),
                                      (changeModule ? QMessageBox::Yes | QMessageBox::No : QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
        if (reply == QMessageBox::Yes)
        {
            actionSave_file->trigger();
        }
        else if (reply == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void ProjectUserInterface_Qt::refreshMenu()
{
    actionSpatial_resection->setEnabled(availableSR());
    actionFoto_Tri->setEnabled(availablePhotoTri());

    actionStereo->setEnabled(availableStereoPlotter());
    actionDEMExtraction->setEnabled(availableDemExtraction());
    actionOrtho_rectification->setEnabled(availableOrthoImage());
    actionPTReport->setEnabled(availablePhotoTri());
}

QString ProjectUserInterface_Qt::getSavedIn()
{
    //HeaderForm* parent =(HeaderForm*)&(this->headerForm);
    //qDebug("saved in %s",savedIn.c_str());

    //cout<< "cout "<<savedIn.c_str()<<"\n"<<endl;
    //qDebug()<<headerForm.lineEditFilePath->text();

    //return  QString(manager->savedIn.c_str()).left(manager->savedIn.find_last_of('/'));
    return  QString(QString::fromLocal8Bit(manager->savedIn.c_str())).left(manager->savedIn.find_last_of('/'));
    //return parent->lineEditFilePath->text();
}

/** This function import points from a *.txt file with a one point in each line
*  in pattern GCPID\tE\tN\tH\tdE\tdN\tdH or GCPID\tE\tN\tH
*/
void ProjectUserInterface_Qt::importPointsFromTxt()
{
    QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import File"),".","*.txt");
    if(importFileName=="")
        return;
    QFile *importFile = new QFile(importFileName);
    QStringList pointsList;

    importFile->open(QIODevice::ReadOnly);

    while(!importFile->atEnd())
    {
        QByteArray line = importFile->readLine();
        QString aux(line);
        //pointsList << aux.left(aux.lastIndexOf('\n'));
        pointsList << aux.simplified();;
    }
    importFile->close();

    QWidget *loadWidget= new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    QProgressBar loading;
    QPushButton cancelButton("Cancel");
    loading.setRange(0,pointsList.size());

    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading,Qt::AlignCenter);
    loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
    connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Loading Points"));
    loading.setMinimumSize(300,30);
    loadWidget->show();

    std::string newPointXML;
    for (int i=0; i<pointsList.length() && loadWidget!=NULL;i++)
    {
        loading.setFormat(tr("%v/%m : %p%"));
        loading.setValue(i+1);
        newPointXML+=pointTxtToXml(pointsList.at(i),manager->getFreePointId()+i,i+1);
        /*if ( newPointXML.hasTagName("pointId") )
  {
   manager->addComponent(newPointXML.getContent().data(),"points");
  }*/
    }
    manager->addComponent(newPointXML,"points");
    loadWidget->close();

    updateTree();
    viewPoints();
    actionSave_file->setEnabled(true);
}

/** This function convert a point data from a *.txt line in a children point XML valid
*/
std::string ProjectUserInterface_Qt::pointTxtToXml(QString point, int key, int line, std::string typePoint)
{
    std::stringstream aux;
    std::string gcpIdField, typeField, eField, nField, hField, dEField, dNField, dHField;
    QStringList fields= point.split(QRegExp("\\s+")); // split by any sequences of whitespace
    // check	control	 tie
    if (fields.length() == 7)
    {

        gcpIdField = fields.at(0).toLocal8Bit().constData();
        /*
  typeField = point.split("\t").at(1).toStdString().c_str();
  if(typeField == "Tie")
   typePoint="photogrammetric";
  else if (typeField== "Control")
   typePoint="control";
  else if (typeField== "Check")
   typePoint="verification";
*/
        eField = fields.at(1).toStdString();
        nField = fields.at(2).toStdString();
        hField = fields.at(3).toStdString();
        dEField = fields.at(4).toStdString();
        dNField = fields.at(5).toStdString();
        dHField = fields.at(6).toStdString();

        aux << "<point key=\""<< Conversion::intToString(key)<<"\" type=\"" << typePoint << "\">\n";
        aux << "<pointId>" << gcpIdField << "</pointId>\n";
        aux << "<description>" << "Put point description here" << "</description>\n";
        aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
        aux << "<gml:pos>" << eField << " " << nField << " " << hField << "</gml:pos>\n";
        aux << "<sigma>\n";
        aux << "<mml:matrix>\n";
        aux << "<mml:matrixrow>\n";
        aux << "<mml:cn>" << dEField << "</mml:cn>\n";
        aux << "</mml:matrixrow>\n";
        aux << "<mml:matrixrow>\n";
        aux << "<mml:cn>" << dNField << "</mml:cn>\n";
        aux << "</mml:matrixrow>\n";
        aux << "<mml:matrixrow>\n";
        aux << "<mml:cn>" << dHField << "</mml:cn>\n";
        aux << "</mml:matrixrow>\n";
        aux << "</mml:matrix>\n";
        aux << "</sigma>\n";
        aux << "</spatialCoordinates>\n";
        aux << "<imagesMeasurements>\n";
        aux << "</imagesMeasurements>\n";
        aux << "</point>";
    }
    else if (fields.length() == 4)
    {
        //gcpIdField = point.split("\t").at(0).toStdString().c_str();
        gcpIdField = fields.at(0).toStdString();
        /*
  typeField = point.split("\t").at(1).toStdString().c_str();
  if(typeField == "Tie")
   typePoint="photogrammetric";
  else if (typeField== "Control")
   typePoint="control";
  else if (typeField== "Check")
   typePoint="verification";
*/
        eField = fields.at(1).toStdString();
        nField = fields.at(2).toStdString();
        hField = fields.at(3).toStdString();

        aux << "<point key=\""<< Conversion::intToString(key)<<"\" type=\"" << typePoint << "\">\n";
        aux << "<pointId>" << gcpIdField << "</pointId>\n";
        aux << "<description>" << "Put point description here" << "</description>\n";
        aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
        aux << "<gml:pos>" << eField << " " << nField << " " << hField << "</gml:pos>\n";
        aux << "<sigma>Not Available</sigma>\n";
        aux << "</spatialCoordinates>\n";
        aux << "<imagesMeasurements>\n";
        aux << "</imagesMeasurements>\n";
        aux << "</point>";
    }else{
        QMessageBox::warning(this, tr(" fileWarning "), tr("The point in line %1 from imported file\nhas incomplete or corrupted data").arg(line));
        return "";
    }

    return aux.str();
    //EDomElement newPointXml(newXml.c_str());

}

/** This function exports all points in current XML to *.txt file with a one point in
*   each line in pattern GCPID\tE\tN\tH\tdE\tdN\tdH or GCPID\tE\tN\tH
*/
void ProjectUserInterface_Qt::exportPointsToTxt()
{
    QFileDialog salvar(this,tr("Export file"),".","*.txt");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString fileSaveName = salvar.selectedFiles()[0];
        if (fileSaveName.isEmpty())
            return;

        if (!fileSaveName.endsWith(".txt"))
            fileSaveName.append(".txt");


        QFile *exportFileName= new QFile(fileSaveName);
        exportFileName->setFileName(fileSaveName);
        exportFileName->open(QIODevice::WriteOnly);

        EDomElement points(manager->getXml("points").c_str());

        QWidget *loadWidget= new QWidget();
        loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
        QProgressBar loading;
        QPushButton cancelButton("Cancel");
        loading.setRange(0,points.children().size());

        QVBoxLayout loadLayout;
        loadLayout.addWidget(&loading,Qt::AlignCenter);
        loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
        connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

        loadWidget->setLayout(&loadLayout);
        loadWidget->setWindowTitle(tr("Exporting Points"));
        loading.setMinimumSize(300,30);
        loadWidget->show();

        std::deque<EDomElement> point=points.elementsByTagName("point");
        for (int i=0; i< (int)point.size(); i++)
        {
            loading.setFormat(tr("Point %v/%m : %p%"));
            loading.setValue(i);
            exportFileName->write(edomPointToTxt(point.at(i)).data());
        }
        loadWidget->close();
        exportFileName->close();

        //exportDigitalCoordinates();
    }
}

/** This function convert a EDomElement children Point in a line *.txt point format
*/
std::string ProjectUserInterface_Qt::edomPointToTxt(EDomElement points)
{
    std::stringstream aux;
    std::stringstream stdev;
    QString gmlpos=points.elementByTagName("gml:pos").toString().c_str();
    aux << points.elementByTagName("pointId").toString().c_str()<< "\t";
    //aux << points.attribute("type")<< "\t";

    aux << (gmlpos.split(" ").at(0)).toStdString().c_str() <<"\t"<<(gmlpos.split(" ").at(1)).toStdString().c_str()<<"\t"<<(gmlpos.split(" ").at(2)).toStdString().c_str();

    Matrix stdevMatrix;
    stdevMatrix.xmlSetData(points.elementByTagName("mml:matrix").getContent());

    if (points.hasTagName("mml:matrix"))
    {
        /*
        aux << "\t" << points.elementsByTagName("mml:cn").at(0).toString().c_str() <<"\t";
        aux << points.elementsByTagName("mml:cn").at(1).toString().c_str() <<"\t";
        aux << points.elementsByTagName("mml:cn").at(2).toString().c_str();
        */
        aux << "\t" << stdevMatrix.get(1,1) <<"\t";
        aux << stdevMatrix.get(2,2) <<"\t";
        aux << stdevMatrix.get(3,3);
    }

    aux <<"\n";
    std::string result=aux.str();
    //qDebug("tamanho: %d",points.elementsByTagName("mml:matrix").size());

    //qDebug("stdev:%s",stdev.str().c_str());
    //qDebug("result:%s",result.c_str());

    return result.c_str();
}

/** This function import images from a *.txt file with a one image in each line
*   in pattern ...
*/
void ProjectUserInterface_Qt::importImagesFromTxt()
{
    QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import File"),".","*.txt");
    QFile *importFile = new QFile(importFileName);
    QStringList imagesList;

    importFile->open(QIODevice::ReadOnly);

    while(!importFile->atEnd())
    {
        QByteArray line = importFile->readLine();
        QString aux(line);
        imagesList << aux.remove('\n');
    }

    importFile->close();

    for (int i=0; i<imagesList.length();i++)
    {
        EDomElement newImageXML=imageTxtToXml(/*imagesList.at(i),manager->getFreeImageId(),i+1*/);
        if ( newImageXML.hasTagName("imageId") )
        {
            manager->addComponent(newImageXML.getContent().data(),"images");
        }
    }
    updateTree();
    viewImages();

}

/** This function convert image data from a *.txt line in a children image XML valid
*
*/
EDomElement ProjectUserInterface_Qt::imageTxtToXml(/*QString image, int key, int line, int sensorKey, int flightKey*/)
{
    std::stringstream aux;

    // implementar o xml de images aqui
    /*
 aux << "<image key=\""<< intToString(key)<<"\" sensor_key=\"" << sensorKey << "\">" << "\" flight_key=\"" << flightKey << "\">\n";
 aux << "<imageId>" << "" << "</imageId>\n";
 aux << "<width uom=\"#px\"" << "" << "</width>\n";
 aux << "<height uom=\"#px\"" << ""  << "</height>\n";
 aux << "<fileName>" << "" << "</fileName>\n";
 aux << "<filePath>" << "" << "</filePath>\n";
 aux << "<resolution uom=\"#dpi\">" << "" << "</resolution>\n";
 aux << "<GNSS uom=\"#m\" type=\"Unknown\">\n";
 aux << "<gml:pos>" << "E" << " " << "N" << " " << "H" << "</gml:pos>\n";
 aux << "<sigma>Not Available</sigma>\n";
 aux << "</GNSS>\n";
 aux << "<INS uom=\"#rad\" type=\"Unknown\">\n";
 aux << "<omega>" << "" << "</omega>\n";
 aux << "<phi>" << "" << "</phi>\n";
 aux << "<kappa>" << "" << "</kappa>\n";
 aux << "<sigma>Not Available</sigma>\n";
 aux << "</INS>\n";
 aux << "</image>";

 QMessageBox::warning(this, tr(" Warning "), tr("The point in line %1 from imported file\nhas incomplete or corrupted data").arg(line));

 */
    std::string newXml=aux.str();
    EDomElement newImageXml(newXml.c_str());

    return newImageXml;
}

//inserido pelo Paulo 05/09/2011
// Coloca as coordenadas digitais linha e coluna nos pontos;
void ProjectUserInterface_Qt::importDigitalCoordinatesFromTxt()
{
    importPointsFromTxt2();

    QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import File"),".","*.txt");
    if(importFileName=="")
        return;
    QFile *importFile = new QFile(importFileName);
    QStringList pointsList;

    importFile->open(QIODevice::ReadOnly);

    while(!importFile->atEnd())
    {
        QByteArray line = importFile->readLine();
        QString aux(line);
        pointsList << aux.remove('\n');
    }

    importFile->close();

    QWidget *loadWidget= new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    QProgressBar loading;
    QPushButton cancelButton("Cancel");
    loading.setRange(0,pointsList.length());

    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading,Qt::AlignCenter);
    loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
    connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Loading Digitals Coordinates"));
    loading.setMinimumSize(300,30);
    loadWidget->show();

    EDomElement imagesXml(manager->getXml("points").c_str());
    std::deque<EDomElement> imagesEdom=imagesXml.elementsByTagName("point");

    for (int i=0; i<pointsList.length() ;i++)
    {
        /*
            loading.setFormat(tr("Coordinate %v/%m : %p%"));
            loading.setValue(i+1);
            QStringList digitalPoint=coordinates.split("\t");
            string imagekey=digitalPoint.at(0).toStdString().c_str();
            string pointkey=digitalPoint.at(1).toStdString().c_str();
            string colValue=digitalPoint.at(2).toStdString().c_str();
            string linValue=digitalPoint.at(3).toStdString().c_str();

            if (pointkey=imagesEdom.at(i))
                stringstream aux;
            aux << "<imageCoordinates uom=\"#px\" image_key=\""<< imagekey <<"\">\n";
            aux << "<gml:pos>" << colValue << " " << linValue << "</gml:pos>\n";
            aux << "</imageCoordinates>";
            */
        if(!insertDigitalCoordinates(pointsList.at(i)))//,imagesEdom.at()))
            QMessageBox::warning(this, tr(" Warning "), tr("The point in line %1 from imported file\nhas incomplete or corrupted data").arg(i));
    }
    loadWidget->close();
    updateTree();
    viewPoints();
    actionSave_file->setEnabled(true);
}

bool ProjectUserInterface_Qt::insertDigitalCoordinates(QString coordinates)
{
    QStringList digitalPoint=coordinates.split("\t");
    if (digitalPoint.size()!=4)
        return false;
    else
    {
        std::string imagekey=digitalPoint.at(0).toStdString().c_str();
        std::string pointkey=digitalPoint.at(1).toStdString().c_str();
        std::string colValue=digitalPoint.at(2).toStdString().c_str();
        std::string linValue=digitalPoint.at(3).toStdString().c_str();

        EDomElement pointsXML(manager->getXml("points").c_str());
        EDomElement point(pointsXML.elementByTagAtt("point","key",pointkey));
        std::stringstream aux;
        aux << "<imageCoordinates uom=\"#px\" image_key=\""<< imagekey <<"\">";
        aux << "<gml:pos>" << colValue << " " << linValue << "</gml:pos>";
        aux << "</imageCoordinates>\n";
        point.addChildAtTagName("imagesMeasurements",aux.str());
        manager->editComponent("Point",Conversion::stringToInt(pointkey),point.getContent());
        return true;
    }
}

bool ProjectUserInterface_Qt::availablePhotoTri()
{
    EDomElement ois(manager->getXml("interiorOrientation"));
    EDomElement images(manager->getXml(("images")));
    //qDebug("numero Images = %d, OIS feitas %d",images.children().size(),ois.children().size());

    if(images.children().size()<=ois.children().size())
        return true;
    return false;
}

// ******
// Bug reported by Martin Veermer
// Big fixed by Marcelo Teixeira Silveira
bool ProjectUserInterface_Qt::availableSR()
{
    return availablePhotoTri();
}

bool ProjectUserInterface_Qt::availableLP()
{
    QSettings efotoSettings("uerj","efoto");
    return efotoSettings.contains("lastProject");
}

bool ProjectUserInterface_Qt::availableStereoPlotter()
{
    std::deque<EDomElement> images = EDomElement(manager->getXml(("images"))).children();
    EDomElement oes(manager->getXml("exteriorOrientation"));
    EDomElement pt(manager->getXml(("phototriangulation")));
    EDomElement srs(manager->getXml(("spatialResections")));

    bool hasAllEOs = true;

    for (unsigned int i = 0;i < images.size(); i++)
    {
        EDomElement img(images.at(i));
        int key = Conversion::stringToInt(img.attribute("key"));
        EDomElement EOXml = oes.elementByTagAtt("imageEO","image_key",Conversion::intToString(key));

        std::string type = EOXml.attribute("type");
        if (EOXml.getContent() == "")
        {
            hasAllEOs = false;
            break;
        }

        bool converged = true;
        if (type == "photoTriangulation")
            converged = pt.elementByTagName("converged").toBool();
        else if (type == "spatialResection")
            converged = srs.elementByTagAtt("imageSR","image_key",Conversion::intToString(key)).elementByTagName("converged").toBool();

        if(!converged)
        {
            hasAllEOs = false;
            break;
        }
    }
    return hasAllEOs;
}

bool ProjectUserInterface_Qt::availableDemExtraction()
{
    return availableStereoPlotter();
}

bool ProjectUserInterface_Qt::availableOrthoImage()
{
    return availableStereoPlotter();
}
//**********

/*
// Se houver imagens cadastradas, poderÃÂ¡ se fazer a OrientaÃÂ§ÃÂ£o interior
bool ProjectUserInterface_Qt::availabeOI()
{
 EDomElement images(manager->getXml(("images")));
 if(images.children().size()>0)
  return true;
 else
  return false;
}
// Paulo 24/09/2011
// Se houver pelo menos uma OI e um ponto cadastrado na imagem, poderÃÂ¡ se fazer a OrientaÃÂ§ao exterior
// OBS.: Aqui nÃÂ£o ÃÂ© feita a filtragem de quais imagens poderÃÂ£o ser feitas as OEs!!!!
bool ProjectUserInterface_Qt::availableOE()
{
 EDomElement ois(manager->getXml("interiorOrientation"));
 EDomElement points(manager->getXml("points"));
 deque<EDomElement> point=points.elementsByTagName("point");

 //points.elementByTagAtt("imageCoordinates","key",intToString(i))
 //points.hasTagName("imageCoordinates")
 for (int i=0;i<points.children().size();i++)
 {
  if (point.at(i).hasTagName("imageCoordinates"))
   return true;
 }
 return false;
}
*/
void ProjectUserInterface_Qt::updateCurrentForm()
{
    if(currentForm!=NULL)
    {
        if (currentForm== &headerForm)
            viewHeader();
        else if (currentForm==&terrainForm)
            viewTerrain();
        else if (currentForm==&sensorForm)
            viewSensor(currentItemId);
        else if (currentForm==&flightForm)
            viewFlight(currentItemId);
        else if (currentForm==&imagesForm)
            viewImages();
        else if (currentForm==&imageForm)
            viewImage(currentItemId);
        else if (currentForm==&pointsForm)
            viewPoints();
        else if (currentForm==&pointForm)
            viewPoint(currentItemId);
    }
}

void ProjectUserInterface_Qt::importPointsFromTxt2()
{
    //primeiro arquivo
    QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import ENH file"),".","*.txt");
    if(importFileName=="")
        return;
    QFile *importFile = new QFile(importFileName);
    QStringList pointsList;

    importFile->open(QIODevice::ReadOnly);

    while(!importFile->atEnd())
    {
        QByteArray line = importFile->readLine();
        QString aux(line);
        //pointsList << aux.left(aux.lastIndexOf('\n'));
        pointsList << aux.remove('\n');
    }
    importFile->close();

    //segundo arquivo
    QString importFileName2 = QFileDialog::getOpenFileName(this,tr("Open Import Digital File"),".","*.txt");
    if(importFileName2=="")
        return;
    QFile *importFile2 = new QFile(importFileName2);
    QStringList pointsList2;

    importFile2->open(QIODevice::ReadOnly);

    while(!importFile2->atEnd())
    {
        QByteArray line2 = importFile2->readLine();
        QString aux2(line2);
        //pointsList << aux.left(aux.lastIndexOf('\n'));
        pointsList2 << aux2.remove('\n');
    }
    importFile2->close();

    // Procura e concatena o ponto as suas coordenadas digitais nas imagens
    int cont=0;
    for (int i=0;i<pointsList.size();i++ )
    {
        QStringList fields= pointsList.at(i).split("\t");
        QString gcpIdField = fields.at(0);
        for (int j=0;j<pointsList2.size() && cont!=-1;j++)
        {
            QStringList fieldsDigital= pointsList2.at(j).split("\t");
            //qDebug()<<fieldsDigital;
            QString gcpIdFieldDigital = fieldsDigital.at(1);
            if(gcpIdField.compare(gcpIdFieldDigital)==0)
            {
                //field.append(pointsList2.at(j));
                QString temp=pointsList.at(i);
                temp+="\t";
                temp +=pointsList2.at(j);
                pointsList.replace(i, temp);
                cont++;
            }
        }

    }
    //for (int i=0;i<pointsList.size();i++ )
    //qDebug()<< "Lista " << pointsList.at(0);

    QWidget *loadWidget= new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    QProgressBar loading;
    QPushButton cancelButton("Cancel");
    loading.setRange(0,pointsList.size());

    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading,Qt::AlignCenter);
    loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
    connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Loading Points"));
    loading.setMinimumSize(300,30);
    loadWidget->show();

    std::string newPointXML;
    for (int i=0; i<pointsList.length() && loadWidget!=NULL;i++)
    {
        loading.setFormat(tr("%v/%m : %p%"));
        loading.setValue(i+1);
        newPointXML+=pointTxtToXml2(pointsList.at(i),manager->getFreePointId()+i/*,i+1*/);
        /*if ( newPointXML.hasTagName("pointId") )
  {
   manager->addComponent(newPointXML.getContent().data(),"points");
  }*/
    }
    manager->addComponent(newPointXML,"points");
    loadWidget->close();
    updateTree();
    viewPoints();
    actionSave_file->setEnabled(true);

}

/** This function convert a point data from a *.txt line in a children point XML valid*/
std::string ProjectUserInterface_Qt::pointTxtToXml2(QString point, int key/*, int line*/, std::string typePoint)
{
    bool ok;
    std::stringstream aux;
    std::string gcpIdField, typeField, eField, nField, hField, dEField, dNField, dHField;
    QStringList fields= point.split("\t");
    int numImagesInPoint=fields.size()/4-1;

    gcpIdField = fields.at(0).toStdString().c_str();
    /*
        typeField = point.split("\t").at(1).toStdString().c_str();
            if(typeField == "Tie")
                typePoint="photogrammetric";
            else if (typeField== "Control")
                typePoint="control";
            else if (typeField== "Check")
                typePoint="verification";
        */
    eField = fields.at(1).toStdString();
    nField = fields.at(2).toStdString();
    hField = fields.at(3).toStdString();
    //		dEField = fields.at(4).toStdString();
    //		dNField = fields.at(5).toStdString();
    //		dHField = fields.at(6).toStdString();

    aux << "<point key=\""<< Conversion::intToString(key)<<"\" type=\"" << typePoint << "\">\n";
    aux << "<pointId>" << gcpIdField << "</pointId>\n";
    aux << "<description>" << "Put point description here" << "</description>\n";
    aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
    aux << "<gml:pos>" << eField << " " << nField << " " << hField << "</gml:pos>\n";
    /*aux << "<sigma>\n";
        aux << "<mml:matrix>\n";
        aux << "<mml:matrixrow>\n";
        aux << "<mml:cn>" << dEField << "</mml:cn>\n";
        aux << "</mml:matrixrow>\n";
        aux << "<mml:matrixrow>\n";
        aux << "<mml:cn>" << dNField << "</mml:cn>\n";
        aux << "</mml:matrixrow>\n";
        aux << "<mml:matrixrow>\n";
        aux << "<mml:cn>" << dHField << "</mml:cn>\n";
        aux << "</mml:matrixrow>\n";
        aux << "</mml:matrix>\n";
        aux << "</sigma>\n";*/
    aux << "</spatialCoordinates>\n";
    aux << "<imagesMeasurements>\n";
    for(int i=0;i<numImagesInPoint;i++)
    {
        std::string imagekey =fields.at(4*(i+1)).toStdString();
        //string colValue =(fields.at(4*(i+1)+2).toInt(&ok)).toStdString();
        //string linValue =fields.at(4*(i+1)+3).toStdString();

        int colValue =(fields.at(4*(i+1)+2)).toInt(&ok);
        int linValue =(fields.at(4*(i+1)+3)).toInt(&ok);

        aux << "<imageCoordinates uom=\"#px\" image_key=\""<< imagekey <<"\">";
        aux << "<gml:pos>" << colValue << " " << linValue << "</gml:pos>";
        aux << "</imageCoordinates>\n";
    }

    aux << "</imagesMeasurements>\n";
    aux << "</point>\n";

    return aux.str();
    //EDomElement newPointXml(newXml.c_str());

}

void ProjectUserInterface_Qt::deleteEmptyPoints()
{
    EDomElement points(manager->getXml("points"));
    std::deque<EDomElement> pnts=(points.elementsByTagName("point"));

    //stringstream allPoints;
    //int cont=0;
    for (int i=0; i< (int)pnts.size();i++)
    {
        //		points.attribute();
        if (!pnts.at(i).hasTagName("imageCoordinates"))
        {
            //	allPoints << pnts.at(i).getContent();
            //qDebug("%s",pnts.at(i).getContent().c_str()) ;
            //	cont++;
            manager->editComponent("point",Conversion::stringToInt(pnts.at(i).attribute("key")),"");
        }
    }
    //qDebug("%d",cont);

}

void ProjectUserInterface_Qt::exportDigitalCoordinates()
{
    QFileDialog salvar(this,tr("Export Digital Coordinates"),".","*.txt");
    salvar.setAcceptMode(QFileDialog::AcceptSave);
    salvar.setDefaultSuffix("txt");
    if(salvar.exec())
    {
        QString fileSaveName = salvar.selectedFiles()[0];
        if (fileSaveName.isEmpty())
            return;

        QFile *exportFileName= new QFile(fileSaveName);
        exportFileName->setFileName(fileSaveName);
        exportFileName->open(QIODevice::WriteOnly);

        EDomElement points(manager->getXml("points").c_str());

        QWidget *loadWidget= new QWidget();
        loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
        QProgressBar loading;
        //	QPushButton cancelButton("Cancel");
        loading.setRange(0,points.children().size());

        QVBoxLayout loadLayout;
        loadLayout.addWidget(&loading,Qt::AlignCenter);
        //	loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
        //	connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

        loadWidget->setLayout(&loadLayout);
        loadWidget->setWindowTitle(tr("Exporting Points"));
        loading.setMinimumSize(300,30);
        loadWidget->show();
        std::deque<EDomElement> point=points.elementsByTagName("point");
        for (int i=0; i<(int)point.size(); i++)
        {
            loading.setFormat(tr("Point %v/%m : %p%"));
            loading.setValue(i);
            exportFileName->write(edomDigitalCoordinatesPointToTxt(point.at(i)).data());
        }
        loadWidget->close();
        exportFileName->close();
    }
}

/** This function convert a EDomElement children Point in a line *.txt point format
 */

std::string ProjectUserInterface_Qt::edomDigitalCoordinatesPointToTxt(EDomElement points)
{
    std::stringstream aux;
    std::deque<EDomElement> digitalCoordinates=points.elementsByTagName("imageCoordinates");

    std::string pointId=points.elementByTagName("pointId").toString();
    EDomElement ede;
    QString gmlpos;
    bool ok;
    for (int i=0; i<(int)digitalCoordinates.size();i++)
    {
        ede=digitalCoordinates.at(i);
        gmlpos=ede.elementByTagName("gml:pos").toString().c_str();
        //string col = (gmlpos.split(" ").at(0)).toStdString();
        //string lin = (gmlpos.split(" ").at(1)).toStdString();
        double col = (gmlpos.split(" ").at(0)).toDouble(&ok);
        double lin = (gmlpos.split(" ").at(1)).toDouble(&ok);
        aux << ede.attribute("image_key") << "\t" << pointId << "\t" << col << "\t" << lin <<"\n";
    }

    std::string result=aux.str();
    return result.c_str();
}


void ProjectUserInterface_Qt::importImagesBatch()
{
    //primeiro arquivo
    //QString importDirName = QFileDialog::getExistingDirectory(this,tr("Open directory of images"),".",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //if(importDirName=="")
    //return;

    QStringList importFilesName = QFileDialog::getOpenFileNames(this, "Select all images files", ".", "*.tif *.png *.bmp *.jpg");
    if(importFilesName.size()==0)
        return;

    std::string xmlImages="";

    QWidget *loadWidget= new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    QProgressBar loading;
    loading.setRange(0,importFilesName.size());
    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading,Qt::AlignCenter);
    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Exporting Points"));
    loading.setMinimumSize(300,30);


    QMessageBox msgBox;
    msgBox.setText("Loading images");
    msgBox.setInformativeText("All images has the same dimensions? (Yes to fast import)");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int resp=msgBox.exec();

    loadWidget->show();
    if (resp==QMessageBox::No)
    {
        int dpi = 300;
        for (int i=0; i<importFilesName.size();i++)
        {
            bool ok;
            dpi = QInputDialog::getInt(this, tr("Inform images dpi"), QString("dpi for image ").append(importFilesName.at(i)).append(":"), dpi, 0, 10000, 1, &ok);
            loading.setFormat(tr("Image %v/%m : %p%"));
            // Paulo: Quando for liberado para o usuario futuramente, o metodo ira procurar uma key disponivel para a imagem, mesmo que as keys tenham "buracos"
            xmlImages+=addImageXml(importFilesName.at(i),manager->getFreeImageId()+i,dpi);
            loading.setValue(i+1);
        }
    }
    else
    {
        QImage firstImage(importFilesName.at(0));
        int imageWidth=firstImage.width();
        int imageHeight=firstImage.height();

        bool ok;
        int dpi = QInputDialog::getInt(this, tr("Inform images dpi"), tr("dpi for all images:"), 300, 0, 10000, 1, &ok);
        for (int i=0; i<importFilesName.size();i++)
        {
            loading.setFormat(tr("Image %v/%m : %p%"));
            //QFileInfo imageFileInfo(importFilesName.at(i));
            // Paulo: Quando for liberado para o usuario futuramente, o metodo ira procurar uma key disponivel para a imagem, mesmo que as keys tenham "buracos"
            xmlImages+=addImageXml(importFilesName.at(i),manager->getFreeImageId()+i,imageWidth,imageHeight,dpi);
            loading.setValue(i+1);
        }
    }

    manager->addComponent(xmlImages,"images");

    loadWidget->close();
    updateTree();
    viewImages();
    actionSave_file->setEnabled(true);

}

std::string ProjectUserInterface_Qt::addImageXml(QString fileName, int keyImage, int dpi)
{

    std::stringstream imageXml;
    QImage image(fileName);
    QDir absolutePath(QString::fromLocal8Bit(manager->savedIn.c_str()));
    int i=fileName.lastIndexOf("/");
    int j=absolutePath.relativeFilePath(fileName).lastIndexOf(('/'));

    QString fileImagePath(".");
    if (j>0)
        fileImagePath=(absolutePath.relativeFilePath(fileName).left(j));

    QString sugestionID=fileName.right(fileName.length()-i-1);
    sugestionID.chop(4);//Retira a extensao do arquivo, considerando que a extensao e formada por 3 letras

    imageXml << "\t<image key=\""<< Conversion::intToString(keyImage) << "\" sensor_key=\"1\" flight_key=\"1\">\n";
    //imageXml << "\t\t<imageId>"<< sugestionID.toStdString()<<"</imageId>\n";
    imageXml << "\t\t<imageId>"<< sugestionID.toLocal8Bit().constData()<<"</imageId>\n";
    imageXml << "\t\t<width uom=\"#px\">"<<Conversion::intToString(image.width())<<"</width>\n";
    imageXml << "\t\t<height uom=\"#px\">"<<Conversion::intToString(image.height())<<"</height>\n";
    //imageXml << "\t\t<fileName>"<< fileName.right(fileName.length()-i-1).toStdString()<<"</fileName>\n";
    imageXml << "\t\t<fileName>"<< fileName.right(fileName.length()-i-1).toLocal8Bit().constData()<<"</fileName>\n";
    //imageXml << "\t\t<filePath>"<< fileImagePath.toStdString()<<"</filePath>\n";
    imageXml << "\t\t<filePath>"<< fileImagePath.toLocal8Bit().constData()<<"</filePath>\n";
    imageXml << "\t\t<resolution uom=\"#dpi\">"<< dpi << "</resolution>\n";
    imageXml << "\t</image>\n";

    return imageXml.str();
}


std::string ProjectUserInterface_Qt::addImageXml(QString fileName, int keyImage, int widthImages, int heightImages, int dpi)
{

    std::stringstream imageXml;
    QDir absolutePath (QString::fromLocal8Bit(manager->savedIn.c_str()));

    int j=absolutePath.relativeFilePath(fileName).lastIndexOf(('/'));
    QString fileImagePath(".");
    if (j>0)
        fileImagePath=(absolutePath.relativeFilePath(fileName).left(j));
    fileName=fileName.right(fileName.length()-fileName.lastIndexOf('/')-1);

    QString sugestionID=fileName;//Retira a extensao do arquivo, considerando que a extensao e formada por 3 letras
    sugestionID.chop(4);
    imageXml << "\t<image key=\""<< Conversion::intToString(keyImage) << "\" sensor_key=\"1\" flight_key=\"1\">\n";
    //imageXml << "\t\t<imageId>"<< sugestionID.toStdString()<<"</imageId>\n";
    imageXml << "\t\t<imageId>"<< sugestionID.toLocal8Bit().constData()<<"</imageId>\n";
    imageXml << "\t\t<width uom=\"#px\">"<<Conversion::intToString(widthImages)<<"</width>\n";
    imageXml << "\t\t<height uom=\"#px\">"<<Conversion::intToString(heightImages)<<"</height>\n";
    //imageXml << "\t\t<fileName>"<< fileName.toStdString()<<"</fileName>\n";
    imageXml << "\t\t<fileName>"<< fileName.toLocal8Bit().constData()<<"</fileName>\n";
    //imageXml << "\t\t<filePath>"<< fileImagePath.toStdString()<<"</filePath>\n";
    imageXml << "\t\t<filePath>"<< fileImagePath.toLocal8Bit().constData()<<"</filePath>\n";
    imageXml << "\t\t<resolution uom=\"#dpi\">"<< dpi << "</resolution>\n";
    imageXml << "\t</image>\n";

    return imageXml.str();
}


void ProjectUserInterface_Qt::importOIDigitalMarks()
{
    QString importFileName = QFileDialog::getOpenFileName(this,tr("Open Import OI Digital File"),".","*.txt");
    if(importFileName=="")
        return;
    QFile *importFile = new QFile(importFileName);
    QStringList marksList;

    importFile->open(QIODevice::ReadOnly);

    while(!importFile->atEnd())
    {
        QByteArray line = importFile->readLine();
        QString aux(line);
        marksList << aux.remove('\n');
    }
    importFile->close();

    QWidget *loadWidget= new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose,true);
    QProgressBar loading;
    QPushButton cancelButton("Cancel");
    loading.setRange(0,marksList.size());

    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading,Qt::AlignCenter);
    loadLayout.addWidget(&cancelButton,Qt::AlignCenter);
    connect(&cancelButton,SIGNAL(clicked()),loadWidget,SLOT(close()));

    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Loading OIs"));
    loading.setMinimumSize(300,30);
    loadWidget->show();

    std::string newOIXML;
    int imagescount=1;
    for (int i=0; i<marksList.length() && loadWidget!=NULL;i+=4)
    {
        loading.setFormat(tr("%v/%m : %p%"));

        QStringList OIMarks;
        OIMarks<<marksList.at(i)<<marksList.at(i+1)<<marksList.at(i+2)<<marksList.at(i+3);
        newOIXML+=OIToXml(OIMarks,imagescount++);
        loading.setValue(i+1);
    }

    manager->addComponent(newOIXML,"interiorOrientation");
    loadWidget->close();

    updateTree();
    viewPoints();
    actionSave_file->setEnabled(true);
}

std::string ProjectUserInterface_Qt::OIToXml(QStringList oiMarks,int imageKey)
{
    std::stringstream OIxml;
    bool ok;
    //Paulo: Metodo Lusitano
    double markx1=oiMarks.at(0).section('\t',0,0).toDouble(&ok);
    double marky1=oiMarks.at(0).section('\t',1,1).toDouble(&ok);

    double markx2=oiMarks.at(1).section('\t',0,0).toDouble(&ok);
    double marky2=oiMarks.at(1).section('\t',1,1).toDouble(&ok);

    double markx3=oiMarks.at(2).section('\t',0,0).toDouble(&ok);
    double marky3=oiMarks.at(2).section('\t',1,1).toDouble(&ok);

    double markx4=oiMarks.at(3).section('\t',0,0).toDouble(&ok);
    double marky4=oiMarks.at(3).section('\t',1,1).toDouble(&ok);

    OIxml << "<imageIO type=\"Affine\" image_key=\""<< Conversion::doubleToString(imageKey)<<"\">\n";
    OIxml << "\t<fiducialMarks uom=\"#px\">\n";
    OIxml << "\t\t<fiducialMark key=\"1\">\n";
    OIxml << "\t\t\t<gml:pos>"<< Conversion::doubleToString(markx1) << " " << Conversion::doubleToString(marky1) << "</gml:pos>\n";
    OIxml << "\t\t</fiducialMark>\n";

    OIxml << "\t\t<fiducialMark key=\"2\">\n";
    OIxml << "\t\t\t<gml:pos>"<< Conversion::doubleToString(markx2) << " " << Conversion::doubleToString(marky2) << "</gml:pos>\n";
    OIxml << "\t\t</fiducialMark>\n";

    OIxml << "\t\t<fiducialMark key=\"3\">\n";
    OIxml << "\t\t\t<gml:pos>"<< Conversion::doubleToString(markx3) << " " << Conversion::doubleToString(marky3) << "</gml:pos>\n";
    OIxml << "\t\t</fiducialMark>\n";

    OIxml << "\t\t<fiducialMark key=\"4\">\n";
    OIxml << "\t\t\t<gml:pos>"<< Conversion::doubleToString(markx4) << " " << Conversion::doubleToString(marky4) << "</gml:pos>\n";
    OIxml << "\t\t</fiducialMark>\n";

    OIxml << "\t</fiducialMarks>\n";

    OIxml << "</imageIO>\n";

    return OIxml.str();
}

void ProjectUserInterface_Qt::updateLabelFileName()
{
    if (actionSave_file->isEnabled())
        projectDockWidget->setWindowTitle(QString(tr("Open Project: *")) + headerForm.lineEditFileName->text());
    else
        projectDockWidget->setWindowTitle(QString(tr("Open Project: ")) + headerForm.lineEditFileName->text());
}

void ProjectUserInterface_Qt::loadLastProject()
{
    QSettings efotoSettings("uerj","efoto");
    //QString filename = efotoSettings.value("lastProject").toString();
    QString filename = QString::fromLocal8Bit(efotoSettings.value("lastProject").toByteArray().constData());

    //loadFile(filename.toStdString());
    loadFile(filename.toLocal8Bit().constData());
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

