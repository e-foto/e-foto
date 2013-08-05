#ifndef PROJECTUSERINTERFACE_QT_H
#define PROJECTUSERINTERFACE_QT_H

#include "ETreeModel.h"
#include "EDom.h"

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
#include "AboutForm.h"
#include "ControlButtons.h"
#include "LoadingScreen.h"

#include <QList>
#include <QModelIndex>
#include <QStackedWidget>
#include <QScrollArea>
#include <QFile>


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
	string savedIn;
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
	//
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
	string pointTxtToXml(QString point, int key, int line, string typePoint="control");
	string edomPointToTxt(EDomElement points);

	string pointTxtToXml2(QString point, int key, int line, string typePoint="photogrammetric");



	EDomElement imageTxtToXml(QString image, int key, int line, int sensorKey=0, int flightKey=0);
	string edomImageToTxt(EDomElement images);

	// Inserido pelo Paulo 05/09/2011
	//insere as coordenadas digitais no ponto
	bool insertDigitalCoordinates(QString coordinates);
        bool availableSR();
	bool availablePhotoTri();
        bool availableStereoPlotter();
        bool availableDemExtraction();
        bool availableOrthoImage();
	void updateLabelFileName();
	//bool availabeOI();
	//bool availableOE();


public slots:
	virtual void closeEvent(QCloseEvent *event);
	virtual void newProject();
	virtual void loadFile(string filenameAtStart = "");
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
	void importImagesFromTxt();
	void importPointsFromTxt2();

	/*Paulo importar imagens em batch 26/01/12*/
	void importImagesBatch();
    string addImageXml(QString fileName, int keyImage, int dpi = 0);
    string addImageXml(QString fileName, int keyImage, int widthImages, int heightImages, int dpi = 0 );

	/*Paulo importar Ois em batch 26/01/12*/
	void importOIDigitalMarks();
	string OIToXml(QStringList oiMarks, int imageKey);
	//void exportImagesToTxt();

	void importDigitalCoordinatesFromTxt();

	void updateCurrentForm();
	void deleteEmptyPoints();


	void exportDigitalCoordinates();
	string edomDigitalCoordinatesPointToTxt(EDomElement points);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECTUSERINTERFACE_QT_H
