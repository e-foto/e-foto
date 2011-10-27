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
	EDomElement pointTxtToXml(QString point, int key, int line, string typePoint="control");
	string edomPointToTxt(EDomElement points);

	EDomElement imageTxtToXml(QString image, int key, int line, int sensorKey=0, int flightKey=0);
	string edomImageToTxt(EDomElement images);


public slots:
	virtual void closeEvent(QCloseEvent *event);
	virtual void newProject();
	virtual void loadFile(string filenameAtStart = "");
	virtual void saveFile();
	virtual bool saveFileAs(bool onNewProject = false);
        virtual void executeIO();
        virtual void executeSR();
        virtual void executeDEM();
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
	//void exportImagesToTxt();

};

#endif // PROJECTUSERINTERFACE_QT_H
