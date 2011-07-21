#ifndef PTMANAGER_H
#define PTMANAGER_H

#include "EObject.h"
#include <deque>
#include "BundleAdjustment.h"
#include "Matrix.h"

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
        deque<Image*> listImages;
        deque<Point*> listPoints;
        deque<InteriorOrientation*> listOis;

        BundleAdjustment* pt;
        Matrix AFP;

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
    Matrix getMatrixAFP();

    /**serao inutilizados*/
    Matrix getMatrixA1();
    Matrix getMatrixA2();
    Matrix getMatrixN11();
    Matrix getMatrixN12();
    Matrix getMatrixN22();
    Matrix getMatrixL0();
    Matrix getMatrixLb();


protected:
	Matrix Col,Lin,ENH, BLC;

	void setENH();
	void setColLin();
	void setBLC();

	Matrix getCol();
    Matrix getLin();

	Matrix getBLC();
    Matrix getX();
    Matrix getY();
    Matrix getZ();

	Matrix getOis();


};

#endif // PTMANAGER_H
