#include "PTManager.h"
#include "EFotoManager.h"
#include "PTUserInterface_Qt.h"

#include "EDom.h"
#include "Point.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "SensorWithFiducialMarks.h"

PTManager::PTManager()
{
        efotoManager  = NULL;
        mySensor = NULL;
        myFlight = NULL;

	started = false;
	status = false;
}

PTManager::PTManager(EFotoManager *newManager, deque<Image*>images, deque<InteriorOrientation*> ois,Sensor *sensor, Flight *flight)
{
        efotoManager = newManager;
        listImages = images;
        listOis = ois;
        mySensor = sensor;
        myFlight = flight;

	started = false;
	status = false;
}

PTManager::~PTManager()
{

}

void PTManager::setInterface(PTUserInterface *newInterface)
{
	myInterface = newInterface;
}

PTUserInterface* PTManager::getInterface()
{
	return myInterface;
}

bool PTManager::exec()
{
        if (efotoManager != NULL && mySensor != NULL && myFlight != NULL && listImages.size()> 1 && listOis.size()>1)
	{
                if (efotoManager->getInterfaceType().compare("Qt") == 0)
		{
                    myInterface = PTUserInterface_Qt::instance(this);
		}
		started = true;

                myFlight->setSensor(mySensor);
                for (int i=0; i<listImages.size(); i++)
                {
                    listImages.at(i)->setSensor(mySensor);
                    listImages.at(i)->setFlight(myFlight);
                    listImages.at(i)->setIO(listOis.at(i));
                    //listOis.at(i)->setImage(listImages.at(i));
                }

		if (myInterface != NULL)
		{
			myInterface->exec();
		}
	}
	return status=true;
}

void PTManager::returnProject()
{
    //if (efotoManager->is("EFotoManager"))
        efotoManager->reloadProject();
    //else
      //  qDebug("nao e efotomanager");
}

string PTManager::getImagefile(int imageId)
{
    //Image *img;
    //img->getFilename();
    string imagefile=listImages.at(imageId)->getFilepath();
    imagefile +="/";
    imagefile +=listImages.at(imageId)->getFilename();

    return imagefile;
}
