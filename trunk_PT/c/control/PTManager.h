#ifndef PTMANAGER_H
#define PTMANAGER_H

#include "EObject.h"
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
	bool started;
	bool status;
        EFotoManager* efotoManager;
    PTUserInterface* myInterface;

        Sensor *mySensor;
        Flight *myFlight;
        deque<Image*> listImages;
        deque<Point*> listPoints;
        deque<InteriorOrientation*> listOis;

public:
    PTManager();
        //como as imagens contens os pontos não é necessário passar os pontos
        PTManager(EFotoManager* newManager, deque<Image*> images, deque<InteriorOrientation*> ois, Sensor* sensor,Flight* flight);
    ~PTManager();

    string getImagefile(int imageId);



    void setInterface(PTUserInterface* newInterface);
    PTUserInterface* getInterface();
	bool exec();
	void returnProject();

};

#endif // PTMANAGER_H
