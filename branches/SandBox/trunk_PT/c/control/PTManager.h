#ifndef PTMANAGER_H
#define PTMANAGER_H

#include "EObject.h"
#include <deque>
#include "BundleAdjustment.h"
#include "Matrix.h"
#include <QStringList>
#include <deque>


class PTUserInterface;
class EFotoManager;


class Point;
class Image;
class Sensor;
class Flight;
class InteriorOrientation;

//FotoTri Manager precisa conhecer: os pontos, as imagens, alguns dados do sensor e os parametros da OI de todas as imagens
class PTManager
{
protected:
	bool started;
	bool status;
	EFotoManager* efotoManager;
	PTUserInterface* myInterface;

	Sensor *mySensor;
	Flight *myFlight;
	deque<Image*> listAllImages;
	deque<Point*> listAllPoints;
	deque<InteriorOrientation*> listOis;

	deque<Image*> listSelectedImages;
	deque<Point*> listSelectedPoints;

	BundleAdjustment* pt;
	Matrix ENH,AFP;

public:
    PTManager();
    //como as imagens contens os pontos não é necessário passar os pontos
	PTManager(EFotoManager* newManager, deque<Image*> images, deque<InteriorOrientation*> ois, Sensor* sensor);//,Flight* flight);
    ~PTManager();

    string getImagefile(int imageId);

    void setInterface(PTUserInterface* newInterface);
    PTUserInterface* getInterface();
    bool calculatePT();
    bool exec();
    void returnProject();
    void setMatrixAFP(Matrix afp);
	Matrix getMatrixOE();
    Matrix getResiduos();
	void setENH();
	Matrix getENH();


	Matrix getColLin(string imageFilename);
	bool connectImagePoints();
	void setListPoint();

	deque<string> getStringKeysPoints(string imageFileName="");
	deque<string> getStringTypePoints(string imageFileName="");
	deque<string> getStringIdPoints(string imageFileName="");
	deque<string> getStringImages();

	void selectPoints(deque<string> selectedPointsList);
	void selectImages(deque<string> selectedImagesList);
	// metodo para dar um update nas coordenadas digitais do pont
	void updateDigitalCoordinatesPoint(int imageKey,int pointKey,int col, int lin);
	int getImageId(string imageFilename);
	PositionMatrix getImageDimensions(string filename);


	BundleAdjustment* getBundleAdjustment();
	void sortPointsSelected();

	Matrix getPhotogrammetricENH();
	deque<string> getSelectedPointIdPhotogrammetric();
protected:

};

#endif // PTMANAGER_H
