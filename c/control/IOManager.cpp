/**************************************************************************
		  IOManager.cpp
**************************************************************************/
/*Copyright 2002-2021 e-foto team (UERJ)
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
#include "IOManager.h"
#include "EFotoManager.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "IOUserInterface.h"
#include "IOUserInterface_Qt.h"

#include <fstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


IOManager::IOManager(EFotoManager* newmanager,
                     Sensor* newmySensor,
                     Image* newmyImage,
                     InteriorOrientation* newmyIO):
    manager{newmanager},
    mySensor{newmySensor},
    myImage{newmyImage},
    myIO{newmyIO}
{
}

IOManager::~IOManager()
{
}

PositionMatrix IOManager::getAnalogMarks()
{
    if (mySensor != NULL) {
        if (mySensor->is("SensorWithFiducialMarks")) {
            SensorWithFiducialMarks* mySensorWithFiducialMarks =
                static_cast<SensorWithFiducialMarks*> (mySensor);
            return PositionMatrix(mySensorWithFiducialMarks->getLb());
        }

        if (mySensor->is("SensorWithKnowDimensions")) {
            SensorWithKnowDimensions* mySensorWithKnowDimensions =
                static_cast<SensorWithKnowDimensions*>(mySensor);
            return PositionMatrix(mySensorWithKnowDimensions->forgeLb());
        }
    }

    return PositionMatrix();
}

bool IOManager::measureMark(int id, double col, double lin)
{
    if (started) {
        ImageFiducialMark* myMark = new ImageFiducialMark(id, myImage->getId(), col,
                lin);
        myImage->deleteDigFidMark(id);
        myImage->putDigFidMark(*myMark);
        return true;
    }

    return false;
}

unsigned int IOManager::countMarks()
{
    return myImage->countDigFidMarks();
}

std::deque<std::string> IOManager::markData(int index)
{
    std::deque<std::string> result;

    if (started) {
        PositionMatrix analogMarks = getAnalogMarks();
        result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 1), 3));
        result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 2), 3));
        ImageFiducialMark myMark = myImage->getDigFidMark(index + 1);

        if (myMark.available) {
            result.push_back(Conversion::doubleToString(myMark.getCol()));
            result.push_back(Conversion::doubleToString(myMark.getLin()));
        }
    }

    return result;
}

unsigned int IOManager::getTotalMarks()
{
    return getAnalogMarks().getRows() / 2;
}

int IOManager::getCalculationMode()
{
    std::string mode = mySensor->getCalculationMode();
    return mode == "With Fiducial Marks" ? 1 : mode == "With Sensor Dimensions" ?
           2 : mode == "Fixed Parameters" ? 3 : 0;
}

bool IOManager::calculateIO()
{
    if (started) {
        if (mySensor->is("SensorWithFiducialMarks")) {
            SensorWithFiducialMarks* mySensorWithFiducialMarks =
                static_cast<SensorWithFiducialMarks*>(mySensor);

            if ((unsigned int) myImage->countDigFidMarks() == (unsigned int)
                    mySensorWithFiducialMarks->getLb().getRows() / 2) {
                // Comparação entre o número de marcas digitais e analógicas.
                // "mySensorWithFiducialMarks->getLb()->getCols() / 2" dá o
                // número de marcas analógicas validadas, tendo Lb sido carregado
                // direto ou através das marcas.
                myIO->calculate();
                status = true;
            }
        }
        else if (mySensor->is("SensorWithKnowDimensions")) {
            myIO->calculate();
            status = true;
        }
        else if (mySensor->is("SensorWithKnowParameters")) {
            myIO->calculate();
            status = true;
        }
    }

    return status;
}

bool IOManager::interiorDone()
{
    if (myIO->getXa().getRows() != 6) {
        return false;
    }

    return true;
}

std::deque<std::string> IOManager::makeReport()
{
    std::deque<std::string> result;
    result.push_back(myIO->getXa().xmlGetData());
    result.push_back(myIO->getLa().xmlGetData());
    result.push_back(Conversion::doubleToString(
                         myIO->getQuality().getsigma0Squared()));
    result.push_back(myIO->getQuality().getV().xmlGetData());
    result.push_back(myIO->getQuality().getSigmaXa().xmlGetData());
    result.push_back(myIO->getQuality().getSigmaLa().xmlGetData());
    return result;
}

bool IOManager::exec()
{
    if (manager != NULL && mySensor != NULL && myImage != NULL && myIO != NULL)
        if (myImage->getSensorId() == mySensor->getId()
                && myIO->getImageId() == myImage->getId()) {
            if (manager->getInterfaceType().compare("Qt") == 0) {
                myInterface = IOUserInterface_Qt::instance(this);
            }

            myImage->setSensor(mySensor);
            myIO->setImage(myImage);
            started = true;

            if (myInterface != NULL) {
                myInterface->exec();
            }
        }

    return status;
}

int IOManager::getId()
{
    if (myImage != NULL) {
        return myImage->getId();
    }

    return 0;
}

void IOManager::returnProject()
{
    manager->reloadProject();
}

std::string IOManager::getImageFile()
{
    if (myImage->getFilepath() == ".") {
        return myImage->getFilename();
    }
    else {
        std::string result = "";
        result += myImage->getFilepath();
        result += "/";
        result += myImage->getFilename();
        return result;
    }
}

int IOManager::getFrameRows()
{
    int rows = 0;

    if (mySensor->is("SensorWithKnowDimensions")) {
        SensorWithKnowDimensions* sensor = static_cast<SensorWithKnowDimensions*>
                                           (mySensor);
        rows = sensor->getFrameRows();
    }

    return rows;
}

int IOManager::getFrameColumns()
{
    int cols = 0;

    if (mySensor->is("SensorWithKnowDimensions")) {
        SensorWithKnowDimensions* sensor = static_cast<SensorWithKnowDimensions*>(mySensor);
        cols = sensor->getFrameColumns();
    }

    return cols;
}

void IOManager::acceptIO()
{
    EDomElement newXml(manager->xmlGetData());

    if (newXml.elementByTagAtt("imageIO", "image_key",
                               Conversion::intToString(myImage->getId())).getContent() != "") {
        newXml.replaceChildByTagAtt("imageIO", "image_key",
                                    Conversion::intToString(myImage->getId()), myIO->xmlGetData());
    }
    else {
        newXml.addChildAtTagName("interiorOrientation", myIO->xmlGetData());
    }

    manager->xmlSetData(newXml.getContent());
    manager->setSavedState(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
