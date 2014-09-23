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

#include "Project.h"

#include <QApplication>

#include "ProjectHeader.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Terrain.h"
#include "Flight.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "ExteriorOrientation.h"
#include "SpatialRessection.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructor
//

/**
 *
 */
Project::Project()
{
	xmlData = "";
	processStates = "";
    theTerrain = NULL;
    theHeader = NULL;
    thePhotoTri = NULL;
}

/**
 *
 */
Project::~Project()
{

}


// Instance objects accessor methods
//

/**
 *
 */
ProjectHeader *Project::instanceHeader()
{
	if (theHeader != NULL)
		return theHeader;
	EDomElement root(xmlData);
	EDomElement xmlHeader = root.elementByTagName("projectHeader");
	if (xmlHeader.getContent().compare("") == 0)
		return NULL;
	theHeader = new ProjectHeader;
	theHeader->xmlSetData(xmlHeader.getContent());
	return theHeader;

}

Terrain* Project::instanceTerrain()
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
Sensor* Project::instanceSensor(int id)
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
Flight* Project::instanceFlight(int id)
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
Image* Project::instanceImage(int id)
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

void Project::instanceAllImages()
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
void Project::instanceAllPoints()
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


Point* Project::instancePoint(int id)
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
InteriorOrientation* Project::instanceIO(int imageId)
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

void Project::instanceAllIOs()
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
ExteriorOrientation* Project::instanceEO(int imageId)
{
	for (unsigned int i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == imageId)
			return EOs.at(i);
	EDomElement root(xmlData);
	EDomElement xmlEO = root.elementByTagAtt("imageEO", "image_key", Conversion::intToString(imageId));
	EDomElement xmlSR = root.elementByTagAtt("imageSR", "image_key", Conversion::intToString(imageId));
	if (xmlEO.getContent().compare("") == 0 || xmlSR.getContent().compare("") == 0)
		return NULL;
	//if (xmlEO.attribute("type").compare("spatialRessection") == 0)
	{
		SpatialRessection* newEO = new SpatialRessection();
		newEO->xmlSetData(xmlEO.getContent().append(xmlSR.getContent()));
		EOs.push_back(newEO);
		return (ExteriorOrientation*) newEO;
	}
	return NULL;
}

void Project::instanceAllEOs()
{
	EDomElement root(xmlData);
    std::deque<EDomElement> xmlAllEOs = root.elementsByTagName("imageEO");
    std::deque<EDomElement> xmlAllSRs = root.elementsByTagName("imageSR");
	for (unsigned int i = 0 ;i < xmlAllEOs.size();i++)
	{
		qApp->processEvents();
		if (xmlAllEOs.at(i).getContent().compare("") == 0)
			continue;
		bool notAvailable = true;
		for (unsigned int j = 0; j < EOs.size() && notAvailable; j++)
			if (EOs.at(j)->getImageId() == Conversion::stringToInt(xmlAllEOs.at(i).attribute("image_key")))
				notAvailable = false;
		//InteriorOrientation *pkj;
		//pkj->getImageId()
		if (notAvailable)
		{
			SpatialRessection* newEO = new SpatialRessection();
			newEO->setImage(image(Conversion::stringToInt(xmlAllEOs.at(i).attribute("image_key"))));

			bool hasSR = false;
			for (unsigned int k = 0; k < xmlAllSRs.size() && !hasSR; k++)
				if (xmlAllEOs.at(i).attribute("image_key") == xmlAllSRs.at(k).attribute("image_key"))
				{
					hasSR = true;
					newEO->xmlSetData(xmlAllEOs.at(i).getContent()+xmlAllSRs.at(k).getContent());
					xmlAllSRs.erase(xmlAllSRs.begin()+k);
				}
			if (!hasSR)
				newEO->xmlSetData(xmlAllEOs.at(i).getContent());
			newEO->setImage(NULL);
			EOs.push_back(newEO);
		}
	}
}

PhotoTri *Project::instancePhotoTri()
{
    if (thePhotoTri != NULL)
        return thePhotoTri;
    EDomElement root(xmlData);
    EDomElement xmlPhoto = root.elementByTagName("phototriangulation");
    if (xmlPhoto.getContent().compare("") == 0)
        return NULL;
    thePhotoTri = new PhotoTri;
    thePhotoTri->xmlSetData(xmlPhoto.getContent());
    return thePhotoTri;
}

void Project::deleteHeader(bool makeReconnections)
{
	if (theHeader != NULL)
	{
		delete(theHeader);
		theHeader = NULL;
	}
	if (makeReconnections)
		linkAll();
}


/**
 *
 */
void Project::deleteTerrain(bool makeReconnections)
{
	if (theTerrain != NULL)
	{
		delete(theTerrain);
		theTerrain = NULL;
	}
	if (makeReconnections)
		linkAll();
}

/**
 *
 */
void Project::deleteSensor(int id, bool makeReconnections)
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
	if (makeReconnections)
		linkAll();
}

/**
 *
 */
void Project::deleteFlight(int id, bool makeReconnections)
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
	if (makeReconnections)
		linkAll();
}

/**
 *
 */
void Project::deleteImage(int id, bool makeReconnections)
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
	if (makeReconnections)
		linkAll();
}

/**
 *
 */
void Project::deletePoint(int id, bool makeReconnections)
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
	if (makeReconnections)
		linkAll();
}

/**
 *
 */
void Project::deleteIO(int id, bool makeReconnections)
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
	if (makeReconnections)
		linkAll();
}

/**
 *
 */
void Project::deleteEO(int id, bool makeReconnections)
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
		//EDomElement xmlEO(myEO->xmlGetData());
		//if (xmlEO.attribute("type").compare("spatialRessection") == 0)
		if (myEO->is("SpatialResection"))
		{
			SpatialRessection* mySR = (SpatialRessection*) myEO;
			delete(mySR);
		}
		EOs.erase(EOs.begin() + i);
	}
	if (makeReconnections)
		linkAll();
}

void Project::deletePhotoTri(bool makeReconnections)
{
    if (thePhotoTri != NULL)
    {
        delete(thePhotoTri);
        thePhotoTri = NULL;
    }
    if (makeReconnections)
        linkAll();
}

void Project::closeProject()
{
	//Rever
}

void Project::addSensor(std::string data, bool makeReconnections)
{
	if (data != "")
	{
		EDomElement xmlSensor(data);
		int id = Conversion::stringToInt(xmlSensor.elementByTagName("sensor").attribute("key"));
		for (int i = sensors.size()-1; i >= 0; i--)
			if (sensors.at(i)->getId() == id)
			{
				delete sensors.at(i);
				sensors.erase(sensors.begin()+i);
			}
		if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
				xmlSensor.elementByTagName("calculationMode").toString().compare("With Fiducial Marks") == 0)
		{
			SensorWithFiducialMarks* newSensorWithFiducialMarks = new SensorWithFiducialMarks();
			newSensorWithFiducialMarks->xmlSetData(xmlSensor.getContent());
			sensors.push_back(newSensorWithFiducialMarks);
		}
		else if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
				 xmlSensor.elementByTagName("calculationMode").toString().compare("With Sensor Dimensions") == 0)
		{
			SensorWithKnowDimensions* newSensorWithKnowDimensions = new SensorWithKnowDimensions();
			newSensorWithKnowDimensions->xmlSetData(xmlSensor.getContent());
			sensors.push_back(newSensorWithKnowDimensions);
		}
		else if (xmlSensor.elementByTagName("geometry").toString().compare("frame") == 0 &&
				 xmlSensor.elementByTagName("calculationMode").toString().compare("Fixed Parameters") == 0)
		{
			SensorWithKnowParameters* newSensorWithKnowParameters = new SensorWithKnowParameters();
			newSensorWithKnowParameters->xmlSetData(xmlSensor.getContent());
			sensors.push_back(newSensorWithKnowParameters);
		}
	}
	else
	{
		sensors.push_back(new SensorWithFiducialMarks(getFreeSensorId()));
	}

	if (makeReconnections)
		linkAll();
}

void Project::addFlight(std::string data, bool makeReconnections)
{
	if (data != "")
	{
		EDomElement xmlFlight(data);
		int id = Conversion::stringToInt(xmlFlight.elementByTagName("flight").attribute("key"));
		for (int i = flights.size()-1; i >= 0; i--)
			if (flights.at(i)->getId() == id)
			{
				delete flights.at(i);
				flights.erase(flights.begin()+i);
			}
		Flight* flight = new Flight();
		flight->xmlSetData(xmlFlight.getContent());
		flights.push_back(flight);
	}
	else
	{
		flights.push_back(new Flight(getFreeFlightId(),1));
	}

	if (makeReconnections)
		linkAll();
}

void Project::addImage(std::string data, bool makeReconnections)
{
	if (data != "")
	{
		EDomElement xmlImage(data);
		int id = Conversion::stringToInt(xmlImage.elementByTagName("image").attribute("key"));
		for (int i = images.size()-1; i >= 0; i--)
			if (images.at(i)->getId() == id)
			{
				delete images.at(i);
				images.erase(images.begin()+i);
			}
		Image* image = new Image();
		image->xmlSetData(xmlImage.getContent());
		images.push_back(image);
	}
	else
	{
		images.push_back(new Image(getFreeImageId(),1));
	}

	if (makeReconnections)
		linkAll();
}

void Project::addPoint(std::string data, bool makeReconnections)
{
	if (data != "")
	{
		EDomElement xmlPoint(data);
		int id = Conversion::stringToInt(xmlPoint.elementByTagName("point").attribute("key"));
		for (int i = points.size()-1; i >= 0; i--)
			if (points.at(i)->getId() == id)
			{
				delete points.at(i);
				points.erase(points.begin()+i);
			}
		Point* point = new Point();
		point->xmlSetData(xmlPoint.getContent());
		points.push_back(point);
	}
	else
	{
		points.push_back(new Point(getFreePointId()));
	}

	if (makeReconnections)
		linkAll();
}

void Project::addIO(std::string data, bool makeReconnections)
{
	if (data != "")
	{
		EDomElement xmlIO(data);
		int id = Conversion::stringToInt(xmlIO.elementByTagName("imageIO").attribute("key"));
		for (int i = IOs.size()-1; i >= 0; i--)
			if (IOs.at(i)->getImageId() == id)
			{
				delete IOs.at(i);
				IOs.erase(IOs.begin()+i);
			}
		InteriorOrientation* io = new InteriorOrientation();
		io->xmlSetData(xmlIO.getContent());
		IOs.push_back(io);
	}

	if (makeReconnections)
		linkAll();
}

void Project::addEO(std::string data, bool makeReconnections)
{
	if (data != "")
	{
		EDomElement xmlEO(data);
		int id = Conversion::stringToInt(xmlEO.elementByTagName("imageEO").attribute("key"));
		for (int i = EOs.size()-1; i >= 0; i--)
			if (EOs.at(i)->getImageId() == id)
			{
				delete EOs.at(i);
				EOs.erase(EOs.begin()+i);
			}
		SpatialRessection* eo = new SpatialRessection();
		eo->xmlSetData(xmlEO.getContent());
		EOs.push_back((ExteriorOrientation*)eo);
	}

	if (makeReconnections)
		linkAll();
}

ProjectHeader *Project::header()
{
	if (theHeader != NULL)
		return theHeader;
	return NULL;
}

/**
 *
 */
Terrain* Project::terrain()
{
	if (theTerrain != NULL)
		return theTerrain;
	return NULL;
}

/**
 *
 */
Sensor* Project::sensor(int id)
{
	for (unsigned int i = 0; i < sensors.size(); i++)
		if (sensors.at(i)->getId() == id)
			return sensors.at(i);
	return NULL;
}

/**
 *
 */
Flight* Project::flight(int id)
{
	for (unsigned int i = 0; i < flights.size(); i++)
		if (flights.at(i)->getId() == id)
			return flights.at(i);
	return NULL;
}

/**
 *
 */
Image* Project::image(int id)
{
	for (unsigned int i = 0; i < images.size(); i++)
		if (images.at(i)->getId() == id)
			return images.at(i);
	return NULL;
}

/**
 *
 */
Point* Project::point(int id)
{
	for (unsigned int i = 0; i < points.size(); i++)
		if (points.at(i)->getId() == id)
			return points.at(i);
	return NULL;
}

/**
 *
 */
InteriorOrientation* Project::IO(int id)
{
	for (unsigned int i = 0; i < IOs.size(); i++)
		if (IOs.at(i)->getImageId() == id)
			return IOs.at(i);
	return NULL;
}

/**
 *
 */
ExteriorOrientation* Project::EO(int id)
{
	for (unsigned int i = 0; i < EOs.size(); i++)
		if (EOs.at(i)->getImageId() == id)
			return EOs.at(i);
	return NULL;
}

PhotoTri *Project::photoTri()
{
    if (thePhotoTri != NULL)
        return thePhotoTri;
    return NULL;
}

/**
 *
 */
std::string Project::getXml(std::string tagname)
{
	EDomElement root(getXml());
	return root.elementByTagName(tagname).getContent();
}

/**
 *
 */
std::string Project::getXml(std::string tagname, std::string att, std::string value)
{
	EDomElement root(getXml());
	return root.elementByTagAtt(tagname, att, value).getContent();
}


// EObject methods
//

/**
 *
 */
std::string Project::objectType(void)
{
	return "Project";
}

/**
 *
 */
std::string Project::objectAssociations(void)
{
	return "";
}

/**
 *
 */
bool Project::is(std::string s)
{
	return (s == "Project" ? true : false);
}


// XML methods
//
void Project::setXml(std::string xml)
{
	xmlData = xml;

	EDomElement ede(xmlData);
	//EDomElement dem = ede.elementByTagName("DEMs");
	//EDomElement eoi = ede.elementByTagName("orthoImages");
    //EDomElement feat = ede.elementByTagName("features");
	EDomElement sr = ede.elementByTagName("spatialResections");

	// Rever a parte de estados de processos... armazenados e acessados na string processStates
    processStates = /*dem.getContent() + eoi.getContent() + feat.getContent() +*/ sr.getContent();
	xmlData = ede.indent('\t').getContent();

	instanceAllImages();
	instanceAllPoints();
	instanceAllIOs();
	instanceAllEOs();

	instanceHeader();
	instanceTerrain();
	instanceSensor(1);
	instanceFlight(1);
    instancePhotoTri();

	//Rever a criação dos instance DEMs, EOIs e FEATs.

	linkAll();
}

void Project::linkAll()
{
	Sensor *sns = sensor(1);
	Flight *flt = flight(1);

	if (flt)
	{
		flt->setTerrain(theTerrain);
		flt->setSensor(sns);
		flt->clearImages();
	}
	if (sns)
	{
		sns->clearImages();
		sns->clearFlights();
		sns->putFlight(flt);
	}

	for (unsigned int i = 0; i < points.size(); i++)
	{
		Point* point = points.at(i);
		point->clearImages();
	}

	for (unsigned int j = 0; j < images.size(); j++)
	{
		Image *img = images.at(j);
		img->clearPoints();

		for (unsigned int i = 0; i < points.size(); i++)
		{
			Point* pointToInsert = points.at(i);

			if (pointToInsert->hasImageCoordinate(img->getId()))
			{
				img->putPoint(pointToInsert);
				pointToInsert->putImage(img);
			}
		}

		img->setSensor(sns);
		img->setFlight(flt);
		if (sns) sns->putImage(img);
		if (flt) flt->putImage(img);

	}

	for (int i=0;i<IOs.size();i++)
	{
		InteriorOrientation *io=IOs.at(i);
		Image *img = image(io->getImageId());
		io->setImage(img);
		if (img) img->setIO(io);
	}

	for (int i=0;i<EOs.size();i++)
	{
		ExteriorOrientation *eo=EOs.at(i);
		Image *img = image(eo->getImageId());
		eo->setImage(img);
		if (img) img->setEO((SpatialRessection*)eo);
	}

    if (thePhotoTri)
    {
        thePhotoTri->clearImages();
        for (int i=0; i<thePhotoTri->getImageKeys().size(); i++)
        {
            Image *img = image(thePhotoTri->getImageKeys().at(i));
            if (img) thePhotoTri->putImage(img);
        }

        thePhotoTri->clearPoints();
        for (int i=0; i<thePhotoTri->getPointKeys().size(); i++)
        {
            Point *pt = point(thePhotoTri->getPointKeys().at(i));
            if (pt) thePhotoTri->putPoint(pt);
        }
    }
}

std::string Project::getXml()
{
	if (xmlData.empty())
		return "";

    std::string xmlData = "";
	xmlData += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xmlData += "<?xml-stylesheet type=\"text/xsl\" href=\"xsl/epp.xsl\"?>\n\n";
	xmlData += "<efotoPhotogrammetricProject version=\"1.0.42\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	xmlData += "xsi:noNamespaceSchemaLocation=\"EPPSchema/epp_structure.xsd\"\n";
	xmlData += "xmlns:gml=\"http://www.opengis.net/gml\"\n";
	xmlData += "xmlns:mml=\"http://www.w3.org/1998/Math/MathML\">\n";
	xmlData += header() ? header()->xmlGetData() : "";
	xmlData += terrain() ? terrain()->xmlGetData() : "";
	xmlData += "<sensors>\n";
	xmlData += sensor(1) ? sensor(1)->xmlGetData() : "";
	xmlData += "</sensors>\n";
	xmlData += "<flights>\n";
	xmlData += flight(1) ? flight(1)->xmlGetData() : "";
	xmlData += "</flights>\n";
	xmlData += "<points>\n";
	for(int i=0; i<points.size(); i++)
	{
		xmlData += points.at(i)->xmlGetData();
	}
	xmlData += "</points>\n";
	xmlData += "<images>\n";
	for(int i=0; i<images.size(); i++)
	{
		xmlData += images.at(i)->xmlGetData();
	}
	xmlData += "</images>\n";
	xmlData += "<interiorOrientation>\n";
	for(int i=0; i<IOs.size(); i++)
	{
		xmlData += IOs.at(i)->xmlGetData();
	}
	xmlData += "</interiorOrientation>\n";
	xmlData += "<exteriorOrientation>\n";
	for(int i=0; i<EOs.size(); i++)
	{
		SpatialRessection* sp = (SpatialRessection*) EOs.at(i);
		xmlData += sp->xmlGetDataEO();
	}
	xmlData += "</exteriorOrientation>\n";

	// Rever aqui para adicionar os DEMs, EOIs e FEATs ao xml de saida.

	xmlData += "<spatialResections>\n";
	for(int i=0; i<EOs.size(); i++)
	{
		SpatialRessection* sp = (SpatialRessection*) EOs.at(i);
		xmlData += sp->xmlGetDataEO();
	}
	xmlData += "</spatialResections>\n";

	if (photoTri())
		xmlData += photoTri()->xmlGetData();

	xmlData += "</efotoPhotogrammetricProject>";

	EDomElement xml(xmlData);
	xmlData = xml.indent('\t').getContent();

	return xmlData;
}


int Project::getFreeSensorId()
{
	int result = 1;
	for (int i = sensors.size()-1; i >= 0; i--)
	{
		if (sensors.at(i)->getId() >= result)
			result = sensors.at(i)->getId()+1;
	}
	return result;
}

int Project::getFreeFlightId()
{
	int result = 1;
	for (int i = flights.size()-1; i >= 0; i--)
	{
		if (flights.at(i)->getId() >= result)
			result = flights.at(i)->getId()+1;
	}
	return result;
}

int Project::getFreeImageId()
{
	int result = 1;
	for (int i = images.size()-1; i >= 0; i--)
	{
		if (images.at(i)->getId() >= result)
			result = images.at(i)->getId()+1;
	}
	return result;
}

int Project::getFreePointId()
{
	int result = 1;
	for (int i = points.size()-1; i >= 0; i--)
	{
		if (points.at(i)->getId() >= result)
			result = points.at(i)->getId()+1;
	}
	return result;
}

bool Project::getSaveState()
{
	return xmlData.compare(getXml()) == 0;
}


PhotoTri::PhotoTri()
{

}

std::string PhotoTri::xmlGetData()
{
    // Em branco por enquanto;
}

void PhotoTri::xmlSetData(std::string xml)
{
    EDomElement root(xml);

    totalIterations = root.elementByTagName("iterations").toInt();
    converged = root.elementByTagName("converged").toString() == "true";
    metricConvergency = root.elementByTagName("metricConvergency").toDouble();
    angularConvergency = root.elementByTagName("angularConvergency").toDouble();
    rmse = root.elementByTagName("rmse").toDouble();

    std::deque<EDomElement> pts = root.elementByTagName("usedPoints").children();
    for (int i = 0; i <pts.size(); i++)
        pointKey.push_back(pts.at(i).elementByTagName("pointKey").toInt());

    std::deque<EDomElement> imgs = root.elementByTagName("usedImages").children();
    for (int i = 0; i <imgs.size(); i++)
        imageKey.push_back(imgs.at(i).elementByTagName("imageKey").toInt());

}

void PhotoTri::putImage(Image *img)
{
    bool insert = true;
    // Eliminamos primeiro a possibilidade duplicar uma associação.
    for (unsigned int i = 0; i < image.size(); i++)
        if (image.at(i) == img)
            insert = false;
    // Fazemos a nova associação.
    if (insert)
        image.push_back(img);
}

void PhotoTri::putPoint(Point *pt)
{
    bool insert = true;
    // Eliminamos primeiro a possibilidade duplicar uma associação.
    for (unsigned int i = 0; i < point.size(); i++)
        if (point.at(i) == pt)
            insert = false;
    // Fazemos a nova associação.
    if (insert)
        point.push_back(pt);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

