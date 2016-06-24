/**************************************************************************
DEMManager.cpp
**************************************************************************/
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "DEMManager.h"

#include "Point.h"
#include "EFotoManager.h"
#include "SpatialRessection.h"
#include "DEMUserInterface_Qt.h"
#include "ImageMatching.h"
#include "DemGrid.h"
#include "DemFeatures.h"
#include "SpatialIntersection.h"
#include "FrameSensor.h"
#include "Image.h"

#include "ProjectiveRay.h"

#include <sstream>

// Constructors and destructors
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DEMManager::DEMManager()
{
    started = false;
    status = false;
}

DEMManager::DEMManager(EFotoManager* manager, std::deque<Image*>images, std::deque<ExteriorOrientation*> eos)
{
    this->manager = manager;
    started = false;
    status = false;
    cancel_flag = false;
    listAllImages = images;
    listEOs = eos;
    grid = NULL;
    im = NULL;
    df = NULL;
    isShowImage = false;
    dem_unsaved = false;
    grid_unsaved = false;
    elim_bad_pts = false;
    bb_empty = true;
        lsm_temp_growth_step = 2;
        lsm_temp_max_size = 50;
        ncc_temp_growth_step = 2;
        ncc_temp_max_size = 50;
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
    for (unsigned i=0; i<listAllImages.size(); i++)
    {
        if (listAllImages.at(i)->getId() == id)
            return listAllImages.at(i);
    }

    return NULL;
}

void DEMManager::setListPoint()
{
    EDomElement xmlPoints(manager->getXml("points"));
    std::deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
    for(unsigned i=0;i<allPoints.size();i++)
    {
        Point* point= manager->instancePoint(Conversion::stringToInt(allPoints.at(i).attribute("key")));
        if (point != NULL)
            listAllPoints.push_back(point);
    }
}

bool DEMManager::connectImagePoints()
{
    if (!(started))
    {
        qApp->processEvents();
        EDomElement xmlPoints(manager->getXml("points"));
        std::deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
        for (unsigned int j = 0; j < listAllImages.size(); j++)
        {
            //listAllImages.at(j)->clearPoints();
            for (unsigned int i = 0; i < allPoints.size(); i++)
            {
                std::string data = allPoints.at(i).elementByTagAtt("imageCoordinates", "image_key", Conversion::intToString(listAllImages.at(j)->getId())).getContent();
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

void DEMManager::updateNoSeeds()
{
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->updateSeedsLabel(seeds.size());
}

double DEMManager::getTerrainMeanAltitude()
{
    double Z=0.0;
    int num_points = listAllPoints.size();
    Point *p;

    for (int i=0; i<num_points; i++)
    {
        p = listAllPoints.at(i);
        Z += p->getObjectCoordinate().getZ();
    }

    return Z / double(num_points);
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

        if (fabs(delta_X - 0.0) < 0.0000000000001)
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
    // This Method was unified in EFotoManager Class

    manager->getPairs(listPairs);
    addPairsToInterface();
    return (listPairs.size() > 0);


    /* old Method
        //
        // List Pairs description (0 - N-1):
        //
        // num = left + no_imgs*right // Encoding
        // left = num % no_imgs // Decoding
        // right =  num / no_imgs // Decoding
        //  Image ID ranges from 1-N
        // Clear list
        listPairs.clear();

        Image *img;
        double X1, Y1, X2, Y2, R, dist, overlap, terrain_mean_Z;
        int p1, p2, img_code, id1, id2;
        Matrix Xa;

        // Calculate Images Radius, using the first image as reference
        ObjectSpaceCoordinate osc;
        img = listAllImages.at(0);
        ProjectiveRay pr(img);
        Xa = img->getEO()->getXa();
        X1 = Xa.get(1,1);
        Y1 = Xa.get(2,1);
        p1 = img->getWidth();
        p2 = img->getHeight()/2;
        terrain_mean_Z = getTerrainMeanAltitude();
        osc = pr.imageToObject(p1,p2,terrain_mean_Z,false);
        X2 = osc.getX();
        Y2 = osc.getY();
        R = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

        for (unsigned i=0; i<listAllImages.size(); i++)
        {
                // Get reference image data
                img = listAllImages.at(i);
                Xa = img->getEO()->getXa();
                X1 = Xa.get(1,1);
                Y1 = Xa.get(2,1);

                // Calculate the shortest image center
                for (unsigned j=0; j<listAllImages.size(); j++)
                {
                        // Skip same image check
                        if (i==j)
                                continue;

                        // Get test image data
                        img = listAllImages.at(j);
                        Xa = img->getEO()->getXa();
                        X2 = Xa.get(1,1);
                        Y2 = Xa.get(2,1);

                        // Calculate dist
                        dist = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

                        // Check images overlapping
                        overlap = 100*(2*R - dist)/(2*R);

                        if (overlap < 55.0 || overlap > 100.0)
                                continue;

                        // Assign image ids (in this case, the verctor number - image id ranges from 1-N
                        id1 = i+1;
                        id2 = j+1;

                        // Check if pair exists
                        if (!existPair(id1, id2))
                        {
                            // Add images to list
                            img_code = (id1-1) + listAllImages.size()*(id2-1);
                            listPairs.push_back(img_code);
                        }
                }
        }

        addPairsToInterface();

        qDebug("=========>%d<=========",listPairs.size());

        return (listPairs.size() > 0);
        */
}

// Check if pair already exists and sort ids
bool DEMManager::existPair(int &id1, int &id2)
{
    int aux, pair_id1, pair_id2;

    if (listPairs.size() == 0)
        return false;

    // Sort id
    if (id1 > id2)
    {
        aux = id1;
        id1 = id2;
        id2 = aux;
    }

    for (unsigned i=0; i<listPairs.size(); i++)
    {
        // Decode image list
        getImagesId(i, pair_id1, pair_id2);
        if (pair_id1 == id1 && pair_id2 == id2)
            return true;
    }

    return false;
}

/*
 *  End of pair automatic identification
 */

void DEMManager::addPairsToInterface()
{
    // Add pairs to the interface
    int left_id, right_id;
    std::string str_left, str_right;
    std::stringstream txt;
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    for (unsigned i=0; i<listPairs.size(); i++)
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
    if (pos < 0 || (unsigned)pos > listPairs.size()-1)
        return;

    // Decode
    int no_imgs = listAllImages.size();
    left = 1 + (listPairs.at(pos) % no_imgs);
    right = 1 + (listPairs.at(pos) / no_imgs);
}

void DEMManager::setAutoExtractionSettings(int _rad_cor, int _match_method, int _rgx, double downs)
{
    rad_cor = _rad_cor;
    match_method = _match_method;

        // Unifor step for Region Growing
    rgx = _rgx;
        rgy = _rgx;
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

        if (grid != NULL)
                grid->setCancel();
}

void DEMManager::updateBoundingBox(double Xi, double Yi, double Xf, double Yf)
{
    if (bb_empty)
    {
        bb_Xi = Xi; bb_Xf = Xf;
        bb_Yi = Yi; bb_Yf = Yf;
        bb_empty = false;
        return;
    }

    if (Xi < bb_Xi) bb_Xi = Xi;
    if (Xf > bb_Xf) bb_Xf = Xf;
    if (Yi < bb_Yi) bb_Yi = Yi;
    if (Yf > bb_Yf) bb_Yf = Yf;
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
    double X, Y, Z;
    ImageSpaceCoordinate left_dic, right_dic;

    for (unsigned i=0; i<listAllPoints.size(); i++)
    {
        p = listAllPoints.at(i);
        no_imgs_pt = p->countImages();

        // There is no pair, if points # is less than 2
        if (no_imgs_pt < 2)
            continue;

        // Read pair list
        for (unsigned k=0; k<listPairs.size(); k++)
        {
            getImagesId(k, left_id, right_id);

            if (!p->hasImageCoordinate(left_id) || !p->hasImageCoordinate(right_id))
                continue;

            left_dic = p->getImageCoordinate(left_id);
            right_dic = p->getImageCoordinate(right_id);
            X = p->getObjectCoordinate().getX();
            Y = p->getObjectCoordinate().getY();
            Z = p->getObjectCoordinate().getZ();

            seeds.add(left_id, right_id, double(left_dic.getCol()), double(left_dic.getLin()), double(right_dic.getCol()), double(right_dic.getLin()), X, Y, Z, 0.0);
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
    // Expanção do XML
    addPairsToXML(std::string(filename));

    dem_unsaved = false;
}

void DEMManager::saveDemGrid(char * filename, int fileType)
{
    grid->saveDem(filename,fileType);
    grid_unsaved = false;
    // Expanção do XML
    addDEMToXML(std::string(filename));
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
    updateBoundingBox(Xi, Yi, Xf, Yf);
    dui->setAutoExtInfo(seeds.size(),pairs.size(),Zi,Zf);
    dui->setBoundingBox(bb_Xi, bb_Yi, bb_Xf, bb_Yf);

    // Show image, if selected
    if (isShowImage)
    {
        Matrix * img = pairs.getDemImage(1.0, 1.0);
                dui->showImage2D(img, Xi, 1.0, Yi, 1.0, 0);
        //dui->saveImage((char *)"teste_img.bmp",img);
        delete img;
    }

    dem_unsaved = false;

    return 1;
}

int DEMManager::loadDemGrid(char * filename, int fileType)
{

        // Create custom grid. Load will fix these values.
        if (grid != NULL)
            delete grid;
        grid = new DemGrid(1.0, 2.0, 1.0, 2.0, 1.0, 1.0);
        grid->loadDem(filename,fileType);

        // Update info
        DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
        double Xi, Yi, Zi, Xf, Yf, Zf, res_x, res_y;
        grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
        int w = grid->getWidth(), h = grid->getHeight();
        grid->getMinMax(Zi,Zf);
        dui->setGridData(Xi,Yi,Xf,Yf,Zi,Zf,res_x,res_y,w,h);

        // Show image, if selected
        if (isShowImage && !cancel_flag)
        {
                double res_z = (Zf-Zi)/255.0;
                Matrix * img = grid->getDemImage();
                dui->showImage3D(img, Xi, res_x, Yi, res_y, Zi, res_z, 1);
                delete img;
        }

        return 1;
}

int DEMManager::loadDemFeature(char *filename)
{
    if (df != NULL)
        delete df;

    df = new DemFeatures();

    // Stereoplotter 1.65, mode = 0
    // Append = false
    bool dfFlag = df->loadFeatures(filename/*, 0*/, false);
//df->saveFeatures("/home/marts/teste.txt",0,false);
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;

    if (!dfFlag)
    {
        dui->showErrorMessage("Bad file type !");
        return 0;
    }

    // Update info
    MatchingPointsList ptLineList, allList;
    df->addFeaturesToPairList(&ptLineList, false);
    df->addFeaturesToPairList(&allList, true);

    double Xi, Yi, Zi, Xf, Yf, Zf;
    allList.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    updateBoundingBox(Xi, Yi, Xf, Yf);
    ptLineList.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    dui->setStatus((char *)"Done");
    dui->progressBar->setValue(0);
    dui->setManualExtInfo(ptLineList.size(),Zi,Zf);
    dui->setBoundingBox(bb_Xi, bb_Yi, bb_Xf, bb_Yf);

    return 1;
}

/*
 * DEM extraction
 **/

void DEMManager::interpolateGrid(/*int source, */ int method, int garea, double Xi, double Yi, double Xf, double Yf, double res_x, double res_y, int tsurface, double ma_exp, double ma_dist, int ma_weight, int gridSource)
{
    double Zi, Zf;

    cancel_flag = false;

    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setStatus((char *)"Interpolating grid ...");

    dui->disableOptions();
    dui->setAllowClose(false);

    // Grid source:
    // 0- Automatic extraction
    // 1- Stereoplotter points and lines
    // 2- Both
    MatchingPointsList modPairs;

    // Copy of automatic extraction
    if (gridSource != 1)
        modPairs = pairs;

    if (gridSource !=0)
    {
        if (df != NULL)
            df->addFeaturesToPairList(&modPairs, false);
    }

    if (garea == 0)
    {
        Xi = bb_Xi; Xf = bb_Xf;
        Yi = bb_Yi; Yf = bb_Yf;
    }

    if (grid != NULL)
        delete grid;

    grid = new DemGrid(Xi,Yi,Xf,Yf,res_x,res_y);
    grid->linkManager(this);

    grid->setPointList(&modPairs);

    switch (method)
    {
                case 1: grid->interpolateMovingSurface(ma_exp, ma_dist, ma_weight, tsurface); break;
                case 2: grid->interpolateTrendSurface(tsurface); break;
                case 3: grid->interpolateNearestPoint(); break;
                default : grid->interpolateMovingAverage(ma_exp, ma_dist, ma_weight);
    }

        // Add polygons, if selected
        if (gridSource > 0)
        {
            Matrix overMap = df->createPolygonMap(Xi, Yi, Xf, Yf, res_x, res_y);
            grid->overlayMap(&overMap);
        }

        // Update Zi and Zf
        grid->getMinMax(Zi, Zf);

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
        dui->setElapsedTime(grid->getElapsedTime(), 1);

    // Add polygons, if selected
    //if (gridSource > 0)
    //{
        //Matrix overMap = df->createPolygonMap(Xi, Yi, Xf, Yf, res_x, res_y);
        //grid->overlayMap(&overMap);
    //}

    // Show image, if selected
    if (isShowImage && !cancel_flag)
    {
        double res_z = (Zf-Zi)/255.0;
        Matrix * img = grid->getDemImage();
        dui->showImage3D(img, Xi, res_x, Yi, res_y, Zi, res_z, 1);
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

    for (unsigned int i=0; i<pairs.size(); i++)
    {
        mp = pairs.get(i+1);
        left = listAllImages.at(mp->left_image_id - 1);
        sp.setLeftImage(left);
        right = listAllImages.at(mp->right_image_id - 1);
        sp.setRightImage(right);

        spc_inter.setStereoPair(&sp);

        object = spc_inter.calculateIntersectionSubPixel(mp->left_x, mp->left_y, mp->right_x, mp->right_y);

        mp->X = object.getX();
        mp->Y = object.getY();
        mp->Z = object.getZ();
    }
}

void DEMManager::resamplePoints(MatchingPointsList *list, double resample)
{
    MatchingPoints *mp;
    for (unsigned int i=1; i<=list->size(); i++)
    {
        mp = list->get(i);
        mp->left_x = mp->left_x * resample;
        mp->left_y = mp->left_y * resample;
        mp->right_x = mp->right_x * resample;
        mp->right_y = mp->right_y * resample;
    }
}

int DEMManager::extractDEM(int option, bool clearMList)
{
    cancel_flag = false;

        // Reset extraction time
        dem_total_elapsed_time = 0.0;

    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->disableOptions();
    dui->setAllowClose(false);

    // Clear list, if selected
    if (clearMList)
        pairs.clear();

    // Downsample seeds and matching points lists
    resamplePoints(&seeds,downsample);
    resamplePoints(&pairs,downsample);

    // Extract all pairs
    if (option==0)
    {
        for (unsigned i = 0; i<listPairs.size(); i++)
        {
            extractDEMPair(i);
            if (cancel_flag)
                break;
        }
    }
    else // Or just one pair
        extractDEMPair(option-1);

    // Upsample seeds
    resamplePoints(&seeds,1.0/downsample);
    resamplePoints(&pairs,1.0/downsample);

    // Convert points to 3D
    calcPointsXYZ();

    // Update info
    double Xi, Yi, Zi, Xf, Yf, Zf;
    pairs.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    updateBoundingBox(Xi, Yi, Xf, Yf);
    dui->setStatus((char *)"Done");
    dui->progressBar->setValue(0);
    dui->setAutoExtInfo(seeds.size(),pairs.size(),Zi,Zf);
    dui->setBoundingBox(bb_Xi, bb_Yi, bb_Xf, bb_Yf);

    // Create histogram
    MatchingPoints *mp;
    int hist[6] = {0,0,0,0,0,0};
    int p;

    dui->setAllowClose(true);
    dui->enableOptions();

    if (pairs.size() > 0)
    {
        for (unsigned int i=1; i<=pairs.size(); i++)
        {
            mp = pairs.get(i);
            p = 10 - int(mp->matching_accuracy*10);
            if (p>5) p=5;
            hist[p] += 1;
        }

        for (int i=0; i<6; i++)
            hist[i] = (100*hist[i])/pairs.size();

        dui->setMathcingHistogram(hist);
        dem_unsaved = true;
    }
    else
    {
        dui->showErrorMessage("Could not find any pair");
        return 0;
    }

        dui->setElapsedTime(dem_total_elapsed_time, 0);

    // Show image, if selected
    if (isShowImage && !cancel_flag)
    {
        Matrix * img = pairs.getDemImage(1.0, 1.0);
                dui->showImage2D(img, Xi, 1.0, Yi, 1.0, 0);
        delete img;
    }

    return 1;
}

void DEMManager::extractDEMPair(int pair)
{
    if (pair < 0 || pair > static_cast<int>(listPairs.size())-1)
        return;

    //
    // Get images IDs
    //
    int left_id,right_id;
    getImagesId(pair, left_id, right_id);

    //
    // Load images
    //
        Matrix img1, img2;
    DEMUserInterface_Qt *dui = (DEMUserInterface_Qt *)myInterface;
    dui->setStatus((char *)"Loading left image ...");
    std::string filename = getImage(left_id)->getFilepath() + "/" + getImage(left_id)->getFilename();
        dui->loadImage(img1, (char *)filename.c_str(), downsample);
    dui->setStatus((char *)"Loading right image ...");
    filename = getImage(right_id)->getFilepath() + "/" + getImage(right_id)->getFilename();
        dui->loadImage(img2, (char *)filename.c_str(), downsample);

    //
    // Start matching
    //

    std::stringstream txt;
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
        im->setPerformRegionGrowing(perf_RG);
    im->setImagesIds(left_id,right_id);
    im->setCorrelationThreshold(corr_th);
    im->setPerformRadiometric(rad_cor > 0);
    im->setMatchingMethod(match_method);
    im->setRadiometricMode(rad_cor-1);
    im->setMinStd(std);
    im->setElimanteBadPoints(elim_bad_pts);
        im->getNCC()->setTemplateGrothStep(ncc_temp_growth_step);
        im->getNCC()->setTemplateMaximumSize(ncc_temp_max_size);
    im->getNCC()->setTemplate(ncc_temp);
    im->getNCC()->setSearchWindow(ncc_sw);
        im->getLSM()->setTemplateGrothStep(lsm_temp_growth_step);
        im->getLSM()->setTemplateMaximumSize(lsm_temp_max_size);
    im->getLSM()->setTemplate(lsm_temp);
    im->getLSM()->setMaxIterations(lsm_it);
    im->getLSM()->setConvergenceLimits(lsm_shift, lsm_shear, lsm_scale);
    im->getLSM()->setMaxDistance(lsm_dist);
    im->getLSM()->setOverIt(over_it);
    im->getLSM()->setOverItDist(over_it_dist);

        im->performImageMatching(&img1, &img2, &seeds, &pairs);
    //  dui->saveImage((char *)"Map.bmp",&im->getMap());

        dem_total_elapsed_time += im->getElapsedTime();

    delete im;
}

void DEMManager::getPointList(MatchingPointsList &sd, MatchingPointsList &pr)
{
    sd = seeds;
    pr = pairs;
}

std::string DEMManager::getDemQuality(char *filename, int option)
{
    MatchingPointsList mpl;
    DemFeatures df2;

    if (option == 0)
    {
        if (!mpl.load(filename, MatchingPointsList::loadXYZ))
            return "";
    }
    else
    {
        if (!df2.loadFeatures(filename/*, 0*/, false))
            return "";
        df2.addFeaturesToPairList(&mpl, true);
    }

    if (mpl.size() < 1)
        return "";

    return grid->calculateDemQuality(mpl);
}

double DEMManager::calculateDemRes(double ds)
{
        if (listAllImages.size() < 1)
            return 0.0;

        Image *img = listAllImages.at(0);

        // Calculate image approximate resolution scanning
        int img_width = img->getWidth(), img_height = img->getHeight();
        double DPI = (2.54/23.0) * (img_width + img_height)/2;

        // Calculate resolution in image space (mm)
        double resolution_mm = 0.0254/DPI;

        // Calculate scale
        double focal = img->getSensor()->getFocalDistance() / 1000.0;
        double Z0 = img->getEO()->getXa().get(3,1);
        double scale = Z0 / focal;

        double resolution = resolution_mm * scale;

        return resolution*ds;
}

//#include <QDebug>
void DEMManager::addPairsToXML(std::string filename)
{
    std::stringstream add;
    add << "<pairsFilename>";
    add << filename;
    add << "</pairsFilename>";

    EDomElement newXml(manager->xmlGetData());

    if (newXml.elementByTagName("DEMs").getContent() == "")
        newXml.addChildAtTagName("efotoPhotogrammetricProject","<DEMs>\n</DEMs>");
    newXml.addChildAtTagName("DEMs", add.str());

    manager->xmlSetData(newXml.getContent());
    manager->setSavedState(false);
}

void DEMManager::addSeedsToXML(std::string filename)
{
    std::stringstream add;
    add << "<seedsFilename>";
    add << filename;
    add << "</seedsFilename>";

    EDomElement newXml(manager->xmlGetData());

    if (newXml.elementByTagName("DEMs").getContent() == "")
        newXml.addChildAtTagName("efotoPhotogrammetricProject","<DEMs>\n</DEMs>");
    newXml.addChildAtTagName("DEMs", add.str());

    manager->xmlSetData(newXml.getContent());
    manager->setSavedState(false);
}

void DEMManager::addDEMToXML(std::string filename)
{
    std::stringstream add;
    add << "<dsmFilename>";
    add << filename;
    add << "</dsmFilename>";

    EDomElement newXml(manager->xmlGetData());

    if (newXml.elementByTagName("DEMs").getContent() == "")
        newXml.addChildAtTagName("efotoPhotogrammetricProject","<DEMs>\n</DEMs>");
    newXml.addChildAtTagName("DEMs", add.str());

    manager->xmlSetData(newXml.getContent());
    manager->setSavedState(false);
}

/* Method into disuse:
 *
void DEMManager::addDEMQualityToXML(std::string filename)
{
    //Fazer
    //qDebug("DEMQuality: %s",filename.c_str());
}
*/

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
