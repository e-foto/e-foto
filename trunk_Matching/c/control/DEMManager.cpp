/**************************************************************************
                                                           DEMManager.cpp
**************************************************************************/


#include "EDom.h"
#include "Point.h"
#include "DEMManager.h"
#include "EFotoManager.h"
#include "DEMUserInterface.h"
#include "DEMUserInterface_Qt.h"


// Constructors and destructors
//

DEMManager::DEMManager()
{
    started = false;
    status = false;
}

DEMManager::DEMManager(EFotoManager* manager, deque<Image*>images, deque<ExteriorOrientation*> eos)
{
    this->manager = manager;
    started = false;
    status = false;
    img1 = NULL;
    img2 = NULL;
    listAllImages = images;
    listEOs = eos;
}

DEMManager::~DEMManager()
{

}

// Association Methods
//

void DEMManager::setInterface(DEMUserInterface* newInterface)
{
    myInterface = newInterface;
}

DEMUserInterface* DEMManager::getInterface()
{
    return myInterface;
}

// Other Methods
//

bool DEMManager::exec()
{
    if (manager != NULL)
    {
        if (manager->getInterfaceType().compare("Qt") == 0)
        {
            //myInterface = new IOUserInterface_Qt(this);
            myInterface = DEMUserInterface_Qt::instance(this);
        }
        connectImagePoints();
        createInitialSeeds();
        started = true;
        if (myInterface != NULL)
        {
            myInterface->exec();
            getPairs();
        }
    }
    return status;
}

void DEMManager::returnProject()
{
    manager->reloadProject();
}

void DEMManager::getPairs()
{
    //for (int i=0; i < manager->EOs.size())
    printf("Images: %d\n",listAllImages.size());
    printf("EO: %d\n",listEOs.size());

}

bool DEMManager::connectImagePoints()
{
        if (!(started))
        {
                qApp->processEvents();
                EDomElement xmlPoints(manager->getXml("points"));
                deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
                for (unsigned int j = 0; j < listAllImages.size(); j++)
                {
                        //listAllImages.at(j)->clearPoints();
                        for (unsigned int i = 0; i < allPoints.size(); i++)
                        {
                            string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key", Conversion::intToString(listAllImages.at(j)->getId())).getContent();
                                //qDebug("%s\n",data.c_str());
                                if (data != "")
                                {
                                        Point* pointToInsert = manager->instancePoint(Conversion::stringToInt(allPoints.at(i).attribute("key")));
                                        if (pointToInsert != NULL)
                                        {
                                                //qDebug("connectImagePoints(): colocou um ponto: %s",pointToInsert->getPointId().c_str());
                                                listAllImages.at(j)->putPoint(pointToInsert);
                                                pointToInsert->putImage(listAllImages.at(j));//novo em teste:06/08/2011
                                        }
                                }
                        }
                        //qDebug("Image %s",listAllImages.at(j)->getFilename().c_str());
                        //listAllImages.at(j)->sortPoints();
                }
                //qDebug("\n\n");
                return true;
        }
        return false;
        printf("Ok\n");
}

void DEMManager::createInitialSeeds()
{
    // Clear seeds list before adding measured point as seeds
    seeds.clear();

    MatchingPoints mp;
    Point *p;
    deque<DigitalImageSpaceCoordinate> Dic;

    printf("Pontos: %d\n",listAllPoints.size());
    for (int i=0; i<listAllPoints.size(); i++)
    {
        p = listAllPoints.at(i);
        printf("Ponto %d, imagens: %d\n",i,p->countImages());
//        Dic = p->getDigitalCoordinate(listAllImages.at(i));


    }
/*
    Point *pnt=efotoManager->instancePoint(pointKey);
    int numImages=pnt->countImages();
    for(int i=0;i<numImages;i++)
            appearance.push_back(pnt->getImageAt(i)->getFilename());
*/

}

/*
 * DEM extraction
 **/

void DEMManager::resamplePoints(MatchingPointsList *list, double resample)
{
        MatchingPoints *mp;
        for (int i=1; i<=list->size(); i++)
        {
                mp = list->get(i);
                mp->left_x = mp->left_x * resample;
                mp->left_y = mp->left_y * resample;
                mp->right_x = mp->right_x * resample;
                mp->right_y = mp->right_y * resample;
        }
}


