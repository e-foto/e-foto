/*Copyright 2002-2025 e-foto team (UERJ)
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

#include "ProjectUserInterface_Qt.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QRegularExpression>
#include <QScrollArea>
#include <QActionGroup>
#include <QEvent>
#include <QMenu>
#include <QSettings>
#include <QShortcut>
#include <filesystem>
#include <iostream>
#include <sstream>

#include <QtDebug>

#include "AboutForm.h"
#include "ETreeModel.h"
#include "Language.h"
#include "LoadingScreen.h"
#include "PathUtils.h"
#include "ProjectManager.h"
#include "RasterResource.h"
#include "revision_info.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

namespace {
// Fixed child order of ETreeModel: projectHeader, terrain, sensors, flights,
// images, points.
const int ImageTreeNode = 4;
const int PointTreeNode = 5;
}  // namespace

ProjectUserInterface_Qt::ProjectUserInterface_Qt(ProjectManager *manager,
                                                 QWidget *parent,
                                                 Qt::WindowFlags fl)
    : QMainWindow(parent, fl) {
  setupUi(this);
  setupLanguageMenu();

  // Ler versão
  QString ver = BUILD_DATE;
  QString rev = BUILD_COMMIT_HASH;
  QDate date = QDate::fromString(ver, "MMM dd yyyy");
  if (date.isValid()) {
    int year = date.year();
    int month = date.month();
    ver = QString::number(year) + "." + QString::number(month);
  }

  // Montar mensagem
  QString date_ver_name = tr("Version ");
  QString size_ver_name = tr(" (64 bits);");
  QString msg = date_ver_name + ver + "." + rev + size_ver_name;
  version_info->setText(msg);

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
  this->connect(actionLoad_last_project, SIGNAL(triggered()), this,
                SLOT(loadLastProject()));
  this->connect(actionSave_file, SIGNAL(triggered()), this, SLOT(saveFile()));
  this->connect(actionSave_file_as, SIGNAL(triggered()), this,
                SLOT(saveFileAs()));
  this->connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  this->connect(actionInterior_Orientation, SIGNAL(triggered()), this,
                SLOT(executeIO()));
  this->connect(actionSpatial_resection, SIGNAL(triggered()), this,
                SLOT(executeSR()));
  this->connect(actionExport_Stereo, SIGNAL(triggered()), this,
                SLOT(exportSPFile()));
  this->connect(actionDEMExtraction, SIGNAL(triggered()), this,
                SLOT(executeDEM()));
  this->connect(actionOrtho_rectification, SIGNAL(triggered()), this,
                SLOT(executeOrtho()));
  this->connect(actionFoto_Tri, SIGNAL(triggered()), this, SLOT(executeFT()));
  this->connect(actionStereo, SIGNAL(triggered()), this, SLOT(executeSP()));
  this->connect(actionPTReport, SIGNAL(triggered()), this,
                SLOT(executePTReport()));
  this->connect(actionReport, SIGNAL(triggered()), this, SLOT(executeReport()));
  this->connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
  this->connect(treeWidget, SIGNAL(clicked(QModelIndex)), this,
                SLOT(processTreeClick(QModelIndex)));
  this->connect(&imagesForm, SIGNAL(clicked(int)), this,
                SLOT(selectImage(int)));
  this->connect(&imagesForm, SIGNAL(activated(int)), this,
                SLOT(openImage(int)));
  this->connect(&pointsForm, SIGNAL(clicked(int)), this,
                SLOT(selectPoint(int)));
  this->connect(&pointsForm, SIGNAL(activated(int)), this,
                SLOT(openPoint(int)));
  this->connect(imageForm.imageIDLine, SIGNAL(editingFinished()), this,
                SLOT(validatingImage()));
  this->connect(imageForm.resolutionSpin, SIGNAL(editingFinished()), this,
                SLOT(validatingImage()));
  this->connect(imageForm.fileNameLine, SIGNAL(textChanged(QString)), this,
                SLOT(validatingImage()));
  this->connect(pointForm.lineEdit_gcp_id, SIGNAL(editingFinished()), this,
                SLOT(validatingPoint()));
  this->connect(pointForm.sigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingPoint()));
  this->connect(imageForm.gnssSigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingImage()));
  this->connect(imageForm.insSigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingImage()));
  this->connect(flightForm.flightIDLineEdit, SIGNAL(editingFinished()), this,
                SLOT(validatingFlight()));
  this->connect(sensorForm.sensorIdLineEdit, SIGNAL(editingFinished()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid0SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid1SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid2SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid3SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid4SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid5SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid6SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.fid7SigmaController, SIGNAL(validateChanged()), this,
                SLOT(validatingSensor()));
  this->connect(sensorForm.calibratedSigmaController, SIGNAL(validateChanged()),
                this, SLOT(validatingSensor()));
  this->connect(sensorForm.radialSigmaController, SIGNAL(validateChanged()),
                this, SLOT(validatingSensor()));
  this->connect(sensorForm.decenteredSigmaController, SIGNAL(validateChanged()),
                this, SLOT(validatingSensor()));
  this->connect(sensorForm.principalSigmaController, SIGNAL(validateChanged()),
                this, SLOT(validatingSensor()));

  connect(pointsForm.importButton, SIGNAL(clicked()), this,
          SLOT(importPointsFromTxt()));
  connect(pointsForm.exportToTxtButton, SIGNAL(clicked()), this,
          SLOT(exportPointsToTxt()));
  connect(imagesForm.importButton, SIGNAL(clicked()), this,
          SLOT(importImagesBatch()));

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

  // Adiciona um atalho para os desenvolvedores observarem as mudanças no XML
  // durante o runtime
  QShortcut *shortcut =
      new QShortcut(QKeySequence(tr("Ctrl+Shift+D", "Debug")), this);
  connect(shortcut, SIGNAL(activated()), this, SLOT(toggleDebug()));

  // Inserido pelo Paulo 05/09/2011
  // Adiciona um atalho para os desenvolvedores dar upload das coordenadas
  // digitais do export do LPS
  // QShortcut* shortcut2 = new QShortcut(QKeySequence(tr("Ctrl+Shift+P",
  // "Import")),this); connect(shortcut2, SIGNAL(activated()), this,
  // SLOT(importPointsFromTxt2()));

  // QShortcut* shortcut3 = new QShortcut(QKeySequence(tr("Ctrl+Shift+I",
  // "Import")),this); connect(shortcut3, SIGNAL(activated()), this,
  // SLOT(importImagesBatch()));

  // QShortcut* shortcut4 = new QShortcut(QKeySequence(tr("Ctrl+Shift+O",
  // "Import")),this); connect(shortcut4, SIGNAL(activated()), this,
  // SLOT(importOIDigitalMarks()));

  refreshMenu();
  actionLoad_last_project->setEnabled(availableLP());
}

ProjectUserInterface_Qt::~ProjectUserInterface_Qt() {
  // no need to delete child widgets, Qt does it all for us
}

void ProjectUserInterface_Qt::closeEvent(QCloseEvent *event) {
  if (changeModule || confirmToClose())
    event->accept();
  else
    event->ignore();

  refreshMenu();
}

void ProjectUserInterface_Qt::newProject() {
  if (!confirmToClose()) return;

  if (!saveFileAs(true)) return;

  addDockWidget(Qt::LeftDockWidgetArea, projectDockWidget);
  addDockWidget(Qt::BottomDockWidgetArea, debuggerDockWidget);
  setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
  setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
  projectDockWidget->setVisible(true);
  introWidget->setVisible(false);
  centerArea.setVisible(true);
  controlButtons.setVisible(true);
  offset.setVisible(true);

  manager->newProject(manager->savedIn);
  // savedIn = "";
  actionSave_file->setEnabled(true);
  actionSave_file_as->setEnabled(true);
  // viewHeader();
  newTree();

  // Os comandos a seguir so serao uteis enquanto o projeto ficar restrito a
  // apenas um sensor e um flight

  std::string text = "";

  text += "<sensor key=\"1\">\n";
  text += "<sensorId></sensorId>\n";
  text += "<type>\n";
  text += "<geometry>frame</geometry>\n";
  text += "<platform>aerial</platform>\n";
  text += "<detector>film</detector>\n";
  text += "<energySource>natural</energySource>\n";
  text += "<calculationMode>With Fiducial Marks</calculationMode>";
  text += "<spectralRanges uom=\"#um\">\n";
  text += "</spectralRanges>\n";
  text += "</type>\n";
  text += "<description></description>\n";
  text += "<calibrationCertificate>\n";
  text += "<number></number>\n";
  text += "<dispatch></dispatch>\n";
  text += "<expiration></expiration>\n";
  text += "</calibrationCertificate>\n";
  text += "<focalDistance uom=\"#mm\">\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</focalDistance>\n";
  text += "<distortionCoefficients>\n";
  text += "<radialSymmetric considered=\"false\">\n";
  text += "<k0>\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</k0>\n";
  text += "<k1>\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</k1>\n";
  text += "<k2>\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</k2>\n";
  text += "<k3>\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</k3>\n";
  text += "</radialSymmetric>\n";
  text += "<decentered considered=\"false\">\n";
  text += "<P1>\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</P1>\n";
  text += "<P2>\n";
  text += "<value></value>\n";
  text += "<sigma></sigma>\n";
  text += "</P2>\n";
  text += "</decentered>\n";
  text += "</distortionCoefficients>\n";
  text += "<principalPointCoordinates uom=\"#mm\">\n";
  text += "<gml:pos></gml:pos>\n";
  text += "<sigma>\n";
  text += "</sigma>\n";
  text += "</principalPointCoordinates>\n";
  text += "<fiducialMarks uom=\"#mm\">\n";
  text += "<fiducialMark key=\"1\">\n";
  text += "<gml:pos></gml:pos>\n";
  text += "<sigma></sigma>\n";
  text += "</fiducialMark>\n";
  text += "<fiducialMark key=\"2\">\n";
  text += "<gml:pos></gml:pos>\n";
  text += "<sigma></sigma>\n";
  text += "</fiducialMark>\n";
  text += "<fiducialMark key=\"3\">\n";
  text += "<gml:pos></gml:pos>\n";
  text += "<sigma></sigma>\n";
  text += "</fiducialMark>\n";
  text += "<fiducialMark key=\"4\">\n";
  text += "<gml:pos></gml:pos>\n";
  text += "<sigma></sigma>\n";
  text += "</fiducialMark>\n";
  text += "</fiducialMarks>\n";
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

  // Estas linhas fazem parte realmente deste código e não são parte
  // da gambiarra a cima
  saveFile();

  refreshMenu();
}

void ProjectUserInterface_Qt::loadFile(std::string filenameAtStart) {
  if (!confirmToClose()) return;

  QString input_filename;

  if (filenameAtStart.empty()) {
    input_filename = QFileDialog::getOpenFileName(this, tr("Open File"), ".", "*.epp");
  } else {
    input_filename = QString::fromStdString(filenameAtStart);
  }

  if (input_filename.isEmpty()) return;

  if (manager->loadFile(input_filename.toUtf8().data())) {
    // manager->loadFile() anchored the project directory: every relative
    // <filePath> is now resolved against it, never against the working
    // directory. Resolve and, when needed, repair the stored image paths.
    std::deque<std::string> missingImages;
    const int repairedImages = manager->repairImagePaths(missingImages);

    std::string imagesMissing;
    for (size_t i = 0; i < missingImages.size(); i++) {
      // Format the output (4 items per line)
      if (i % 4 == 3 && i + 1 < missingImages.size()) {
        imagesMissing += missingImages.at(i) + "  \n";
      } else if (i + 1 == missingImages.size()) {
        imagesMissing += missingImages.at(i);
      } else {
        imagesMissing += missingImages.at(i) + " , ";
      }
    }

    if (!imagesMissing.empty()) {
      QString message = "Those images are missing:\n" +
                        QString::fromStdString(imagesMissing) + ".";
      QMessageBox *alertImages = new QMessageBox(
          QMessageBox::Warning,
          "Images missing",
          message);
      alertImages->show();
    }

    if (repairedImages > 0) {
      // The stored relative paths were pointing to another folder (the project
      // was moved or copied): let the user know it is worth saving again.
      QMessageBox *alertRelocated = new QMessageBox(
          QMessageBox::Information, "Image paths updated",
          QString::number(repairedImages) +
              " image path(s) were relocated and updated in memory.\n"
              "Save the project to keep the new locations.");
      alertRelocated->show();
    }

    addDockWidget(Qt::LeftDockWidgetArea, projectDockWidget);
    addDockWidget(Qt::BottomDockWidgetArea, debuggerDockWidget);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    projectDockWidget->setVisible(true);
    introWidget->setVisible(false);
    centerArea.setVisible(true);
    controlButtons.setVisible(true);
    offset.setVisible(true);

    manager->savedIn = input_filename.toUtf8().data();

    // Some features still assume the project folder as the working directory.
    // This is not needed to resolve the images anymore, so a failure here is
    // harmless and must not abort opening the project.
    std::error_code cwdError;
    std::filesystem::current_path(
        PathUtils::toPath(PathUtils::projectDirectory()), cwdError);

    actionSave_file->setEnabled(repairedImages > 0);
    actionSave_file_as->setEnabled(true);

    const std::string projectFile =
        PathUtils::absolute(input_filename.toUtf8().data());
    const QString filename =
        "<fileName>" +
        QString::fromStdString(PathUtils::xmlEscape(
            PathUtils::fileName(projectFile))) +
        "</fileName>";
    const QString filepath =
        "<filePath>" +
        QString::fromStdString(PathUtils::xmlEscape(
            PathUtils::parentDirectory(projectFile))) +
        "</filePath>";

    EDomElement node(manager->getXml("projectHeader"));

    node.replaceChildByTagName("fileName", filename.toUtf8().data());
    node.replaceChildByTagName("filePath", filepath.toUtf8().data());

    manager->editComponent("Header", node.getContent());

    newTree();
  } else {
    QMessageBox *alert = NULL;
    switch (manager->informFileVersionError()) {
      case 0:
        alert = new QMessageBox(
            QMessageBox::Warning, "Unable to open file",
            "The e-foto software was unable to open the selected file.\nThis "
            "may be due to:\n\n - Unsupported characters in the file's name or "
            "path (maybe accented characters or whitespace);\n - The file does "
            "not exist;\n - A bug in the program.\n\nTry changing the file's "
            "name or path and try again.");
        break;
      case 1:
      case 2:
      case 3:
      case 4:
        alert = new QMessageBox(
            QMessageBox::Warning, "Unable to load file",
            "The e-foto software was unable to load the selected file.\nThis "
            "may be due to:\n\n - Unsupported file version;\n - The file is "
            "not a valid .epp (e-foto Photogrammetric Project) file;\n - A bug "
            "in the program.\n\nTry changing the file or version of the "
            "software and try again.");
        break;
      default:
        alert = new QMessageBox(
            QMessageBox::Warning, "Unable to open or load file",
            "The e-foto software was unable to open or load the selected "
            "file.\nThis may be due to:\n\n - Unsupported characters in the "
            "file's name or path (maybe accented characters or whitespace);\n "
            "- Unsupported file version;\n - The file is not a valid .epp "
            "(e-foto Photogrammetric Project) file;\n - A bug in the "
            "program.\n\nTry changing the file's name or path or changing the "
            "file or version of the software and try again.");
        break;
    }
    alert->show();
  }

  updateLabelFileName();

  refreshMenu();
}

void ProjectUserInterface_Qt::saveFile() {
  if (!manager->savedIn.empty()) {
    QDateTime dateTimeBackup =
        headerForm.dateTimeEditModificationDate->dateTime();
    headerForm.dateTimeEditModificationDate->setDateTime(
        QDateTime::currentDateTime());
    manager->editComponent("Header", headerForm.getvalues());
    if (manager->saveFile(manager->savedIn)) {
      actionSave_file->setEnabled(false);
    } else {
      headerForm.dateTimeEditModificationDate->setDateTime(dateTimeBackup);
      manager->editComponent("Header", headerForm.getvalues());
      QMessageBox *alert = new QMessageBox(
          QMessageBox::Warning, "Unable to save file",
          "The e-foto software was unable to save the file.\nThis may be due "
          "to:\n\n - The disk does not have enought free space;\n - You do not "
          "have the needed permissions;\n - A bug in the program.\n\nCheck "
          "your disk space and permissions and try again.");
      alert->show();
    }
    if (centerArea.currentIndex() == 0) viewHeader();
    updateTree();
  } else
    saveFileAs();
}

bool ProjectUserInterface_Qt::saveFileAs(bool onNewProject) {
  QString target_filename;
  QString caption;
  if (onNewProject)
    caption = tr("Save A New File");
  else
    caption = tr("Save File As");

  QFileDialog salvar(this, caption, ".", "*.epp");
  salvar.setAcceptMode(QFileDialog::AcceptSave);
  salvar.setDefaultSuffix("epp");
  if (salvar.exec()) {
    target_filename = salvar.selectedFiles()[0];
    if (target_filename.isEmpty())
      return false;
    else {
      QString extension = target_filename.right(4);
      if (extension.toLower() != ".epp") target_filename.append(".epp");

      // Work in UTF-8 from here on: toStdString()/toLocal8Bit() would encode
      // the path with the local code page and break non ASCII folders.
      const std::string targetFile = target_filename.toUtf8().data();
      const std::string targetDirectory = PathUtils::parentDirectory(targetFile);

      const QString filename = QString::fromStdString(PathUtils::fileName(targetFile));
      const QString filepath = QString::fromStdString(targetDirectory);

      QString fileNameBackup = headerForm.lineEditFileName->text();
      QString filePathBackup = headerForm.lineEditFilePath->text();
      QDateTime dateTimeCreatBackup =
          headerForm.dateTimeEditCreationDate->dateTime();
      QDateTime dateTimeModifBackup =
          headerForm.dateTimeEditModificationDate->dateTime();

      headerForm.lineEditFilePath->setText(filepath);
      headerForm.lineEditFileName->setText(filename);
      headerForm.dateTimeEditCreationDate->setDateTime(
          QDateTime::currentDateTime());
      headerForm.dateTimeEditModificationDate->setDateTime(
          QDateTime::currentDateTime());

      manager->editComponent("Header", headerForm.getvalues());

      // Move the image paths from the old project directory to the new one.
      // An image that cannot be expressed relative to the new directory (other
      // drive, for instance) is stored as an absolute path.
      const std::string oldProjectDirectory = manager->projectDirectory();
      EDomElement imagesBackup(manager->getXml("images"));
      std::deque<EDomElement> imagesList = imagesBackup.children();
      for (size_t i = 0; i < imagesList.size(); i++) {
        const int key = Conversion::stringToInt(imagesList.at(i).attribute("key"));
        EDomElement image = imagesList.at(i);
        const std::string storedName = PathUtils::xmlUnescape(PathUtils::trim(
            image.elementByTagName("fileName").toString()));
        const std::string storedPath = PathUtils::xmlUnescape(PathUtils::trim(
            image.elementByTagName("filePath").toString()));
        const std::string absoluteImage =
            PathUtils::resolveImagePath(storedPath, storedName, oldProjectDirectory);
        std::string adjustedPath = PathUtils::relativeOrAbsolute(
            PathUtils::parentDirectory(absoluteImage), targetDirectory);
        if (adjustedPath.empty()) adjustedPath = ".";
        EDomElement adjustedImage = image;
        adjustedImage.replaceChildByTagName(
            "filePath", "<filePath>" + PathUtils::xmlEscape(adjustedPath) +
                            "</filePath>");
        manager->editComponent("Image", key, adjustedImage.getContent());
      }

      if (manager->saveFile(targetFile)) {
        // savedIn is the full project file path, not only its name.
        manager->savedIn = targetFile;
        actionSave_file->setEnabled(false);
        // When changing the perception of the current project directory, the
        // GUI must be communicated to avoid errors
        std::error_code cwdError;
        std::filesystem::current_path(PathUtils::toPath(targetDirectory),
                                      cwdError);
      } else {
        headerForm.lineEditFilePath->setText(filePathBackup);
        headerForm.lineEditFileName->setText(fileNameBackup);
        headerForm.dateTimeEditCreationDate->setDateTime(dateTimeCreatBackup);
        headerForm.dateTimeEditModificationDate->setDateTime(
            dateTimeModifBackup);
        manager->editComponent("Header", headerForm.getvalues());

        std::deque<EDomElement> imagesBkp = imagesBackup.children();
        for (size_t i = 0; i < imagesBkp.size(); i++) {
          int key = Conversion::stringToInt(imagesBkp.at(i).attribute("key"));
          std::string data = imagesBkp.at(i).getContent();
          manager->editComponent("Image", key, data);
        }

        QMessageBox *alert = new QMessageBox(
            QMessageBox::Warning, "Unable to save file",
            "The e-foto software was unable to save the file.\nThis may be due "
            "to:\n\n - The disk does not have enought free space;\n - You do "
            "not have the needed permissions;\n - The file's name or path is "
            "invalid (maybe accented characters or whitespace);\n - A bug in "
            "the program.\n\nCheck your disk space and permissions and try "
            "again.");
        alert->show();
        return false;
      }
      if (centerArea.currentIndex() == 0) viewHeader();
      updateTree();
    }
    return true;
  }
  return false;
}

void ProjectUserInterface_Qt::executeIO() {
  if (manager->getXml(std::string("sensorId"))
          .compare("<sensorId></sensorId>") ==
      0)  // Check if an sensor is not defined to avoid runtime errors
  {
    QMessageBox msgBox;
    msgBox.setText(
        tr("Before proceeding, it is necessary to register the sensor data!"));
    msgBox.exec();
    return;
  }
  bool ok;
  QStringList items;
  std::deque<std::string> strItems = manager->listImages();
  for (unsigned int i = 0; i < strItems.size(); i++)
    items << strItems.at(i).c_str();
  QString chosen = QInputDialog::getItem(
      this, tr("Select your image!"), tr("Image name:"), items, 0, false, &ok);
  if (ok) {
    int value = manager->getImageId(chosen.toUtf8().data());
    if (value != -1) {
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

void ProjectUserInterface_Qt::executeSR() {
  bool ok;
  QStringList items;
  std::deque<std::string> strItems = manager->listImages();
  for (unsigned int i = 0; i < strItems.size(); i++)
    items << strItems.at(i).c_str();
  QString chosen = QInputDialog::getItem(
      this, tr("Select your image!"), tr("Image name:"), items, 0, false, &ok);
  if (ok) {
    int value = manager->getImageId(chosen.toUtf8().data());
    if (value != -1) {
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

void ProjectUserInterface_Qt::executeDEM() {
  changeModule = true;
  confirmToClose();
  LoadingScreen::instance().show();
  qApp->processEvents();
  this->close();
  manager->startModule("DEM-Extraction", 0);
  changeModule = false;
}

void ProjectUserInterface_Qt::executeSP() {
  changeModule = true;
  confirmToClose();
  LoadingScreen::instance().show();
  qApp->processEvents();
  this->close();
  manager->startModule("StereoPlotter", 0);
  changeModule = false;
}

void ProjectUserInterface_Qt::executeReport() {
  changeModule = true;
  confirmToClose();
  LoadingScreen::instance().show();
  qApp->processEvents();
  this->close();
  manager->startModule("Report", 0);
  changeModule = false;
}

void ProjectUserInterface_Qt::executePTReport() {
  changeModule = true;
  confirmToClose();
  LoadingScreen::instance().show();
  qApp->processEvents();
  this->close();
  manager->startModule("PT-Report", 0);
  changeModule = false;
}

void ProjectUserInterface_Qt::executeFT() {
  changeModule = true;
  confirmToClose();
  LoadingScreen::instance().show();
  qApp->processEvents();
  this->close();
  manager->startModule("FotoTriangulation", 0);
  changeModule = false;
}

void ProjectUserInterface_Qt::executeOrtho() {
  changeModule = true;
  confirmToClose();
  LoadingScreen::instance().show();
  qApp->processEvents();
  this->close();
  manager->startModule("Ortho-rectification", 0);
  changeModule = false;
}

void ProjectUserInterface_Qt::processTreeClick(QModelIndex index) {
  if ((editState && index != currentIndex) || addNewState) {
    if (controlButtons.saveButton->isEnabled()) {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(
          this, tr(" Warning: leaving form in edit mode"),
          tr("Do you want to keep all changes?"),
          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      if (reply == QMessageBox::Yes) {
        controlButtons.saveButton->click();
      } else if (reply == QMessageBox::No) {
        controlButtons.cancelButton->click();
      } else {
        treeWidget->setCurrentIndex(currentIndex);
        return;
      }
    } else {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(
          this, tr(" Warning: leaving form in edit mode"),
          tr("Registration data is not complete and will be lost. Continue?"),
          QMessageBox::Yes | QMessageBox::Cancel);
      if (reply == QMessageBox::Yes) {
        controlButtons.cancelButton->click();
      } else {
        treeWidget->setCurrentIndex(currentIndex);
        return;
      }
    }
  }
  currentIndex = index;
  ETreeModel *etm = manager->getTreeModel();
  if (index.parent() == QModelIndex()) {
    if (etm->dataAt(index.row()) == "projectHeader") {
      viewHeader();
    } else if (etm->dataAt(index.row()) == "terrain") {
      viewTerrain();
    } else if (etm->dataAt(index.row()) == "sensors") {
      viewSensor(1);
    } else if (etm->dataAt(index.row()) == "flights") {
      viewFlight(1);
    } else if (etm->dataAt(index.row()) == "images") {
      viewImages();
    } else if (etm->dataAt(index.row()) == "points") {
      viewPoints();
    }
  } else {
    if (etm->dataAt(index.parent().row()) == "sensors") {
      viewSensor(1);
    } else if (etm->dataAt(index.parent().row()) == "flights") {
      viewFlight(1);
    } else if (etm->dataAt(index.parent().row()) == "images") {
      viewImage(etm->idAt(index.parent().row(), index.row()));
    } else if (etm->dataAt(index.parent().row()) == "points") {
      viewPoint(etm->idAt(index.parent().row(), index.row()));
    }
  }
}

void ProjectUserInterface_Qt::exportSPFile() {
  bool ok;
  QStringList items;
  std::deque<std::string> strItems = manager->listImages();
  for (unsigned int i = 0; i < strItems.size(); i++)
    items << strItems.at(i).c_str();

  QString chosen1 = QInputDialog::getItem(
      this, tr("Select left image"), tr("Image name:"), items, 0, false, &ok);
  if (!ok) return;

  items.removeOne(chosen1);

  QString chosen2 = QInputDialog::getItem(
      this, tr("Select right image!"), tr("Image name:"), items, 0, false, &ok);
  if (!ok) return;

  QFileDialog salvar(this, "Save File", ".", "*.txt");
  salvar.setAcceptMode(QFileDialog::AcceptSave);
  salvar.setDefaultSuffix("txt");
  if (salvar.exec()) {
    QString filename = salvar.selectedFiles()[0];
    if (filename.isEmpty()) return;

    int image1 = manager->getImageId(chosen1.toUtf8().data());
    int image2 = manager->getImageId(chosen2.toUtf8().data());
    if (image1 != -1 && image2 != -1) {
      bool result =
          manager->makeSPFile(filename.toUtf8().data(), image1, image2);
      if (result == false) {
        QMessageBox msgBox;
        msgBox.setText(tr("Error: invalid input parameters."));
        msgBox.exec();
      }
    }
  }
}

void ProjectUserInterface_Qt::languageChange() { retranslateUi(this); }

void ProjectUserInterface_Qt::setupLanguageMenu() {
  languageMenu = new QMenu(tr("Language"), this);
  QActionGroup* group = new QActionGroup(this);
  group->setExclusive(true);

  QAction* systemAction = languageMenu->addAction(tr("System default"));
  systemLanguageAction = systemAction;
  QAction* englishAction = languageMenu->addAction(QStringLiteral("English"));
  QAction* chineseAction = languageMenu->addAction(QStringLiteral("简体中文"));

  systemAction->setData(QStringLiteral("system"));
  englishAction->setData(QStringLiteral("en"));
  chineseAction->setData(QStringLiteral("zh_CN"));

  const QString current = Language::current();
  for (QAction* action : {systemAction, englishAction, chineseAction}) {
    action->setCheckable(true);
    action->setChecked(action->data().toString() == current ||
                       (current == "system" && action == systemAction));
    group->addAction(action);
  }

  connect(group, &QActionGroup::triggered, this, [](QAction* action) {
    Language::setCurrent(action->data().toString());
  });

  menubar->addMenu(languageMenu);
}

void ProjectUserInterface_Qt::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    retranslateUi(this);
    if (languageMenu) languageMenu->setTitle(tr("Language"));
    if (systemLanguageAction) systemLanguageAction->setText(tr("System default"));
    headerForm.retranslateUi(&headerForm);
    terrainForm.retranslateUi(&terrainForm);
    sensorForm.retranslateUi(&sensorForm);
    flightForm.retranslateUi(&flightForm);
    imagesForm.retranslateUi(&imagesForm);
    imageForm.retranslateUi(&imageForm);
    pointsForm.retranslateUi(&pointsForm);
    pointForm.retranslateUi(&pointForm);
    refreshMenu();
    if (!headerForm.lineEditName->text().isEmpty()) {
      setWindowTitle(headerForm.lineEditName->text() +
                     tr(" - efoto[Project Manager]"));
    } else {
      setWindowTitle(tr("efoto[Project Manager]"));
    }
  }
  QMainWindow::changeEvent(event);
}

void ProjectUserInterface_Qt::newTree() {
  treeWidget->clear();
  treeItems.clear();
  if (manager->savedIn.empty()) {
    projectDockWidget->setWindowTitle(tr("Open Project: *Unsaved"));
  } else {
    if (actionSave_file->isEnabled())
      projectDockWidget->setWindowTitle(QString(tr("Open Project: *")) +
                                        headerForm.lineEditFileName->text());
    else
      projectDockWidget->setWindowTitle(QString(tr("Open Project: ")) +
                                        headerForm.lineEditFileName->text());
  }
  if (headerForm.lineEditName->text() != "")
    setWindowTitle(headerForm.lineEditName->text() +
                   tr(" - efoto[Project Manager]"));
  else
    setWindowTitle(tr("efoto[Project Manager]"));

  ETreeModel *etm = manager->getTreeModel();

  for (unsigned int i = 0; i < etm->countChildren(); i++) {
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
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
    if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" ||
          etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights")) {
      for (unsigned int j = 0; j < etm->countGrandchildren(i); j++) {
        QTreeWidgetItem *leafItem = new QTreeWidgetItem(rootItem);
        leafItem->setText(0, tr(etm->dataAt(i, j).c_str()));
      }
    }
  }
  treeWidget->setCurrentItem(treeItems.at(0));
  processTreeClick(treeWidget->currentIndex());
}

void ProjectUserInterface_Qt::updateTree() {
  if (manager->savedIn.empty()) {
    projectDockWidget->setWindowTitle(tr("Open Project: *Unsaved"));
  } else {
    updateLabelFileName();
  }
  if (headerForm.lineEditName->text() != "")
    setWindowTitle(headerForm.lineEditName->text() +
                   tr(" - efoto[Project Manager]"));
  else
    setWindowTitle(tr("efoto[Project Manager]"));

  ETreeModel *etm = manager->getTreeModel();

  for (int i = 0; i < (int)etm->countChildren(); i++) {
    if (i < treeItems.size()) {
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
      if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" ||
            etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights")) {
        for (int j = 0; j < (int)etm->countGrandchildren(i); j++) {
          if (j < treeItems.at(i)->childCount()) {
            treeItems.at(i)->child(j)->setText(0,
                                               tr(etm->dataAt(i, j).c_str()));
          } else {
            QTreeWidgetItem *leafItem = new QTreeWidgetItem(treeItems.at(i));
            leafItem->setText(0, tr(etm->dataAt(i, j).c_str()));
          }
        }
        // The model is rebuilt on every change, so an item that was removed (a
        // deleted image, for instance) only disappears from the tree here.
        while (treeItems.at(i)->childCount() > (int)etm->countGrandchildren(i)) {
          delete treeItems.at(i)->takeChild(treeItems.at(i)->childCount() - 1);
        }
      }
    } else {
      QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
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
      if (!(etm->dataAt(i) == "projectHeader" || etm->dataAt(i) == "terrain" ||
            etm->dataAt(i) == "sensors" || etm->dataAt(i) == "flights")) {
        for (unsigned int j = 0; j < etm->countGrandchildren(i); j++) {
          QTreeWidgetItem *leafItem = new QTreeWidgetItem(rootItem);
          leafItem->setText(0, tr(etm->dataAt(i, j).c_str()));
        }
      }
    }
  }
}

bool ProjectUserInterface_Qt::exec() {
  actionSave_file->setEnabled(!manager->getSavedState());

  refreshMenu();

  // PAULO -> codigo para dar um refresh no formulario
  updateCurrentForm();
  this->show();
  LoadingScreen::instance().close();
  qApp->processEvents();
  changeModule = false;
  updateLabelFileName();
  return true;
}

void ProjectUserInterface_Qt::syncTreeToChild(int node, int row) {
  if (node < 0 || row < 0 || treeItems.size() <= node) return;
  QTreeWidgetItem *parentItem = treeItems.at(node);
  if (parentItem == NULL || row >= parentItem->childCount()) return;
  QTreeWidgetItem *childItem = parentItem->child(row);
  if (treeWidget->currentItem() == childItem) return;
  treeWidget->setCurrentItem(childItem);
  currentIndex = treeWidget->currentIndex();
}

void ProjectUserInterface_Qt::selectImage(int pos) {
  // A click on the images table only selects the image: the user may want to
  // delete it (or just look at it) instead of opening its form. The form is
  // opened by a double click, see openImage().
  const int key = imagesForm.keyAtRow(pos);
  if (key <= 0) {
    controlButtons.setDeleteEnabled(false);
    return;
  }
  currentItemId = key;
  syncTreeToChild(ImageTreeNode, pos);
  controlButtons.setDeleteEnabled(true);
}

void ProjectUserInterface_Qt::openImage(int pos) {
  if (addNewState || editState) return;
  const int key = imagesForm.keyAtRow(pos);
  if (key <= 0) return;
  currentItemId = key;
  syncTreeToChild(ImageTreeNode, pos);
  viewImage(key);
}

void ProjectUserInterface_Qt::selectPoint(int pos) {
  // Same behaviour as the images table: a click selects, a double click opens.
  const int key = pointsForm.keyAtRow(pos);
  if (key <= 0) {
    controlButtons.setDeleteEnabled(false);
    return;
  }
  currentItemId = key;
  syncTreeToChild(PointTreeNode, pos);
  controlButtons.setDeleteEnabled(true);
}

void ProjectUserInterface_Qt::openPoint(int pos) {
  if (addNewState || editState) return;
  const int key = pointsForm.keyAtRow(pos);
  if (key <= 0) return;
  currentItemId = key;
  syncTreeToChild(PointTreeNode, pos);
  viewPoint(key);
}

void ProjectUserInterface_Qt::viewHeader() {
  controlButtons.disconnectAll();
  controlButtons.singleConnect();
  connect((&controlButtons)->editButton, SIGNAL(clicked()), this,
          SLOT(enableForm()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveHeader()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelHeader()));

  EDomElement node(manager->getXml("projectHeader"));

  headerForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
  centerArea.setCurrentIndex(0);
  currentForm = &headerForm;
  currentForm->setReadOnly(true);

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
  controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::viewTerrain() {
  controlButtons.disconnectAll();
  controlButtons.singleConnect();
  connect((&controlButtons)->editButton, SIGNAL(clicked()), this,
          SLOT(enableForm()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveTerrain()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelTerrain()));

  EDomElement node(manager->getXml("terrain"));

  terrainForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
  centerArea.setCurrentIndex(1);
  currentForm = &terrainForm;
  currentForm->setReadOnly(true);

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
  controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::viewSensor(int id) {
  currentItemId = id;
  controlButtons.disconnectAll();
  controlButtons.singleConnect();
  connect((&controlButtons)->editButton, SIGNAL(clicked()), this,
          SLOT(enableForm()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveSensor()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelSensor()));
  connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this,
          SLOT(deleteSensor()));
  EDomElement node(
      manager->getXml("sensor", "key", Conversion::intToString(id)));
  sensorForm.clearForm();
  sensorForm.setFormLocale(QLocale(QLocale::system()));
  sensorForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
  centerArea.setCurrentIndex(2);
  currentForm = &sensorForm;
  currentForm->setReadOnly(true);
  validatingSensor();

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewFlight(int id) {
  currentItemId = id;
  controlButtons.disconnectAll();
  controlButtons.singleConnect();
  connect((&controlButtons)->editButton, SIGNAL(clicked()), this,
          SLOT(enableForm()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveFlight()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelFlight()));
  connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this,
          SLOT(deleteFlight()));

  EDomElement node(
      manager->getXml("flight", "key", Conversion::intToString(id)));

  flightForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
  centerArea.setCurrentIndex(3);
  currentForm = &flightForm;
  currentForm->setReadOnly(true);
  validatingFlight();

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewImages() {
  // Keep the selected image across refreshes (new image, delete, return from a
  // module), so the user does not have to select it again.
  const int selectedKey = imagesForm.selectedKey();

  controlButtons.disconnectAll();
  controlButtons.multiConnect();
  connect((&controlButtons)->newButton, SIGNAL(clicked()), this,
          SLOT(newImage()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveNewImage()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelImages()));
  // The generic modes hide every button when "Delete" is pressed; here it
  // deletes the selected image instead, so the confirmation dialog does not
  // make the toolbar disappear when the user answers "No".
  disconnect((&controlButtons)->deleteButton, SIGNAL(clicked()),
             (&controlButtons), SLOT(toInvisibleMode()));
  connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this,
          SLOT(deleteImage()));
  controlButtons.setDeleteEnabled(false);

  EDomElement imgs(manager->getXml("images"));

  imagesForm.fillvalues(imgs.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget "
      "{border: 0px}");
  centerArea.setCurrentIndex(4);
  currentForm = &imagesForm;
  currentForm->setReadOnly(true);

  EDomElement ios(manager->getXml("interiorOrientation"));
  if (ios.getContent() != "") {
    imagesForm.setIOsAvailable(ios.getContent());
  }

  EDomElement eos(manager->getXml("exteriorOrientation"));
  EDomElement srs(manager->getXml("spatialResections"));
  EDomElement ptri(manager->getXml("phototriangulation"));
  std::string eoContent = eos.getContent();
  eoContent.append(srs.getContent());
  eoContent.append(ptri.getContent());
  if (eos.getContent() != "") {
    imagesForm.setEOsAvailable(eoContent);
  }
  int height = (imagesForm.imagesTable->rowCount() + 2) *
                   imagesForm.imagesTable->rowHeight(0) +
               imagesForm.label->height() + imagesForm.importButton->height();
  imagesForm.setMinimumSize(imagesForm.imagesTable->width(), height);

  const int selectedRow = imagesForm.rowOfKey(selectedKey);
  imagesForm.setSelectedRow(selectedRow);
  if (selectedRow >= 0) selectImage(selectedRow);

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(imgs.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewImage(int id) {
  currentItemId = id;
  controlButtons.disconnectAll();
  controlButtons.singleConnect();
  connect((&controlButtons)->editButton, SIGNAL(clicked()), this,
          SLOT(enableForm()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveImage()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelImage()));
  connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this,
          SLOT(deleteImage()));

  EDomElement node(
      manager->getXml("image", "key", Conversion::intToString(id)));

  imageForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea {border: 0px}");
  centerArea.setCurrentIndex(5);
  currentForm = &imageForm;
  currentForm->setReadOnly(true);
  validatingImage();

  EDomElement node2(
      manager->getXml("imageIO", "image_key", Conversion::intToString(id)));
  if (node2.getContent() != "") {
    imageForm.groupBox->setVisible(true);
    imageForm.a0Label->setText(
        tr("a0: ") +
        QString::number(node2.elementByTagName("a0").toDouble(), 'f', 4));
    imageForm.a1Label->setText(
        tr("a1: ") +
        QString::number(node2.elementByTagName("a1").toDouble(), 'f', 4));
    imageForm.a2Label->setText(
        tr("a2: ") +
        QString::number(node2.elementByTagName("a2").toDouble(), 'f', 4));
    imageForm.b0Label->setText(
        tr("b0: ") +
        QString::number(node2.elementByTagName("b0").toDouble(), 'f', 4));
    imageForm.b1Label->setText(
        tr("b1: ") +
        QString::number(node2.elementByTagName("b1").toDouble(), 'f', 4));
    imageForm.b2Label->setText(
        tr("b2: ") +
        QString::number(node2.elementByTagName("b2").toDouble(), 'f', 4));
  } else {
    imageForm.groupBox->setVisible(false);
  }

  EDomElement node3(
      manager->getXml("imageEO", "image_key", Conversion::intToString(id)));
  if (node3.getContent() != "") {
    imageForm.groupBox_2->setVisible(true);

    if (node3.attribute("type") == "spatialResection")
      imageForm.groupBox_2->setTitle(tr("EO Parameters (Spatial Resection)"));
    else if (node3.attribute("type") == "user")
      imageForm.groupBox_2->setTitle(
          tr("EO Parameters (3D Direct Georeferencing)"));
    else
      imageForm.groupBox_2->setTitle(tr("EO Parameters (Phototriangulation)"));

    imageForm.x0Label->setText(
        tr("X0: ") +
        QString::number(node3.elementByTagName("X0").toDouble(), 'f', 4) +
        tr(" m"));
    imageForm.y0Label->setText(
        tr("Y0: ") +
        QString::number(node3.elementByTagName("Y0").toDouble(), 'f', 4) +
        tr(" m"));
    imageForm.z0Label->setText(
        tr("Z0: ") +
        QString::number(node3.elementByTagName("Z0").toDouble(), 'f', 4) +
        tr(" m"));
    imageForm.omegaLabel->setText(
        QString::fromUtf8(" ω: ") +
        QString::number(node3.elementByTagName("omega").toDouble() * 180 / M_PI,
                        'f', 4) +
        QString::fromUtf8("°"));
    imageForm.phiLabel->setText(
        QString::fromUtf8(" φ: ") +
        QString::number(node3.elementByTagName("phi").toDouble() * 180 / M_PI,
                        'f', 4) +
        QString::fromUtf8("°"));
    imageForm.kappaLabel->setText(
        QString::fromUtf8(" κ: ") +
        QString::number(node3.elementByTagName("kappa").toDouble() * 180 / M_PI,
                        'f', 4) +
        QString::fromUtf8("°"));
  } else {
    imageForm.groupBox_2->setVisible(false);
  }

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewPoints() {
  // Keep the selected point across refreshes, so the user does not have to
  // select it again after a new import or a deletion.
  const int selectedKey = pointsForm.selectedKey();

  controlButtons.disconnectAll();
  controlButtons.multiConnect();
  connect((&controlButtons)->newButton, SIGNAL(clicked()), this,
          SLOT(newPoint()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(saveNewPoint()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelPoints()));
  // As in the images list, "Delete" removes the selected point instead of
  // hiding the whole button bar (see viewImages).
  disconnect((&controlButtons)->deleteButton, SIGNAL(clicked()),
             (&controlButtons), SLOT(toInvisibleMode()));
  connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this,
          SLOT(deletePoint()));
  controlButtons.setDeleteEnabled(false);

  EDomElement node(manager->getXml("points"));

  pointsForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget "
      "{border: 0px}");
  centerArea.setCurrentIndex(6);
  currentForm = &pointsForm;
  currentForm->setReadOnly(true);
  int height = (pointsForm.pointsTable->rowCount() + 2) *
                   pointsForm.pointsTable->rowHeight(0) +
               pointsForm.importButton->height() + pointsForm.label->height();
  pointsForm.setMinimumSize(pointsForm.pointsTable->width(), height);

  const int selectedRow = pointsForm.rowOfKey(selectedKey);
  pointsForm.setSelectedRow(selectedRow);
  if (selectedRow >= 0) selectPoint(selectedRow);

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::viewPoint(int id) {
  currentItemId = id;
  controlButtons.disconnectAll();
  controlButtons.singleConnect();
  connect((&controlButtons)->editButton, SIGNAL(clicked()), this,
          SLOT(enableForm()));
  connect((&controlButtons)->saveButton, SIGNAL(clicked()), this,
          SLOT(savePoint()));
  connect((&controlButtons)->cancelButton, SIGNAL(clicked()), this,
          SLOT(cancelPoint()));
  connect((&controlButtons)->deleteButton, SIGNAL(clicked()), this,
          SLOT(deletePoint()));

  EDomElement node(
      manager->getXml("point", "key", Conversion::intToString(id)));

  pointForm.setImageList(manager->listImageKeys(), manager->listImages());
  pointForm.fillvalues(node.getContent());
  centerArea.setStyleSheet(
      "QScrollArea, QWidget {background: #FFFFFF} QScrollArea, QTableWidget "
      "{border: 0px}");
  centerArea.setCurrentIndex(7);
  currentForm = &pointForm;
  currentForm->setReadOnly(true);
  validatingPoint();

  debuggerTextEdit->clear();
  debuggerTextEdit->setText(
      QString::fromUtf8(node.indent('\t').getContent().c_str()));
  debuggerTextEdit->setReadOnly(true);

  menuProject->setEnabled(true);
  enableExecuteMenu(true);
}

void ProjectUserInterface_Qt::enableExecuteMenu(bool status) {
  menuExecute->setEnabled(status);

  refreshMenu();
}

// Habilitando cada view...
void ProjectUserInterface_Qt::enableForm() {
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
void ProjectUserInterface_Qt::saveHeader() {
  editState = false;
  manager->editComponent("Header", headerForm.getvalues());
  viewHeader();
  actionSave_file->setEnabled(true);
  updateTree();
}

void ProjectUserInterface_Qt::saveTerrain() {
  editState = false;
  manager->editComponent("Terrain", terrainForm.getvalues());
  viewTerrain();
  actionSave_file->setEnabled(true);
  updateTree();
}

void ProjectUserInterface_Qt::saveSensor() {
  editState = false;
  manager->editComponent("Sensor", currentItemId, sensorForm.getvalues());
  viewSensor(currentItemId);
  actionSave_file->setEnabled(true);
  updateTree();
}

void ProjectUserInterface_Qt::saveFlight() {
  editState = false;
  manager->editComponent("Flight", currentItemId, flightForm.getvalues());
  viewFlight(currentItemId);
  actionSave_file->setEnabled(true);
  updateTree();
}

void ProjectUserInterface_Qt::saveImage() {
  std::string xmlString = "";
  std::stringstream eoXML;
  bool existOE, userOE = false;

  editState = false;
  xmlString = imageForm.getvalues();
  manager->editComponent("Image", currentItemId, xmlString);
  EDomElement imageUserEO(xmlString);
  if (imageUserEO.hasTagName("GNSS") && imageUserEO.hasTagName("INS")) {
    existOE = !(manager->getXml("imageEO", "image_key",
                                Conversion::intToString(currentItemId)) == "");
    if (existOE) {
      EDomElement oeXMLObj(manager->getXml(
          "imageEO", "image_key", Conversion::intToString(currentItemId)));
      userOE = oeXMLObj.attribute("type") == "user";
      if (userOE) manager->removeComponent("EO", currentItemId);
    }
    if (!existOE || userOE) {
      EDomElement xyzXml(imageUserEO.elementByTagName("gml:pos"));
      EDomElement omegaXML(imageUserEO.elementByTagName("omega"));
      EDomElement phiXML(imageUserEO.elementByTagName("phi"));
      EDomElement kappaXML(imageUserEO.elementByTagName("kappa"));
      std::deque<double> XA;
      XA = xyzXml.toGmlPos();
      eoXML << "\t<imageEO type=\"user\" image_key=\""
            << Conversion::intToString(currentItemId) << "\">\n";
      eoXML << "\t\t\t<Xa>\n";
      eoXML << "\t\t\t\t<X0 uom=\"#m\">" << Conversion::doubleToString(XA.at(0))
            << "</X0>\n";
      eoXML << "\t\t\t\t<Y0 uom=\"#m\">" << Conversion::doubleToString(XA.at(1))
            << "</Y0>\n";
      eoXML << "\t\t\t\t<Z0 uom=\"#m\">" << Conversion::doubleToString(XA.at(2))
            << "</Z0>\n";
      eoXML << "\t\t\t\t<omega uom=\"#rad\">" << omegaXML.toString()
            << "</omega>\n";
      eoXML << "\t\t\t\t<phi uom=\"#rad\">" << phiXML.toString() << "</phi>\n";
      eoXML << "\t\t\t\t<kappa uom=\"#rad\">" << kappaXML.toString()
            << "</kappa>\n";
      eoXML << "\t\t\t</Xa>\n";
      eoXML << "</imageEO>\n";
      manager->addComponent(eoXML.str(), "exteriorOrientation");
      actionStereo->setEnabled(availableStereoPlotter());
      actionDEMExtraction->setEnabled(availableDemExtraction());
      actionOrtho_rectification->setEnabled(availableOrthoImage());
    }
  }
  viewImage(currentItemId);
  actionSave_file->setEnabled(true);
  updateTree();
}

void ProjectUserInterface_Qt::savePoint() {
  editState = false;
  manager->editComponent("Point", currentItemId, pointForm.getvalues());
  viewPoint(currentItemId);
  actionSave_file->setEnabled(true);
  updateTree();
}

// Cancelando cada view...
void ProjectUserInterface_Qt::cancelHeader() {
  editState = false;
  viewHeader();
}

void ProjectUserInterface_Qt::cancelTerrain() {
  editState = false;
  viewTerrain();
}

void ProjectUserInterface_Qt::cancelSensor() {
  editState = false;
  viewSensor(currentItemId);
}

void ProjectUserInterface_Qt::cancelFlight() {
  editState = false;
  viewFlight(currentItemId);
}

void ProjectUserInterface_Qt::cancelImage() {
  editState = false;
  viewImage(currentItemId);
}

void ProjectUserInterface_Qt::cancelPoint() {
  editState = false;
  viewPoint(currentItemId);
}

void ProjectUserInterface_Qt::cancelImages() {
  addNewState = false;
  viewImages();
}

void ProjectUserInterface_Qt::cancelPoints() {
  addNewState = false;
  viewPoints();
}

// Criando um vazio de cada view...
void ProjectUserInterface_Qt::newImage() {
  addNewState = true;
  currentItemId = manager->getFreeImageId();

  std::string text = "";
  text += "<image key=\"" + Conversion::intToString(currentItemId) +
          "\" sensor_key=\"1\" flight_key=\"1\">\n";
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

void ProjectUserInterface_Qt::newPoint() {
  addNewState = true;
  std::string text = "";

  currentItemId = manager->getFreePointId();
  text += "<point key=\"" + Conversion::intToString(currentItemId) +
          "\" type=\"\">\n";
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
void ProjectUserInterface_Qt::saveNewImage() {
  addNewState = false;
  manager->addComponent(imageForm.getvalues(), "images");
  viewImages();
  actionSave_file->setEnabled(true);
  updateTree();
}

void ProjectUserInterface_Qt::saveNewPoint() {
  addNewState = false;
  manager->addComponent(pointForm.getvalues(), "points");
  viewPoints();
  actionSave_file->setEnabled(true);
  updateTree();
}

// Apagando cada view...
void ProjectUserInterface_Qt::deleteFlight() {}

void ProjectUserInterface_Qt::deleteSensor() {}

void ProjectUserInterface_Qt::deletePoint() {
  const int deletedKey = currentItemId;
  const int deletedRow = pointsForm.rowOfKey(deletedKey);
  const std::string keyText = Conversion::intToString(deletedKey);

  // A point carries its measurements on the images, and it may be part of the
  // phototriangulation / spatial resections, so say what else goes away.
  QString question = tr("Are you sure you want to delete this point?");
  const std::string pointXml = manager->getXml("point", "key", keyText);
  const bool hasMeasurements =
      pointXml.find("<imageCoordinates") != std::string::npos;
  const std::string usedPoint = "<pointKey>" + keyText + "</pointKey>";
  const std::string ptXml = manager->getXml("phototriangulation");
  const std::string srXml = manager->getXml("spatialResections");
  const bool isUsed = ptXml.find(usedPoint) != std::string::npos ||
                      srXml.find(usedPoint) != std::string::npos;
  if (hasMeasurements || isUsed) {
    question += "\n\n";
    question += tr("Its measurements on the images will be deleted too.");
  }
  if (isUsed) {
    question += " ";
    question += tr("It will also be removed from the phototriangulation and "
                   "from the spatial resections that used it.");
  }

  QMessageBox::StandardButton reply = QMessageBox::question(
      this, tr("Delete Point"), question, QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    // Removes the point, its measurements and the references to it (see
    // ProjectManager::removeComponent).
    manager->removeComponent("Point", deletedKey);

    updateTree();
    viewPoints();

    // Keep the neighbour of the deleted point selected, so deleting a sequence
    // of points does not need a new click for each one.
    const int rows = pointsForm.pointsTable->rowCount();
    if (rows > 0) {
      int row = deletedRow < 0 ? 0 : deletedRow;
      if (row > rows - 1) row = rows - 1;
      pointsForm.setSelectedRow(row);
      selectPoint(row);
    } else {
      controlButtons.setDeleteEnabled(false);
    }

    actionSave_file->setEnabled(true);
  }
}

void ProjectUserInterface_Qt::deleteImage() {
  const int deletedKey = currentItemId;
  const int deletedRow = imagesForm.rowOfKey(deletedKey);

  // Deleting an image also deletes its orientations and the measurements of the
  // points on it, so say it when there are any.
  QString question = tr("Are you sure you want to delete this image?");
  const std::string keyText = Conversion::intToString(deletedKey);
  const bool hasOrientations =
      !manager->getXml("imageIO", "image_key", keyText).empty() ||
      !manager->getXml("imageEO", "image_key", keyText).empty();
  const bool hasMeasurements =
      manager->getXml("points").find("image_key=\"" + keyText + "\"") !=
      std::string::npos;
  if (hasOrientations || hasMeasurements) {
    question += "\n\n";
    question += tr(
        "Its interior/exterior orientation and the measurements of the points "
        "on it will be deleted too.");
  }

  QMessageBox::StandardButton reply = QMessageBox::question(
      this, tr("Delete Image"), question, QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    // Removes the image and the orientations that belong to it (see
    // ProjectManager::removeComponent).
    manager->removeComponent("Image", deletedKey);

    updateTree();
    viewImages();

    // Keep the neighbour of the deleted image selected, so deleting a
    // sequence of images does not need a new click for each one.
    const int rows = imagesForm.imagesTable->rowCount();
    if (rows > 0) {
      int row = deletedRow < 0 ? 0 : deletedRow;
      if (row > rows - 1) row = rows - 1;
      imagesForm.setSelectedRow(row);
      selectImage(row);
    } else {
      controlButtons.setDeleteEnabled(false);
    }

    actionSave_file->setEnabled(true);
  }
}

void ProjectUserInterface_Qt::toggleDebug() {
  if (debuggerDockWidget->isVisible())
    debuggerDockWidget->setVisible(false);
  else
    debuggerDockWidget->setVisible(true);
}

void ProjectUserInterface_Qt::showAbout() {
  AboutForm *about = new AboutForm();
  about->show();
}

void ProjectUserInterface_Qt::validatingSensor() {
  if ((sensorForm.sensorIdLineEdit->text() == "") ||
      (!sensorForm.fid0SigmaController->getValidate()) ||
      (!sensorForm.fid1SigmaController->getValidate()) ||
      (!sensorForm.fid2SigmaController->getValidate()) ||
      (!sensorForm.fid3SigmaController->getValidate()) ||
      ((!sensorForm.fid4SigmaController->getValidate()) &&
       (sensorForm.fiducialMarksCombo->currentIndex() == 1)) ||
      ((!sensorForm.fid5SigmaController->getValidate()) &&
       (sensorForm.fiducialMarksCombo->currentIndex() == 1)) ||
      ((!sensorForm.fid6SigmaController->getValidate()) &&
       (sensorForm.fiducialMarksCombo->currentIndex() == 1)) ||
      ((!sensorForm.fid7SigmaController->getValidate()) &&
       (sensorForm.fiducialMarksCombo->currentIndex() == 1)) ||
      (!sensorForm.principalSigmaController->getValidate()) ||
      (!sensorForm.radialSigmaController->getValidate()) ||
      (!sensorForm.decenteredSigmaController->getValidate()) ||
      (!sensorForm.calibratedSigmaController->getValidate())) {
    controlButtons.saveButton->setEnabled(false);
  } else
    controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingFlight() {
  if ((flightForm.flightIDLineEdit->text() == "")) {
    controlButtons.saveButton->setEnabled(false);
  } else
    controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingImage() {
  if ((imageForm.imageIDLine->text() == "") ||
      (imageForm.resolutionSpin->text() == " ") ||
      (imageForm.fileNameLine->text() == "") ||
      (!imageForm.gnssSigmaController->getValidate()) ||
      (!imageForm.insSigmaController->getValidate())) {
    controlButtons.saveButton->setEnabled(false);
  } else
    controlButtons.saveButton->setEnabled(true);
}

void ProjectUserInterface_Qt::validatingPoint() {
  if ((pointForm.lineEdit_gcp_id->text() == "") ||
      (!pointForm.sigmaController->getValidate()))
    controlButtons.saveButton->setEnabled(false);
  else
    controlButtons.saveButton->setEnabled(true);
}

bool ProjectUserInterface_Qt::confirmToClose() {
  if (editState || addNewState) {
    if (controlButtons.saveButton->isEnabled()) {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(
          this, tr(" Warning: leaving form in edit mode"),
          tr("Do you want to keep all changes?"),
          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      if (reply == QMessageBox::Yes) {
        controlButtons.saveButton->click();
      } else if (reply == QMessageBox::No) {
        controlButtons.cancelButton->click();
      } else {
        return false;
      }
    } else {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(
          this, tr(" Warning: leaving form in edit mode"),
          tr("Registration data is not complete and will be lost. Continue?"),
          QMessageBox::Yes | QMessageBox::Cancel);
      if (reply == QMessageBox::Yes) {
        controlButtons.cancelButton->click();
      } else {
        return false;
      }
    }
  }
  if (actionSave_file->isEnabled()) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, tr(" Warning: You have unsaved data"),
        tr("Do you want to save all changes?"),
        (changeModule
             ? QMessageBox::Yes | QMessageBox::No
             : QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
    if (reply == QMessageBox::Yes) {
      actionSave_file->trigger();
    } else if (reply == QMessageBox::Cancel) {
      return false;
    }
  }
  return true;
}

void ProjectUserInterface_Qt::refreshMenu() {
  actionSpatial_resection->setEnabled(availableSR());
  actionFoto_Tri->setEnabled(availablePhotoTri());

  actionStereo->setEnabled(availableStereoPlotter());
  actionDEMExtraction->setEnabled(availableDemExtraction());
  actionOrtho_rectification->setEnabled(availableOrthoImage());
  actionPTReport->setEnabled(availablePhotoTri());
}

QString ProjectUserInterface_Qt::getSavedIn() {
  return QString::fromStdString(PathUtils::projectDirectory());
}

/** This function import points from a *.txt file with a one point in each line
 *  in pattern GCPID\tE\tN\tH\tdE\tdN\tdH or GCPID\tE\tN\tH
 */
void ProjectUserInterface_Qt::importPointsFromTxt() {
  QString importFileName =
      QFileDialog::getOpenFileName(this, tr("Open Import File"), ".", "*.txt");
  if (importFileName == "") return;
  QFile *importFile = new QFile(importFileName);
  QStringList pointsList;

  importFile->open(QIODevice::ReadOnly);

  while (!importFile->atEnd()) {
    QByteArray line = importFile->readLine();
    QString aux(line);
    pointsList << aux.simplified();
    ;
  }
  importFile->close();

  QWidget *loadWidget = new QWidget();
  loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
  QProgressBar loading;
  QPushButton cancelButton("Cancel");
  loading.setRange(0, pointsList.size());

  QVBoxLayout loadLayout;
  loadLayout.addWidget(&loading, Qt::AlignCenter);
  loadLayout.addWidget(&cancelButton, Qt::AlignCenter);
  connect(&cancelButton, SIGNAL(clicked()), loadWidget, SLOT(close()));

  loadWidget->setLayout(&loadLayout);
  loadWidget->setWindowTitle(tr("Loading Points"));
  loading.setMinimumSize(300, 30);
  loadWidget->show();

  std::string newPointXML;
  for (int i = 0; i < pointsList.length() && loadWidget != NULL; i++) {
    loading.setFormat(tr("%v/%m : %p%"));
    loading.setValue(i + 1);
    newPointXML +=
        pointTxtToXml(pointsList.at(i), manager->getFreePointId() + i, i + 1);
  }
  manager->addComponent(newPointXML, "points");
  loadWidget->close();

  updateTree();
  viewPoints();
  actionSave_file->setEnabled(true);
}

/** This function convert a point data from a *.txt line in a children point XML
 * valid
 */
std::string ProjectUserInterface_Qt::pointTxtToXml(QString point, int key,
                                                   int line,
                                                   std::string typePoint) {
  std::stringstream aux;
  std::string gcpIdField, typeField, eField, nField, hField, dEField, dNField,
      dHField;
  QStringList fields = point.split(
      QRegularExpression("\\s+"));  // split by any sequences of whitespace
  // check	control	 tie
  if (fields.length() == 7) {
    gcpIdField = fields.at(0).toUtf8().data();
    eField = fields.at(1).toStdString();
    nField = fields.at(2).toStdString();
    hField = fields.at(3).toStdString();
    dEField = fields.at(4).toStdString();
    dNField = fields.at(5).toStdString();
    dHField = fields.at(6).toStdString();

    aux << "<point key=\"" << Conversion::intToString(key) << "\" type=\""
        << typePoint << "\">\n";
    aux << "<pointId>" << gcpIdField << "</pointId>\n";
    aux << "<description>" << "Put point description here"
        << "</description>\n";
    aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
    aux << "<gml:pos>" << eField << " " << nField << " " << hField
        << "</gml:pos>\n";
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
  } else if (fields.length() == 4) {
    gcpIdField = fields.at(0).toStdString();
    eField = fields.at(1).toStdString();
    nField = fields.at(2).toStdString();
    hField = fields.at(3).toStdString();

    aux << "<point key=\"" << Conversion::intToString(key) << "\" type=\""
        << typePoint << "\">\n";
    aux << "<pointId>" << gcpIdField << "</pointId>\n";
    aux << "<description>" << "Put point description here"
        << "</description>\n";
    aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
    aux << "<gml:pos>" << eField << " " << nField << " " << hField
        << "</gml:pos>\n";
    aux << "<sigma>Not Available</sigma>\n";
    aux << "</spatialCoordinates>\n";
    aux << "<imagesMeasurements>\n";
    aux << "</imagesMeasurements>\n";
    aux << "</point>";
  } else {
    QMessageBox::warning(this, tr(" fileWarning "),
                         tr("The point in line %1 from imported file\nhas "
                            "incomplete or corrupted data")
                             .arg(line));
    return "";
  }
  return aux.str();
}

/** This function exports all points in current XML to *.txt file with a one
 * point in each line in pattern GCPID\tE\tN\tH\tdE\tdN\tdH or GCPID\tE\tN\tH
 */
void ProjectUserInterface_Qt::exportPointsToTxt() {
  QFileDialog salvar(this, tr("Export file"), ".", "*.txt");
  salvar.setAcceptMode(QFileDialog::AcceptSave);
  salvar.setDefaultSuffix("txt");
  if (salvar.exec()) {
    QString fileSaveName = salvar.selectedFiles()[0];
    if (fileSaveName.isEmpty()) return;

    if (!fileSaveName.endsWith(".txt")) fileSaveName.append(".txt");

    QFile *exportFileName = new QFile(fileSaveName);
    exportFileName->setFileName(fileSaveName);
    exportFileName->open(QIODevice::WriteOnly);

    EDomElement points(manager->getXml("points").c_str());

    QWidget *loadWidget = new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    QProgressBar loading;
    QPushButton cancelButton("Cancel");
    loading.setRange(0, points.children().size());

    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading, Qt::AlignCenter);
    loadLayout.addWidget(&cancelButton, Qt::AlignCenter);
    connect(&cancelButton, SIGNAL(clicked()), loadWidget, SLOT(close()));

    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Exporting Points"));
    loading.setMinimumSize(300, 30);
    loadWidget->show();

    std::deque<EDomElement> point = points.elementsByTagName("point");
    for (int i = 0; i < (int)point.size(); i++) {
      loading.setFormat(tr("Point %v/%m : %p%"));
      loading.setValue(i);
      exportFileName->write(edomPointToTxt(point.at(i)).data());
    }
    loadWidget->close();
    exportFileName->close();
  }
}

/** This function convert a EDomElement children Point in a line *.txt point
 * format
 */
std::string ProjectUserInterface_Qt::edomPointToTxt(EDomElement points) {
  std::stringstream aux;
  std::stringstream stdev;
  QString gmlpos = points.elementByTagName("gml:pos").toString().c_str();
  aux << points.elementByTagName("pointId").toString().c_str() << "\t";

  aux << (gmlpos.split(" ").at(0)).toStdString().c_str() << "\t"
      << (gmlpos.split(" ").at(1)).toStdString().c_str() << "\t"
      << (gmlpos.split(" ").at(2)).toStdString().c_str();

  Matrix stdevMatrix;
  stdevMatrix.xmlSetData(points.elementByTagName("mml:matrix").getContent());

  if (points.hasTagName("mml:matrix")) {
    aux << "\t" << stdevMatrix.get(1, 1) << "\t";
    aux << stdevMatrix.get(2, 2) << "\t";
    aux << stdevMatrix.get(3, 3);
  }

  aux << "\n";
  std::string result = aux.str();
  return result.c_str();
}

/** This function import images from a *.txt file with a one image in each line
 *   in pattern ...
 */
void ProjectUserInterface_Qt::importImagesFromTxt() {
  QString importFileName =
      QFileDialog::getOpenFileName(this, tr("Open Import File"), ".", "*.txt");
  QFile *importFile = new QFile(importFileName);
  QStringList imagesList;

  importFile->open(QIODevice::ReadOnly);

  while (!importFile->atEnd()) {
    QByteArray line = importFile->readLine();
    QString aux(line);
    imagesList << aux.remove('\n');
  }

  importFile->close();

  for (int i = 0; i < imagesList.length(); i++) {
    EDomElement newImageXML =
        imageTxtToXml(/*imagesList.at(i),manager->getFreeImageId(),i+1*/);
    if (newImageXML.hasTagName("imageId")) {
      manager->addComponent(newImageXML.getContent().data(), "images");
    }
  }
  updateTree();
  viewImages();
}

/** This function convert image data from a *.txt line in a children image XML
 * valid
 *
 */
EDomElement ProjectUserInterface_Qt::imageTxtToXml() {
  std::stringstream aux;

  std::string newXml = aux.str();
  EDomElement newImageXml(newXml.c_str());

  return newImageXml;
}

// inserido pelo Paulo 05/09/2011
//  Coloca as coordenadas digitais linha e coluna nos pontos;
void ProjectUserInterface_Qt::importDigitalCoordinatesFromTxt() {
  importPointsFromTxt2();

  QString importFileName =
      QFileDialog::getOpenFileName(this, tr("Open Import File"), ".", "*.txt");
  if (importFileName == "") return;
  QFile *importFile = new QFile(importFileName);
  QStringList pointsList;

  importFile->open(QIODevice::ReadOnly);

  while (!importFile->atEnd()) {
    QByteArray line = importFile->readLine();
    QString aux(line);
    pointsList << aux.remove('\n');
  }

  importFile->close();

  QWidget *loadWidget = new QWidget();
  loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
  QProgressBar loading;
  QPushButton cancelButton("Cancel");
  loading.setRange(0, pointsList.length());

  QVBoxLayout loadLayout;
  loadLayout.addWidget(&loading, Qt::AlignCenter);
  loadLayout.addWidget(&cancelButton, Qt::AlignCenter);
  connect(&cancelButton, SIGNAL(clicked()), loadWidget, SLOT(close()));

  loadWidget->setLayout(&loadLayout);
  loadWidget->setWindowTitle(tr("Loading Digitals Coordinates"));
  loading.setMinimumSize(300, 30);
  loadWidget->show();

  EDomElement imagesXml(manager->getXml("points").c_str());
  std::deque<EDomElement> imagesEdom = imagesXml.elementsByTagName("point");

  for (int i = 0; i < pointsList.length(); i++) {
    if (!insertDigitalCoordinates(pointsList.at(i)))  //,imagesEdom.at()))
      QMessageBox::warning(this, tr(" Warning "),
                           tr("The point in line %1 from imported file\nhas "
                              "incomplete or corrupted data")
                               .arg(i));
  }
  loadWidget->close();
  updateTree();
  viewPoints();
  actionSave_file->setEnabled(true);
}

bool ProjectUserInterface_Qt::insertDigitalCoordinates(QString coordinates) {
  QStringList digitalPoint = coordinates.split("\t");
  if (digitalPoint.size() != 4)
    return false;
  else {
    std::string imagekey = digitalPoint.at(0).toStdString().c_str();
    std::string pointkey = digitalPoint.at(1).toStdString().c_str();
    std::string colValue = digitalPoint.at(2).toStdString().c_str();
    std::string linValue = digitalPoint.at(3).toStdString().c_str();

    EDomElement pointsXML(manager->getXml("points").c_str());
    EDomElement point(pointsXML.elementByTagAtt("point", "key", pointkey));
    std::stringstream aux;
    aux << "<imageCoordinates uom=\"#px\" image_key=\"" << imagekey << "\">";
    aux << "<gml:pos>" << colValue << " " << linValue << "</gml:pos>";
    aux << "</imageCoordinates>\n";
    point.addChildAtTagName("imagesMeasurements", aux.str());
    manager->editComponent("Point", Conversion::stringToInt(pointkey),
                           point.getContent());
    return true;
  }
}

bool ProjectUserInterface_Qt::availablePhotoTri() {
  EDomElement ois(manager->getXml("interiorOrientation"));
  EDomElement images(manager->getXml(("images")));
  if (images.children().size() <= ois.children().size()) return true;
  return false;
}

// ******
// Bug reported by Martin Veermer
// Big fixed by Marcelo Teixeira Silveira
bool ProjectUserInterface_Qt::availableSR() { return availablePhotoTri(); }

bool ProjectUserInterface_Qt::availableLP() {
  QSettings efotoSettings("uerj", "efoto");
  return efotoSettings.contains("lastProject");
}

bool ProjectUserInterface_Qt::availableStereoPlotter() {
  std::deque<EDomElement> images =
      EDomElement(manager->getXml(("images"))).children();
  EDomElement oes(manager->getXml("exteriorOrientation"));
  EDomElement pt(manager->getXml(("phototriangulation")));
  EDomElement srs(manager->getXml(("spatialResections")));

  bool hasAllEOs = true;

  for (unsigned int i = 0; i < images.size(); i++) {
    EDomElement img(images.at(i));
    int key = Conversion::stringToInt(img.attribute("key"));
    EDomElement EOXml = oes.elementByTagAtt("imageEO", "image_key",
                                            Conversion::intToString(key));

    std::string type = EOXml.attribute("type");
    if (EOXml.getContent() == "") {
      hasAllEOs = false;
      break;
    }

    bool converged = true;
    if (type == "photoTriangulation")
      converged = pt.elementByTagName("converged").toBool();
    else if (type == "spatialResection")
      converged = srs.elementByTagAtt("imageSR", "image_key",
                                      Conversion::intToString(key))
                      .elementByTagName("converged")
                      .toBool();

    if (!converged) {
      hasAllEOs = false;
      break;
    }
  }
  return hasAllEOs;
}

bool ProjectUserInterface_Qt::availableDemExtraction() {
  return availableStereoPlotter();
}

bool ProjectUserInterface_Qt::availableOrthoImage() {
  return availableStereoPlotter();
}

void ProjectUserInterface_Qt::updateCurrentForm() {
  if (currentForm != NULL) {
    if (currentForm == &headerForm)
      viewHeader();
    else if (currentForm == &terrainForm)
      viewTerrain();
    else if (currentForm == &sensorForm)
      viewSensor(currentItemId);
    else if (currentForm == &flightForm)
      viewFlight(currentItemId);
    else if (currentForm == &imagesForm)
      viewImages();
    else if (currentForm == &imageForm)
      viewImage(currentItemId);
    else if (currentForm == &pointsForm)
      viewPoints();
    else if (currentForm == &pointForm)
      viewPoint(currentItemId);
  }
}

void ProjectUserInterface_Qt::importPointsFromTxt2() {
  // primeiro arquivo
  QString importFileName = QFileDialog::getOpenFileName(
      this, tr("Open Import ENH file"), ".", "*.txt");
  if (importFileName == "") return;
  QFile *importFile = new QFile(importFileName);
  QStringList pointsList;

  importFile->open(QIODevice::ReadOnly);

  while (!importFile->atEnd()) {
    QByteArray line = importFile->readLine();
    QString aux(line);
    pointsList << aux.remove('\n');
  }
  importFile->close();

  // segundo arquivo
  QString importFileName2 = QFileDialog::getOpenFileName(
      this, tr("Open Import Digital File"), ".", "*.txt");
  if (importFileName2 == "") return;
  QFile *importFile2 = new QFile(importFileName2);
  QStringList pointsList2;

  importFile2->open(QIODevice::ReadOnly);

  while (!importFile2->atEnd()) {
    QByteArray line2 = importFile2->readLine();
    QString aux2(line2);
    pointsList2 << aux2.remove('\n');
  }
  importFile2->close();

  // Procura e concatena o ponto as suas coordenadas digitais nas imagens
  int cont = 0;
  for (int i = 0; i < pointsList.size(); i++) {
    QStringList fields = pointsList.at(i).split("\t");
    QString gcpIdField = fields.at(0);
    for (int j = 0; j < pointsList2.size() && cont != -1; j++) {
      QStringList fieldsDigital = pointsList2.at(j).split("\t");
      QString gcpIdFieldDigital = fieldsDigital.at(1);
      if (gcpIdField.compare(gcpIdFieldDigital) == 0) {
        QString temp = pointsList.at(i);
        temp += "\t";
        temp += pointsList2.at(j);
        pointsList.replace(i, temp);
        cont++;
      }
    }
  }

  QWidget *loadWidget = new QWidget();
  loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
  QProgressBar loading;
  QPushButton cancelButton("Cancel");
  loading.setRange(0, pointsList.size());

  QVBoxLayout loadLayout;
  loadLayout.addWidget(&loading, Qt::AlignCenter);
  loadLayout.addWidget(&cancelButton, Qt::AlignCenter);
  connect(&cancelButton, SIGNAL(clicked()), loadWidget, SLOT(close()));

  loadWidget->setLayout(&loadLayout);
  loadWidget->setWindowTitle(tr("Loading Points"));
  loading.setMinimumSize(300, 30);
  loadWidget->show();

  std::string newPointXML;
  for (int i = 0; i < pointsList.length() && loadWidget != NULL; i++) {
    loading.setFormat(tr("%v/%m : %p%"));
    loading.setValue(i + 1);
    newPointXML += pointTxtToXml2(pointsList.at(i),
                                  manager->getFreePointId() + i /*,i+1*/);
  }
  manager->addComponent(newPointXML, "points");
  loadWidget->close();
  updateTree();
  viewPoints();
  actionSave_file->setEnabled(true);
}

/** This function convert a point data from a *.txt line in a children point XML
 * valid*/
std::string ProjectUserInterface_Qt::pointTxtToXml2(QString point,
                                                    int key /*, int line*/,
                                                    std::string typePoint) {
  bool ok;
  std::stringstream aux;
  std::string gcpIdField, typeField, eField, nField, hField, dEField, dNField,
      dHField;
  QStringList fields = point.split("\t");
  int numImagesInPoint = fields.size() / 4 - 1;

  gcpIdField = fields.at(0).toStdString().c_str();
  eField = fields.at(1).toStdString();
  nField = fields.at(2).toStdString();
  hField = fields.at(3).toStdString();

  aux << "<point key=\"" << Conversion::intToString(key) << "\" type=\""
      << typePoint << "\">\n";
  aux << "<pointId>" << gcpIdField << "</pointId>\n";
  aux << "<description>" << "Put point description here" << "</description>\n";
  aux << "<spatialCoordinates uom=\"#" << "m" << "\">\n";
  aux << "<gml:pos>" << eField << " " << nField << " " << hField
      << "</gml:pos>\n";
  aux << "</spatialCoordinates>\n";
  aux << "<imagesMeasurements>\n";
  for (int i = 0; i < numImagesInPoint; i++) {
    std::string imagekey = fields.at(4 * (i + 1)).toStdString();

    int colValue = (fields.at(4 * (i + 1) + 2)).toInt(&ok);
    int linValue = (fields.at(4 * (i + 1) + 3)).toInt(&ok);

    aux << "<imageCoordinates uom=\"#px\" image_key=\"" << imagekey << "\">";
    aux << "<gml:pos>" << colValue << " " << linValue << "</gml:pos>";
    aux << "</imageCoordinates>\n";
  }

  aux << "</imagesMeasurements>\n";
  aux << "</point>\n";

  return aux.str();
}

void ProjectUserInterface_Qt::deleteEmptyPoints() {
  EDomElement points(manager->getXml("points"));
  std::deque<EDomElement> pnts = (points.elementsByTagName("point"));

  for (int i = 0; i < (int)pnts.size(); i++) {
    if (!pnts.at(i).hasTagName("imageCoordinates")) {
      manager->editComponent(
          "point", Conversion::stringToInt(pnts.at(i).attribute("key")), "");
    }
  }
}

void ProjectUserInterface_Qt::exportDigitalCoordinates() {
  QFileDialog salvar(this, tr("Export Digital Coordinates"), ".", "*.txt");
  salvar.setAcceptMode(QFileDialog::AcceptSave);
  salvar.setDefaultSuffix("txt");
  if (salvar.exec()) {
    QString fileSaveName = salvar.selectedFiles()[0];
    if (fileSaveName.isEmpty()) return;

    QFile *exportFileName = new QFile(fileSaveName);
    exportFileName->setFileName(fileSaveName);
    exportFileName->open(QIODevice::WriteOnly);

    EDomElement points(manager->getXml("points").c_str());

    QWidget *loadWidget = new QWidget();
    loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
    QProgressBar loading;
    loading.setRange(0, points.children().size());

    QVBoxLayout loadLayout;
    loadLayout.addWidget(&loading, Qt::AlignCenter);

    loadWidget->setLayout(&loadLayout);
    loadWidget->setWindowTitle(tr("Exporting Points"));
    loading.setMinimumSize(300, 30);
    loadWidget->show();
    std::deque<EDomElement> point = points.elementsByTagName("point");
    for (int i = 0; i < (int)point.size(); i++) {
      loading.setFormat(tr("Point %v/%m : %p%"));
      loading.setValue(i);
      exportFileName->write(
          edomDigitalCoordinatesPointToTxt(point.at(i)).data());
    }
    loadWidget->close();
    exportFileName->close();
  }
}

/** This function convert a EDomElement children Point in a line *.txt point
 * format
 */
std::string ProjectUserInterface_Qt::edomDigitalCoordinatesPointToTxt(
    EDomElement points) {
  std::stringstream aux;
  std::deque<EDomElement> digitalCoordinates =
      points.elementsByTagName("imageCoordinates");

  std::string pointId = points.elementByTagName("pointId").toString();
  EDomElement ede;
  QString gmlpos;
  bool ok;
  for (int i = 0; i < (int)digitalCoordinates.size(); i++) {
    ede = digitalCoordinates.at(i);
    gmlpos = ede.elementByTagName("gml:pos").toString().c_str();
    double col = (gmlpos.split(" ").at(0)).toDouble(&ok);
    double lin = (gmlpos.split(" ").at(1)).toDouble(&ok);
    aux << ede.attribute("image_key") << "\t" << pointId << "\t" << col << "\t"
        << lin << "\n";
  }

  std::string result = aux.str();
  return result.c_str();
}

void ProjectUserInterface_Qt::importImagesBatch() {
  QStringList importFilesName = QFileDialog::getOpenFileNames(
      this, "Select all images files", ".", "*.tif *.png *.bmp *.jpg");
  if (importFilesName.size() == 0) return;

  std::string xmlImages = "";

  QWidget *loadWidget = new QWidget();
  loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
  QProgressBar loading;
  loading.setRange(0, importFilesName.size());
  QVBoxLayout loadLayout;
  loadLayout.addWidget(&loading, Qt::AlignCenter);
  loadWidget->setLayout(&loadLayout);
  loadWidget->setWindowTitle(tr("Exporting Points"));
  loading.setMinimumSize(300, 30);

  QMessageBox msgBox;
  msgBox.setText(tr("Loading images"));
  msgBox.setInformativeText(
      tr("All images has the same dimensions? (Yes to fast import)"));
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);

  int resp = msgBox.exec();

  loadWidget->show();
  if (resp == QMessageBox::No) {
    int dpi = 300;
    for (int i = 0; i < importFilesName.size(); i++) {
      bool ok;
      dpi = QInputDialog::getInt(
          this, tr("Inform images dpi"),
          QString("dpi for image ").append(importFilesName.at(i)).append(":"),
          dpi, 0, 10000, 1, &ok);
      loading.setFormat(tr("Image %v/%m : %p%"));
      // Paulo: Quando for liberado para o usuario futuramente, o metodo ira
      // procurar uma key disponivel para a imagem, mesmo que as keys tenham
      // "buracos"
      xmlImages += addImageXml(importFilesName.at(i),
                               manager->getFreeImageId() + i, dpi);
      loading.setValue(i + 1);
    }
  } else {
    RasterResource firstImage(importFilesName.at(0),false,false,true); // true is load only metadata
    int imageWidth = firstImage.width();
    int imageHeight = firstImage.height();

    bool ok;
    int dpi =
        QInputDialog::getInt(this, tr("Inform images dpi"),
                             tr("dpi for all images:"), 300, 0, 10000, 1, &ok);
    for (int i = 0; i < importFilesName.size(); i++) {
      loading.setFormat(tr("Image %v/%m : %p%"));
      //  Paulo: Quando for liberado para o usuario futuramente, o metodo ira
      //  procurar uma key disponivel para a imagem, mesmo que as keys tenham
      //  "buracos"
      xmlImages +=
          addImageXml(importFilesName.at(i), manager->getFreeImageId() + i,
                      imageWidth, imageHeight, dpi);
      loading.setValue(i + 1);
    }
  }

  manager->addComponent(xmlImages, "images");

  loadWidget->close();
  updateTree();
  viewImages();
  actionSave_file->setEnabled(true);
}

std::string ProjectUserInterface_Qt::addImageXml(QString fileName, int keyImage,
                                                 int dpi) {
  std::stringstream imageXml;
  RasterResource image(fileName, false, false, true); // true is load only metadata

  const std::string imageFile = fileName.toUtf8().data();
  const std::string imageDirectory = PathUtils::parentDirectory(imageFile);
  const std::string projectDirectory = PathUtils::projectDirectory();

  // The project directory (not the project file) is the base of the relative
  // image paths, and an absolute path is used when both are on different
  // drives.
  std::string fileImagePath =
      PathUtils::relativeOrAbsolute(imageDirectory, projectDirectory);
  if (fileImagePath.empty()) fileImagePath = ".";
  const std::string targetFileName = PathUtils::fileName(imageFile);
  const std::string sugestionID = PathUtils::stem(imageFile);

  imageXml << "\t<image key=\"" << Conversion::intToString(keyImage)
           << "\" sensor_key=\"1\" flight_key=\"1\">\n";
  imageXml << "\t\t<imageId>" << PathUtils::xmlEscape(sugestionID)
           << "</imageId>\n";
  imageXml << "\t\t<width uom=\"#px\">"
           << Conversion::intToString(image.width()) << "</width>\n";
  imageXml << "\t\t<height uom=\"#px\">"
           << Conversion::intToString(image.height()) << "</height>\n";
  imageXml << "\t\t<fileName>" << PathUtils::xmlEscape(targetFileName)
           << "</fileName>\n";
  imageXml << "\t\t<filePath>" << PathUtils::xmlEscape(fileImagePath)
           << "</filePath>\n";
  imageXml << "\t\t<resolution uom=\"#dpi\">" << dpi << "</resolution>\n";
  imageXml << "\t</image>\n";

  return imageXml.str();
}

std::string ProjectUserInterface_Qt::addImageXml(QString fileName, int keyImage,
                                                 int widthImages,
                                                 int heightImages, int dpi) {
  std::stringstream imageXml;

  const std::string imageFile = fileName.toUtf8().data();
  const std::string imageDirectory = PathUtils::parentDirectory(imageFile);
  const std::string projectDirectory = PathUtils::projectDirectory();

  std::string fileImagePath =
      PathUtils::relativeOrAbsolute(imageDirectory, projectDirectory);
  if (fileImagePath.empty()) fileImagePath = ".";
  const std::string targetFileName = PathUtils::fileName(imageFile);
  const std::string sugestionID = PathUtils::stem(imageFile);

  imageXml << "\t<image key=\"" << Conversion::intToString(keyImage)
           << "\" sensor_key=\"1\" flight_key=\"1\">\n";
  imageXml << "\t\t<imageId>" << PathUtils::xmlEscape(sugestionID)
           << "</imageId>\n";
  imageXml << "\t\t<width uom=\"#px\">" << Conversion::intToString(widthImages)
           << "</width>\n";
  imageXml << "\t\t<height uom=\"#px\">"
           << Conversion::intToString(heightImages) << "</height>\n";
  imageXml << "\t\t<fileName>" << PathUtils::xmlEscape(targetFileName)
           << "</fileName>\n";
  imageXml << "\t\t<filePath>" << PathUtils::xmlEscape(fileImagePath)
           << "</filePath>\n";
  imageXml << "\t\t<resolution uom=\"#dpi\">" << dpi << "</resolution>\n";
  imageXml << "\t</image>\n";

  return imageXml.str();
}

void ProjectUserInterface_Qt::importOIDigitalMarks() {
  QString importFileName = QFileDialog::getOpenFileName(
      this, tr("Open Import OI Digital File"), ".", "*.txt");
  if (importFileName == "") return;
  QFile *importFile = new QFile(importFileName);
  QStringList marksList;

  importFile->open(QIODevice::ReadOnly);

  while (!importFile->atEnd()) {
    QByteArray line = importFile->readLine();
    QString aux(line);
    marksList << aux.remove('\n');
  }
  importFile->close();

  QWidget *loadWidget = new QWidget();
  loadWidget->setAttribute(Qt::WA_DeleteOnClose, true);
  QProgressBar loading;
  QPushButton cancelButton("Cancel");
  loading.setRange(0, marksList.size());

  QVBoxLayout loadLayout;
  loadLayout.addWidget(&loading, Qt::AlignCenter);
  loadLayout.addWidget(&cancelButton, Qt::AlignCenter);
  connect(&cancelButton, SIGNAL(clicked()), loadWidget, SLOT(close()));

  loadWidget->setLayout(&loadLayout);
  loadWidget->setWindowTitle(tr("Loading OIs"));
  loading.setMinimumSize(300, 30);
  loadWidget->show();

  std::string newOIXML;
  int imagescount = 1;
  for (int i = 0; i < marksList.length() && loadWidget != NULL; i += 4) {
    loading.setFormat(tr("%v/%m : %p%"));

    QStringList OIMarks;
    OIMarks << marksList.at(i) << marksList.at(i + 1) << marksList.at(i + 2)
            << marksList.at(i + 3);
    newOIXML += OIToXml(OIMarks, imagescount++);
    loading.setValue(i + 1);
  }

  manager->addComponent(newOIXML, "interiorOrientation");
  loadWidget->close();

  updateTree();
  viewPoints();
  actionSave_file->setEnabled(true);
}

std::string ProjectUserInterface_Qt::OIToXml(QStringList oiMarks,
                                             int imageKey) {
  std::stringstream OIxml;
  bool ok;
  // Paulo: Metodo Lusitano
  double markx1 = oiMarks.at(0).section('\t', 0, 0).toDouble(&ok);
  double marky1 = oiMarks.at(0).section('\t', 1, 1).toDouble(&ok);

  double markx2 = oiMarks.at(1).section('\t', 0, 0).toDouble(&ok);
  double marky2 = oiMarks.at(1).section('\t', 1, 1).toDouble(&ok);

  double markx3 = oiMarks.at(2).section('\t', 0, 0).toDouble(&ok);
  double marky3 = oiMarks.at(2).section('\t', 1, 1).toDouble(&ok);

  double markx4 = oiMarks.at(3).section('\t', 0, 0).toDouble(&ok);
  double marky4 = oiMarks.at(3).section('\t', 1, 1).toDouble(&ok);

  OIxml << "<imageIO type=\"Affine\" image_key=\""
        << Conversion::doubleToString(imageKey) << "\">\n";
  OIxml << "\t<fiducialMarks uom=\"#px\">\n";
  OIxml << "\t\t<fiducialMark key=\"1\">\n";
  OIxml << "\t\t\t<gml:pos>" << Conversion::doubleToString(markx1) << " "
        << Conversion::doubleToString(marky1) << "</gml:pos>\n";
  OIxml << "\t\t</fiducialMark>\n";
  OIxml << "\t\t<fiducialMark key=\"2\">\n";
  OIxml << "\t\t\t<gml:pos>" << Conversion::doubleToString(markx2) << " "
        << Conversion::doubleToString(marky2) << "</gml:pos>\n";
  OIxml << "\t\t</fiducialMark>\n";
  OIxml << "\t\t<fiducialMark key=\"3\">\n";
  OIxml << "\t\t\t<gml:pos>" << Conversion::doubleToString(markx3) << " "
        << Conversion::doubleToString(marky3) << "</gml:pos>\n";
  OIxml << "\t\t</fiducialMark>\n";
  OIxml << "\t\t<fiducialMark key=\"4\">\n";
  OIxml << "\t\t\t<gml:pos>" << Conversion::doubleToString(markx4) << " "
        << Conversion::doubleToString(marky4) << "</gml:pos>\n";
  OIxml << "\t\t</fiducialMark>\n";
  OIxml << "\t</fiducialMarks>\n";
  OIxml << "</imageIO>\n";

  return OIxml.str();
}

void ProjectUserInterface_Qt::updateLabelFileName() {
  if (actionSave_file->isEnabled())
    projectDockWidget->setWindowTitle(QString(tr("Open Project: *")) +
                                      headerForm.lineEditFileName->text());
  else
    projectDockWidget->setWindowTitle(QString(tr("Open Project: ")) +
                                      headerForm.lineEditFileName->text());
}

void ProjectUserInterface_Qt::loadLastProject() {
  QSettings efotoSettings("uerj", "efoto");
  const QString filename = efotoSettings.value("lastProject").toString();
  loadFile(filename.toUtf8().data());
}

void ProjectUserInterface_Qt::updatePath() {
  saveSettings("binPath", PathUtils::absolute("."));
}

void ProjectUserInterface_Qt::saveSettings(std::string setting,
                                           std::string value) {
  QSettings efotoSettings("uerj", "efoto");
  efotoSettings.setValue(QString::fromStdString(setting), QString::fromStdString(value));
}

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
