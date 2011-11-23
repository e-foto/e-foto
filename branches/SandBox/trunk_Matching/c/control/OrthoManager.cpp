/**************************************************************************
                                                           OrthoManager.cpp
**************************************************************************/


#include "EDom.h"
#include "Point.h"
#include "OrthoManager.h"
#include "EFotoManager.h"
#include "OrthoUserInterface.h"
#include "OrthoUserInterface_Qt.h"


// Constructors and destructors
//

OrthoManager::OrthoManager()
{
    started = false;
    status = false;
}

OrthoManager::OrthoManager(EFotoManager* manager, deque<Image*>images, deque<ExteriorOrientation*> eos)
{
    this->manager = manager;
    started = false;
    status = false;
    listAllImages = images;
    listEOs = eos;
    grid = NULL;
}

OrthoManager::~OrthoManager()
{
    if (grid != NULL)
        delete grid;
}

// Association Methods
//

void OrthoManager::setInterface(OrthoUserInterface* newInterface)
{
    myInterface = newInterface;
}

OrthoUserInterface* OrthoManager::getInterface()
{
    return myInterface;
}

// Other Methods
//

bool OrthoManager::exec()
{
    if (manager != NULL)
    {
        if (manager->getInterfaceType().compare("Qt") == 0)
        {
            myInterface = OrthoUserInterface_Qt::instance(this);
        }
        started = true;
        if (myInterface != NULL)
        {
            myInterface->exec();
            addImagesToForm();
        }
    }
    return status;
}

void OrthoManager::addImagesToForm()
{
    OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;

    for (int i=1; i<=listAllImages.size(); i++)
            oui->comboBox->addItem("Image "+QString::number(i));
}

int OrthoManager::loadDemGrid(char * filename, int fileType)
{
    if (grid != NULL)
        delete grid;

    // Create custom grid. Load will fix these values.
    grid = new DemGrid(1.0, 2.0, 1.0, 2.0, 1.0, 1.0);

    grid->loadDem(filename,fileType);
    grid->printData();

    return 1;
}

int OrthoManager::orthoRectification(char * filename, int fileType)
{
    if (!loadDemGrid(filename, fileType))
        return -1;
}

void OrthoManager::returnProject()
{
    manager->reloadProject();
}

void OrthoManager::setProgress(int progress)
{
    OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
    oui->setProgress(progress);
}
