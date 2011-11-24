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
    ortho = NULL;
}

OrthoManager::~OrthoManager()
{
    if (grid != NULL)
        delete grid;

    if (ortho != NULL)
        delete ortho;
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

    // Report grid resolution
    double Xi, Yi, Xf, Yf, res_x, res_y;
    grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
    OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
    oui->doubleSpinBox1->setValue(res_x);
    oui->doubleSpinBox2->setValue(res_y);

    return 1;
}

void OrthoManager::runOrthoIndividual(int image)
{
    if (image < 1 || image > listAllImages.size())
        return;

    // Get image data
    Image *img = listAllImages.at(image-1);
    unsigned int img_width, img_height;
    img_width = img->getWidth();
    img_height = img->getHeight();
/*
    // 3D coordinates
    SpatialIntersection spc_inter;
    Image *left;
    ObjectSpaceCoordinate object;
    left = listAllImages.at(image - 1);
    sp.setLeftImage(left);
    sp.setRightImage(left); // Ok, left is used as a pair
    spc_inter.setStereoPair(&sp);
*/
    // Calculate image bounding box (check whole DEM where is inside image)
    double meanZ = grid->getMeanZ();
    double Xi, Yi, Xf, Yf, res_x, res_y;
    double Xi2, Yi2, Xf2, Yf2;
    Xi2 = Xf; Yi2 = Yf; Xf2 = Xi; Yf2 = Yi;

    grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
    for (double Y=0; Y<Yf; Y+=res_y)
    {
        for (double X=0; X<Xf; X+=res_x)
        {
  //          object = spc_inter.getLeftSpatialResection(X, Y, meanZ);

        }
    }

    // Load image as matrix
    OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
    string filename = img->getFilepath() + "/" + img->getFilename();
    oui->loadImage((char *)filename.c_str(),1.0);

}

int OrthoManager::orthoRectification(char * filename, int fileType, int option, double user_res_x, double user_res_y)
{
    // Create new orthoimage
    if (ortho != NULL)
        delete ortho;

    double Xi, Yi, Xf, Yf, res_x, res_y;
    grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);

    ortho = new Orthorectification(Xi, Yi, Xf, Yf, user_res_x, user_res_y);
/*
    if (option == 0)
        runOrthoAllTogheter();

    if (option == 1)
    {
        for (int i=1; i<=listAllImages.size(); i++)
            runOrthoIndividual(i);
    }
*/
    if (option > 1)
        runOrthoIndividual(option-1);

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
