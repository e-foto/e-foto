/**************************************************************************
						   ProjectManager.cpp
**************************************************************************/

#include "ProjectManager.h"
#include "EFotoManager.h"
#include "ProjectUserInterface_Qt.h"

#include "ProjectHeader.h"
#include "Terrain.h"
#include "Sensor.h"
#include "Flight.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "ExteriorOrientation.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "SpatialRessection.h"

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
}

ProjectManager::ProjectManager(EFotoManager* manager)
{
    this->manager = manager;
	this->treeModel = NULL;
    project = project;
}

ProjectManager::~ProjectManager()
{
	if (treeModel != NULL)
        delete treeModel;
}

// Other Methods
//

int ProjectManager::informFileVersionError()
{
#ifdef INTEGRATED_EFOTO
    if (manager != NULL && project != NULL)
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    if (manager != NULL && !project.isNull())
#endif //SYNAPSE_EFOTO
    {
        return project->getError();
    }

	return 0;
}

bool ProjectManager::addComponent(string data, string parent)
{
	if (manager != NULL)
    {
		if (parent == "sensors")
		{
            project->addSensor(data);
		}
		else if (parent == "flights")
		{
            project->addFlight(data);
		}
		else if (parent == "images")
		{
            project->addImage(data);
		}
		else if (parent == "points")
		{
            project->addPoint(data);
        }

        EDomElement newXml(project->getXml());
		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
	}
	return false;
}

bool ProjectManager::removeComponent(string type, int id)
{
    // Rever! o uso deste método. E habilitar os botões para isso funcionar.
	if (manager != NULL)
    {
        if (type == "Sensor")
            project->deleteSensor(id);
        else if (type == "Flight")
            project->deleteFlight(id);
        else if (type == "Image")
        {
            project->deleteImage(id);
            project->deleteIO(id);
            project->deleteEO(id);
        }
        else if (type == "Point")
            project->deletePoint(id);

        EDomElement newXml(project->getXml());
		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
	}
	return false;
}

bool ProjectManager::editComponent(string type, string data)
{
	if (manager != NULL)
    {
		if (type == "Header")
        {
            project->header()->xmlSetData(data);
		}
		else if (type == "Terrain")
        {
            project->terrain()->xmlSetData(data);
        }

        EDomElement newXml(project->getXml());
		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
	}
    return false;
}

bool ProjectManager::editComponent(string type, int id, string data)
{
	if (manager != NULL)
    {
		if (type == "Sensor")
        {
            project->addSensor(data); // Uso add aqui, pois o tipo do sensor pode mudar e necessitar de uma reconstrução do objeto correto.
		}
		else if (type == "Flight")
        {
            project->flight(1)->xmlSetData(data);
		}
		else if (type == "Image")
        {
            project->image(id)->xmlSetData(data);
		}
		else if (type == "Point")
        {
            project->point(id)->xmlSetData(data);
        }

        EDomElement newXml(project->getXml());
		if (treeModel != NULL)
			delete treeModel;
		treeModel = new ETreeModel(newXml.elementByTagName("efotoPhotogrammetricProject").getContent());
        return true;
	}
    return false;
}

EObject* ProjectManager::viewComponent(string type, int id) // Deprecated
{
	return NULL;
}

ETreeModel* ProjectManager::getTreeModel()
{
	return treeModel;
}

deque<int> ProjectManager::listImageKeys()
{
	deque<int> result;
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		result.push_back(treeModel->idAt(4, i));
	return result;
}

deque<string> ProjectManager::listImages()
{
	deque<string> result;
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		result.push_back(treeModel->dataAt(4, i));
	return result;
}

int ProjectManager::getImageId(string imageName)
{
	for (unsigned int i = 0; i < treeModel->getChild(4).countChildren(); i++)
		if (treeModel->dataAt(4, i) == imageName)
			return treeModel->idAt(4, i);
	return -1;
}

int ProjectManager::getFreeImageId()
{
    return project->getFreeImageId();
}

int ProjectManager::getFreePointId()
{
    return project->getFreePointId();
}

bool ProjectManager::startModule(string module, int image)
{
	if (manager != NULL)
	{
		if (module.compare("InteriorOrientation") == 0)
			manager->setNextModule(3);
		else if (module.compare("SpatialRessection") == 0)
			manager->setNextModule(4);
		else if (module.compare("FotoTriangulation") == 0)
			manager->setNextModule(5);
		else if (module.compare("DEM-Extraction") == 0)
			manager->setNextModule(6);
		else if (module.compare("Ortho-rectification") == 0)
			manager->setNextModule(7);
		else if (module.compare("StereoPlotter") == 0)
			manager->setNextModule(8);

		manager->setNextImage(image);
		manager->exec();
		return true;
	}
	return false;
}

bool ProjectManager::exec(string filename)
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
			return myInterface->exec();
		}
		return false;
	}
	return false;
}

string ProjectManager::getXml(string tagname)
{
    return project->getXml(tagname);
}

string ProjectManager::getXml(string tagname, string att, string value)
{
    return project->getXml(tagname, att, value);
}

bool ProjectManager::getSavedState()
{
#ifdef INTEGRATED_EFOTO
    if (manager != NULL && project != NULL)
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    if (manager != NULL && !project.isNull())
#endif //SYNAPSE_EFOTO
    {
        return project->getSaveState();
	}
	return true;
}

bool ProjectManager::makeSPFile(string filename, int image1, int image2) // Deprecated
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

        InteriorOrientation* io1 = project->IO(image1);
        InteriorOrientation* io2 = project->IO(image2);
        SpatialRessection* sr1 = (SpatialRessection*)project->EO(image1);
        SpatialRessection* sr2 = (SpatialRessection*)project->EO(image2);
        SensorWithFiducialMarks* sensor = (SensorWithFiducialMarks*)project->sensor(project->image(image1)->getSensorId());
        Flight* flight = project->flight(project->image(image1)->getFlightId());
        Terrain* terrain = project->terrain();

		if (io1 == NULL || io2 == NULL || sr1 == NULL || sr2 == NULL || sensor == NULL || flight == NULL || terrain == NULL)
		{
			return false;
		}

        EDomElement xml(project->getXml());
		string value = "";
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

		string scale = flight->getScale();
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

		ofstream myFile (filename.c_str());
		if (myFile.is_open())
		{
			myFile << value;
			myFile.close();
			return true;
		}
		else cout << "Unable to open file";
        return false;
	}
	return false;
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
