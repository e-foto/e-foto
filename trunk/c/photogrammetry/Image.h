/*******************************************************************************
		  Image.h
*******************************************************************************/

#ifndef IMAGE_H
#define IMAGE_H
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
//#include "EObject.h"
#include "ImageFiducialMark.h"
#include "ObjectSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Sensor;
class Flight;
class Point;
class InteriorOrientation;
class SpatialRessection;

/**
* class Image
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class Image : public EObject
{
	//EOBJECT

	// Private attributes
	//
	int id;
	int sensorId;
	int flightId;
	unsigned int resolution;
    std::string resolutionUnit;
	unsigned int width;
	unsigned int height;
    std::string filename;
    std::string filepath;
    double flightDirection;
    bool flightDirectionAvailable;

    std::string imageId;
	ObjectSpaceCoordinate spatialCoordinates;

	// IMPORTANTE: Gambiarra temporaria...
	bool gnssAvailable; bool gnssSigmaAvailable; bool insAvailable; bool insSigmaAvailable;
    std::string gnssType; std::string insType;
	double gnssX0; double gnssY0; double gnssZ0;
	Matrix gnssSigma;
	double insOmega; double insPhi; double insKappa;
	Matrix insSigma;

	// Composed objects
	//
    std::deque<ImageFiducialMark> digFidMarks;

	// Associated objects
	//
	Sensor* mySensor;
	Flight* myFlight;
	InteriorOrientation* myIO;
	SpatialRessection* myEO; // Muito em breve (quando definirmos a fototri) a classe usada aqui será uma ExteriorOrientation.
    std::deque<Point*> myPoints;

public:

	// Constructors and destructors
    Image();
	Image(int myId, int mySensorId); // Constructor with ids only, needed in project use.
	virtual ~Image();

	// Private attribute accessor methods
	//
	void setId(int newId);
	void setSensorId(int newSensorId);
	void setFlightId(int newFlightId);
	void setResolution(unsigned int newResolution);
    void setResolutionUnit(std::string newResolutionUnit);
	void setWidth(unsigned int newWidth);
	void setHeight(unsigned int newHeight);
    void setFilename(std::string newFilename);
    void setFilepath(std::string newFilepath);
	int getId();
	int getSensorId();
	int getFlightId();
	unsigned int getResolution();
    std::string getResolutionUnit();
	unsigned int getWidth();
	unsigned int getHeight();
    std::string getFilename();
    std::string getFilepath();
    std::string getImageId();
	bool isGnssAvailable();
	bool isInsAvailable();
	double getGnssX0();
	double getGnssY0();
	double getGnssZ0();
    std::string getGnssType();
	double getInsOmega();
	double getInsPhi();
	double getInsKappa();
    std::string getInsType();

	void setFlightDirection(double radianAngle);//Paulo 27/09/11
	double getFlightDirection();//Paulo 27/09/11
    bool isFlightDirectionAvailable() {return flightDirectionAvailable;}

	// Composed objects accessor methods
	//
    void setDigFidMarks(std::deque<ImageFiducialMark> newDigFidMarks); // Set for many DigFidMarks using deque.
    std::deque<ImageFiducialMark> getDigFidMarks(); // Get for all DigFidMarks using deque.

	void putDigFidMark(ImageFiducialMark newDigFidMark); // Put one DigFidMark between the components of the image.
	ImageFiducialMark getDigFidMark(int id);
	ImageFiducialMark getDigFidMarkAt(unsigned int index); // Get one DigFidMark by index for iterations.
	int countDigFidMarks();  // Inform the number of DigFidMarks composed in the image.

	void deleteDigFidMark(int id); // Delete one DigFidMark by id in the image.
	void deleteDigFidMarkAt(unsigned int index); // Delete one DigFidMark by index in the image.
	void clearDigFidMarks(); // Delete all DigFidMarks composed in the image.

	// Associated object accessor methods
	//
	void setSensor(Sensor* newSensor); // Set image association. Generally one sensor only.
	Sensor* getSensor();  // Get associated sensor at image.
	void setFlight(Flight* newFlight); // Set image association. Generally one sensor only.
	Flight* getFlight();  // Get associated sensor at image.

	void setIO(InteriorOrientation* newIO); // Set image association. Generally zero or one IO.
	InteriorOrientation* getIO(); // Get associated IO at image if there.
	void setEO(SpatialRessection* newEO); // Set image association. Generally zero or one EO.
	SpatialRessection* getEO(); // Get associated EO at image if there.

	void putPoint(Point* newPointAssociation); // Set image association. Generally from zero to many points.
    void clearPoints(); // Remove associations.
    void removePoint(int id); // Remove one association;
	Point* getPoint(int pointId); // Get associated point at image by id.
	int countPoints(); // Inform the number of points associated the image.
	Point* getPointAt(unsigned int index); // Get associated point at image by index in deque for iterations.

	void sortPoints(); // sort points with ControlPoints before PhotogrammetricPoints //Paulo 27/09/2011
	// EObject methods
	//
    std::string objectType(void);
    std::string objectAssociations(void);
    bool is(std::string s);

	// XML methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();

	//Other methods
	//
	int load(); // Try load the image using filePath and fileName sets in the image.

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGE_H
