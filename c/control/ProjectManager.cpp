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
#include <math.h>

// Constructors and Destructor
//

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ProjectManager::ProjectManager()
{
	this->manager = NULL;
	this->treeModel = NULL;
	this->updater = NULL;
	//loadSettings();
}

ProjectManager::ProjectManager(EFotoManager* manager)
{
	this->manager = manager;
	this->treeModel = NULL;
	this->updater = NULL;
}

ProjectManager::~ProjectManager()
{
	if (treeModel != NULL)
		delete treeModel;
	if (updater != NULL)
		delete updater;
}

// Other Methods
//

bool ProjectManager::connectDatabase()
{
	if (manager != NULL)
	{

	}
	return false;
}

bool ProjectManager::disconnectDatabase()
{
	if (manager != NULL)
	{

	}
	return false;
}

bool ProjectManager::newProject(std::string filename)
{
	if (manager != NULL)
	{
        std::string xmlData = "";
		xmlData += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		xmlData += "<?xml-stylesheet type=\"text/xsl\" href=\"xsl/epp.xsl\"?>\n\n";
		xmlData += "<efotoPhotogrammetricProject version=\"1.0.42\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
		xmlData += "xsi:noNamespaceSchemaLocation=\"EPPSchema/epp_structure.xsd\"\n";
		xmlData += "xmlns:gml=\"http://www.opengis.net/gml\"\n";
		xmlData += "xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";
		xmlData += "<projectHeader>\n";
		xmlData += "<name></name>\n";
		xmlData += "<description></description>\n";

		int i = filename.rfind('/');

        std::string filePath = "<filePath>"+filename.substr(0,i)+"</filePath>\n";
        std::string fileName = "<fileName>"+filename.erase(0,i+1)+"</fileName>\n";

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

		manager->xmlSetData(xmlData);

		if (treeModel != NULL)
			delete treeModel;

		treeModel = new ETreeModel(EDomElement(xmlData).elementByTagName("efotoPhotogrammetricProject").getContent());

		return true;
	}
	return false;
}

bool ProjectManager::loadProject()
{
	if (manager != NULL)
	{

	}
	return false;
}

bool ProjectManager::saveProject()
{
	if (manager != NULL)
	{

	}
	return false;
}

bool ProjectManager::loadFile(std::string filename)
{
	if (manager != NULL)
	{
        std::stringstream myData;
        std::ifstream myFile(filename.c_str());
		if (updater != NULL)
		{
			delete updater;
			updater = NULL;
		}
		if (myFile.is_open())
		{
            std::string line;
			while (!myFile.eof())
			{
                getline (myFile,line);
                myData << line << std::endl;
			}
			myFile.close();

            std::string xmlData = EDomElement(myData.str()).removeBlankLines(true).getContent();
			updater = new XmlUpdater(xmlData);
			if (updater->isUpdated())
			{
				xmlData = updater->getAllXml().getContent();

				/* Paulo 6/3/12 : Se deu um update salva o arquivo automaticamente.*/
				manager->xmlSetData(xmlData);
				//saveFile(filename);
			}
			else
			{
				return false;
			}
			// Aqui deve entrar um codigo para validar o XML.
			manager->xmlSetData(xmlData);


			if (treeModel != NULL)
				delete treeModel;

			treeModel = new ETreeModel(EDomElement(xmlData).elementByTagName("efotoPhotogrammetricProject").getContent());
			saveSettings(filename);
			return true;
		}
		return false;
	}

	return false;
}

bool ProjectManager::saveFile(std::string filename)
{
	if (manager != NULL)
	{
        std::ofstream myFile (filename.c_str());
		if (myFile.is_open())
		{
			EDomElement xml(manager->xmlGetData());
			//myFile << xml.indent('\t').getContent(); // O adequado é que em breve a linha a baixo possa ser substituida por esta aqui.
			myFile << xml.removeBlankLines(true).indent('\t').getContent();
			myFile.close();
			return true;
		}
        else std::cout << "Unable to open file";
		return false;
	}
	return false;
}

int ProjectManager::informFileVersionError()
{
	if (manager != NULL && updater != NULL)
	{
		return updater->getError();
	}
	return 0;
}

bool ProjectManager::addComponent(std::string data, std::string parent)
{
	if (manager != NULL)
	{
		EDomElement newXml(manager->xmlGetData());
		newXml.addChildAtTagName(parent, data);
		manager->xmlSetData(newXml.getContent());

		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
		return true;
	}
	return false;
}

bool ProjectManager::removeComponent(std::string type, int id)
{
	if (manager != NULL)
	{
		EDomElement newXml(manager->xmlGetData());
		if (type == "Sensor")
			newXml.replaceChildByTagAtt("sensor", "key", Conversion::intToString(id), "");
		else if (type == "Flight")
			newXml.replaceChildByTagAtt("flight", "key", Conversion::intToString(id), "");
		else if (type == "Image")
			newXml.replaceChildByTagAtt("image", "key", Conversion::intToString(id), "");
		else if (type == "Point")
			newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(id), "");
		else if (type == "IO")
			newXml.replaceChildByTagAtt("imageIO", "image_key", Conversion::intToString(id), "");
		else if (type == "EO")
			newXml.replaceChildByTagAtt("imageEO", "image_key", Conversion::intToString(id), "");
		manager->xmlSetData(newXml.getContent());

		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
		return true;
	}
	return false;
}

bool ProjectManager::editComponent(std::string type, std::string data)
{
	if (manager != NULL)
	{
		EDomElement newXml(manager->xmlGetData());
		if (type == "Header")
			newXml.replaceChildByTagName("projectHeader", data);
		else if (type == "Terrain")
			newXml.replaceChildByTagName("terrain", data);
		manager->xmlSetData(newXml.getContent());

		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
		return true;
	}
	return false;
}

bool ProjectManager::editComponent(std::string type, int id, std::string data)
{
	if (manager != NULL)
	{
		EDomElement newXml(manager->xmlGetData());
		if (type == "Sensor")
			newXml.replaceChildByTagAtt("sensor", "key", Conversion::intToString(id), data);
		else if (type == "Flight")
			newXml.replaceChildByTagAtt("flight", "key", Conversion::intToString(id), data);
		else if (type == "Image")
			newXml.replaceChildByTagAtt("image", "key", Conversion::intToString(id), data);
		else if (type == "Point")
			newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(id), data);
		else if (type == "IO")
			newXml.replaceChildByTagAtt("imageIO", "image_key", Conversion::intToString(id), data);
		else if (type == "EO")
			newXml.replaceChildByTagAtt("imageEO", "image_key", Conversion::intToString(id), data);
		manager->xmlSetData(newXml.getContent());

		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
		return true;
	}
	return false;
}

EObject* ProjectManager::viewComponent(std::string type, int id)
{
	if (manager != NULL)
	{
		if (type == "Sensor")
			return (EObject*) manager->instanceSensor(id);
		else if (type == "Flight")
			return (EObject*) manager->instanceFlight(id);
		else if (type == "Image")
			return (EObject*) manager->instanceImage(id);
		else if (type == "Point")
			return (EObject*) manager->instancePoint(id);
		else if (type == "IO")
			return (EObject*) manager->instanceIO(id);
		else if (type == "EO")
			return (EObject*) manager->instanceEO(id);
	}
	return NULL;
}

ETreeModel* ProjectManager::getTreeModel()
{
	return treeModel;
}

std::deque<int> ProjectManager::listImageKeys()
{
    std::deque<int> result;
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		result.push_back(treeModel->idAt(4, i));
	return result;
}

std::deque<std::string> ProjectManager::listImages()
{
    std::deque<std::string> result;
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		result.push_back(treeModel->dataAt(4, i));
	return result;
}

int ProjectManager::getImageId(std::string imageName)
{
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		if (treeModel->dataAt(4, i) == imageName)
			return treeModel->idAt(4, i);
	return -1;
}

int ProjectManager::getFreeImageId()
{
	return manager->getFreeImageId();
}

int ProjectManager::getFreePointId()
{
	return manager->getFreePointId();
}

bool ProjectManager::startModule(std::string module, int image)
{
	if (manager != NULL)
	{
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

bool ProjectManager::exec(std::string filename)
{
	if (manager != NULL)
	{
		if (manager->getInterfaceType().compare("Qt") == 0)
		{
			ProjectUserInterface_Qt* newInterface = new ProjectUserInterface_Qt(this);
			if (filename != "")
				newInterface->loadFile(filename);
			myInterface = newInterface;
		}
		if (myInterface != NULL)
		{
			return myInterface->exec();
		}
		return false;
	}
	return false;
}

bool ProjectManager::reload()
{
	if (manager != NULL)
	{
		if (myInterface != NULL)
		{
			manager->getSavedState();
			return myInterface->exec();
		}
		return false;
	}
	return false;
}

std::string ProjectManager::getXml(std::string tagname)
{
	return manager->getXml(tagname);
}

std::string ProjectManager::getXml(std::string tagname, std::string att, std::string value)
{
	return manager->getXml(tagname, att, value);
}

bool ProjectManager::getSavedState()
{
	if (manager != NULL)
	{
		return manager->getSavedState();
	}
	return true;
}

bool ProjectManager::makeSPFile(std::string filename, int image1, int image2)
{
	if (manager != NULL)
	{
		if (image1 == image2 || filename == "")
		{
			return false;
		}
		if (image1 == 0 || image2 == 0)
		{
			return false;
		}

		InteriorOrientation* io1 = manager->instanceIO(image1);
		InteriorOrientation* io2 = manager->instanceIO(image2);
		SpatialRessection* sr1 = (SpatialRessection*)manager->instanceEO(image1);
		SpatialRessection* sr2 = (SpatialRessection*)manager->instanceEO(image2);
		SensorWithFiducialMarks* sensor = (SensorWithFiducialMarks*)manager->instanceSensor(manager->instanceImage(image1)->getSensorId());
		Flight* flight = manager->instanceFlight(manager->instanceImage(image1)->getFlightId());
		Terrain* terrain = manager->instanceTerrain();

		if (io1 == NULL || io2 == NULL || sr1 == NULL || sr2 == NULL || sensor == NULL || flight == NULL || terrain == NULL)
		{
			return false;
		}

		EDomElement xml(manager->xmlGetData());
        std::string value = "";
		// O texto aqui de baixo esta escrito errado de proposito para ficar compativel com o teste que o Marcelo
		// programou no stereoplotter. La ele procura por "Mesure" quando o correto seria "Measure".
		// Isto e compativel com a versao 1.6 do modulo e devera mudar logo que ele corrija a sua parte.
		value += "Stereoscopic Visualization and Mesure Module Data\n";

		value += Conversion::doubleToString(io1->getXa().get(1, 1)); value += "\n";
		value += Conversion::doubleToString(io1->getXa().get(2, 1)); value += "\n";
		value += Conversion::doubleToString(io1->getXa().get(3, 1)); value += "\n";
		value += Conversion::doubleToString(io1->getXa().get(4, 1)); value += "\n";
		value += Conversion::doubleToString(io1->getXa().get(5, 1)); value += "\n";
		value += Conversion::doubleToString(io1->getXa().get(6, 1)); value += "\n";

		value += Conversion::doubleToString(io2->getXa().get(1, 1)); value += "\n";
		value += Conversion::doubleToString(io2->getXa().get(2, 1)); value += "\n";
		value += Conversion::doubleToString(io2->getXa().get(3, 1)); value += "\n";
		value += Conversion::doubleToString(io2->getXa().get(4, 1)); value += "\n";
		value += Conversion::doubleToString(io2->getXa().get(5, 1)); value += "\n";
		value += Conversion::doubleToString(io2->getXa().get(6, 1)); value += "\n";

		value += Conversion::doubleToString(sensor->getFocalDistance()); value += "\n";
		value += Conversion::doubleToString(flight->getHeight()); value += "\n";

        std::string scale = flight->getScale();
		value += scale.substr(scale.find(':') + 1); value += "\n";

		double X01 = sr1->getXa().get(1,1);
		double X02 = sr2->getXa().get(1,1);
		double Y01 = sr1->getXa().get(2,1);
		double Y02 = sr2->getXa().get(2,1);
		double airbase = sqrt(pow(X01 - X02, 2) + pow(Y01 - Y02, 2));
		double photobase = airbase * sensor->getFocalDistance() / flight->getHeight();

		value += Conversion::doubleToString(photobase); value += "\n";
		value += Conversion::doubleToString(airbase); value += "\n";

		value += Conversion::doubleToString(sr1->getXa().get(1, 1)); value += "\n";
		value += Conversion::doubleToString(sr1->getXa().get(2, 1)); value += "\n";
		value += Conversion::doubleToString(sr1->getXa().get(3, 1)); value += "\n";
		value += Conversion::doubleToString(sr1->getXa().get(4, 1)); value += "\n";
		value += Conversion::doubleToString(sr1->getXa().get(5, 1)); value += "\n";
		value += Conversion::doubleToString(sr1->getXa().get(6, 1)); value += "\n";

		value += Conversion::doubleToString(sr2->getXa().get(1, 1)); value += "\n";
		value += Conversion::doubleToString(sr2->getXa().get(2, 1)); value += "\n";
		value += Conversion::doubleToString(sr2->getXa().get(3, 1)); value += "\n";
		value += Conversion::doubleToString(sr2->getXa().get(4, 1)); value += "\n";
		value += Conversion::doubleToString(sr2->getXa().get(5, 1)); value += "\n";
		value += Conversion::doubleToString(sr2->getXa().get(6, 1)); value += "\n";

		value += Conversion::doubleToString(sensor->getPrincipalPointCoordinates().getXi()); value += "\n";
		value += Conversion::doubleToString(sensor->getPrincipalPointCoordinates().getEta()); value += "\n";

		value += Conversion::doubleToString(terrain->getMeanAltitude()); value += "\n";

		value += "End of data";

        std::ofstream myFile (filename.c_str());
		if (myFile.is_open())
		{
			myFile << value;
			myFile.close();
			return true;
		}
        else std::cout << "Unable to open file";
		return false;
	}
	return false;
}


 
void ProjectManager::saveSettings(std::string filename)
{
     QSettings efotoSettings("uerj","efoto");
     efotoSettings.setValue("lastProject",filename.c_str());
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
