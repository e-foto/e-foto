/**************************************************************************
          SRManager.cpp
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
#include "SRManager.h"
#include "EFotoManager.h"
#include "EDom.h"
#include "Terrain.h"
#include "SensorWithFiducialMarks.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "SpatialRessection.h"
#include "SRUserInterface_Qt.h"
#include "Flight.h"

#include <fstream>

// Constructors and destructors
//
namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SRManager::SRManager():
    SRManager(nullptr,
              nullptr,
              nullptr,
              nullptr,
              nullptr,
              nullptr,
              nullptr,
              std::deque<Point*>())
{
}

SRManager::SRManager(EFotoManager* manager,
                     Terrain* myTerrain,
                     Sensor* mySensor,
                     Flight* myFlight,
                     Image* myImage,
                     InteriorOrientation* myIO,
                     SpatialRessection* mySR,
                     std::deque<Point*> myPoints):
    manager_{manager},
    myTerrain_{myTerrain},
    mySensor_{mySensor},
    myFlight_{myFlight},
    myImage_{myImage},
    myIO_{myIO},
    mySR_{mySR},
    myPoints_{myPoints},
    myInterface_{nullptr},
    started_{false}
{
}

SRManager::~SRManager()
{
}

bool SRManager::measurePoint(int id, double col, double lin)
{
    if (started_) {
        Point* pointToMeasure = myImage_->getPoint(id);

        if (pointToMeasure == NULL) {
            pointToMeasure = manager_->point(id);
            myImage_->putPoint(pointToMeasure);
        }

        pointToMeasure->putImageCoordinate(ImageSpaceCoordinate(myImage_->getId(), col,
                                           lin));
        mySR_->selectPoint(id);
        /* ERRO: ponto fora da imagem. */
        return true;
    }

    return false;
}

void SRManager::selectPoint(int id)
{
    if (started_) {
        mySR_->selectPoint(id);
    }
}

void SRManager::unselectPoint(int id)
{
    if (started_) {
        mySR_->unselectPoint(id);
    }
}

std::deque<double> SRManager::pointToDetector(double col, double lin)
{
    std::deque<double> result;
    result.push_back(myIO_->imageToDetector(col, lin).getXi());
    result.push_back(myIO_->imageToDetector(col, lin).getEta());
    return result;
}

bool SRManager::removePoint(int id)
{
    if (started_) {
        Point* pointToRemove = myImage_->getPoint(id);

        if (pointToRemove != NULL) {
            mySR_->unselectPoint(id);
        }

        /* ERRO: ponto fora da imagem. */
        return true;
    }

    return false;
}

std::deque<std::string> SRManager::listSelectedPoints()
{
    std::deque<std::string> result;

    if (started_) {
        std::deque<int> selectedPoints = mySR_->getSelectedPoints();

        for (unsigned int i = 0; i < selectedPoints.size(); i++) {
            Point* myPoint = myImage_->getPoint(selectedPoints.at(i));
            std::string value = Conversion::intToString(myPoint->getId());
            value += " ";
            value += myPoint->getPointId();
            value += " ";
            value += myPoint->getDescription();
            result.push_back(value);
        }
    }

    return result;
}

std::deque<std::string> SRManager::listAllPoints()
{
    std::deque<std::string> result;

    if (started_) {
        EDomElement xmlPoints(manager_->getXml("points"));
        std::deque<EDomElement> allPoints = xmlPoints.children();

        for (unsigned int i = 0; i < allPoints.size(); i++) {
            std::string data = allPoints.at(i).getContent();

            if (data != "") {
                std::string value = allPoints.at(i).attribute("key");
                value += " ";
                value += allPoints.at(i).elementByTagName("pointId").toString();
                value += " ";
                value += allPoints.at(i).elementByTagName("description").toString();
                result.push_back(value);
            }
        }
    }

    return result;
}

std::deque<std::string> SRManager::pointData(int index)
{
    std::deque<std::string> result;

    if (started_) {
        Point* myPoint = myPoints_.at(index);
        result.push_back(Conversion::intToString(myPoint->getId()));
        result.push_back(myPoint->getType() == Point::CONTROL ? "true" : "false");
        result.push_back(myPoint->getPointId());
        result.push_back(myPoint->getDescription());
        result.push_back(Conversion::doubleToString(
                             myPoint->getObjectCoordinate().getX(), 3));
        result.push_back(Conversion::doubleToString(
                             myPoint->getObjectCoordinate().getY(), 3));
        result.push_back(Conversion::doubleToString(
                             myPoint->getObjectCoordinate().getZ(), 3));

        if (myPoint->hasImageCoordinate(myImage_->getId())
                && myPoint->getImageCoordinate(myImage_->getId()).isAvailable()) {
            result.push_back(Conversion::doubleToString(myPoint->getImageCoordinate(
                                 myImage_->getId()).getCol()));
            result.push_back(Conversion::doubleToString(myPoint->getImageCoordinate(
                                 myImage_->getId()).getLin()));
            DetectorSpaceCoordinate aisc = myImage_->getIO()->imageToDetector(
                                               myPoint->getImageCoordinate(myImage_->getId()));
            result.push_back(Conversion::doubleToString(aisc.getXi(), 3));
            result.push_back(Conversion::doubleToString(aisc.getEta(), 3));
        }
    }

    return result;
}

unsigned int SRManager::countSelectedPoints() const
{
    return mySR_->getSelectedPoints().size();
}

bool SRManager::connectImagePoints()
{
    if (!(started_)) { /* Sim, esse método é executado antes do módulo ser iniciado, e não deve ser executado depois. */
        EDomElement xmlPoints(manager_->getXml("points"));
        std::deque<EDomElement> allPoints = xmlPoints.children();

        for (unsigned int i = 0; i < allPoints.size(); i++) {
            std::string data = allPoints.at(i).elementByTagAtt("imageCoordinates",
                               "image_key", Conversion::intToString(myImage_->getId())).getContent();

            if (data != "") {
                Point* pointToInsert = manager_->instancePoint(Conversion::stringToInt(
                                           allPoints.at(i).attribute("key")));

                if (pointToInsert != NULL) {
                    myImage_->putPoint(pointToInsert);
                }
            }
        }

        return true;
    }

    return false;
}

bool SRManager::updatePointsTable() const
{
    if (started_) {
        return true;
    }

    return false;
}

bool SRManager::flightDirection(int MarkId)
{
    if (started_) {
        mySR_->selectFiducialMarkForFlightDirection(MarkId);
        return true;
    }

    return false;
}

bool SRManager::flightDirection(double col, double lin)
{
    if (started_) {
        mySR_->setPointForFlightDirection(col, lin);
        return true;
    }

    return false;
}

bool SRManager::flightDirection(double kappa0)
{
    //if (started_)
    {
        mySR_->setFlightDirection(kappa0);
        return true;
    }
    return false;
}

bool SRManager::calculateSR(int iterations, double gnssPrecision,
                            double insPrecision)
{
    if (started_) {
        if (mySR_->countSelectedPoints() > 3) {
            mySR_->initialize();
            mySR_->calculate(iterations, gnssPrecision, insPrecision);
        }

        return true;
    }

    return false;
}

bool SRManager::exteriorDone() const
{
    if (mySR_->getXa().getRows() != 6) {
        return false;
    }

    return true;
}

std::deque<std::string> SRManager::makeReport()
{
    // Modificado em 27/06/2011 a pedido do Prof Nunes para exibir La e Sigma La se existirem. A saber o codigo anterior exibia Lb e sigma de Lb.
    std::deque<std::string> result;
    result.push_back(mySR_->getXa().xmlGetData());
    result.push_back(mySR_->getLa().xmlGetData());
    result.push_back(Conversion::doubleToString(
                         myIO_->getQuality().getsigma0Squared()));
    result.push_back(mySR_->getQuality().getV().xmlGetData());
    result.push_back(mySR_->getQuality().getSigmaXa().xmlGetData());
    result.push_back(mySR_->getQuality().getSigmaLa().xmlGetData());
    result.push_back(Conversion::intToString(mySR_->getTotalIterations()));

    if (mySR_->getConverged()) {
        result.push_back("yes");
    } else {
        result.push_back("no");
    }

    return result;
}

bool SRManager::exec()
{
    if (manager_ != NULL && mySensor_ != NULL && myFlight_ != NULL && myImage_ != NULL
            && myIO_ != NULL && mySR_ != NULL)
        if (myImage_->getSensorId() == mySensor_->getId()
                && myImage_->getFlightId() == myFlight_->getId() &&
                myFlight_->getSensorId() == mySensor_->getId()
                && myIO_->getImageId() == myImage_->getId() &&
                mySR_->getImageId() == myImage_->getId()) {
            if (manager_->getInterfaceType().compare("Qt") == 0) {
                //myInterface_ = new SRUserInterface_Qt(this);
                myInterface_ = SRUserInterface_Qt::instance(this);
            }

            myImage_->setSensor(mySensor_);
            myImage_->setFlight(myFlight_);
            myImage_->setIO(myIO_);
            myFlight_->setSensor(mySensor_);
            myIO_->setImage(myImage_);
            mySR_->setImage(myImage_);
            connectImagePoints();
            started_ = true;

            if (myInterface_ != NULL) {
                myInterface_->exec();
            }
        }

    return true;
}

int SRManager::getId() const
{
    if (myImage_ != NULL) {
        return myImage_->getId();
    }

    return 0;
}

void SRManager::returnProject()
{
    manager_->reloadProject();
}

bool SRManager::save(std::string path)
{
    if (started_) {
        FILE* pFile;
        std::string output = "IO state data for Image " + Conversion::intToString(
                                 myImage_->getId()) + "\n\n";
        output += mySensor_->xmlGetData();
        output += "\n";
        output += myFlight_->xmlGetData();
        output += "\n";
        output += myImage_->xmlGetData();
        output += "\n";
        output += myIO_->xmlGetData();
        output += "\n";
        output += mySR_->xmlGetData();
        output += "\n";

        for (unsigned int i = 0; i < myPoints_.size(); i++) {
            output += myPoints_.at(i)->xmlGetData();
            output += "\n";
        }

        EDomElement e(output);
        output = e.indent('\t').getContent();
        const char* buffer = output.c_str();
        pFile = fopen (path.c_str(), "wb");
        fwrite (buffer, 1, output.length(), pFile);
        fclose (pFile);
        return true;
    }

    return false;
}

bool SRManager::load(std::string path)
{
    if (started_) {
        FILE* pFile;
        long lSize;
        char* buffer;
        size_t result;
        pFile = fopen (path.c_str(), "rb");

        if (pFile == NULL) {
            fputs ("File error", stderr);
            exit (1);
        }

        fseek (pFile , 0 , SEEK_END);
        lSize = ftell (pFile);
        rewind (pFile);
        buffer = (char*) malloc (sizeof(char) * lSize);

        if (buffer == NULL) {
            fputs ("Memory error", stderr);
            exit (2);
        }

        result = fread (buffer, 1, lSize, pFile);

        if (result != size_t(lSize)) {
            fputs ("Reading error", stderr);
            exit (3);
        }

        std::string strxml(buffer);
        EDomElement xml(strxml);
        mySensor_->xmlSetData(xml.elementByTagName("Sensor").getContent());
        myFlight_->xmlSetData(xml.elementByTagName("Flight").getContent());
        myImage_->xmlSetData(xml.elementByTagName("image").getContent());
        myIO_->xmlSetData(xml.elementByTagName("imageIO").getContent());
        mySR_->xmlSetData(xml.elementByTagName("imageEO").getContent());
        std::deque<EDomElement> xmlPoints = xml.elementsByTagName("Point");

        for (unsigned int i = 0; i < xmlPoints.size(); i++) {
        }

        fclose (pFile);
        free (buffer);
    }

    return false;
}

std::string SRManager::getImageFile() const
{
    if (myImage_->getFilepath() == ".") {
        return myImage_->getFilename();
    } else {
        std::string result = "";
        result += myImage_->getFilepath();
        result += "/";
        result += myImage_->getFilename();
        return result;
    }
}

void SRManager::acceptSR()
{
    EDomElement newXml(manager_->xmlGetData());

    if (newXml.elementByTagName("spatialResections").getContent() == "") {
        newXml.addChildAtTagName("efotoPhotogrammetricProject",
                                 "<spatialResections>\n</spatialResections>");
    }

    if (newXml.elementByTagAtt("imageEO", "image_key",
                               Conversion::intToString(myImage_->getId())).getContent() != "") {
        newXml.replaceChildByTagAtt("imageEO", "image_key",
                                    Conversion::intToString(myImage_->getId()), mySR_->xmlGetDataEO());
    } else {
        newXml.addChildAtTagName("exteriorOrientation", mySR_->xmlGetDataEO());
    }

    if (newXml.elementByTagAtt("imageSR", "image_key",
                               Conversion::intToString(myImage_->getId())).getContent() != "") {
        newXml.replaceChildByTagAtt("imageSR", "image_key",
                                    Conversion::intToString(myImage_->getId()), mySR_->xmlGetData());
    } else {
        newXml.addChildAtTagName("spatialResections", mySR_->xmlGetData());
    }

    newXml.replaceChildByTagAtt("image", "key",
                                Conversion::intToString(myImage_->getId()), myImage_->xmlGetData());

    for (int i = 0; i < myImage_->countPoints(); i++) {
        int currentPointId = myImage_->getPointAt(i)->getId();
        newXml.replaceChildByTagAtt("point", "key",
                                    Conversion::intToString(currentPointId), myImage_->getPointAt(i)->xmlGetData());
    }

    manager_->xmlSetData(newXml.getContent());
    manager_->setSavedState(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
