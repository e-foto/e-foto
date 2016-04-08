/*******************************************************************************
		  EFotoManager.cpp
*******************************************************************************/
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
#include "EFotoManager.h"
#include "IOManager.h"
#include "InteriorOrientation.h"
#include "ExteriorOrientation.h"
#include "SpatialRessection.h"
#include "SRManager.h"
#include "PTManager.h"
#include "ProjectManager.h"
#include "DEMManager.h"
#include "OrthoManager.h"
#include "SPManager.h"
#include "ReportManager.h"
#include "PhotoTriReport.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Point.h"
#include "Terrain.h"
#include "Flight.h"
#include "Image.h"

#include <QApplication>
#include <QFileDialog>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructor
//

/**
 *
 */
EFotoManager::EFotoManager() : updater("")
{
	xmlData = "";
	savedState = true;
	project = NULL;
	interiorOrientation = NULL;
	spatialRessection = NULL;
	fotoTri = NULL;
	dem = NULL;
	ortho = NULL;
	sp = NULL;
	theTerrain = NULL;
	nextModule = NEXT_PROJECT;
	report = NULL;
}

/**
 *
 */
EFotoManager::~EFotoManager()
{

}

// Instance objects accessor methods
//

/**
 *
 */
Terrain* EFotoManager::instanceTerrain()
{
	if (theTerrain != NULL)
		return theTerrain;
	EDomElement root(xmlData);
	EDomElement xmlTerrain = root.elementByTagName("terrain");
	if (xmlTerrain.getContent().compare("") == 0)
		return NULL;
	theTerrain = new Terrain();
	theTerrain->xmlSetData(xmlTerrain.getContent());
	return theTerrain;
}

/**
 *
 */
Sensor* EFotoManager::instanceSensor(int id)
{
	for (unsigned int i = 0; i < sensors.size(); i++)
		if (sensors.at(i)->getId() == id)
			return sensors.at(i);
	EDomElement root(xmlData);
	EDomElement xmlSensor = root.elementByTagAtt("sensor", "key", Conversion::intToString(id));
	if (xmlSensor.getContent().compare("") == 0)
		return NULL;
	if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
			xmlSensor.elementByTagName("calculationMode").toString().compare("With Fiducial Marks") == 0)
	{
		SensorWithFiducialMarks* newSensorWithFiducialMarks = new SensorWithFiducialMarks();
		newSensorWithFiducialMarks->xmlSetData(xmlSensor.getContent());
		sensors.push_back(newSensorWithFiducialMarks);
		return (Sensor*) newSensorWithFiducialMarks;
	}
	if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
		xmlSensor.elementByTagName("calculationMode").toString().compare("With Sensor Dimensions") == 0)
	{
		SensorWithKnowDimensions* newSensorWithKnowDimensions = new SensorWithKnowDimensions();
		newSensorWithKnowDimensions->xmlSetData(xmlSensor.getContent());
		sensors.push_back(newSensorWithKnowDimensions);
		return (Sensor*) newSensorWithKnowDimensions;
	}
	if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
		xmlSensor.elementByTagName("calculationMode").toString().compare("Fixed Parameters") == 0)
	{
		SensorWithKnowParameters* newSensorWithKnowParameters = new SensorWithKnowParameters();
		newSensorWithKnowParameters->xmlSetData(xmlSensor.getContent());
		sensors.push_back(newSensorWithKnowParameters);
		return (Sensor*) newSensorWithKnowParameters;
	}
	return NULL;
}

/**
 *
 */
Flight* EFotoManager::instanceFlight(int id)
{
	for (unsigned int i = 0; i < flights.size(); i++)
		if (flights.at(i)->getId() == id)
			return flights.at(i);
	EDomElement root(xmlData);
	EDomElement xmlFlight = root.elementByTagAtt("flight", "key", Conversion::intToString(id));
	if (xmlFlight.getContent().compare("") == 0)
		return NULL;
	Flight* newFlight = new Flight();
	newFlight->xmlSetData(xmlFlight.getContent());
	flights.push_back(newFlight);
	return newFlight;
}

/**
 *
 */
Image* EFotoManager::instanceImage(int id)
{
	for (unsigned int i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
			return images.at(i);
	EDomElement root(xmlData);
	EDomElement xmlImage = root.elementByTagAtt("image", "key", Conversion::intToString(id));
	if (xmlImage.getContent().compare("") == 0)
		return NULL;
	Image* newImage = new Image();
	newImage->xmlSetData(xmlImage.getContent());
	images.push_back(newImage);
	return newImage;
}

void EFotoManager::instanceAllImages()
{
	EDomElement root(xmlData);
	std::deque<EDomElement> xmlAllImages = root.elementsByTagName("image");
	for (unsigned int i = 0 ;i < xmlAllImages.size();i++)
	{
		qApp->processEvents();
		if (xmlAllImages.at(i).getContent().compare("") == 0)
			continue;
		bool notAvailable = true;
		for (unsigned int j = 0; j < images.size() && notAvailable; j++)
			if (images.at(j)->getId() == Conversion::stringToInt(xmlAllImages.at(i).attribute("key")))
				notAvailable = false;
		if (notAvailable)
		{
			Image* newImage = new Image();
			newImage->xmlSetData(xmlAllImages.at(i).getContent());
			images.push_back(newImage);
		}
	}
}

/**
 *
 */
void EFotoManager::instanceAllPoints()
{
	EDomElement root(xmlData);
	std::deque<EDomElement> xmlAllPoint = root.elementsByTagName("point");

	for (unsigned int i = 0; i < xmlAllPoint.size(); i++)
	{
		qApp->processEvents();
		if (xmlAllPoint.at(i).getContent().compare("") == 0)
			continue;

		// Evita reinstaciar pontos que ja estejam disponiveis
		bool notAvailable = true;
		for (unsigned int j = 0; j < points.size() && notAvailable; j++)
			if (points.at(j)->getId() == Conversion::stringToInt(xmlAllPoint.at(i).attribute("key")))
				notAvailable = false;
		if (notAvailable)
		{
			Point* newPoint = new Point();
			newPoint->xmlSetData(xmlAllPoint.at(i).getContent());
			points.push_back(newPoint);
		}
	}
}


Point* EFotoManager::instancePoint(int id)
{
	for (unsigned int i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
			return points.at(i);
	EDomElement root(xmlData);
	EDomElement xmlPoint = root.elementByTagAtt("point", "key", Conversion::intToString(id));
	if (xmlPoint.getContent().compare("") == 0)
		return NULL;

	Point* newPoint = new Point();
	newPoint->xmlSetData(xmlPoint.getContent());
	points.push_back(newPoint);
	return (Point*) newPoint;
}

/**
 *
 */
InteriorOrientation* EFotoManager::instanceIO(int imageId)
{
	for (unsigned int i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == imageId)
			return IOs.at(i);
	EDomElement root(xmlData);
	EDomElement xmlIO = root.elementByTagAtt("imageIO", "image_key", Conversion::intToString(imageId));
	if (xmlIO.getContent().compare("") == 0)
		return NULL;
	InteriorOrientation* newIO = new InteriorOrientation();
	newIO->setImage(image(imageId));
	newIO->xmlSetData(xmlIO.getContent());
	newIO->setImage(NULL);
	IOs.push_back(newIO);
	return newIO;
}

void EFotoManager::instanceAllIOs()
{
	EDomElement root(xmlData);
	std::deque<EDomElement> xmlAllIOs = root.elementsByTagName("imageIO");
	for (unsigned int i = 0 ;i < xmlAllIOs.size();i++)
	{
		qApp->processEvents();
		if (xmlAllIOs.at(i).getContent().compare("") == 0)
			continue;
		bool notAvailable = true;
		for (unsigned int j = 0; j < IOs.size() && notAvailable; j++)
			if (IOs.at(j)->getImageId() == Conversion::stringToInt(xmlAllIOs.at(i).attribute("image_key")))
				notAvailable = false;
		//InteriorOrientation *pkj;
		//pkj->getImageId()
		if (notAvailable)
		{
			InteriorOrientation* newIO = new InteriorOrientation();
			newIO->setImage(image(Conversion::stringToInt(xmlAllIOs.at(i).attribute("image_key"))));
			newIO->xmlSetData(xmlAllIOs.at(i).getContent());
			newIO->setImage(NULL);
			IOs.push_back(newIO);
		}
	}
}

/**
 *
 */
ExteriorOrientation* EFotoManager::instanceEO(int imageId)
{
	for (unsigned int i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == imageId)
			return EOs.at(i);
	EDomElement root(xmlData);
	EDomElement xmlEO = root.elementByTagAtt("imageEO", "image_key", Conversion::intToString(imageId));
	EDomElement xmlSR = root.elementByTagAtt("imageSR", "image_key", Conversion::intToString(imageId));
	if (xmlEO.getContent().compare("") == 0 || xmlSR.getContent().compare("") == 0)
		return NULL;

	SpatialRessection* newEO = new SpatialRessection();
		newEO->xmlSetData(xmlEO.getContent().append(xmlSR.getContent()));
	EOs.push_back(newEO);

	return (ExteriorOrientation*) newEO;
}

void EFotoManager::instanceAllEOs()
{
	EDomElement root(xmlData);
	std::deque<EDomElement> xmlAllEOs = root.elementsByTagName("imageEO");
	for (unsigned int i = 0 ;i < xmlAllEOs.size();i++)
	{
		qApp->processEvents();
		if (xmlAllEOs.at(i).getContent().compare("") == 0)
			continue;
		bool notAvailable = true;
		for (unsigned int j = 0; j < EOs.size() && notAvailable; j++)
			if (EOs.at(j)->getImageId() == Conversion::stringToInt(xmlAllEOs.at(i).attribute("image_key")))
				notAvailable = false;
		if (notAvailable)
		{
			SpatialRessection* newEO = new SpatialRessection(); // SpatialRessection vai virar ExteriorOrientation em breve.
			newEO->setImage(image(Conversion::stringToInt(xmlAllEOs.at(i).attribute("image_key"))));
			newEO->xmlSetData(xmlAllEOs.at(i).getContent());
			newEO->setImage(NULL);
			EOs.push_back(newEO);
		}
	}
}

/**
 *
 */
void EFotoManager::deleteTerrain()
{
	if (theTerrain != NULL)
	{
		delete(theTerrain);
		theTerrain = NULL;
	}
}

/**
 *
 */
void EFotoManager::deleteSensor(int id)
{
	unsigned int i;
	Sensor* mySensor = NULL;
	for (i = 0; i < sensors.size(); i++)
		if (sensors.at(i)->getId() == id)
		{
			mySensor = sensors.at(i);
			break;
		}
	if (mySensor != NULL)
	{
		EDomElement xmlSensor(mySensor->xmlGetData());
		if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
				xmlSensor.elementByTagName("platform").toString().compare("aerial") == 0 &&
				xmlSensor.elementByTagName("detector").toString().compare("film") == 0)
		{
			SensorWithFiducialMarks* mySensorWithFiducialMarks = (SensorWithFiducialMarks*) mySensor;
			delete(mySensorWithFiducialMarks);
		}
		sensors.erase(sensors.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteFlight(int id)
{
	unsigned int i;
	Flight* myFlight = NULL;
	for (i = 0; i < flights.size(); i++)
		if (flights.at(i)->getId() == id)
		{
			myFlight = flights.at(i);
			break;
		}
	if (myFlight != NULL)
	{
		delete(myFlight);
		flights.erase(flights.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteImage(int id)
{
	unsigned int i;
	Image* myImage = NULL;
	for (i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
		{
			myImage = images.at(i);
			break;
		}
	if (myImage != NULL)
	{
		delete(myImage);
		images.erase(images.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deletePoint(int id)
{
	unsigned int i;
	Point* myPoint = NULL;
	for (i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
		{
			myPoint = points.at(i);
			break;
		}
	if (myPoint != NULL)
	{
		Point* point = myPoint;
		delete(point);
		points.erase(points.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteIO(int id)
{
	unsigned int i;
	InteriorOrientation* myIO = NULL;
	for (i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == id)
		{
			myIO = IOs.at(i);
			break;
		}
	if (myIO != NULL)
	{
		delete(myIO);
		IOs.erase(IOs.begin() + i);
	}
}

/**
 *
 */
void EFotoManager::deleteEO(int id)
{
	unsigned int i;
	ExteriorOrientation* myEO = NULL;
	for (i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == id)
		{
			myEO = EOs.at(i);
			break;
		}
	if (myEO != NULL)
	{
		if (myEO->is("SpatialRessection"))
		{
			SpatialRessection* mySR = (SpatialRessection*) myEO;
			delete(mySR);
		}
		EOs.erase(EOs.begin() + i);
	}
}

/**
 *
 */
Terrain* EFotoManager::terrain()
{
	if (theTerrain != NULL)
		return theTerrain;
	return NULL;
}

/**
 *
 */
Sensor* EFotoManager::sensor(int id)
{
	for (unsigned int i = 0; i < sensors.size(); i++)
		if (sensors.at(i)->getId() == id)
			return sensors.at(i);
	return NULL;
}

/**
 *
 */
Flight* EFotoManager::flight(int id)
{
	for (unsigned int i = 0; i < flights.size(); i++)
		if (flights.at(i)->getId() == id)
			return flights.at(i);
	return NULL;
}

/**
 *
 */
Image* EFotoManager::image(int id)
{
	for (unsigned int i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
			return images.at(i);
	return NULL;
}

/**
 *
 */
Point* EFotoManager::point(int id)
{
	for (unsigned int i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
			return points.at(i);
	return NULL;
}

/**
 *
 */
InteriorOrientation* EFotoManager::IO(int id)
{
	for (unsigned int i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == id)
			return IOs.at(i);
	return NULL;
}

/**
 *
 */
ExteriorOrientation* EFotoManager::EO(int id)
{
	for (unsigned int i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == id)
			return EOs.at(i);
	return NULL;
}

/**
 *
 */
std::string EFotoManager::getXml(std::string tagname)
{
	EDomElement root(xmlData);
	return root.elementByTagName(tagname).getContent();
}

/**
 *
 */
std::string EFotoManager::getXml(std::string tagname, std::string att, std::string value)
{
	EDomElement root(xmlData);
	return root.elementByTagAtt(tagname, att, value).getContent();
}


// EObject methods
//

/**
 *
 */
std::string EFotoManager::objectType(void)
{
	return "EFotoManager";
}

/**
 *
 */
std::string EFotoManager::objectAssociations(void)
{
	return "";
}

/**
 *
 */
bool EFotoManager::is(std::string s)
{
	return (s == "EFotoManager" ? true : false);
}


// XML methods
//
void EFotoManager::xmlSetData(std::string xml)
{
	xmlData = xml;
}

std::string EFotoManager::xmlGetData()
{
	return xmlData;
}


//Other methods
//

void EFotoManager::setSavedState(bool state)
{
	savedState = state;
}

bool EFotoManager::getSavedState()
{
	return savedState; // O uso desta variável precisar ser revisto!
}

/**
 *
 */
void EFotoManager::setInterfaceType(std::string newInterfaceType)
{
	interfaceType = newInterfaceType;
}

/**
 *
 */
std::string EFotoManager::getInterfaceType()
{
	return interfaceType;
}

/**
 *
 */
void EFotoManager::setNextModule(nextModule_t newModule)
{
	nextModule = newModule;
}

/**
 *
 */
void EFotoManager::setNextImage(int newImage)
{
	nextImage = newImage;
}

/**
 *
 */
bool EFotoManager::execProject(std::string filename)
{
	bool result;
	nextModule = NEXT_NONE;
		if (project == NULL)
	{
		project = new ProjectManager(this);
	}
		result = project->exec(filename);
	return result;
}

/**
 *
 */
bool EFotoManager::reloadProject()
{
	nextModule = NEXT_NONE;
		if (project != NULL)
	{
		if (fotoTri !=NULL)
		{
			stopPT();
		}
		if (interiorOrientation != NULL)
		{
			stopIO(interiorOrientation->getId());
		}
		if (spatialRessection != NULL)
		{
			stopSR(spatialRessection->getId());
		}
		if (dem != NULL)
		{
			stopDEM();
		}
		if (ortho != NULL)
		{
			stopOrtho();
				}
				if (sp != NULL)
				{
					stopSP();
				}
				return project->reload();
		}
	else
	{
			return false;
	}
}

/**
 *
 */
bool EFotoManager::execIO(int id)
{
	bool result;
	nextModule = NEXT_RELOAD;
	Image* ioImage = instanceImage(id);
	if (ioImage == NULL)
	{
		return false;
	}
	Sensor* ioSensor = instanceSensor(ioImage->getSensorId());
	InteriorOrientation* io = instanceIO(id);
	if (io == NULL)
	{
		io = new InteriorOrientation(id);
		IOs.push_back(io);
	}

	interiorOrientation = new IOManager(this, ioSensor, ioImage, io);
	result = interiorOrientation->exec();

	return result;
}

void EFotoManager::stopIO(int id)
{
	Image* ioImage = instanceImage(id);
	delete interiorOrientation;
	interiorOrientation = NULL;
	deleteIO(id);
	deleteSensor(ioImage->getSensorId());
	deleteImage(id);
}

/**
 *
 */
bool EFotoManager::execSR(int id)
{
	bool result;
	nextModule = NEXT_RELOAD;
	Image* srImage = instanceImage(id);
	SpatialRessection* sr = (SpatialRessection*) instanceEO(id);
	if (sr == NULL)
	{
		sr = new SpatialRessection(id);
		EOs.push_back(sr);
	}
	if (srImage == NULL)
	{
		return false;
	}
	sr->setImageId(id);
	sr->setImage(srImage);
	Sensor* srSensor = instanceSensor(srImage->getSensorId());
	Flight* srFlight = instanceFlight(srImage->getFlightId());
	InteriorOrientation* srIO = instanceIO(id);

	Terrain* srTerrain = instanceTerrain();
	srFlight->setTerrain(srTerrain);

	instanceAllPoints();
	spatialRessection = new SRManager(this, srTerrain, srSensor, srFlight, srImage, srIO, sr, points);
	result = spatialRessection->exec();

	return result;
}

void EFotoManager::stopSR(int id)
{
	Image* srImage = instanceImage(id);
	delete spatialRessection;
	spatialRessection = NULL;
	deleteEO(id);
	deleteIO(id);
	deleteFlight(srImage->getFlightId());
	deleteSensor(srImage->getSensorId());
	deleteTerrain();
	deleteImage(id);
	points.clear();
}

/**
 *
 */
bool EFotoManager::execSP()
{
	bool result;

	nextModule = NEXT_RELOAD;

	instanceAllImages();
	instanceAllPoints();
		instanceAllIOs();
		instanceAllEOs();

	for (int i = images.size() - 1; i >=0; i--)
	{
		Image* img = images.at(i);
		Sensor* sensor = instanceSensor(img->getSensorId());
		Flight* flight = instanceFlight(img->getFlightId());
		InteriorOrientation* imgIO = instanceIO(img->getId());
		ExteriorOrientation* imgEO = instanceEO(img->getId());

		img->setSensor(sensor);
		img->setFlight(flight);
		img->setIO(imgIO);
		img->setEO(imgEO);

		if (imgIO == NULL || imgEO == NULL)
		{
			deleteImage(img->getId());
		}
	}

	sp = new SPManager(this, images, EOs);

	result = sp->exec();

	return result;
}

void EFotoManager::stopSP()
{
	delete sp;
	sp = NULL;
	//deleteSensor(images.at(0)->getSensorId());
	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
		deleteEO(images.at(0)->getId());
		deleteSensor(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}
}

bool EFotoManager::execPT()
{
	bool result;
	nextModule = NEXT_RELOAD;

	instanceAllImages();
	instanceAllPoints();
	instanceAllIOs();

	EDomElement sensor(getXml("sensor"));
	Sensor *ptSensor = instanceSensor(Conversion::stringToInt(sensor.attribute("key")));

	Flight *ptFlight = instanceFlight(1);
	Terrain* ptTerrain = instanceTerrain();
	fotoTri = new PTManager(this,images,IOs,ptSensor,ptFlight,ptTerrain);
	result = fotoTri->exec();

	return result;
}

void EFotoManager::stopPT()
{

	delete fotoTri;
	fotoTri = NULL;
	deleteSensor(images.at(0)->getSensorId());
	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}

}

bool EFotoManager::execDEM()
{
	bool result;

	nextModule = NEXT_RELOAD;

	instanceAllImages();
		instanceAllPoints();
		instanceAllIOs();
		instanceAllEOs();

	for (int i = images.size() - 1; i >=0; i--)
	{
		Image* img = images.at(i);
		Sensor* sensor = instanceSensor(img->getSensorId());
		InteriorOrientation* imgIO = instanceIO(img->getId());
		ExteriorOrientation* imgEO = instanceEO(img->getId());

		img->setSensor(sensor);
		img->setIO(imgIO);
		img->setEO(imgEO);

		if (imgIO == NULL || imgEO == NULL)
		{
			deleteImage(img->getId());
		}
	}

	dem = new DEMManager(this, images, EOs);

	result = dem->exec();

	return result;
}

void EFotoManager::stopDEM()
{
	delete dem;
	dem = NULL;

	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
				deleteEO(images.at(0)->getId());
				deleteSensor(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}
}

bool EFotoManager::execOrtho()
{
	bool result;

	nextModule = NEXT_RELOAD;

	instanceAllImages();
		instanceAllIOs();
		instanceAllEOs();

	for (int i = images.size() - 1; i >=0; i--)
	{
		Image* img = images.at(i);
		Sensor* sensor = instanceSensor(img->getSensorId());
		InteriorOrientation* imgIO = instanceIO(img->getId());
		ExteriorOrientation* imgEO = instanceEO(img->getId());

		img->setSensor(sensor);
		img->setIO(imgIO);
		img->setEO(imgEO);

		if (imgIO == NULL || imgEO == NULL)
		{
			deleteImage(img->getId());
		}
	}

	ortho = new OrthoManager(this, images, EOs);

	result = ortho->exec();

	return result;
}

void EFotoManager::stopOrtho()
{
	delete ortho;
	ortho = NULL;

	int numPoints=points.size();
	int numImages=images.size();

	for (int i=0;i<numPoints;i++)
	{
		deletePoint(points.at(0)->getId());
	}
	for (int i=0;i<numImages;i++)
	{
		deleteIO(images.at(0)->getId());
		deleteImage(images.at(0)->getId());
	}
}

bool EFotoManager::execEPR()
{
	bool result;

		nextModule = NEXT_RELOAD;

		report_project.setXml(xmlData);

	report = new ReportManager(this);

	result = report->exec();

	return result;
}

void EFotoManager::stopEPR()
{
	if (report != NULL)
		delete report;
	report = NULL;
}


/**
 *
 */
bool EFotoManager::exec(std::string filename)
{
	if (filename != "")
	{
		execProject(filename);
	}
	switch (nextModule)
	{
	case NEXT_PROJECT:
		execProject();
		break;
	case NEXT_RELOAD:
		reloadProject();
		break;
	case NEXT_IO:
		savedState = true;
		execIO(nextImage);
		break;
	case NEXT_SR:
		savedState = true;
		execSR(nextImage);
		break;
	case NEXT_PT:
		savedState = true;
		execPT();
		break;
	case NEXT_DEM:
		savedState = true;
		execDEM();
		break;
	case NEXT_ORTHO:
		savedState = true;
		execOrtho();
		break;
	case NEXT_SP:
		savedState = true;
		execSP();
		break;
	case NEXT_Report:
		savedState = true;
		execEPR();
		break;
	case NEXT_PTReport:
		savedState = true;
		execPTReport();
				break;
	default:
		nextModule = NEXT_NONE;
	}

	return true;
}

int EFotoManager::getFreeImageId()
{

	int result = 0;

	std::deque<EDomElement> images = EDomElement(getXml("images")).children();
	result = 1;
	for (int i = images.size()-1; i >= 0; i--)
	{
		if (Conversion::stringToInt(images.at(i).attribute("key")) >= result)
			result = Conversion::stringToInt(images.at(i).attribute("key"))+1;
	}

	return result;

}

int EFotoManager::getFreePointId()
{

	int result = 0;

	std::deque<EDomElement> points = EDomElement(getXml("points")).children();
	result = 1;
	for (int i = points.size()-1; i >= 0; i--)
	{
		if (Conversion::stringToInt(points.at(i).attribute("key")) >= result)
			result = Conversion::stringToInt(points.at(i).attribute("key"))+1;
	}

	return result;

}

void EFotoManager::execPTReport()
{
	report_project.setXml(xmlData);

	QString fileExport= QFileDialog::getSaveFileName(0,"Save full PT report",".","*.txt");

	if (fileExport.isEmpty())
	{
		reloadProject();
		return;
	}

	if(!fileExport.endsWith(".txt"))
		fileExport.append(".txt");

	// Create and run full report
	PhotoTriReport pt_report(this);
	pt_report.createReport((char *) fileExport.toStdString().c_str());

	reloadProject();
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
