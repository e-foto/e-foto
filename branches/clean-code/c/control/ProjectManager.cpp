/**************************************************************************
ProjectManager.cpp
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
#include "Image.h"
#include "Terrain.h"
#include "ProjectManager.h"
#include "EFotoManager.h"
#include "ProjectUserInterface_Qt.h"
#include "Project.h"
#include "ETreeModel.h"
#include "SensorWithFiducialMarks.h"
#include "SpatialRessection.h"
#include "InteriorOrientation.h"
#include "Flight.h"
#include <QSettings>

#include <sstream>
#include <fstream>
#include <iostream>
//#include <math.h>

// Constructors and Destructor
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectManager::ProjectManager():ProjectManager(nullptr)
{
}

ProjectManager::ProjectManager(EFotoManager* manager):
    manager_{manager},
    treeModel_{nullptr},
    myInterface_{nullptr},
    updater_{nullptr},
    savedIn{""}
{
}

ProjectManager::~ProjectManager()
{
    if (treeModel_ != NULL) {
        delete treeModel_;
    }

    if (updater_ != NULL) {
        delete updater_;
    }
}

// Other Methods
bool ProjectManager::newProject(std::string filename)
{
    if (manager_ != NULL) {
        std::string xmlData = "";
        xmlData += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        xmlData += "<?xml-stylesheet type=\"text/xsl\" href=\"xsl/epp.xsl\"?>\n\n";
        xmlData +=
            "<efotoPhotogrammetricProject version=\"1.0.42\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
        xmlData += "xsi:noNamespaceSchemaLocation=\"EPPSchema/epp_structure.xsd\"\n";
        xmlData += "xmlns:gml=\"http://www.opengis.net/gml\"\n";
        xmlData += "xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";
        xmlData += "<projectHeader>\n";
        xmlData += "<name></name>\n";
        xmlData += "<description></description>\n";
        int i = filename.rfind('/');
        std::string filePath = "<filePath>" + filename.substr(0, i) + "</filePath>\n";
        std::string fileName = "<fileName>" + filename.erase(0,
                               i + 1) + "</fileName>\n";
        xmlData += fileName;
        xmlData += filePath;
        xmlData += "<creation></creation>\n";
        xmlData += "<modification></modification>\n";
        xmlData += "<owner></owner>\n";
        xmlData += "<aims></aims>\n";
        xmlData += "<context></context>\n";
        xmlData += "</projectHeader>\n";
        xmlData += "<terrain>\n";
        xmlData += "<meanAltitude uom=\"#m\"></meanAltitude>\n";
        xmlData += "<minAltitude uom=\"#m\"></minAltitude>\n";
        xmlData += "<maxAltitude uom=\"#m\"></maxAltitude>\n";
        xmlData += "<GRS>WGS84</GRS>\n";
        xmlData += "<CPS>UTM</CPS>\n";
        xmlData += "<workAreaCenterCoordinates>\n";
        xmlData += "<Lat direction=\"S\">\n";
        xmlData += "<degrees></degrees>\n";
        xmlData += "<minutes></minutes>\n";
        xmlData += "<seconds></seconds>\n";
        xmlData += "</Lat>\n";
        xmlData += "<Long direction=\"W\">\n";
        xmlData += "<degrees></degrees>\n";
        xmlData += "<minutes></minutes>\n";
        xmlData += "<seconds></seconds>\n";
        xmlData += "</Long>\n";
        xmlData += "<utmFuse></utmFuse>\n";
        xmlData += "</workAreaCenterCoordinates>\n";
        xmlData += "</terrain>\n";
        xmlData += "<sensors>\n";
        xmlData += "</sensors>\n";
        xmlData += "<flights>\n";
        xmlData += "</flights>\n";
        xmlData += "<points>\n";
        xmlData += "</points>\n";
        xmlData += "<images>\n";
        xmlData += "</images>\n";
        xmlData += "<interiorOrientation>\n";
        xmlData += "</interiorOrientation>\n";
        xmlData += "<exteriorOrientation>\n";
        xmlData += "</exteriorOrientation>\n";
        xmlData += "</efotoPhotogrammetricProject>";
        manager_->xmlSetData(xmlData);

        if (treeModel_ != NULL) {
            delete treeModel_;
        }

        treeModel_ = new ETreeModel(EDomElement(
                                       xmlData).elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }

    return false;
}

bool ProjectManager::loadFile(const char* filename)
{
    if (manager_ != NULL) {
        std::stringstream myData;
        std::ifstream myFile(filename);

        if (updater_ != NULL) {
            delete updater_;
            updater_ = NULL;
        }

        if (myFile.is_open()) {
            std::string line;

            while (!myFile.eof()) {
                getline (myFile, line);
                myData << line << std::endl;
            }

            myFile.close();
            std::string xmlData = EDomElement(myData.str()).removeBlankLines(
                                      true).getContent();
            updater_ = new XmlUpdater(xmlData);

            if (updater_->isUpdated()) {
                xmlData = updater_->getAllXml().getContent();
                /* Paulo 6/3/12 : Se deu um update salva o arquivo automaticamente.*/
                manager_->xmlSetData(xmlData);
                //saveFile(filename);
            } else {
                return false;
            }

            // Aqui deve entrar um codigo para validar o XML.
            manager_->xmlSetData(xmlData);

            if (treeModel_ != NULL) {
                delete treeModel_;
            }

            treeModel_ = new ETreeModel(EDomElement(
                                           xmlData).elementByTagName("efotoPhotogrammetricProject").getContent());
            saveSettings(filename);
            return true;
        }

        return false;
    }

    return false;
}

bool ProjectManager::saveFile(std::string filename)
{
    if (manager_ != NULL) {
        std::ofstream myFile (filename.c_str());

        if (myFile.is_open()) {
            EDomElement xml(manager_->xmlGetData());
            //myFile << xml.indent('\t').getContent(); // O adequado é que em breve a linha a baixo possa ser substituida por esta aqui.
            myFile << xml.removeBlankLines(true).indent('\t').getContent();
            myFile.close();
            return true;
        } else {
            std::cout << "Unable to open file";
        }

        return false;
    }

    return false;
}

int ProjectManager::informFileVersionError() const
{
    if (manager_ != NULL && updater_ != NULL) {
        return updater_->getError();
    }

    return 0;
}

bool ProjectManager::addComponent(std::string data, std::string parent)
{
    if (manager_ != NULL) {
        EDomElement newXml(manager_->xmlGetData());
        newXml.addChildAtTagName(parent, data);
        manager_->xmlSetData(newXml.getContent());

        if (treeModel_ != NULL) {
            delete treeModel_;
        }

        treeModel_ = new ETreeModel(
            newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }

    return false;
}

bool ProjectManager::removeComponent(std::string type, int id)
{
    if (manager_ != NULL) {
        EDomElement newXml(manager_->xmlGetData());

        if (type == "Sensor") {
            newXml.replaceChildByTagAtt("sensor", "key", Conversion::intToString(id), "");
        } else if (type == "Flight") {
            newXml.replaceChildByTagAtt("flight", "key", Conversion::intToString(id), "");
        } else if (type == "Image") {
            newXml.replaceChildByTagAtt("image", "key", Conversion::intToString(id), "");
        } else if (type == "Point") {
            newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(id), "");
        } else if (type == "IO") {
            newXml.replaceChildByTagAtt("imageIO", "image_key", Conversion::intToString(id),
                                        "");
        } else if (type == "EO") {
            newXml.replaceChildByTagAtt("imageEO", "image_key", Conversion::intToString(id),
                                        "");
        }

        manager_->xmlSetData(newXml.getContent());

        if (treeModel_ != NULL) {
            delete treeModel_;
        }

        treeModel_ = new ETreeModel(
            newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }

    return false;
}

bool ProjectManager::editComponent(std::string type, std::string data)
{
    if (manager_ != NULL) {
        EDomElement newXml(manager_->xmlGetData());

        if (type == "Header") {
            newXml.replaceChildByTagName("projectHeader", data);
        } else if (type == "Terrain") {
            newXml.replaceChildByTagName("terrain", data);
        }

        manager_->xmlSetData(newXml.getContent());

        if (treeModel_ != NULL) {
            delete treeModel_;
        }

        treeModel_ = new ETreeModel(
            newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }

    return false;
}

bool ProjectManager::editComponent(std::string type, int id, std::string data)
{
    if (manager_ != NULL) {
        EDomElement newXml(manager_->xmlGetData());

        if (type == "Sensor") {
            newXml.replaceChildByTagAtt("sensor", "key", Conversion::intToString(id),
                                        data);
        } else if (type == "Flight") {
            newXml.replaceChildByTagAtt("flight", "key", Conversion::intToString(id),
                                        data);
        } else if (type == "Image") {
            newXml.replaceChildByTagAtt("image", "key", Conversion::intToString(id), data);
        } else if (type == "Point") {
            newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(id), data);
        } else if (type == "IO") {
            newXml.replaceChildByTagAtt("imageIO", "image_key", Conversion::intToString(id),
                                        data);
        } else if (type == "EO") {
            newXml.replaceChildByTagAtt("imageEO", "image_key", Conversion::intToString(id),
                                        data);
        }

        manager_->xmlSetData(newXml.getContent());

        if (treeModel_ != NULL) {
            delete treeModel_;
        }

        treeModel_ = new ETreeModel(
            newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
    }

    return false;
}

ETreeModel* ProjectManager::getTreeModel()
{
    return treeModel_;
}

std::deque<int> ProjectManager::listImageKeys()
{
    std::deque<int> result;

    for (unsigned int i = 0; i < treeModel_->getChild(4).countChildren(); i++) {
        result.push_back(treeModel_->idAt(4, i));
    }

    return result;
}

std::deque<std::string> ProjectManager::listImages()
{
    std::deque<std::string> result;

    for (unsigned int i = 0; i < treeModel_->getChild(4).countChildren(); i++) {
        result.push_back(treeModel_->dataAt(4, i));
    }

    return result;
}

int ProjectManager::getImageId(std::string imageName)
{
    for (unsigned int i = 0; i < treeModel_->getChild(4).countChildren(); i++)
        if (treeModel_->dataAt(4, i) == imageName) {
            return treeModel_->idAt(4, i);
        }

    return -1;
}

int ProjectManager::getFreeImageId()
{
    return manager_->getFreeImageId();
}

int ProjectManager::getFreePointId()
{
    return manager_->getFreePointId();
}

bool ProjectManager::startModule(std::string module, int image)
{
    if (manager_ != NULL) {
        if (module.compare("InteriorOrientation") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_IO);
        } else if (module.compare("SpatialRessection") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_SR);
        } else if (module.compare("FotoTriangulation") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_PT);
        } else if (module.compare("DEM-Extraction") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_DEM);
        } else if (module.compare("Ortho-rectification") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_ORTHO);
        } else if (module.compare("StereoPlotter") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_SP);
        } else if (module.compare("Report") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_Report);
        } else if (module.compare("PT-Report") == 0) {
            manager_->setNextModule(EFotoManager::NEXT_PTReport);
        }

        manager_->setNextImage(image);
        manager_->exec();
        return true;
    }

    return false;
}

bool ProjectManager::exec(std::string filename)
{
    if (manager_ != NULL) {
        if (manager_->getInterfaceType().compare("Qt") == 0) {
            ProjectUserInterface_Qt* newInterface = new ProjectUserInterface_Qt(this);

            if (filename != "") {
                newInterface->loadFile(filename);
            }

            myInterface_ = newInterface;
        }

        if (myInterface_ != NULL) {
            return myInterface_->exec();
        }

        return false;
    }

    return false;
}

bool ProjectManager::reload()
{
    if (manager_ != NULL) {
        if (myInterface_ != NULL) {
            manager_->getSavedState();
            return myInterface_->exec();
        }

        return false;
    }

    return false;
}

std::string ProjectManager::getXml(std::string tagname)
{
    return manager_->getXml(tagname);
}

std::string ProjectManager::getXml(std::string tagname, std::string att,
                                   std::string value)
{
    return manager_->getXml(tagname, att, value);
}

bool ProjectManager::getSavedState() const
{
    if (manager_ != NULL) {
        return manager_->getSavedState();
    }

    return true;
}

bool ProjectManager::makeSPFile(std::string filename, int image1, int image2)
{
    if (manager_ != NULL) {
        if (image1 == image2 || filename == "") {
            return false;
        }

        if (image1 == 0 || image2 == 0) {
            return false;
        }

        InteriorOrientation* io1 = manager_->instanceIO(image1);
        InteriorOrientation* io2 = manager_->instanceIO(image2);
        SpatialRessection* sr1 = dynamic_cast<SpatialRessection*>(manager_->instanceEO(image1));
        SpatialRessection* sr2 = dynamic_cast<SpatialRessection*>(manager_->instanceEO(image2));
        SensorWithFiducialMarks* sensor = dynamic_cast<SensorWithFiducialMarks*>
                                          (manager_->instanceSensor(manager_->instanceImage(image1)->getSensorId()));
        Flight* flight = manager_->instanceFlight(manager_->instanceImage(
                             image1)->getFlightId());
        Terrain* terrain = manager_->instanceTerrain();

        if (io1 == NULL || io2 == NULL || sr1 == NULL || sr2 == NULL || sensor == NULL
                || flight == NULL || terrain == NULL) {
            return false;
        }

        EDomElement xml(manager_->xmlGetData());
        std::string value = "";
        // O texto aqui de baixo esta escrito errado de proposito para ficar compativel com o teste que o Marcelo
        // programou no stereoplotter. La ele procura por "Mesure" quando o correto seria "Measure".
        // Isto e compativel com a versao 1.6 do modulo e devera mudar logo que ele corrija a sua parte.
        value += "Stereoscopic Visualization and Mesure Module Data\n";
        value += Conversion::doubleToString(io1->getXa().get(1, 1));
        value += "\n";
        value += Conversion::doubleToString(io1->getXa().get(2, 1));
        value += "\n";
        value += Conversion::doubleToString(io1->getXa().get(3, 1));
        value += "\n";
        value += Conversion::doubleToString(io1->getXa().get(4, 1));
        value += "\n";
        value += Conversion::doubleToString(io1->getXa().get(5, 1));
        value += "\n";
        value += Conversion::doubleToString(io1->getXa().get(6, 1));
        value += "\n";
        value += Conversion::doubleToString(io2->getXa().get(1, 1));
        value += "\n";
        value += Conversion::doubleToString(io2->getXa().get(2, 1));
        value += "\n";
        value += Conversion::doubleToString(io2->getXa().get(3, 1));
        value += "\n";
        value += Conversion::doubleToString(io2->getXa().get(4, 1));
        value += "\n";
        value += Conversion::doubleToString(io2->getXa().get(5, 1));
        value += "\n";
        value += Conversion::doubleToString(io2->getXa().get(6, 1));
        value += "\n";
        value += Conversion::doubleToString(sensor->getFocalDistance());
        value += "\n";
        value += Conversion::doubleToString(flight->getHeight());
        value += "\n";
        std::string scale = flight->getScale();
        value += scale.substr(scale.find(':') + 1);
        value += "\n";
        double X01 = sr1->getXa().get(1, 1);
        double X02 = sr2->getXa().get(1, 1);
        double Y01 = sr1->getXa().get(2, 1);
        double Y02 = sr2->getXa().get(2, 1);
        double airbase = sqrt(pow(X01 - X02, 2) + pow(Y01 - Y02, 2));
        double photobase = airbase * sensor->getFocalDistance() / flight->getHeight();
        value += Conversion::doubleToString(photobase);
        value += "\n";
        value += Conversion::doubleToString(airbase);
        value += "\n";
        value += Conversion::doubleToString(sr1->getXa().get(1, 1));
        value += "\n";
        value += Conversion::doubleToString(sr1->getXa().get(2, 1));
        value += "\n";
        value += Conversion::doubleToString(sr1->getXa().get(3, 1));
        value += "\n";
        value += Conversion::doubleToString(sr1->getXa().get(4, 1));
        value += "\n";
        value += Conversion::doubleToString(sr1->getXa().get(5, 1));
        value += "\n";
        value += Conversion::doubleToString(sr1->getXa().get(6, 1));
        value += "\n";
        value += Conversion::doubleToString(sr2->getXa().get(1, 1));
        value += "\n";
        value += Conversion::doubleToString(sr2->getXa().get(2, 1));
        value += "\n";
        value += Conversion::doubleToString(sr2->getXa().get(3, 1));
        value += "\n";
        value += Conversion::doubleToString(sr2->getXa().get(4, 1));
        value += "\n";
        value += Conversion::doubleToString(sr2->getXa().get(5, 1));
        value += "\n";
        value += Conversion::doubleToString(sr2->getXa().get(6, 1));
        value += "\n";
        value += Conversion::doubleToString(
                     sensor->getPrincipalPointCoordinates().getXi());
        value += "\n";
        value += Conversion::doubleToString(
                     sensor->getPrincipalPointCoordinates().getEta());
        value += "\n";
        value += Conversion::doubleToString(terrain->getMeanAltitude());
        value += "\n";
        value += "End of data";
        std::ofstream myFile (filename.c_str());

        if (myFile.is_open()) {
            myFile << value;
            myFile.close();
            return true;
        } else {
            std::cout << "Unable to open file";
        }

        return false;
    }

    return false;
}



void ProjectManager::saveSettings(const char* filename)
{
    QSettings efotoSettings("uerj", "efoto");
    efotoSettings.setValue("lastProject", filename);
}

bool ProjectManager::execAutosave()
{
    if (myInterface_ != NULL) {
        myInterface_->saveFile();
    } else {
        return false;
    }

    return true;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
