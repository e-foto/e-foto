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
    cancel_flag = false;
    listAllImages = images;
    listEOs = eos;
    grid = NULL;
    im = NULL;
    isShowImage = false;
    dem_unsaved = false;
    grid_unsaved = false;
    setListPoint();
}

DEMManager::~DEMManager()
{
    if (grid != NULL)
        delete grid;
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
            if (!getPairs())
            {
                returnProject();
                return status;
            }
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

/*
 * Pair detection / Angle functions
 */
double DEMManager::fixAngle(double angle)
{
        bool negative = angle < 0.0;

        // Positive signal
        angle = fabs(angle);

        // Bring angle to the first lap
        if (angle > 2*M_PI)
        {
                int laps = floor(angle/(2*M_PI));
                angle = angle - laps*(2*M_PI);
        }

        if (negative)
                angle = 2*M_PI - angle;

        return angle;
}

double DEMManager::getAngleBetweenImages(double X1, double Y1, double X2, double Y2)
{
        double delta_X = X2 - X1;
        double delta_Y = Y2 - Y1;

        if (delta_X - 0.0 < 0.0000000000001)
        {
                if (delta_Y > 0.0)
                        return M_PI/2.0;
                else
                        return 3*M_PI/2.0;
        }

        return fixAngle(atan(delta_Y/delta_X));
}

bool DEMManager::checkAnglesAlligned(double angle1, double angle2, double tolerance)
{
        angle1 = fixAngle(angle1);
        angle2 = fixAngle(angle2);

        double distance = fabs(angle1 - angle2);

        if (distance > M_PI)
            distance = 2*M_PI - distance;

        return (distance < tolerance || fabs(M_PI - distance) < tolerance);
}

int DEMManager::getPairs()
{
    //
    // List Pairs description (0 - N-1):
    //
    // num = left + no_imgs*right // Encoding
    // left = num % no_imgs // Decoding
    // right =  num / no_imgs // Decoding
    //  Image ID ranges from 1-N

    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;

    if (listAllImages.size() < 2)
    {
        dui->showErrorMessage("Not enough images to run this application");
        return 0;
    }

    // Clear list
    listPairs.clear();

    Image *img;
    double X1, Y1, X2, Y2, R, dist, best_dist, bX2, bY2, overlap, align_ang, kappa;
    int p1, p2, best_img, img_code;
    Matrix Xa;

    // Calculate Image Radius
    ObjectSpaceCoordinate osc;
    img = listAllImages.at(0);
    ProjectiveRay pr(img);
    Xa = img->getEO()->getXa();
    X1 = Xa.get(1,1);
    Y1 = Xa.get(2,1);
    p1 = img->getWidth();
    p2 = img->getHeight()/2;
    osc = pr.digitalToObject(p1,p2,img->getHeight(),false);
    X2 = osc.getX();
    Y2 = osc.getY();
    R = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

    for (int i=0; i<listAllImages.size()-1; i++)
    {
        // Get base image center
        img = listAllImages.at(i);
        Xa = img->getEO()->getXa();
        X1 = Xa.get(1,1);
        Y1 = Xa.get(2,1);
        kappa = fabs(Xa.get(6,1));

        best_dist = 10e100;

        // Calculate the shortest image center
        for (int j=i+1; j<listAllImages.size(); j++)
        {
            if (i==j)
                continue;
            img = listAllImages.at(j);
            Xa = img->getEO()->getXa();
            X2 = Xa.get(1,1);
            Y2 = Xa.get(2,1);

            // Calculate dist
            dist = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

            if (dist < best_dist)
            {
                best_dist = dist;
                best_img = j;
                bX2 = X2;
                bY2 = Y2;
            }
        }

        // Check images overlapping
        overlap = 100*(2*R-best_dist)/(2*R);

        if (overlap < 60.0 || overlap > 100.0)
            continue;

        // Check images alignment and kappa
        align_ang = getAngleBetweenImages(X1, Y1, bX2, bY2);

        if (!checkAnglesAlligned(align_ang, kappa, 0.174532925)) // 10 Degrees
            continue;

        // Add images to list
        img_code = i + listAllImages.size()*best_img;
        listPairs.push_back(img_code);
    }

    if (listPairs.size() < 1)
    {
        dui->showErrorMessage("Not enough pairs to run this application");
        return 0;
    }

    addPairsToInterface();

    return 1;
}

void DEMManager::addPairsToInterface()
{
    // Add pairs to the interface
    int left_id, right_id;
    string str_left, str_right;
    stringstream txt;
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    for (int i=0; i<listPairs.size(); i++)
    {
        getImagesId(i,left_id,right_id);
        str_left = listAllImages.at(left_id-1)->getImageId();
        str_right = listAllImages.at(right_id-1)->getImageId();
        txt.str(""); // Clear stream
        txt << "Images " << str_left << " and " << str_right;
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

void DEMManager::setAutoExtractionSettings(int _rad_cor, int _match_method, int _rgx, int _rgy, double downs)
{
    rad_cor = _rad_cor;
    match_method = _match_method;
    rgx = _rgx;
    rgy = _rgy;
    downsample = 1.0/downs;

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

void DEMManager::setLSMSettings(int _lsm_temp, int _lsm_it, double _lsm_th, double _lsm_std, int _lsm_dist, double _lsm_shift, double _lsm_shear, double _lsm_scale, int over, double over_dist)
{
    lsm_temp = _lsm_temp;
    lsm_it = _lsm_it;
    lsm_th = _lsm_th;
    lsm_std = _lsm_std;
    lsm_dist = _lsm_dist;
    lsm_shift = _lsm_shift;
    lsm_shear = _lsm_shear;
    lsm_scale = _lsm_scale;
    over_it = over;
    over_it_dist = over_dist;
}

void DEMManager::setProgress(int progress)
{
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setProgress(progress);
}

void DEMManager::setCancel()
{
    cancel_flag = true;

    if (im != NULL)
        im->setCancel();
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

    // Update informations
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setAutoExtInfo(seeds.size(),0,0.0,0.0);
}

/*
 * DEM I/O operations
 **/

void DEMManager::saveDem(char * filename, int fileType)
{
    pairs.save(filename,fileType);
    dem_unsaved = false;
}

void DEMManager::saveDemGrid(char * filename, int fileType)
{
    grid->saveDem(filename,fileType);
    grid_unsaved = false;
}

int DEMManager::loadDem(char * filename, int fileType)
{
    pairs.clear();
    if (pairs.load(filename,fileType) != 1)
        return 0;

    // If file type is 2D (option 1 or 2), must calculate 3D coords
    if (fileType == 1 || fileType == 2)
        calcPointsXYZ();

    // Update info
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    double Xi, Yi, Zi, Xf, Yf, Zf;
    pairs.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    dui->setAutoExtInfo(seeds.size(),pairs.size(),Zi,Zf);
    dui->setBoundingBox(Xi, Yi, Xf, Yf);

    // Show image, if selected
    if (isShowImage)
    {
        Matrix * img = pairs.getDemImage(1.0, 1.0);
        dui->showImage(img, 0);
        //dui->saveImage((char *)"teste_img.bmp",img);
        delete img;
    }

    dem_unsaved = false;

    return 1;
}

/*
 * DEM extraction
 **/

void DEMManager::interpolateGrid(int source, int method, int garea, double Xi, double Yi, double Xf, double Yf, double res_x, double res_y, int tsurface, double ma_exp, double ma_dist, int ma_weight)
{
    double Zi, Zf;

    cancel_flag = false;

    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setStatus((char *)"Interpolating grid ...");

    dui->disableOptions();
    dui->setAllowClose(false);

    if (garea == 0)
        pairs.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);

    if (grid != NULL)
        delete grid;

    grid = new DemGrid(Xi,Yi,Xf,Yf,res_x,res_y);
    grid->linkManager(this);
    grid->setPointList(&pairs);

    switch (method)
    {
        case 1: grid->interpolateMovingSurface(ma_exp, ma_dist, ma_weight, tsurface); break;
        case 2: grid->interpolateTrendSurface(tsurface); break;
        case 3: grid->interpolateNearestPoint(); break;
        default : grid->interpolateMovingAverage(ma_exp, ma_dist, ma_weight);
    }

    // Update info
    double min, max;
    int w = grid->getWidth(), h = grid->getHeight();
    dui->setStatus((char *)"Done");
    dui->progressBar->setValue(0);
    grid->getMinMax(min,max);
    dui->setAutoExtInfo(seeds.size(),pairs.size(),Zi,Zf);
    dui->setGridData(Xi,Yi,Xf,Yf,min,max,res_x,res_y,w,h);

    dui->setAllowClose(true);
    dui->enableOptions();
    grid_unsaved = true;

    // Show image, if selected
    if (isShowImage && !cancel_flag)
    {
        Matrix * img = grid->getDemImage();
        dui->showImage(img, 1);
        delete img;
    }
}

void DEMManager::calcPointsXYZ()
{
    // Calculate pair list XYZ coordinates
    MatchingPoints *mp;
    SpatialIntersection spc_inter;
    Image *left, *right;
    ObjectSpaceCoordinate object;

    for (int i=0; i<pairs.size(); i++)
    {
        mp = pairs.get(i+1);
        left = listAllImages.at(mp->left_image_id - 1);
        sp.setLeftImage(left);
        right = listAllImages.at(mp->right_image_id - 1);
        sp.setRightImage(right);

        spc_inter.setStereoPair(&sp);

        object = spc_inter.calculateIntersection(int(mp->left_x), int(mp->left_y), int(mp->right_x), int(mp->right_y));

        mp->X = object.getX();
        mp->Y = object.getY();
        mp->Z = object.getZ();
    }
}

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

void DEMManager::extractDEM(int option, bool clearMList)
{
    cancel_flag = false;

    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->disableOptions();
    dui->setAllowClose(false);

    // Clear list, if selected
    if (clearMList)
        pairs.clear();

    // Downsample seeds and matching points lists
    resamplePoints(&seeds,downsample);
    resamplePoints(&pairs,downsample);

    if (option==0)
    {
        for (int i=0; i<listPairs.size(); i++)
        {
            extractDEMPair(i);
            if (cancel_flag)
                break;
        }
    }
    else
        extractDEMPair(option-1);

    // Upsample seeds
    resamplePoints(&seeds,1.0/downsample);
    resamplePoints(&pairs,1.0/downsample);

    // Convert points to 3D
    calcPointsXYZ();

    // Update info
    double Xi, Yi, Zi, Xf, Yf, Zf;
    pairs.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    dui->setStatus((char *)"Done");
    dui->progressBar->setValue(0);
    dui->setAutoExtInfo(seeds.size(),pairs.size(),Zi,Zf);
    dui->setBoundingBox(Xi, Yi, Xf, Yf);

    // Create histogram
    MatchingPoints *mp;
    int hist[6] = {0,0,0,0,0,0};
    int p;

    for (int i=1; i<=pairs.size(); i++)
    {
        mp = pairs.get(i);
        p = 10 - int(mp->matching_accuracy*10);
        if (p>5) p=5;
        hist[p] += 1;
    }

    for (int i=0; i<6; i++)
        hist[i] = (100*hist[i])/pairs.size();

    dui->setMathcingHistogram(hist);

    dui->setAllowClose(true);
    dui->enableOptions();
    dem_unsaved = true;

    // Show image, if selected
    if (isShowImage && !cancel_flag)
    {
        Matrix * img = pairs.getDemImage(1.0, 1.0);
        dui->showImage(img, 0);
        delete img;
    }
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
    Matrix *img1 = NULL, *img2 = NULL;
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setStatus((char *)"Loading left image ...");
    string filename = getImage(left_id)->getFilepath() + "/" + getImage(left_id)->getFilename();
    img1 = dui->loadImage((char *)filename.c_str(), downsample);
    dui->setStatus((char *)"Loading right image ...");
    filename = getImage(right_id)->getFilepath() + "/" + getImage(right_id)->getFilename();
    img2 = dui->loadImage((char *)filename.c_str(), downsample);

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

    im = new ImageMatching(this);
    im->setImagesIds(left_id,right_id);
    im->setCorrelationThreshold(corr_th);
    im->setPerformRadiometric(rad_cor > 0);
    im->setMatchingMethod(match_method);
    im->setRadiometricMode(rad_cor-1);
    im->setMinStd(std);
    im->getNCC()->setTemplate(ncc_temp);
    im->getNCC()->setSearchWindow(ncc_sw);
    im->getLSM()->setTemplate(lsm_temp);
    im->getLSM()->setMaxIterations(lsm_it);
    im->getLSM()->setConvergenceLimits(lsm_shift, lsm_shear, lsm_scale);
    im->getLSM()->setMaxDistance(lsm_dist);
    im->getLSM()->setOverIt(over_it);
    im->getLSM()->setOverItDist(over_it_dist);

    im->performImageMatching(img1, img2, &seeds, &pairs);

    delete img1, img2;
    delete im;
}

void DEMManager::getPointList(MatchingPointsList &sd, MatchingPointsList &pr)
{
    sd = seeds;
    pr = pairs;
}
