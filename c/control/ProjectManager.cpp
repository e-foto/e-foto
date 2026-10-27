/**************************************************************************
ProjectManager.cpp
**************************************************************************/
/*Copyright 2002-2025 e-foto team (UERJ)
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
#include "ProjectManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "EFotoManager.h"
#include "ETreeModel.h"
#include "Flight.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "PathUtils.h"
#include "Project.h"
#include "ProjectUserInterface_Qt.h"
#include "SensorWithFiducialMarks.h"
#include "SpatialRessection.h"
#include "Terrain.h"

// Constructors and Destructor
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectManager::ProjectManager() {
  this->manager = NULL;
  this->treeModel = NULL;
  this->updater = NULL;
  this->savedIn = "";
  // loadSettings();
}

ProjectManager::ProjectManager(EFotoManager* manager) {
  this->manager = manager;
  this->treeModel = NULL;
  this->updater = NULL;
  this->savedIn = "";
}

ProjectManager::~ProjectManager() {
  if (treeModel != NULL) delete treeModel;
  if (updater != NULL) delete updater;
}

// Other Methods
//

bool ProjectManager::connectDatabase() {
  if (manager != NULL) {
  }
  return false;
}

bool ProjectManager::disconnectDatabase() {
  if (manager != NULL) {
  }
  return false;
}

bool ProjectManager::newProject(std::string filename) {
  if (manager != NULL) {
    std::string xmlData = "";
    xmlData += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xmlData += "<?xml-stylesheet type=\"text/xsl\" href=\"xsl/epp.xsl\"?>\n\n";
    xmlData +=
        "<efotoPhotogrammetricProject version=\"1.0.42\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
    xmlData +=
        "xsi:noNamespaceSchemaLocation=\"EPPSchema/epp_structure.xsd\"\n";
    xmlData += "xmlns:gml=\"http://www.opengis.net/gml\"\n";
    xmlData += "xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";
    xmlData += "<projectHeader>\n";
    xmlData += "<name></name>\n";
    xmlData += "<description></description>\n";

    // Split the path with the path layer: '/' is not the only separator on
    // Windows, and both the folder and the file name may contain non ASCII or
    // XML special characters.
    PathUtils::setProjectFile(filename);

    const std::filesystem::path projectFile =
        PathUtils::toPath(PathUtils::absolute(filename));
    const std::string filePath = PathUtils::fromPath(projectFile.parent_path());
    const std::string fileName = PathUtils::fromPath(projectFile.filename());

    xmlData +=
        "<fileName>" + PathUtils::xmlEscape(fileName) + "</fileName>\n";
    xmlData +=
        "<filePath>" + PathUtils::xmlEscape(filePath) + "</filePath>\n";
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

    manager->xmlSetData(xmlData);

    if (treeModel != NULL) delete treeModel;

    treeModel =
        new ETreeModel(EDomElement(xmlData)
                           .elementByTagName("efotoPhotogrammetricProject")
                           .getContent());

    return true;
  }
  return false;
}

bool ProjectManager::loadProject() {
  if (manager != NULL) {
  }
  return false;
}

bool ProjectManager::saveProject()  //???
{
  if (manager != NULL) {
  }
  return false;
}

bool ProjectManager::loadFile(std::string filename) {
  if (manager == NULL) return false;

  // Read through the path layer: std::ifstream would interpret the UTF-8
  // bytes with the local code page and fail on any non ASCII folder name,
  // and it cannot open paths longer than MAX_PATH.
  std::string fileContents;
  if (!PathUtils::readFile(filename, fileContents)) return false;

  // Ignore a UTF-8 byte order mark, which some editors add on save.
  if (fileContents.size() >= 3 &&
      static_cast<unsigned char>(fileContents[0]) == 0xEF &&
      static_cast<unsigned char>(fileContents[1]) == 0xBB &&
      static_cast<unsigned char>(fileContents[2]) == 0xBF) {
    fileContents.erase(0, 3);
  }

  std::stringstream myData(fileContents);

  if (updater != NULL) {
    delete updater;
    updater = NULL;
  }

  std::string xmlData =
      EDomElement(fileContents).removeBlankLines(true).getContent();
  updater = new XmlUpdater(xmlData);
  if (!updater->isUpdated()) {
    return false;
  }

  xmlData = updater->getAllXml().getContent();

  /* Paulo 6/3/12 : Se deu um update salva o arquivo automaticamente.*/
  manager->xmlSetData(xmlData);
  // saveFile(filename);

  // Aqui deve entrar um codigo para validar o XML.

  // From now on relative image paths are resolved against this project file.
  PathUtils::setProjectFile(filename);

  if (treeModel != NULL) delete treeModel;

  treeModel = new ETreeModel(EDomElement(xmlData)
                                 .elementByTagName("efotoPhotogrammetricProject")
                                 .getContent());
  if (manager->getInterfaceType().compare("Qt") == 0) {
    ProjectUserInterface_Qt* theInterface =
        (ProjectUserInterface_Qt*)myInterface;
    theInterface->saveSettings("lastProject", PathUtils::absolute(filename));
  }
  return true;
}

bool ProjectManager::saveFile(std::string filename) {
  if (manager == NULL) return false;

  EDomElement xml(manager->xmlGetData());
  const std::string contents =
      xml.removeBlankLines(true).indent('\t').getContent();

  // std::ofstream would write to a path interpreted with the local code page.
  if (!PathUtils::writeFile(filename, contents)) {
    std::cout << "Unable to open file";
    return false;
  }

  // Saving elsewhere ("Save as") moves the anchor used to resolve the
  // relative image paths.
  PathUtils::setProjectFile(filename);

  if (manager->getInterfaceType().compare("Qt") == 0) {
    ProjectUserInterface_Qt* theInterface =
        (ProjectUserInterface_Qt*)myInterface;
    theInterface->saveSettings("lastProject", PathUtils::absolute(filename));
  }
  return true;
}

std::string ProjectManager::projectDirectory() const {
  return PathUtils::projectDirectory();
}

int ProjectManager::repairImagePaths(std::deque<std::string>& missingImages) {
  missingImages.clear();
  if (manager == NULL) return 0;

  const std::string projectDir = PathUtils::projectDirectory();
  EDomElement projectXml(manager->xmlGetData());
  EDomElement imagesXml(projectXml.elementByTagName("images"));
  std::deque<EDomElement> images = imagesXml.elementsByTagName("image");

  int repaired = 0;
  std::map<std::string, std::string> imageIndex;
  bool imageIndexBuilt = false;

  for (size_t i = 0; i < images.size(); i++) {
    EDomElement image = images.at(i);
    const std::string key = image.attribute("key");
    const std::string storedPath = PathUtils::xmlUnescape(
        PathUtils::trim(image.elementByTagName("filePath").toString()));
    const std::string storedName = PathUtils::xmlUnescape(
        PathUtils::trim(image.elementByTagName("fileName").toString()));

    if (storedName.empty()) continue;

    std::string resolved =
        PathUtils::resolveImagePath(storedPath, storedName, projectDir);

    if (!PathUtils::exists(resolved)) {
      // The project folder was moved/copied: look for the image inside it,
      // with a single directory walk shared by every missing image.
      if (!imageIndexBuilt) {
        imageIndex = PathUtils::indexFilesByName(projectDir);
        imageIndexBuilt = true;
      }
      const auto found =
          imageIndex.find(PathUtils::asciiLower(PathUtils::fileName(storedName)));
      if (found != imageIndex.end()) resolved = found->second;
    }

    if (!PathUtils::exists(resolved)) {
      missingImages.push_back(storedName);
      continue;
    }

    // Store a portable relative path when possible, an absolute one when the
    // image lives on another drive than the project.
    std::string newPath = PathUtils::relativeOrAbsolute(
        PathUtils::parentDirectory(resolved), projectDir);
    if (newPath.empty()) newPath = ".";
    const std::string newName = PathUtils::fileName(resolved);

    const bool pathChanged =
        PathUtils::normalizeSeparators(storedPath) !=
        PathUtils::normalizeSeparators(newPath);
    if (!pathChanged && storedName == newName) continue;

    EDomElement fixedImage = image;
    fixedImage.replaceChildByTagName(
        "filePath",
        "<filePath>" + PathUtils::xmlEscape(newPath) + "</filePath>");
    fixedImage.replaceChildByTagName(
        "fileName",
        "<fileName>" + PathUtils::xmlEscape(newName) + "</fileName>");
    projectXml.replaceChildByTagAtt("image", "key", key,
                                    fixedImage.getContent());
    repaired++;
  }

  if (repaired > 0) {
    manager->xmlSetData(projectXml.getContent());
    if (treeModel != NULL) delete treeModel;
    treeModel = new ETreeModel(
        projectXml.elementByTagName("efotoPhotogrammetricProject")
            .getContent());
    manager->setSavedState(false);
  }
  return repaired;
}

int ProjectManager::informFileVersionError() {
  if (manager != NULL && updater != NULL) {
    return updater->getError();
  }
  return 0;
}

bool ProjectManager::addComponent(std::string data, std::string parent) {
  if (manager != NULL) {
    EDomElement newXml(manager->xmlGetData());
    newXml.addChildAtTagName(parent, data);
    manager->xmlSetData(newXml.getContent());

    if (treeModel != NULL) delete treeModel;
    treeModel = new ETreeModel(
        newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
    return true;
  }
  return false;
}

bool ProjectManager::removeComponent(std::string type, int id) {
  if (manager != NULL) {
    EDomElement newXml(manager->xmlGetData());
    if (type == "Sensor")
      newXml.replaceChildByTagAtt("sensor", "key", Conversion::intToString(id),
                                  "");
    else if (type == "Flight")
      newXml.replaceChildByTagAtt("flight", "key", Conversion::intToString(id),
                                  "");
    else if (type == "Image")
      newXml.replaceChildByTagAtt("image", "key", Conversion::intToString(id),
                                  "");
    else if (type == "Point") {
      newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(id),
                                  "");
      // The phototriangulation and the spatial resections keep the keys of the
      // points they used (<usedPoints><pointKey>..</pointKey>): a deleted point
      // has to disappear from those lists too.
      std::string data = newXml.getContent();
      const std::string usedPoint =
          "<pointKey>" + Conversion::intToString(id) + "</pointKey>";
      for (std::string::size_type pos = data.find(usedPoint);
           pos != std::string::npos; pos = data.find(usedPoint, pos))
        data.erase(pos, usedPoint.length());
      newXml.setContent(data);
    } else if (type == "IO")
      newXml.replaceChildByTagAtt("imageIO", "image_key",
                                  Conversion::intToString(id), "");
    else if (type == "EO")
      newXml.replaceChildByTagAtt("imageEO", "image_key",
                                  Conversion::intToString(id), "");
    if (type == "Image") {
      // The interior/exterior orientations of an image are stored outside
      // <image>: they have to go away with it, otherwise the project keeps
      // records of an image that no longer exists. The same applies to the
      // measurements of each point on that image.
      newXml.replaceChildByTagAtt("imageIO", "image_key",
                                  Conversion::intToString(id), "");
      newXml.replaceChildByTagAtt("imageEO", "image_key",
                                  Conversion::intToString(id), "");
      newXml.replaceChildByTagAtt("imageSR", "image_key",
                                  Conversion::intToString(id), "");
      newXml.replaceChildByTagAtt("imageCoordinates", "image_key",
                                  Conversion::intToString(id), "");
    }
    manager->xmlSetData(newXml.getContent());

    if (treeModel != NULL) delete treeModel;
    treeModel = new ETreeModel(
        newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
    return true;
  }
  return false;
}

bool ProjectManager::editComponent(std::string type, std::string data) {
  if (manager != NULL) {
    EDomElement newXml(manager->xmlGetData());
    if (type == "Header")
      newXml.replaceChildByTagName("projectHeader", data);
    else if (type == "Terrain")
      newXml.replaceChildByTagName("terrain", data);
    manager->xmlSetData(newXml.getContent());

    if (treeModel != NULL) delete treeModel;
    treeModel = new ETreeModel(
        newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
    return true;
  }
  return false;
}

bool ProjectManager::editComponent(std::string type, int id, std::string data) {
  if (manager != NULL) {
    EDomElement newXml(manager->xmlGetData());
    if (type == "Sensor")
      newXml.replaceChildByTagAtt("sensor", "key", Conversion::intToString(id),
                                  data);
    else if (type == "Flight")
      newXml.replaceChildByTagAtt("flight", "key", Conversion::intToString(id),
                                  data);
    else if (type == "Image")
      newXml.replaceChildByTagAtt("image", "key", Conversion::intToString(id),
                                  data);
    else if (type == "Point")
      newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(id),
                                  data);
    else if (type == "IO")
      newXml.replaceChildByTagAtt("imageIO", "image_key",
                                  Conversion::intToString(id), data);
    else if (type == "EO")
      newXml.replaceChildByTagAtt("imageEO", "image_key",
                                  Conversion::intToString(id), data);
    manager->xmlSetData(newXml.getContent());

    if (treeModel != NULL) delete treeModel;
    treeModel = new ETreeModel(
        newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
    return true;
  }
  return false;
}

EObject* ProjectManager::viewComponent(std::string type, int id) {
  if (manager != NULL) {
    if (type == "Sensor")
      return (EObject*)manager->instanceSensor(id);
    else if (type == "Flight")
      return (EObject*)manager->instanceFlight(id);
    else if (type == "Image")
      return (EObject*)manager->instanceImage(id);
    else if (type == "Point")
      return (EObject*)manager->instancePoint(id);
    else if (type == "IO")
      return (EObject*)manager->instanceIO(id);
    else if (type == "EO")
      return (EObject*)manager->instanceEO(id);
  }
  return NULL;
}

ETreeModel* ProjectManager::getTreeModel() { return treeModel; }

std::deque<int> ProjectManager::listImageKeys() {
  std::deque<int> result;
  for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
    result.push_back(treeModel->idAt(4, i));
  return result;
}

std::deque<std::string> ProjectManager::listImages() {
  std::deque<std::string> result;
  for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
    // The tree keeps the raw XML text, where '&' is stored as "&amp;".
    result.push_back(PathUtils::xmlUnescape(treeModel->dataAt(4, i)));
  return result;
}

int ProjectManager::getImageId(std::string imageName) {
  for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
    if (PathUtils::xmlUnescape(treeModel->dataAt(4, i)) == imageName)
      return treeModel->idAt(4, i);
  return -1;
}

int ProjectManager::getFreeImageId() { return manager->getFreeImageId(); }

int ProjectManager::getFreePointId() { return manager->getFreePointId(); }

bool ProjectManager::startModule(std::string module, int image) {
  if (manager != NULL) {
    if (module.compare("InteriorOrientation") == 0)
      manager->setNextModule(EFotoManager::NEXT_IO);
    else if (module.compare("SpatialRessection") == 0)
      manager->setNextModule(EFotoManager::NEXT_SR);
    else if (module.compare("FotoTriangulation") == 0)
      manager->setNextModule(EFotoManager::NEXT_PT);
    else if (module.compare("DEM-Extraction") == 0)
      manager->setNextModule(EFotoManager::NEXT_DEM);
    else if (module.compare("Ortho-rectification") == 0)
      manager->setNextModule(EFotoManager::NEXT_ORTHO);
    else if (module.compare("StereoPlotter") == 0)
      manager->setNextModule(EFotoManager::NEXT_SP);
    else if (module.compare("Report") == 0)
      manager->setNextModule(EFotoManager::NEXT_Report);
    else if (module.compare("PT-Report") == 0)
      manager->setNextModule(EFotoManager::NEXT_PTReport);
    manager->setNextImage(image);
    manager->exec();
    return true;
  }
  return false;
}

bool ProjectManager::exec(std::string filename) {
  if (manager != NULL) {
    if (manager->getInterfaceType().compare("Qt") == 0) {
      ProjectUserInterface_Qt* newInterface = new ProjectUserInterface_Qt(this);
      newInterface->updatePath();
      if (filename != "") newInterface->loadFile(filename);
      myInterface = newInterface;
    }
    if (myInterface != NULL) {
      return myInterface->exec();
    }
    return false;
  }
  return false;
}

bool ProjectManager::reload() {
  if (manager != NULL) {
    if (myInterface != NULL) {
      manager->getSavedState();
      return myInterface->exec();
    }
    return false;
  }
  return false;
}

std::string ProjectManager::getXml(std::string tagname) {
  return manager->getXml(tagname);
}

std::string ProjectManager::getXml(std::string tagname, std::string att,
                                   std::string value) {
  return manager->getXml(tagname, att, value);
}

bool ProjectManager::getSavedState() {
  if (manager != NULL) {
    return manager->getSavedState();
  }
  return true;
}

bool ProjectManager::makeSPFile(std::string filename, int image1, int image2) {
  if (manager != NULL) {
    if (image1 == image2 || filename == "") {
      return false;
    }
    if (image1 == 0 || image2 == 0) {
      return false;
    }

    InteriorOrientation* io1 = manager->instanceIO(image1);
    InteriorOrientation* io2 = manager->instanceIO(image2);
    SpatialRessection* sr1 = (SpatialRessection*)manager->instanceEO(image1);
    SpatialRessection* sr2 = (SpatialRessection*)manager->instanceEO(image2);
    SensorWithFiducialMarks* sensor =
        (SensorWithFiducialMarks*)manager->instanceSensor(
            manager->instanceImage(image1)->getSensorId());
    Flight* flight =
        manager->instanceFlight(manager->instanceImage(image1)->getFlightId());
    Terrain* terrain = manager->instanceTerrain();

    if (io1 == NULL || io2 == NULL || sr1 == NULL || sr2 == NULL ||
        sensor == NULL || flight == NULL || terrain == NULL) {
      return false;
    }

    EDomElement xml(manager->xmlGetData());
    std::string value = "";
    // O texto aqui de baixo esta escrito errado de proposito para ficar
    // compativel com o teste que o Marcelo programou no stereoplotter. La ele
    // procura por "Mesure" quando o correto seria "Measure". Isto e compativel
    // com a versao 1.6 do modulo e devera mudar logo que ele corrija a sua
    // parte.
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
    double photobase =
        airbase * sensor->getFocalDistance() / flight->getHeight();

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

    if (PathUtils::writeFile(filename, value)) {
      return true;
    } else
      std::cout << "Unable to open file";
    return false;
  }
  return false;
}

bool ProjectManager::execAutosave() {
  if (myInterface != NULL)
    myInterface->saveFile();
  else
    return false;
  return true;
}

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
