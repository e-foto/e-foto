/**************************************************************************
		  SRManager.cpp
**************************************************************************/

#include "SRManager.h"
#include "EFotoManager.h"
#include "EDom.h"
#include "SRUserInterface_Qt.h"

#include "Terrain.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Flight.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "SpatialRessection.h"


// Constructors and destructors
//
namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SRManager::SRManager()
{
    manager = NULL;
#ifdef INTEGRATED_EFOTO
    project = NULL;
#endif //INTEGRATED_EFOTO
	started = false;
	status = false;
    imageId = 0;
}

SRManager::SRManager(EFotoManager *manager, int id)
{
    this->manager = manager;
    project = manager->getProject();
	started = false;
	status = false;
    imageId = id;
}

SRManager::~SRManager()
{

}

// Association Methods
//

void SRManager::setInterface(SRUserInterface* newInterface)
{
	myInterface = newInterface;
}

SRUserInterface* SRManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

bool SRManager::measurePoint(int id, double col, double lin)
{
	if (started)
	{
        Point* pointToMeasure = project->image(imageId)->getPoint(id);
        if (pointToMeasure == NULL)
        {
            pointToMeasure = project->point(id);
            project->image(imageId)->putPoint(pointToMeasure);
        }
        pointToMeasure->putImageCoordinate(ImageSpaceCoordinate(imageId, col, lin));
        project->image(imageId)->getEO()->selectPoint(id);
		return true;
	}
    return false;
}

void SRManager::selectPoint(int id)
{
	if (started)
	{
        project->image(imageId)->getEO()->selectPoint(id);
    }
}

void SRManager::unselectPoint(int id)
{
	if (started)
	{
        project->image(imageId)->getEO()->unselectPoint(id);
    }
}

deque<double> SRManager::pointToDetector(double col, double lin)
{
	deque<double> result;
    result.push_back(project->image(imageId)->getIO()->imageToDetector(col, lin).getXi());
    result.push_back(project->image(imageId)->getIO()->imageToDetector(col, lin).getEta());
	return result;
}

bool SRManager::removePoint(int id) // Deprecated
{
    return false;
}

bool SRManager::insertPointOnImage(int id) // Deprecated
{
    return false;
}

bool SRManager::removePointFromImage(int id) // Deprecated
{
    return false;
}

deque<string> SRManager::listSelectedPoints()
{
	deque<string> result;
	if (started)
	{
        deque<int> selectedPoints = project->image(imageId)->getEO()->getSelectedPoints();
		for (unsigned int i = 0; i < selectedPoints.size(); i++)
		{
            Point* myPoint = project->image(imageId)->getPoint(selectedPoints.at(i));
			string value = Conversion::intToString(myPoint->getId());
			value += " ";
			value += myPoint->getPointId();
			value += " ";
			value += myPoint->getDescription();
			result.push_back(value);
		}
	}
    return result;
}

deque<string> SRManager::listImagePoints()
{
	deque<string> result;
	if (started)
	{
        for (int i = 0; i < project->image(imageId)->countPoints(); i++)
		{
            Point* myPoint = project->image(imageId)->getPointAt(i);
			string value = Conversion::intToString(myPoint->getId());
			value += " ";
			value += myPoint->getPointId();
			value += " ";
			value += myPoint->getDescription();
			result.push_back(value);
		}
	}
    return result;
}

deque<string> SRManager::listAllPoints()
{
    deque<string> result;
    if (started)
    {
        for (int i = 0; i < project->allPoints().size(); i++)
        {
            Point* myPoint = project->allPoints().at(i);
            string value = Conversion::intToString(myPoint->getId());
            value += " ";
            value += myPoint->getPointId();
            value += " ";
            value += myPoint->getDescription();
            result.push_back(value);
        }
    }
    return result;
}

deque<string> SRManager::pointData(int index)
{
	deque<string> result;
	if (started)
	{
        Point* myPoint = project->allPoints().at(index);
		result.push_back(Conversion::intToString(myPoint->getId()));
        result.push_back(myPoint->getType() == Point::CONTROL ? "true" : "false");
        result.push_back(myPoint->getPointId());
        result.push_back(myPoint->getDescription());
		result.push_back(Conversion::doubleToString(myPoint->getObjectCoordinate().getX(),3));
		result.push_back(Conversion::doubleToString(myPoint->getObjectCoordinate().getY(),3));
		result.push_back(Conversion::doubleToString(myPoint->getObjectCoordinate().getZ(),3));
        if (myPoint->hasImageCoordinate(imageId) && myPoint->getImageCoordinate(imageId).isAvailable())
		{
            result.push_back(Conversion::doubleToString(myPoint->getImageCoordinate(imageId).getCol()));
            result.push_back(Conversion::doubleToString(myPoint->getImageCoordinate(imageId).getLin()));
            DetectorSpaceCoordinate aisc = project->image(imageId)->getIO()->imageToDetector(myPoint->getImageCoordinate(imageId));
			result.push_back(Conversion::doubleToString(aisc.getXi(),3));
			result.push_back(Conversion::doubleToString(aisc.getEta(),3));
		}
	}
    return result;
}

unsigned int SRManager::countSelectedPoints()
{
    return project->image(imageId)->getEO()->getSelectedPoints().size();
}

bool SRManager::connectImagePoints() // Deprecated
{
    return false;
}

bool SRManager::updatePointsTable() // Deprecated
{
	if (started)
	{
		return true;
	}
	return false;
}

bool SRManager::flightDirection(int MarkId)
{
	if (started)
	{
        project->image(imageId)->getEO()->selectFiducialMarkForFlightDirection(MarkId);
		return true;
	}
    return false;
}

bool SRManager::flightDirection(double col, double lin)
{
		if (started)
		{
                project->image(imageId)->getEO()->setPointForFlightDirection(col, lin);
				return true;
		}
        return false;
}

bool SRManager::flightDirection(double kappa0)
{
		if (started)
		{
                project->image(imageId)->getEO()->setFlightDirection(kappa0);
				return true;
		}
        return false;
}

bool SRManager::calculateSR(int iterations, double gnssPrecision, double insPrecision)
{
	if (started)
	{
        if (project->image(imageId)->getEO()->countSelectedPoints() > 3)
		{
            project->image(imageId)->getEO()->initialize();
            project->image(imageId)->getEO()->calculate(iterations, gnssPrecision, insPrecision);
		}
		return true;
	}
    return false;
}

bool SRManager::exteriorDone()
{
    if (project->image(imageId)->getEO()->getXa().getRows() != 6)
		return false;
    return true;
}

deque<string> SRManager::makeReport()
{

	// Modificado em 27/06/2011 a pedido do Prof Nunes para exibir La e Sigma La se existirem. A saber o codigo anterior exibia Lb e sigma de Lb.
	deque<string> result;
    SpatialRessection* mySR = project->image(imageId)->getEO();
    result.push_back(mySR->getXa().xmlGetData());
	result.push_back(mySR->getLa().xmlGetData());
    result.push_back(Conversion::doubleToString(mySR->getQuality().getsigma0Squared()));
	result.push_back(mySR->getQuality().getV().xmlGetData());
	result.push_back(mySR->getQuality().getSigmaXa().xmlGetData());
	result.push_back(mySR->getQuality().getSigmaLa().xmlGetData());
	result.push_back(Conversion::intToString(mySR->getTotalIterations()));
	if (mySR->getConverged())
		result.push_back("yes");
	else
		result.push_back("no");
    return result;
}

bool SRManager::exec()
{
#ifdef INTEGRATED_EFOTO
    if (manager != NULL && project != NULL)
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
    if (manager != NULL && !project.isNull())
#endif //SYNAPSE_EFOTO
        if (project->image(imageId) != NULL && project->image(imageId)->getSensor() != NULL &&
            project->image(imageId)->getFlight() != NULL && project->image(imageId)->getSensor() != NULL &&
            project->image(imageId)->getIO() != NULL)
		{
            if (project->image(imageId)->getEO() == NULL)
            {
                SpatialRessection eo(imageId);
                project->addEO(eo.xmlGetDataEO());
            }
			if (manager->getInterfaceType().compare("Qt") == 0)
            {
				myInterface = SRUserInterface_Qt::instance(this);
            }
			started = true;
			if (myInterface != NULL)
			{
				myInterface->exec();
			}
		}
    return status;
}

int SRManager::getId()
{
    return imageId;
}

void SRManager::returnProject()
{
	manager->reloadProject();
}

bool SRManager::save(string path)
{
	if (started)
	{
		FILE* pFile;
        string output = "IO state data for Image " + Conversion::intToString(imageId) + "\n\n";

        output += project->image(imageId)->getSensor()->xmlGetData();
		output += "\n";
        output += project->image(imageId)->getFlight()->xmlGetData();
		output += "\n";
        output += project->image(imageId)->xmlGetData();
		output += "\n";
        output += project->image(imageId)->getIO()->xmlGetData();
		output += "\n";
        output += project->image(imageId)->getEO()->xmlGetData();
		output += "\n";
        for (unsigned int i = 0; i < project->allPoints().size(); i++)
		{
            output += project->allPoints().at(i)->xmlGetData();
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

bool SRManager::load(string path) // Deprecated
{
    return false;
}

string SRManager::getImageFile()
{
    if (project->image(imageId)->getFilepath() == ".")
        return project->image(imageId)->getFilename();
	else
	{
		string result = "";
        result += project->image(imageId)->getFilepath();
		result += "/";
        result += project->image(imageId)->getFilename();
		return result;
    }
}

void SRManager::acceptSR()
{
    project->image(imageId)->getEO()->setValidate(true);
    //Rever!, pois a seção spatialRessections ainda está inacessível na classe Project
    /*
    EDomElement newXml(manager->xmlGetData());
    if (newXml.elementByTagName("spatialRessections").getContent() == "")
        newXml.addChildAtTagName("efotoPhotogrammetricProject","<spatialRessections>\n</spatialRessections>");
    if (newXml.elementByTagAtt("imageEO", "image_key", Conversion::intToString(myImage->getId())).getContent() != "")
        newXml.replaceChildByTagAtt("imageEO", "image_key", Conversion::intToString(myImage->getId()), mySR->xmlGetDataEO());
    else
        newXml.addChildAtTagName("exteriorOrientation", mySR->xmlGetDataEO());
    if (newXml.elementByTagAtt("imageSR", "image_key", Conversion::intToString(myImage->getId())).getContent() != "")
        newXml.replaceChildByTagAtt("imageSR", "image_key", Conversion::intToString(myImage->getId()), mySR->xmlGetData());
    else
        newXml.addChildAtTagName("spatialRessections", mySR->xmlGetData());

    newXml.replaceChildByTagAtt("image","key",Conversion::intToString(myImage->getId()),myImage->xmlGetData());
	int currentPointId;
	for (int i = 0; i < myImage->countPoints(); i++)
	{
		currentPointId = myImage->getPointAt(i)->getId();
		newXml.replaceChildByTagAtt("point", "key", Conversion::intToString(currentPointId), myImage->getPointAt(i)->xmlGetData());
	}
	manager->xmlSetData(newXml.getContent());
	manager->setSavedState(false);
    */
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
