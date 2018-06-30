/**************************************************************************
          IOManager.cpp
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

// Constructors and destructors
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

IOManager::IOManager():IOManager(nullptr,
                                 nullptr,
                                 nullptr,
                                 nullptr)
{
}

IOManager::IOManager(EFotoManager* manager,
                     Sensor* mySensor,
                     Image* myImage,
                     InteriorOrientation* myIO):
    manager_{manager},
    mySensor_{mySensor},
    myImage_{myImage},
    myIO_{myIO},
    myInterface_{nullptr},
    started_{false},
    status_{false}
{
}

// Other Methods
PositionMatrix IOManager::getAnalogMarks()
{
    if (mySensor_ != NULL) {
        if (mySensor_->is("SensorWithFiducialMarks")) {
            SensorWithFiducialMarks* mySensorWithFiducialMarks = dynamic_cast<SensorWithFiducialMarks*>
                    (mySensor_);
            return PositionMatrix(mySensorWithFiducialMarks->getLb());
        }

        if (mySensor_->is("SensorWithKnowDimensions")) {
            SensorWithKnowDimensions* mySensorWithKnowDimensions =
                dynamic_cast<SensorWithKnowDimensions*>(mySensor_);
            return PositionMatrix(mySensorWithKnowDimensions->forgeLb());
        }
    }

    return PositionMatrix();
}

bool IOManager::measureMark(int id, double col, double lin)
{
    if (started_) {
        ImageFiducialMark* myMark = new ImageFiducialMark(id, myImage_->getId(), col,
                lin);
        myImage_->deleteDigFidMark(id);
        myImage_->putDigFidMark(*myMark);
        return true;
    }

    return false;
}

unsigned int IOManager::countMarks() const
{
    return myImage_->countDigFidMarks();
}

std::deque<std::string> IOManager::markData(int index)
{
    std::deque<std::string> result;

    if (started_) {
        PositionMatrix analogMarks = getAnalogMarks();
        result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 1), 3));
        result.push_back(Conversion::doubleToString(analogMarks.get(2 * index + 2), 3));
        ImageFiducialMark myMark = myImage_->getDigFidMark(index + 1);

        if (myMark.isAvailable()) {
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

int IOManager::getCalculationMode() const
{
    std::string mode = mySensor_->getCalculationMode();
    return mode == "With Fiducial Marks" ? 1 : mode == "With Sensor Dimensions" ?
           2 : mode == "Fixed Parameters" ? 3 : 0;
}

bool IOManager::calculateIO()
{
    if (started_) {
        if (mySensor_->is("SensorWithFiducialMarks")) {
            SensorWithFiducialMarks* mySensorWithFiducialMarks =
                    dynamic_cast<SensorWithFiducialMarks*>(mySensor_);

            if ((unsigned int) myImage_->countDigFidMarks() == (unsigned int)
                    mySensorWithFiducialMarks->getLb().getRows() /
                    2) { // Comparação entre o número de marcas digitais e analógicas. "mySensorWithFiducialMarks->getLb()->getCols() / 2" dá o número de marcas analógicas validadas, tendo Lb sido carregado direto ou através das marcas.
                myIO_->calculate();
                status_ = true;
            }
        } else if (mySensor_->is("SensorWithKnowDimensions")) {
            myIO_->calculate();
            status_ = true;
        } else if (mySensor_->is("SensorWithKnowParameters")) {
            myIO_->calculate();
            status_ = true;
        }
    }

    return status_;
}

bool IOManager::interiorDone() const
{
    if (myIO_->getXa().getRows() != 6) {
        return false;
    }

    return true;
}

std::deque<std::string> IOManager::makeReport()
{
    std::deque<std::string> result;
    result.push_back(myIO_->getXa().xmlGetData());
    result.push_back(myIO_->getLa().xmlGetData());
    result.push_back(Conversion::doubleToString(
                         myIO_->getQuality().getsigma0Squared()));
    result.push_back(myIO_->getQuality().getV().xmlGetData());
    result.push_back(myIO_->getQuality().getSigmaXa().xmlGetData());
    result.push_back(myIO_->getQuality().getSigmaLa().xmlGetData());
    return result;
}

bool IOManager::exec()
{
    if (manager_ != NULL && mySensor_ != NULL && myImage_ != NULL && myIO_ != NULL)
        if (myImage_->getSensorId() == mySensor_->getId()
                && myIO_->getImageId() == myImage_->getId()) {
            if (manager_->getInterfaceType().compare("Qt") == 0) {
                //myInterface = new IOUserInterface_Qt(this);
                myInterface_ = IOUserInterface_Qt::instance(this);
            }

            myImage_->setSensor(mySensor_);
            myIO_->setImage(myImage_);
            started_ = true;

            if (myInterface_ != NULL) {
                myInterface_->exec();
            }

            //if (manager->getInterfaceType().compare("Qt") == 0)
            //{
            //IOUserInterface_Qt::createInstance(this)->exec();
            //}
        }

    return status_;
}

int IOManager::getId() const
{
    if (myImage_ != NULL) {
        return myImage_->getId();
    }

    return 0;
}

void IOManager::returnProject()
{
    manager_->reloadProject();
}

bool IOManager::save(std::string path)
{
    if (started_) {
        FILE* pFile;
        std::string output = "IO state data for Image " + Conversion::intToString(
                                 myImage_->getId()) + "\n\n";
        output += mySensor_->xmlGetData();
        output += "\n";
        output += myImage_->xmlGetData();
        output += "\n";
        output += myIO_->xmlGetData();
        output += "\n";
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

bool IOManager::load(std::string path)
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

        if ((unsigned int) result != (unsigned int) lSize) {
            fputs ("Reading error", stderr);
            exit (3);
        }

        std::string strxml(buffer);
        EDomElement xml(strxml);
        mySensor_->xmlSetData(xml.elementByTagName("Sensor").getContent());
        myImage_->xmlSetData(xml.elementByTagName("image").getContent());
        myIO_->xmlSetData(xml.elementByTagName("imageIO").getContent());
        fclose (pFile);
        free (buffer);
    }

    return false;
}

std::string IOManager::getImageFile() const
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

int IOManager::getFrameRows()
{
    int rows = 0;

    if (mySensor_->is("SensorWithKnowDimensions")) {
        SensorWithKnowDimensions* sensor = dynamic_cast<SensorWithKnowDimensions*>(mySensor_);
        rows = sensor->getFrameRows();
    }

    return rows;
}

int IOManager::getFrameColumns()
{
    int cols = 0;

    if (mySensor_->is("SensorWithKnowDimensions")) {
        SensorWithKnowDimensions* sensor = dynamic_cast<SensorWithKnowDimensions*>(mySensor_);
        cols = sensor->getFrameColumns();
    }

    return cols;
}

void IOManager::acceptIO()
{
    EDomElement newXml(manager_->xmlGetData());

    if (newXml.elementByTagAtt("imageIO", "image_key",
                               Conversion::intToString(myImage_->getId())).getContent() != "") {
        newXml.replaceChildByTagAtt("imageIO", "image_key",
                                    Conversion::intToString(myImage_->getId()), myIO_->xmlGetData());
    } else {
        newXml.addChildAtTagName("interiorOrientation", myIO_->xmlGetData());
    }

    manager_->xmlSetData(newXml.getContent());
    manager_->setSavedState(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
