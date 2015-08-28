#ifndef PROJECTUSERINTERFACE_QT_H
#define PROJECTUSERINTERFACE_QT_H
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
#include "ui_FormProject.h"
#include "ProjectUserInterface.h"

#include "HeaderForm.h"
#include "TerrainForm.h"
#include "SensorForm.h"
#include "FlightForm.h"
#include "ImagesForm.h"
#include "ImageForm.h"
#include "PointsForm.h"
#include "PointForm.h"
#include "ControlButtons.h"

#include <QStackedWidget>


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectUserInterface_Qt : public QMainWindow, public Ui::ProjectMainWindow, public ProjectUserInterface
{
    Q_OBJECT

public:
    ProjectUserInterface_Qt(ProjectManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~ProjectUserInterface_Qt();

protected:
    QList<QTreeWidgetItem*> treeItems;
    int currentItemId;
    QLabel offset;
    QStackedWidget centerArea;
    AbstractForm* currentForm;
    HeaderForm headerForm;
    TerrainForm terrainForm;
    SensorForm sensorForm;
    FlightForm flightForm;
    ImagesForm imagesForm;
    ImageForm imageForm;
    PointsForm pointsForm;
    PointForm pointForm;
    ControlButtons controlButtons;
    QModelIndex currentIndex;
    std::string savedIn;
    bool editState; bool addNewState; bool changeModule;

protected slots:
    virtual void languageChange();
    virtual void newTree();
    virtual void updateTree();
    virtual void validatingSensor();
    virtual void validatingFlight();
    virtual void validatingImage();
    virtual void validatingPoint();
    virtual bool confirmToClose();

public:

    // Other Methods
    bool exec();

    void viewHeader();
    void viewTerrain();
    void viewFlights();
    void viewFlight(int id);
    void viewSensors();
    void viewSensor(int id);
    void viewPoints();
    void viewPoint(int id);
    void viewImages();
    void viewImage(int id);
    QString getSavedIn();
    std::string pointTxtToXml(QString point, int key, int line, std::string typePoint="control");
    std::string edomPointToTxt(EDomElement points);

    std::string pointTxtToXml2(QString point, int key/*, int line*/, std::string typePoint="photogrammetric");



    EDomElement imageTxtToXml(/*QString image, int key, int line, int sensorKey=0, int flightKey=0*/);
    std::string edomImageToTxt(EDomElement images);

    bool insertDigitalCoordinates(QString coordinates);
    bool availableSR();
    bool availablePhotoTri();
    bool availableStereoPlotter();
    bool availableDemExtraction();
    bool availableOrthoImage();
    bool availableLP();
    void updateLabelFileName();


public slots:
    virtual void closeEvent(QCloseEvent *event);
    virtual void newProject();
    virtual void loadFile(std::string filenameAtStart = "");
    virtual void loadLastProject();
    virtual void saveFile();
    virtual bool saveFileAs(bool onNewProject = false);
    virtual void executeIO();
    virtual void executeSR();
    virtual void executeFT();
    virtual void executeDEM();
    virtual void executeOrtho();
    virtual void executeSP();
    virtual void executeReport();
    virtual void executePTReport();
    virtual void processTreeClick(QModelIndex);
    virtual void exportSPFile();

    virtual void selectImage(int);
    virtual void selectPoint(int);
    virtual void enableForm();
    virtual void enableExecuteMenu(bool status);
    // Salvando cada view...
    virtual void saveHeader();
    virtual void saveTerrain();
    virtual void saveFlight();
    virtual void saveSensor();
    virtual void savePoint();
    virtual void saveImage();
    // Cancelando cada view...
    virtual void cancelHeader();
    virtual void cancelTerrain();
    virtual void cancelFlight();
    virtual void cancelSensor();
    virtual void cancelPoint();
    virtual void cancelImage();
    virtual void cancelFlights();
    virtual void cancelSensors();
    virtual void cancelPoints();
    virtual void cancelImages();
    // Criando um vazio de cada view...
    virtual void newFlight();
    virtual void newSensor();
    virtual void newPoint();
    virtual void newImage();
    // Salvando um novo de cada view...
    virtual void saveNewFlight();
    virtual void saveNewSensor();
    virtual void saveNewPoint();
    virtual void saveNewImage();
    // Apagando cada view...
    virtual void deleteFlight();
    virtual void deleteSensor();
    virtual void deletePoint();
    virtual void deleteImage();
    // Debug...
    virtual void toggleDebug();
    virtual void showAbout();

    void importPointsFromTxt();
    void exportPointsToTxt();
    void importImagesBatch();

    void importImagesFromTxt();
    void importPointsFromTxt2();

    std::string addImageXml(QString fileName, int keyImage, int dpi = 0);
    std::string addImageXml(QString fileName, int keyImage, int widthImages, int heightImages, int dpi = 0 );
    void importOIDigitalMarks();
    std::string OIToXml(QStringList oiMarks, int imageKey);

    void importDigitalCoordinatesFromTxt();
    void updateCurrentForm();
    void deleteEmptyPoints();

    void exportDigitalCoordinates();
    std::string edomDigitalCoordinatesPointToTxt(EDomElement points);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECTUSERINTERFACE_QT_H
