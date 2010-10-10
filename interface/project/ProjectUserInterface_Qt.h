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

#include <QList>
#include <QModelIndex>
#include <QStackedWidget>
#include <QScrollArea>

/*class TreeItem
{
public:
	TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
	~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	TreeItem *parent();

private:
	QList<TreeItem*> childItems;
	QList<QVariant> itemData;
	TreeItem *parentItem;
};

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	TreeModel(const QString &data, QObject *parent = 0);
	~TreeModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
					  const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	void setupModelData(const QStringList &lines, TreeItem *parent);

	TreeItem *rootItem;
};*/

class ProjectUserInterface_Qt : public QMainWindow, public Ui::ProjectMainWindow, public ProjectUserInterface
{
	Q_OBJECT

public:
	ProjectUserInterface_Qt(ProjectManager* manager, QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
	~ProjectUserInterface_Qt();

protected:
	//TreeModel* treeModel;
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
	string savedIn;

protected slots:
	virtual void languageChange();
	virtual void newTree();
	virtual void updateTree();
	virtual void validatingSensor();
	virtual void validatingFlight();
	virtual void validatingImage();
	virtual void validatingPoint();

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
	//void updateView(AbstractForm& newView);

public slots:
	virtual void newProject();
	virtual void loadFile();
	virtual void saveFile();
	virtual void saveFileAs();
	virtual void executeIO();
	virtual void executeSR();
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
};

#endif // PROJECTUSERINTERFACE_QT_H
