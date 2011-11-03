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

Image * DEMManager::getImage(int id)
{
    for (int i=0; i<listAllImages.size(); i++)
    {
        if (listAllImages.at(i)->getId() == id)
            return listAllImages.at(i);
    }

    return NULL;
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

    // Criado na mão!! 0-1 e 1-2
    listPairs.clear();
    listPairs.push_back(3);
    listPairs.push_back(7);

    // Add pairs to the interface
    int left_id, right_id;
    stringstream txt;
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    for (int i=0; i<listPairs.size(); i++)
    {
        getImagesId(i,left_id,right_id);
        txt.str(""); // Clear stream
        txt << "Images " << left_id << " and " << right_id;
        dui->addImagePair((char *)txt.str().c_str());
    }
    dui->setStatus((char *)"None");
}

// Internal function. Pos from 0 - N-1.
void DEMManager::getImagesId(int pos, int &left, int &right)
{
    // Check pos
    left = -1;
    right = -1;
    if (pos < 0 || pos > listPairs.size()-1)
        return;

    // Decode
    int no_imgs = listAllImages.size();
    left = 1 + (listPairs.at(pos) % no_imgs);
    right = 1 + (listPairs.at(pos) / no_imgs);
}

void DEMManager::setAutoExtractionSettings(int _rad_cor, int _match_method, int _rgx, int _rgy)
{
    rad_cor = _rad_cor;
    match_method = _match_method;
    rgx = _rgx;
    rgy = _rgy;

//    ncc_temp, ncc_sw;
//    double ncc_th, ncc_std;

}

void DEMManager::setNCCSettings(int _ncc_temp, int _ncc_sw, double _ncc_th, double _ncc_std)
{
    ncc_temp = _ncc_temp;
    ncc_sw = _ncc_sw;
    ncc_th = _ncc_th;
    ncc_std = _ncc_std;
}

void DEMManager::setLSMSettings(int _lsm_temp, int _lsm_it, double _lsm_th, double _lsm_std, int _lsm_dist, double _lsm_shift, double _lsm_shear, double _lsm_scale)
{
    lsm_temp = _lsm_temp;
    lsm_it = _lsm_it;
    lsm_th = _lsm_th;
    lsm_std = _lsm_std;
    lsm_dist = _lsm_dist;
    lsm_shift = _lsm_shift;
    lsm_shear = _lsm_shear;
    lsm_scale = _lsm_scale;
}

/*
 * Seeds
 **/

void DEMManager::createInitialSeeds()
{
    // Clear seeds list before adding measured point as seeds
    seeds.clear();

    Point *p;
    int no_imgs_pt, left_id, right_id;
    DigitalImageSpaceCoordinate left_dic, right_dic;

    for (int i=0; i<listAllPoints.size(); i++)
    {
        p = listAllPoints.at(i);
        no_imgs_pt = p->countImages();

        // There is no pair, if points # is less than 2
        if (no_imgs_pt < 2)
            continue;

        // Read pair list
        for (int k=0; k<listPairs.size(); k++)
        {
            getImagesId(k, left_id, right_id);

            if (!p->hasDigitalCoordinate(left_id) || !p->hasDigitalCoordinate(right_id))
                continue;

            left_dic = p->getDigitalCoordinate(left_id);
            right_dic = p->getDigitalCoordinate(right_id);

            seeds.add(left_id, right_id, double(left_dic.getCol()), double(left_dic.getLin()), double(right_dic.getCol()), double(right_dic.getLin()), 0.0);
        }
    }
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

void DEMManager::extractDEM(int option)
{
    if (option==0)
    {
        for (int i=0; i<listPairs.size(); i++)
            extractDEMPair(i);;
    }
    else
        extractDEMPair(option-1);


}

void DEMManager::extractDEMPair(int pair)
{
    if (pair < 0 || pair > listPairs.size()-1)
        return;

    //
    // Get images IDs
    //
    int left_id,right_id;
    getImagesId(pair, left_id, right_id);

    //
    // Load images
    //
    double sample=1.0;
    Matrix *img1 = NULL, *img2 = NULL;
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setStatus((char *)"Loading left image ...");
    string filename = getImage(left_id)->getFilepath() + "/" + getImage(left_id)->getFilename();
    img1 = dui->loadImage((char *)filename.c_str(), sample);
    dui->setStatus((char *)"Loading right image ...");
    filename = getImage(right_id)->getFilepath() + "/" + getImage(right_id)->getFilename();
    img2 = dui->loadImage((char *)filename.c_str(), sample);

    if (img1 == NULL || img2 == NULL)
        return;

    //
    // Start matching
    //

    stringstream txt;
    txt << "Matching pair " << left_id << " and " << right_id;
    dui->setStatus((char *)txt.str().c_str());
    dui->setProgress(0);

    double corr_th = lsm_th, std = lsm_std;
    if (match_method==0) // NCC
    {
        corr_th = ncc_th;
        std = ncc_std;
    }

    ImageMatching im;
    im.setImagesIds(left_id,right_id);
    im.setCorrelationThreshold(corr_th);
    im.setPerformRadiometric(rad_cor > 0);
    im.setMatchingMethod(match_method);
    im.setRadiometricMode(rad_cor-1);
    im.setMinStd(std);
    im.getNCC()->setTemplate(ncc_temp);
    im.getNCC()->setSearchWindow(ncc_sw);
    im.getLSM()->setTemplate(lsm_temp);
    im.getLSM()->setMaxIterations(lsm_it);
    im.getLSM()->setConvergenceLimits(lsm_shift, lsm_shear, lsm_scale);
    im.getLSM()->setMaxDistance(lsm_dist);

    im.performImageMatching(img1, img2, &seeds, &pairs);

    delete img1, img2;
}
