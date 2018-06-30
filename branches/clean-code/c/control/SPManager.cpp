/**************************************************************************/
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
/**************************************************************************
SPManager.cpp
**************************************************************************/

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "SPManager.h"
#include "EFotoManager.h"
#include "SPUserInterface_Qt.h"
#include "ProjectiveRay.h"
#include "Project.h"
#include "SpatialIntersection.h"
#include "SpatialRessection.h"
#include "StereoPair.h"
#include "Point.h"
#include "Terrain.h"
#include "Image.h"
#include "Flight.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructors
SPManager::SPManager() : SPManager(nullptr,
                                       std::deque<Image * >(),
                                       std::deque<ExteriorOrientation * >())
{
}

SPManager::SPManager(EFotoManager* manager, std::deque<Image*>images,
                     std::deque<ExteriorOrientation*> eos) :
    myInterface{nullptr},
    manager_{manager},
    listAllImages{images},
    listEOs{eos},
    leftImage{nullptr},
    rightImage{nullptr},
    prL{0},
    prR{0},
    leftKey{1},
    rightKey{2},
    rad_cor{0},
    match_method{0},
    rgx{0},
    rgy{0},
    lsm_temp{0},
    lsm_it{0},
    lsm_dist{0},
    ncc_temp{0},
    ncc_sw{0},
    lsm_th{0.0},
    lsm_std{0.0},
    lsm_shift{0.0},
    lsm_shear{0.0},
    lsm_scale{0.0},
    ncc_th{0.0},
    ncc_std{0.0},
    downsample{0.0},
    started{false},
    over_it{false},
    over_it_dist{0.0}
{
    spFeatures.createClassesFromSp165();
    setListPoint();
}

SPManager::~SPManager()
{
}

bool SPManager::exec()
{
    if (manager_ != NULL) {
        if (manager_->getInterfaceType().compare("Qt") == 0) {
            myInterface = SPUserInterface_Qt::instance(this);
        }

        started = true;

        if (myInterface != NULL) {
            // Test if theere are at least two images
            if (listAllImages.size() < 2) {
                SPUserInterface_Qt* spui = dynamic_cast<SPUserInterface_Qt*>(myInterface);
                spui->showErrorMessage("There are no enough images to run this application");
                returnProject();
            }

            myInterface->exec();

            // Check if pairs available
            if (!getPairs()) {
                SPUserInterface_Qt* spui = dynamic_cast<SPUserInterface_Qt*>(myInterface);
                spui->showErrorMessage("Could not find any pair. Please, check your Exterior Orientation.");
                returnProject();
            }

            myInterface->centerImages(getCentralPoint(), 1.0);
        }
    }

    return true;
}

void SPManager::returnProject()
{
    manager_->reloadProject();
}

void SPManager::setListPoint()
{
    EDomElement xmlPoints(manager_->getXml("points"));
    std::deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");

    for(size_t i = 0; i < allPoints.size(); i++) {
        Point* point = manager_->instancePoint(Conversion::stringToInt(allPoints.at(
                i).attribute("key")));

        if (point != NULL) {
            listAllPoints.push_back(point);
        }
    }
}

int SPManager::loadFeatures(char* filename, bool append)
{
    return spFeatures.loadFeatures(filename/*,0*/, append);
}

void SPManager::saveFeatures(char* filename)
{
    spFeatures.saveFeatures(filename);
    // Expanção do XML
    addGeometryToXML(std::string(filename));
}

void SPManager::exportFeatures(char* filename, int mode)
{
    spFeatures.exportFeatures(filename, mode);
}

void SPManager::addFeature(std::string name, int feattype, int featclass)
{
    int fclass = spFeatures.convClassFromSp165(feattype, featclass);
    spFeatures.addNewFeature(name, "", fclass, feattype, 1);
    setSelected(spFeatures.getNumFeatures(), -1);
}

int SPManager::removeFeature()
{
    int sel_feat = spFeatures.selectedFeature();

    if (sel_feat == -1) {
        return 0;
    }

    spFeatures.deleteFeature(sel_feat);
    spFeatures.setSelectedFeature(-1);
    return 1;
}

void SPManager::removeAllFeatures()
{
    spFeatures.deleteAllFeatures();
    spFeatures.setSelectedFeature(-1);
}

bool SPManager::removePoint()
{
    int sel_feat = spFeatures.selectedFeature();

    if (sel_feat == -1) {
        return false;
    }

    int sel_pt = spFeatures.selectedPoint();

    if (sel_pt == -1) {
        return false;
    }

    spFeatures.deletePoint(sel_feat, sel_pt);

    if (unsigned(sel_pt) > spFeatures.getFeature(
                spFeatures.selectedFeature()).points.size() && sel_pt != 1) {
        spFeatures.setSelectedPoint(sel_pt - 1);
    } else if (sel_pt == 1
               && spFeatures.getFeature(spFeatures.selectedFeature()).points.size() == 0) {
        spFeatures.setSelectedPoint(-1);
    }

    return true;
}

void SPManager::setSelected(int feat_id, int pt_id)
{
    if (feat_id < -1 || pt_id < -1 || feat_id > spFeatures.getNumFeatures()) {
        return;
    }

    if (feat_id > 0) {
        if (pt_id > spFeatures.getNumPoints(feat_id)) {
            return;
        }
    }

    if (feat_id == 0 || pt_id == 0) {
        return;
    }

    spFeatures.setSelectedFeature(feat_id);
    spFeatures.setSelectedPoint(pt_id);
}

void SPManager::getSelectedFeatureData(int& sel_feat, std::string& fname,
                                       int& ftype, int& fclass, int& no_points, double& perimeter, double& area)
{
    ftype = -1;
    fclass = -1;
    fname = "";
    sel_feat = spFeatures.selectedFeature();

    if (sel_feat < 1) {
        return;
    }

    fname = spFeatures.getFeature(sel_feat).name;
    ftype = spFeatures.getFeature(sel_feat).feature_type;
    fclass = spFeatures.getClassIdToSp165(spFeatures.getFeature(
            sel_feat).feature_class);
    no_points = spFeatures.getNumPoints(sel_feat);
    perimeter = spFeatures.getFeature(sel_feat).perimeter;
    area = spFeatures.getFeature(sel_feat).area;
}

void SPManager::updateProjections()
{
    leftImage = listAllImages.at(leftKey - 1);
    rightImage = listAllImages.at(rightKey - 1);
    Sensor* sensor = leftImage->getSensor();
    InteriorOrientation* lio = leftImage->getIO();
    InteriorOrientation* rio = rightImage->getIO();
    ExteriorOrientation* lsr = leftImage->getEO();
    ExteriorOrientation* rsr = rightImage->getEO();
    leftImage->setSensor(sensor);
    leftImage->setIO(lio);
    leftImage->setEO(lsr);
    rightImage->setSensor(sensor);
    rightImage->setIO(rio);
    rightImage->setEO(rsr);

    if (leftImage == NULL || rightImage == NULL) {
        return;
    }

    prL = ProjectiveRay(leftImage);
    prR = ProjectiveRay(rightImage);

    // Aqui enquanto não definimos uma estrutura para as geometrias 2d eu vou coordenar a tradução para o que eu já possuia no display para pontos e retas.
    for (int i = 0; i < spFeatures.getNumFeatures(); i++) {
        DemFeature* df = spFeatures.getFeatureLink(i + 1);

        for (size_t j = 0; j < df->points.size(); j++) {
            // Isso vai ficar um pouco ruim, mas vai melhorar em breve quando a classe DigitalImageCoordinates for substituida por algo equivalente usando double (completando o suporte a subpixels)
            ImageSpaceCoordinate pL0 = prL.objectToImage(df->points.at(j).X,
                                       df->points.at(j).Y, df->points.at(j).Z, false);
            ImageSpaceCoordinate pR0 = prR.objectToImage(df->points.at(j).X,
                                       df->points.at(j).Y, df->points.at(j).Z, false);
            df->points.at(j).left_x = pL0.getCol();
            df->points.at(j).left_y = pL0.getLin();
            df->points.at(j).right_x = pR0.getCol();
            df->points.at(j).right_y = pR0.getLin();
        }
    }
}

void SPManager::computeIntersection(double xl, double yl, double xr, double yr,
                                    double& X, double& Y, double& Z)
{
    Image* leftImage = listAllImages.at(leftKey - 1);
    Image* rightImage = listAllImages.at(rightKey - 1);

    if (leftImage == nullptr || rightImage == nullptr) {
        return;
    }

    Sensor* sensor = leftImage->getSensor();
    InteriorOrientation* lio = leftImage->getIO();
    InteriorOrientation* rio = rightImage->getIO();
    SpatialRessection* lsr = dynamic_cast<SpatialRessection*>(leftImage->getEO());
    SpatialRessection* rsr = dynamic_cast<SpatialRessection*>(rightImage->getEO());
    leftImage->setSensor(sensor);
    leftImage->setIO(lio);
    leftImage->setEO(lsr);
    rightImage->setSensor(sensor);
    rightImage->setIO(rio);
    rightImage->setEO(rsr);
    SpatialIntersection si(new StereoPair(leftImage, rightImage));
    ObjectSpaceCoordinate obj = si.calculateIntersectionSubPixel(xl, yl, xr, yr);
    X = obj.getX();
    Y = obj.getY();
    Z = obj.getZ();
}

std::string SPManager::getFullImagePath(int imagekey)
{
    Image* img = listAllImages.at(imagekey - 1);
    std::string result;
    result.append(img->getFilepath());
    result.append("/");
    result.append(img->getFilename());
    return result;
}

/*
 *
 * Pair detection algorithm - Copied from DEM Manager
 *
 */

int SPManager::getPairs()
{
    // This Method was unified in EFotoManager Class
    manager_->getPairs(listPairs);
    addPairsToInterface();
    return (listPairs.size() > 0);
}

/*
 *  End of pair automatic identification
 */

void SPManager::addPairsToInterface()
{
    // Add pairs to the interface
    int left_id, right_id;
    std::string str_left, str_right;
    std::stringstream txt;
    SPUserInterface_Qt* spui = dynamic_cast<SPUserInterface_Qt*>(myInterface);

    for (size_t i = 0; i < listPairs.size(); i++) {
        getImagesId(i, left_id, right_id);
        str_left = listAllImages.at(left_id - 1)->getImageId();
        str_right = listAllImages.at(right_id - 1)->getImageId();
        txt.str(""); // Clear stream
        txt << "Images " << str_left << " and " << str_right;
        spui->addImagePair((char*)txt.str().c_str());
    }
}

void SPManager::addPoint(int fid, int pid, double lx, double ly, double rx,
                         double ry, double X, double Y, double Z)
{
    if (fid < 1 || fid > spFeatures.getNumFeatures()) {
        return;
    }

    if (pid < 1 || unsigned(pid) > spFeatures.getFeature(fid).points.size() + 1) {
        return;
    }

    spFeatures.addNewPoint(fid, pid, X, Y, Z);
    spFeatures.update2DPoint(fid, pid, lx, ly, rx, ry);
    spFeatures.setSelectedFeature(fid);
    spFeatures.setSelectedPoint(pid);
}

void SPManager::updatePoint(int fid, int pid, double lx, double ly, double rx,
                            double ry, double X, double Y, double Z)
{
    if (fid < 1 || fid > spFeatures.getNumFeatures()) {
        return;
    }

    if (pid < 1 || unsigned(pid) > spFeatures.getFeature(fid).points.size()) {
        return;
    }

    spFeatures.updatePoint(fid, pid, X, Y, Z);
    spFeatures.update2DPoint(fid, pid, lx, ly, rx, ry);
}

void SPManager::setSelectedXYZ(double X, double Y, double Z)
{
    int fid, pid;
    spFeatures.getNearestPoint(X, Y, Z, fid, pid);
    spFeatures.setSelectedFeature(fid);
    spFeatures.setSelectedPoint(pid);
}

ObjectSpaceCoordinate SPManager::getBoundingBoxCenter()
{
    double x[8], y[8];
    double xmax, ymax, xmin, ymin;
    double zm = manager_->instanceTerrain()->getMeanAltitude();
    x[0] = prL.imageToObject(0, 0, zm, false).getPosition().get(1);
    x[1] = prL.imageToObject(0, leftImage->getHeight(), zm,
                             false).getPosition().get(1);
    x[2] = prL.imageToObject(leftImage->getWidth(), 0, zm,
                             false).getPosition().get(1);
    x[3] = prL.imageToObject(leftImage->getWidth(), leftImage->getHeight(), zm,
                             false).getPosition().get(1);
    x[4] = prR.imageToObject(0, 0, zm, false).getPosition().get(1);
    x[5] = prR.imageToObject(0, rightImage->getHeight(), zm,
                             false).getPosition().get(1);
    x[6] = prR.imageToObject(rightImage->getWidth(), 0, zm,
                             false).getPosition().get(1);
    x[7] = prR.imageToObject(rightImage->getWidth(), rightImage->getHeight(), zm,
                             false).getPosition().get(1);
    y[0] = prL.imageToObject(0, 0, zm, false).getPosition().get(2);
    y[1] = prL.imageToObject(0, leftImage->getHeight(), zm,
                             false).getPosition().get(2);
    y[2] = prL.imageToObject(leftImage->getWidth(), 0, zm,
                             false).getPosition().get(2);
    y[3] = prL.imageToObject(leftImage->getWidth(), leftImage->getHeight(), zm,
                             false).getPosition().get(2);
    y[4] = prR.imageToObject(0, 0, zm, false).getPosition().get(2);
    y[5] = prR.imageToObject(0, rightImage->getHeight(), zm,
                             false).getPosition().get(2);
    y[6] = prR.imageToObject(rightImage->getWidth(), 0, zm,
                             false).getPosition().get(2);
    y[7] = prR.imageToObject(rightImage->getWidth(), rightImage->getHeight(), zm,
                             false).getPosition().get(2);
    xmax = xmin = x[0];
    ymax = ymin = y[0];

    for (int i = 1; i < 8; i++) {
        if (x[i] > xmax) {
            xmax = x[i];
        }

        if (x[i] < xmin) {
            xmin = x[i];
        }

        if (y[i] > ymax) {
            ymax = y[i];
        }

        if (y[i] < ymin) {
            ymin = y[i];
        }
    }

    const double xm = (xmax + xmin) / 2.0;
    const double ym = (ymax + ymin) / 2.0;
    return ObjectSpaceCoordinate("m", xm, ym, zm);
}

double SPManager::getBoundingBoxIdealZoom(int width, int height)
{

    ImageSpaceCoordinate leftCenter = getLeftPoint(getBoundingBoxCenter());
    ImageSpaceCoordinate rightCenter = getRightPoint(getBoundingBoxCenter());
    double dx, dy;
    double xmax, ymax;

    if (leftCenter.getPosition().get(1) < rightCenter.getPosition().get(1)) {
        dx = rightCenter.getPosition().get(1) - leftCenter.getPosition().get(1);

        if (leftImage->getWidth() + dx > rightImage->getWidth()) {
            xmax = leftImage->getWidth() + dx;
        } else {
            xmax = rightImage->getWidth();
        }
    } else {
        dx = leftCenter.getPosition().get(1) - rightCenter.getPosition().get(1);

        if (leftImage->getWidth() > rightImage->getWidth() + dx) {
            xmax = leftImage->getWidth();
        } else {
            xmax = rightImage->getWidth() + dx;
        }
    }

    if (leftCenter.getPosition().get(2) < rightCenter.getPosition().get(2)) {
        dy = rightCenter.getPosition().get(2) - leftCenter.getPosition().get(2);

        if (leftImage->getHeight() + dy > rightImage->getHeight()) {
            ymax = leftImage->getHeight() + dy;
        } else {
            ymax = rightImage->getHeight();
        }
    } else {
        dy = leftCenter.getPosition().get(2) - rightCenter.getPosition().get(2);

        if (leftImage->getHeight() > rightImage->getHeight() + dy) {
            ymax = leftImage->getHeight();
        } else {
            ymax = rightImage->getHeight() + dy;
        }
    }

    const double wscale = width / xmax;
    const double hscale = height / ymax;

    return wscale < hscale ? wscale : hscale;
}

// Esse método calcula o centro planimétrico na linha de voo entre duas imagens
ObjectSpaceCoordinate SPManager::getCentralPoint()
{
    const double xl = leftImage->getEO()->getXa().get(1, 1);
    const double yl = leftImage->getEO()->getXa().get(2, 1);
    const double xr = rightImage->getEO()->getXa().get(1, 1);
    const double yr = rightImage->getEO()->getXa().get(2, 1);
    const double zm = manager_->instanceTerrain()->getMeanAltitude();
    const double xm = (xl + xr) / 2.0;
    const double ym = (yl + yr) / 2.0;
    return ObjectSpaceCoordinate("m", xm, ym, zm);
}

ImageSpaceCoordinate SPManager::getLeftPoint(ObjectSpaceCoordinate coord)
{
    return ImageSpaceCoordinate(prL.objectToImage(coord, false));
}

ImageSpaceCoordinate SPManager::getRightPoint(ObjectSpaceCoordinate coord)
{
    return ImageSpaceCoordinate(prR.objectToImage(coord, false));
}

// Internal function. Pos from 0 - N-1.
void SPManager::getImagesId(int pos, int& left, int& right)
{
    // Check pos
    left = -1;
    right = -1;

    if (pos < 0 || unsigned(pos) > listPairs.size() - 1) {
        return;
    }

    // Decode
    int no_imgs = listAllImages.size();
    left = 1 + (listPairs.at(pos) % no_imgs);
    right = 1 + (listPairs.at(pos) / no_imgs);
}

void SPManager::changePair(int pos, int& lk, int& rk)
{
    getImagesId(pos, leftKey, rightKey);
    lk = leftKey;
    rk = rightKey;
    updateProjections();
}

void SPManager::addGeometryToXML(std::string filename)
{
    std::stringstream add;
    add << "<featuresFilename>";
    add << filename;
    add << "</featuresFilename>";
    EDomElement newXml(manager_->xmlGetData());

    if (newXml.elementByTagName("features").getContent() == "") {
        newXml.addChildAtTagName("efotoPhotogrammetricProject",
                                 "<features>\n</features>");
    }

    newXml.addChildAtTagName("features", add.str());
    manager_->xmlSetData(newXml.getContent());
    manager_->setSavedState(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
