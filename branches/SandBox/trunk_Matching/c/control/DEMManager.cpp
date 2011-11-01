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
    setListPoint();
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
            myInterface = DEMUserInterface_Qt::instance(this);
        }
        connectImagePoints();
        started = true;
        if (myInterface != NULL)
        {
            myInterface->exec();
            getPairs();
            createInitialSeeds();
        }
    }
    return status;
}

void DEMManager::returnProject()
{
    manager->reloadProject();
}

void DEMManager::setListPoint()
{
        EDomElement xmlPoints(manager->getXml("points"));
        deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
        for(int i=0;i<allPoints.size();i++)
        {
            Point* point= manager->instancePoint(Conversion::stringToInt(allPoints.at(i).attribute("key")));
                if (point != NULL)// && !point->is("CheckingPoint"))
                        listAllPoints.push_back(point);
        }
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
}

void DEMManager::getPairs()
{
    //
    // List Pairs description (0 - N-1):
    //
    // num = left + no_imgs*right // Encoding
    // left = num % no_imgs // Decoding
    // right =  num / no_imgs // Decoding

    //for (int i=0; i < manager->EOs.size())
 //   printf("Images: %d\n",listAllImages.size());
//    printf("EO: %d\n",listEOs.size());

    // Criado na mão!! 0-1 e 1-2
    listPairs.clear();
    listPairs.push_back(3);
    listPairs.push_back(7);
}

/*
 * Seeds
 **/

void DEMManager::createInitialSeeds()
{
    // Clear seeds list before adding measured point as seeds
    seeds.clear();

    MatchingPoints mp;
    Point *p;
    Image *img;
    int no_imgs;
    deque<DigitalImageSpaceCoordinate> Dic;

    printf("Pontos: %d\n",listAllPoints.size());
    for (int i=0; i<listAllPoints.size(); i++)
    {
        p = listAllPoints.at(i);
        no_imgs = p->countImages();

        if (no_imgs < 2)
            continue;

        printf("Ponto %d, imagens: %d - ",i,no_imgs);

        for (int j=0; j<no_imgs; j++)
        {
            img = p->getImageAt(j);
            printf("%d ",img->getId());
        }

        printf("\n");

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


