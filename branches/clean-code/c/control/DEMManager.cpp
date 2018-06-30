/**************************************************************************
DEMManager.cpp
**************************************************************************/
/*Copyright 2002-2018 e-foto team (UERJ)
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
#define _USE_MATH_DEFINES  // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "DEMManager.h"
#include <sstream>

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

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DEMManager::DEMManager(): DEMManager(nullptr,
                                     std::deque<Image*>(),
                                     std::deque<ExteriorOrientation*>())
{
}

DEMManager::DEMManager(EFotoManager* manager,
                       std::deque<Image*>images,
                       std::deque<ExteriorOrientation*> eos):
    manager_ {manager},
    cancel_flag_ {false},
    listAllImages_ {images},
    listEOs_ {eos},
    grid_ {nullptr},
    image_matching_ {nullptr},
    dem_feature_ {nullptr},
    isShowImage_ {false},
    dem_unsaved_ {false},
    grid_unsaved_ {false},
    elim_bad_pts_ {false},
    bounding_box_empty_ {true},
    lsm_temp_growth_step_ {2},
    lsm_temp_max_size_ {50},
    ncc_temp_growth_step_ {2},
    ncc_temp_max_size_ {50},
    bounding_box_Xi_{0.0},
    bounding_box_Yi_{0.0},
    bounding_box_Xf_{0.0},
    bounding_box_Yf_{0.0},
    rad_cor_{0},
    match_method_{0},
    rgx_{0},
    rgy_{0},
    lsm_temp_{0},
    lsm_it_{0},
    lsm_dist_{0},
    ncc_temp_{0},
    ncc_sw_{0},
    lsm_th_{0.0},
    lsm_std_{0.0},
    lsm_shift_{0.0},
    lsm_shear_{0.0},
    lsm_scale_{0.0},
    ncc_th_{0.0},
    ncc_std_{0.0},
    downsample_{0.0},
    perf_RG{false},
    over_it_{false},
    over_it_dist_{0.0},
    myInterface{nullptr},
    dem_total_elapsed_time{0.0},
    started_{false}
{
    setListPoint();
}

DEMManager::~DEMManager()
{
    if (grid_) {
        delete grid_;
    }
}

bool DEMManager::exec()
{
    if (manager_) {
        if (manager_->getInterfaceType().compare("Qt") == 0) {
            myInterface = DEMUserInterface_Qt::instance(this);
        }

        connectImagePoints();
        started_ = true;

        if (myInterface) {
            myInterface->exec();

            if (!getPairs()) {
                returnProject();
                return true;
            }

            createInitialSeeds();
        }
    }

    return true;
}

void DEMManager::returnProject() const
{
    manager_->reloadProject();
}

Image* DEMManager::getImage(const int id) const
{
    for (size_t i = 0; i < listAllImages_.size(); i++) {
        if (listAllImages_.at(i)->getId() == id) {
            return listAllImages_.at(i);
        }
    }

    return nullptr;
}

void DEMManager::setListPoint()
{
    EDomElement xmlPoints(manager_->getXml("points"));
    std::deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");

    for (size_t i = 0; i < allPoints.size(); i++) {
        Point* point = manager_->instancePoint(
                           Conversion::stringToInt(allPoints.at(i).attribute("key")));

        if (point) {
            listAllPoints.push_back(point);
        }
    }
}

bool DEMManager::connectImagePoints()
{
    if (started_) {
        return false;
    }

    qApp->processEvents();
    EDomElement xmlPoints(manager_->getXml("points"));
    std::deque<EDomElement>allPoints = xmlPoints.elementsByTagName("point");

    for (size_t j = 0; j < listAllImages_.size(); j++) {
        for (size_t i = 0; i < allPoints.size(); i++) {
            std::string data = allPoints.at(i).
                               elementByTagAtt("imageCoordinates",
                                               "image_key",
                                               Conversion::intToString(
                                                   listAllImages_.at(j)->getId())).
                               getContent();

            if (data != "") {
                Point* pointToInsert = manager_->
                                       instancePoint(Conversion::stringToInt(
                                               allPoints.at(i).attribute("key")));

                if (pointToInsert) {
                    listAllImages_.at(j)->putPoint(pointToInsert);
                    pointToInsert->putImage(listAllImages_.at(j));
                }
            }
        }
    }

    return true;
}

void DEMManager::updateNoSeeds()
{
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    dui->updateSeedsLabel(seeds_.size());
}

/*
 * Pair detection / Angle functions
 */
int DEMManager::getPairs()
{
    // This Method was unified in EFotoManager Class
    manager_->getPairs(listPairs);
    addPairsToInterface();
    return (listPairs.size() > 0);
}

/*
 *  End of pair automatic identification
 */

void DEMManager::addPairsToInterface() const
{
    // Add pairs to the interface
    std::stringstream txt;
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);

    for (size_t i = 0; i < listPairs.size(); i++) {
        int left_id, right_id;
        std::tie(left_id, right_id) = getImagesId(i);
        auto str_left = listAllImages_.at(left_id - 1)->getImageId();
        auto str_right = listAllImages_.at(right_id - 1)->getImageId();
        txt.str("");  // Clear stream
        txt << "Images " << str_left << " and " << str_right;
        dui->addImagePair(txt.str());
    }

    dui->setStatus("None");
}

// Internal function. Pos from 0 - N-1.
std::tuple<int, int> DEMManager::getImagesId(const int pos) const
{
    // Check pos
    if (pos < 0 || (size_t)pos > listPairs.size() - 1) {
        return std::make_tuple(-1, -1);
    }

    // Decode
    const int no_imgs = listAllImages_.size();
    return std::make_tuple(1 + (listPairs.at(pos) % no_imgs),
                           1 + (listPairs.at(pos) / no_imgs));
}

void DEMManager::setAutoExtractionSettings(const int rad_cor,
        const int match_method,
        const int rgx,
        const double downs)
{
    rad_cor_ = rad_cor;
    match_method_ = match_method;
    // Uniform step for Region Growing
    rgx_ = rgx;
    rgy_ = rgx;
    downsample_ = 1.0 / downs;
}

void DEMManager::setNCCSettings(const int ncc_temp,
                                const int ncc_sw,
                                const double ncc_th,
                                const double ncc_std)
{
    ncc_temp_ = ncc_temp;
    ncc_sw_ = ncc_sw;
    ncc_th_ = ncc_th;
    ncc_std_ = ncc_std;
}

void DEMManager::setLSMSettings(const int lsm_temp,
                                const int lsm_it,
                                const double lsm_th,
                                const double lsm_std,
                                const int lsm_dist,
                                const double lsm_shift,
                                const double lsm_shear,
                                const double lsm_scale,
                                const int over,
                                const double over_dist)
{
    lsm_temp_ = lsm_temp;
    lsm_it_ = lsm_it;
    lsm_th_ = lsm_th;
    lsm_std_ = lsm_std;
    lsm_dist_ = lsm_dist;
    lsm_shift_ = lsm_shift;
    lsm_shear_ = lsm_shear;
    lsm_scale_ = lsm_scale;
    over_it_ = over;
    over_it_dist_ = over_dist;
}

void DEMManager::setProgress(const int progress) const
{
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    dui->setProgress(progress);
}

void DEMManager::setCancel()
{
    cancel_flag_ = true;

    if (image_matching_) {
        image_matching_->setCancel();
    }

    if (grid_) {
        grid_->setCancel();
    }
}

void DEMManager::updateBoundingBox(double Xi, double Yi, double Xf, double Yf)
{
    if (bounding_box_empty_) {
        bounding_box_Xi_ = Xi;
        bounding_box_Xf_ = Xf;
        bounding_box_Yi_ = Yi;
        bounding_box_Yf_ = Yf;
        bounding_box_empty_ = false;
        return;
    }

    if (Xi < bounding_box_Xi_) {
        bounding_box_Xi_ = Xi;
    }

    if (Xf > bounding_box_Xf_) {
        bounding_box_Xf_ = Xf;
    }

    if (Yi < bounding_box_Yi_) {
        bounding_box_Yi_ = Yi;
    }

    if (Yf > bounding_box_Yf_) {
        bounding_box_Yf_ = Yf;
    }
}

/*
 * Seeds
 **/

void DEMManager::createInitialSeeds()
{
    // Clear seeds list before adding measured point as seeds
    seeds_.clear();
    Point* p;
    int left_id, right_id;
    double X, Y, Z;
    ImageSpaceCoordinate left_dic, right_dic;

    for (size_t i = 0; i < listAllPoints.size(); i++) {
        p = listAllPoints.at(i);
        int no_imgs_pt = p->countImages();

        // There is no pair, if points # is less than 2
        if (no_imgs_pt < 2) {
            continue;
        }

        // Read pair list
        for (unsigned k = 0; k < listPairs.size(); k++) {
            std::tie(left_id, right_id) = getImagesId(k);

            if (!p->hasImageCoordinate(left_id) ||
                    !p->hasImageCoordinate(right_id)) {
                continue;
            }

            left_dic = p->getImageCoordinate(left_id);
            right_dic = p->getImageCoordinate(right_id);
            X = p->getObjectCoordinate().getX();
            Y = p->getObjectCoordinate().getY();
            Z = p->getObjectCoordinate().getZ();
            seeds_.add(left_id,
                       right_id,
                       left_dic.getCol(),
                       left_dic.getLin(),
                       right_dic.getCol(),
                       right_dic.getLin(),
                       X, Y, Z, 0.0);
        }
    }

    // Update informations
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    dui->setAutoExtInfo(seeds_.size(), 0, 0.0, 0.0);
}

/*
 * DEM I/O operations
 **/

void DEMManager::saveDem(const char* const filename, const int fileType)
{
    pairs.save(filename, fileType);
    // Expanção do XML
    addPairsToXML(std::string(filename));
    dem_unsaved_ = false;
}

void DEMManager::saveDemGrid(const char* const filename, Filetype fileType)
{
    grid_->saveDem(filename, fileType);
    grid_unsaved_ = false;
    // Expansão do XML
    addDEMToXML(std::string(filename));
}

bool DEMManager::loadDem(char* filename, int fileType)
{
    pairs.clear();

    if (pairs.load(filename, fileType) != 1) {
        return false;
    }

    // If file type is 2D (option 1 or 2), must calculate 3D coords
    if (fileType == 1 || fileType == 2) {
        calcPointsXYZ();
    }

    // Update info
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    double Xi, Yi, Zi, Xf, Yf, Zf;
    pairs.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    updateBoundingBox(Xi, Yi, Xf, Yf);
    dui->setAutoExtInfo(seeds_.size(), pairs.size(), Zi, Zf);
    dui->setBoundingBox(bounding_box_Xi_,
                        bounding_box_Yi_,
                        bounding_box_Xf_,
                        bounding_box_Yf_);

    // Show image, if selected
    if (isShowImage_) {
        Matrix* img = pairs.getDemImage(1.0, 1.0);
        dui->showImage2D(img, Xi, 1.0, Yi, 1.0, 0);
        delete img;
    }

    dem_unsaved_ = false;
    return true;
}

bool DEMManager::loadDemGrid(char* filename, Filetype fileType)
{
    // Create custom grid. Load will fix these values.
    if (grid_) {
        delete grid_;
    }

    grid_ = new DemGrid(1.0, 2.0, 1.0, 2.0, 1.0, 1.0);
    grid_->loadDem(filename, fileType);
    // Update info
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    double Xi, Yi, Zi, Xf, Yf, Zf, res_x, res_y;
    grid_->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
    int w = grid_->getWidth(), h = grid_->getHeight();
    grid_->getMinMax(Zi, Zf);
    dui->setGridData(Xi, Yi, Xf, Yf, Zi, Zf, res_x, res_y, w, h);

    // Show image, if selected
    if (isShowImage_ && !cancel_flag_) {
        const double res_z = (Zf - Zi) / 255.0;
        Matrix* img = grid_->getDemImage();
        dui->showImage3D(img, Xi, res_x, Yi, res_y, Zi, res_z, 1);
        delete img;
    }

    return true;
}

bool DEMManager::loadDemFeature(char* filename)
{
    if (dem_feature_) {
        delete dem_feature_;
    }

    dem_feature_ = new DemFeatures();
    // Stereoplotter 1.65, mode = 0
    // Append = false
    const bool dfFlag = dem_feature_->loadFeatures(filename, false);
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);

    if (!dfFlag) {
        dui->showErrorMessage("Bad file type !");
        return false;
    }

    // Update info
    MatchingPointsList ptLineList, allList;
    dem_feature_->addFeaturesToPairList(&ptLineList, false);
    dem_feature_->addFeaturesToPairList(&allList, true);
    double Xi, Yi, Zi, Xf, Yf, Zf;
    allList.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    updateBoundingBox(Xi, Yi, Xf, Yf);
    ptLineList.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    dui->setStatus("Done");
    dui->progressBar->setValue(0);
    dui->setManualExtInfo(ptLineList.size(), Zi, Zf);
    dui->setBoundingBox(bounding_box_Xi_, bounding_box_Yi_, bounding_box_Xf_,
                        bounding_box_Yf_);
    return true;
}

/*
 * DEM extraction
 **/

void DEMManager::interpolateGrid(InterpolationMethod method,
                                 int garea,
                                 double Xi,
                                 double Yi,
                                 double Xf,
                                 double Yf,
                                 double res_x,
                                 double res_y,
                                 TSSurface tsurface,
                                 double ma_exp,
                                 double ma_dist,
                                 MAWeight ma_weight,
                                 int gridSource)
{
    double Zi, Zf;
    cancel_flag_ = false;
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    dui->setStatus("Interpolating grid ...");
    dui->disableOptions();
    dui->setAllowClose(false);
    // Grid source:
    // 0- Automatic extraction
    // 1- Stereoplotter points and lines
    // 2- Both
    MatchingPointsList modPairs;

    // Copy of automatic extraction
    if (gridSource != 1) {
        modPairs = pairs;
    }

    if (gridSource != 0) {
        if (dem_feature_) {
            dem_feature_->addFeaturesToPairList(&modPairs, false);
        }
    }

    if (garea == 0) {
        Xi = bounding_box_Xi_;
        Xf = bounding_box_Xf_;
        Yi = bounding_box_Yi_;
        Yf = bounding_box_Yf_;
    }

    if (grid_) {
        delete grid_;
    }

    grid_ = new DemGrid(Xi, Yi, Xf, Yf, res_x, res_y);
    grid_->linkManager(this);
    grid_->setPointList(&modPairs);

    switch (method) {
    case InterpolationMethod::MOVING_SURFACE:
        grid_->interpolateMovingSurface(ma_exp, ma_dist, ma_weight, tsurface);
        break;

    case InterpolationMethod::TREND_SURFACE:
        grid_->interpolateTrendSurface(tsurface);
        break;

    case InterpolationMethod::NEAREST_POINT:
        grid_->interpolateNearestPoint();
        break;

    default :
        grid_->interpolateMovingAverage(ma_exp, ma_dist, ma_weight);
    }

    // Add polygons, if selected
    if (gridSource > 0) {
        Matrix overMap = dem_feature_->createPolygonMap(Xi, Yi, Xf, Yf, res_x, res_y);
        grid_->overlayMap(&overMap);
    }

    // Update Zi and Zf
    grid_->getMinMax(Zi, Zf);
    // Update info
    double min, max;
    const int w = grid_->getWidth();
    const int h = grid_->getHeight();
    dui->setStatus("Done");
    dui->progressBar->setValue(0);
    grid_->getMinMax(min, max);
    dui->setAutoExtInfo(seeds_.size(), pairs.size(), Zi, Zf);
    dui->setGridData(Xi, Yi, Xf, Yf, min, max, res_x, res_y, w, h);
    dui->setAllowClose(true);
    dui->enableOptions();
    grid_unsaved_ = true;
    dui->setElapsedTime(grid_->getElapsedTime(), 1);

    // Show image, if selected
    if (isShowImage_ && !cancel_flag_) {
        double res_z = (Zf - Zi) / 255.0;
        Matrix* img = grid_->getDemImage();
        dui->showImage3D(img, Xi, res_x, Yi, res_y, Zi, res_z, 1);
        delete img;
    }
}

void DEMManager::calcPointsXYZ()
{
    // Calculate pair list XYZ coordinates
    SpatialIntersection spc_inter;
    ObjectSpaceCoordinate object;

    for (size_t i = 0; i < pairs.size(); i++) {
        MatchingPoints* mp = pairs.get(i + 1);
        Image* left = listAllImages_.at(mp->left_image_id - 1);
        sp.setLeftImage(left);
        Image* right = listAllImages_.at(mp->right_image_id - 1);
        sp.setRightImage(right);
        spc_inter.setStereoPair(&sp);
        object = spc_inter.calculateIntersectionSubPixel(mp->left_x, mp->left_y,
                 mp->right_x, mp->right_y);
        mp->X = object.getX();
        mp->Y = object.getY();
        mp->Z = object.getZ();
    }
}

void DEMManager::resamplePoints(MatchingPointsList* list, double resample)
{
    MatchingPoints* mp;

    for (size_t i = 1; i <= list->size(); i++) {
        mp = list->get(i);
        mp->left_x = mp->left_x * resample;
        mp->left_y = mp->left_y * resample;
        mp->right_x = mp->right_x * resample;
        mp->right_y = mp->right_y * resample;
    }
}

int DEMManager::extractDEM(int option, bool clearMList)
{
    cancel_flag_ = false;
    // Reset extraction time
    dem_total_elapsed_time = 0.0;
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    dui->disableOptions();
    dui->setAllowClose(false);

    // Clear list, if selected
    if (clearMList) {
        pairs.clear();
    }

    // Downsample seeds and matching points lists
    resamplePoints(&seeds_, downsample_);
    resamplePoints(&pairs, downsample_);

    // Extract all pairs
    if (option == 0) {
        for (unsigned i = 0; i < listPairs.size(); i++) {
            extractDEMPair(i);

            if (cancel_flag_) {
                break;
            }
        }
    } else {  // Or just one pair
        extractDEMPair(option - 1);
    }

    // Upsample seeds
    resamplePoints(&seeds_, 1.0 / downsample_);
    resamplePoints(&pairs, 1.0 / downsample_);
    // Convert points to 3D
    calcPointsXYZ();
    // Update info
    double Xi, Yi, Zi, Xf, Yf, Zf;
    pairs.XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
    updateBoundingBox(Xi, Yi, Xf, Yf);
    dui->setStatus("Done");
    dui->progressBar->setValue(0);
    dui->setAutoExtInfo(seeds_.size(), pairs.size(), Zi, Zf);
    dui->setBoundingBox(bounding_box_Xi_, bounding_box_Yi_, bounding_box_Xf_,
                        bounding_box_Yf_);
    // Create histogram
    int hist[6] = {0, 0, 0, 0, 0, 0};
    dui->setAllowClose(true);
    dui->enableOptions();

    if (pairs.size() > 0) {
        for (unsigned int i = 1; i <= pairs.size(); i++) {
            MatchingPoints* mp = pairs.get(i);
            int p = 10 - int(mp->matching_accuracy * 10);

            if (p > 5) {
                p = 5;
            }

            hist[p] += 1;
        }

        for (int i = 0; i < 6; i++) {
            hist[i] = (100 * hist[i]) / pairs.size();
        }

        dui->setMathcingHistogram(hist);
        dem_unsaved_ = true;
    } else {
        dui->showErrorMessage("Could not find any pair");
        return 0;
    }

    dui->setElapsedTime(dem_total_elapsed_time, 0);

    // Show image, if selected
    if (isShowImage_ && !cancel_flag_) {
        Matrix* img = pairs.getDemImage(1.0, 1.0);
        dui->showImage2D(img, Xi, 1.0, Yi, 1.0, 0);
        delete img;
    }

    return 1;
}

void DEMManager::extractDEMPair(int pair)
{
    if (pair < 0 || pair > static_cast<int>(listPairs.size()) - 1) {
        return;
    }

    //
    // Get images IDs
    //
    int left_id, right_id;
    std::tie(left_id, right_id) = getImagesId(pair);
    //
    // Load images
    //
    Matrix img1, img2;
    DEMUserInterface_Qt* dui = dynamic_cast<DEMUserInterface_Qt*>(myInterface);
    dui->setStatus("Loading left image ...");
    std::string filename = getImage(left_id)->getFilepath() + "/"
                           + getImage(left_id)->getFilename();
    dui->loadImage(img1, (char*)filename.c_str(), downsample_);
    dui->setStatus("Loading right image ...");
    filename = getImage(right_id)->getFilepath() + "/" + getImage(
                   right_id)->getFilename();
    dui->loadImage(img2, (char*)filename.c_str(), downsample_);
    //
    // Start matching
    //
    std::stringstream txt;
    txt << "Matching pair " << left_id << " and " << right_id;
    dui->setStatus(txt.str());
    dui->setProgress(0);
    double corr_th = lsm_th_, std = lsm_std_;

    if (match_method_ == 0) { // NCC
        corr_th = ncc_th_;
        std = ncc_std_;
    }

    image_matching_ = new ImageMatching(this);
    image_matching_->setPerformRegionGrowing(perf_RG);
    image_matching_->setImagesIds(left_id, right_id);
    image_matching_->setCorrelationThreshold(corr_th);
    image_matching_->setPerformRadiometric(rad_cor_ > 0);
    image_matching_->setMatchingMethod(match_method_);
    image_matching_->setRadiometricMode(rad_cor_ - 1);
    image_matching_->setMinStd(std);
    image_matching_->setElimanteBadPoints(elim_bad_pts_);
    image_matching_->getNCC()->setTemplateGrothStep(ncc_temp_growth_step_);
    image_matching_->getNCC()->setTemplateMaximumSize(ncc_temp_max_size_);
    image_matching_->getNCC()->setTemplate(ncc_temp_);
    image_matching_->getNCC()->setSearchWindow(ncc_sw_);
    image_matching_->getLSM()->setTemplateGrothStep(lsm_temp_growth_step_);
    image_matching_->getLSM()->setTemplateMaximumSize(lsm_temp_max_size_);
    image_matching_->getLSM()->setTemplate(lsm_temp_);
    image_matching_->getLSM()->setMaxIterations(lsm_it_);
    image_matching_->getLSM()->setConvergenceLimits(lsm_shift_, lsm_shear_,
            lsm_scale_);
    image_matching_->getLSM()->setMaxDistance(lsm_dist_);
    image_matching_->getLSM()->setOverIt(over_it_);
    image_matching_->getLSM()->setOverItDist(over_it_dist_);
    image_matching_->performImageMatching(&img1, &img2, &seeds_, &pairs);
    //  dui->saveImage((char *)"Map.bmp",&im->getMap());
    dem_total_elapsed_time += image_matching_->getElapsedTime();
    delete image_matching_;
}

void DEMManager::getPointList(MatchingPointsList& sd,
                              MatchingPointsList& pr) const
{
    sd = seeds_;
    pr = pairs;
}

std::string DEMManager::getDemQuality(char* filename, int option) const
{
    MatchingPointsList mpl;
    DemFeatures df2;

    if (option == 0) {
        if (!mpl.load(filename, MatchingPointsList::loadXYZ)) {
            return "";
        }
    } else {
        if (!df2.loadFeatures(filename, false)) {
            return "";
        }

        df2.addFeaturesToPairList(&mpl, true);
    }

    if (mpl.size() < 1) {
        return "";
    }

    return grid_->calculateDemQuality(mpl);
}

double DEMManager::calculateDemRes(double ds)
{
    if (listAllImages_.size() < 1) {
        return 0.0;
    }

    Image* img = listAllImages_.at(0);
    // Calculate image approximate resolution scanning
    const int img_width = img->getWidth();
    const int img_height = img->getHeight();
    const double DPI = (2.54 / 23.0) * (img_width + img_height) / 2;
    // Calculate resolution in image space (mm)
    const double resolution_mm = 0.0254 / DPI;
    // Calculate scale
    const double focal = img->getSensor()->getFocalDistance() / 1000.0;
    const double Z0 = img->getEO()->getXa().get(3, 1);
    const double scale = Z0 / focal;
    const double resolution = resolution_mm * scale;
    return resolution * ds;
}

void DEMManager::addPairsToXML(std::string filename)
{
    std::stringstream add;
    add << "<pairsFilename>";
    add << filename;
    add << "</pairsFilename>";
    EDomElement newXml(manager_->xmlGetData());

    if (newXml.elementByTagName("DEMs").getContent() == "") {
        newXml.addChildAtTagName("efotoPhotogrammetricProject",
                                 "<DEMs>\n</DEMs>");
    }

    newXml.addChildAtTagName("DEMs", add.str());
    manager_->xmlSetData(newXml.getContent());
    manager_->setSavedState(false);
}

void DEMManager::addSeedsToXML(std::string filename)
{
    std::stringstream add;
    add << "<seedsFilename>";
    add << filename;
    add << "</seedsFilename>";
    EDomElement newXml(manager_->xmlGetData());

    if (newXml.elementByTagName("DEMs").getContent() == "") {
        newXml.addChildAtTagName("efotoPhotogrammetricProject",
                                 "<DEMs>\n</DEMs>");
    }

    newXml.addChildAtTagName("DEMs", add.str());
    manager_->xmlSetData(newXml.getContent());
    manager_->setSavedState(false);
}

void DEMManager::addDEMToXML(std::string filename)
{
    std::stringstream add;
    add << "<dsmFilename>";
    add << filename;
    add << "</dsmFilename>";
    EDomElement newXml(manager_->xmlGetData());

    if (newXml.elementByTagName("DEMs").getContent() == "") {
        newXml.addChildAtTagName("efotoPhotogrammetricProject", "<DEMs>\n</DEMs>");
    }

    newXml.addChildAtTagName("DEMs", add.str());
    manager_->xmlSetData(newXml.getContent());
    manager_->setSavedState(false);
}

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
