#ifndef PTMANAGER_H
#define PTMANAGER_H

#include "EObject.h"
#include "BundleAdjustment.h"
#include "Matrix.h"
#include "ConvertionsSystems.h"
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
	bool marksSaveState;
	bool previousData;

	int maxIterations;
	double metricConvergency;
	double angularConvergency;

	EFotoManager* efotoManager;
	PTUserInterface* myInterface;

	Sensor *mySensor;
	Flight *myFlight;
	double flightScale;
	/***/
	deque<Image*> listAllImages;
	deque<Point*> listAllPoints;
	deque<InteriorOrientation*> listOis;

	deque<Image*> listSelectedImages;
	deque<Point*> listSelectedPoints;

	BundleAdjustment* pt;
	Matrix ENH,spareENH,AFP,MVC,residuos;

public:
    PTManager();
    //como as imagens contens os pontos não é necessário passar os pontos
	PTManager(EFotoManager* newManager, deque<Image*> images, deque<InteriorOrientation*> ois, Sensor* sensor,Flight* flight);
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
	Matrix getMVC();

	Matrix getColLin(string imageFilename);
	Matrix getColLin(int imageKey);
	bool connectImagePoints();
	void setListPoint();

	deque<string> getStringKeysPoints(string imageFileName="");
	deque<string> getStringTypePoints(string imageFileName="");
	deque<string> getStringIdPoints(string imageFileName="",string cond1="");
	deque<string> getStringImages();

	void selectPoints(deque<string> selectedPointsList);
	void selectImages(deque<string> selectedImagesList);

	// metodo para dar um update nas coordenadas digitais do pont
	void updateDigitalCoordinatesPoint(int imageId,int pointKey,int col, int lin);

	int getImageId(string imageFilename);

	//PositionMatrix getImageDimensions(string filename);


	BundleAdjustment* getBundleAdjustment();
	void sortPointsSelected();

	Matrix getPhotogrammetricENH();
	Matrix getResiduoPhotogrammetric();
	deque<string> getSelectedPointIdPhotogrammetric();

	string getFilePath(string fileName);
	string getFilePath(int imageKey);

	void saveResults();
	void saveBundleAdjustment();
	void saveMarks();
	void setMarksSavedState(bool marksState);
	bool getMarksSavedState();
	string createBundleAdjustmentXml();
	deque<string> getImagesAppearances(int pointKey);
	void loadFotoTriData(string fotoTriData);
	bool hasPreviousData();

	void setImageFlightDirection(string imageFile,double flightDirection);
	void setImageFlightDirection(int imageKey,double flightDirection);

	double getLongitudinalOverlap(string imageFile);
	double getLongitudinalOverlap(int imageKey);
	double getImageFlightDirection(string imageFile);
	double getImageFlightDirection(int imageKey);
	bool isAvailablePoint(int imageId, int pointKey);

	string exportBlockTokml(string fileName);
	string pointToKml(Point *pnt, int zona,GeoSystem sys ,char hemiLatitude,string pointType);
	double getFlightScale();

	void setMaxIteration(int iterations);
	int getMaxIteration();

	void setMetricConvergencyValue(double value);
	double getMetricConvergencyValue();

	void setAngularConvergencyValue(double value);
	double getAngularConvergencyValue();

	void reloadPointsCoordinates();

	void photogrammetricSort();

	// Retorna a coordenada aproximada EN de um ponto na imagem, desde de que a Fototri tenha sido executada com sucesso.
	int whereInSelectedImages(Image *img);
	int whereInImages(Image *img);
	Matrix digitalToEN(Image *img,int col, int row, Matrix oe);

	Matrix getPointResiduo(int pointKey);

	deque<string> getPointsWithLesserThanOverlap(int overlap);

	void createNewPoint();
	void connectPointInImage(int pointKey, int imageKey);

};

#endif // PTMANAGER_H
