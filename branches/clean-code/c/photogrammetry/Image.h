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
//#include "EDom.h"
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
class ExteriorOrientation;

/**
* class Image
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class Image : public EDom
{
	//EOBJECT

	// Private attributes
	//
    int id{-1};
    int sensorId{-1};
    int flightId{-1};
    int resolution{0};
    std::string resolutionUnit{""};
    int width{0};
    int height{0};
    std::string filename{""};
    std::string filepath{""};
    double flightDirection{0.0};
    bool flightDirectionAvailable{false};

    std::string imageId{""};
	ObjectSpaceCoordinate spatialCoordinates;

    bool gnssAvailable{false};
    bool gnssSigmaAvailable{false};
    bool insAvailable{false};
    bool insSigmaAvailable{false};
    std::string gnssType{};
    std::string insType{};
    double gnssX0{0.0};
    double gnssY0{0.0};
    double gnssZ0{0.0};
	Matrix gnssSigma;
    double insOmega{0.0};
    double insPhi{0.0};
    double insKappa{0.0};
    Matrix insSigma{};

	// Composed objects
	//
	std::deque<ImageFiducialMark> digFidMarks;

	// Associated objects
	//
    Sensor* mySensor{nullptr};
    Flight* myFlight{nullptr};
    InteriorOrientation* myIO{nullptr};
    ExteriorOrientation* myEO{nullptr}; // Muito em breve (quando definirmos a fototri) a classe usada aqui será uma ExteriorOrientation.
    std::deque<Point*> myPoints{};

public:

	// Constructors and destructors
	Image();
    explicit Image(int myId, int mySensorId); // Constructor with ids only, needed in project use.
	virtual ~Image();

	// Private attribute accessor methods
    int getId() const;
    int getSensorId() const;
    int getFlightId() const;
    unsigned int getResolution() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    std::string getFilename() const;
    std::string getFilepath() const;
    std::string getImageId() const;
    bool isGnssAvailable() const;
    bool isInsAvailable() const;
    double getGnssX0() const;
    double getGnssY0() const;
    double getGnssZ0() const;
    std::string getGnssType() const;
    double getInsOmega() const;
    double getInsPhi() const;
    double getInsKappa() const;
    std::string getInsType() const;

	void setFlightDirection(double radianAngle);//Paulo 27/09/11
    double getFlightDirection() const;//Paulo 27/09/11

	// Composed objects accessor methods
    std::deque<ImageFiducialMark> getDigFidMarks() const; // Get for all DigFidMarks using deque.

	void putDigFidMark(ImageFiducialMark newDigFidMark); // Put one DigFidMark between the components of the image.
	ImageFiducialMark getDigFidMark(int id);
	ImageFiducialMark getDigFidMarkAt(unsigned int index); // Get one DigFidMark by index for iterations.
    int countDigFidMarks() const;  // Inform the number of DigFidMarks composed in the image.

	void deleteDigFidMark(int id); // Delete one DigFidMark by id in the image.
	void clearDigFidMarks(); // Delete all DigFidMarks composed in the image.

	// Associated object accessor methods
	//
	void setSensor(Sensor* newSensor); // Set image association. Generally one sensor only.
	Sensor* getSensor();  // Get associated sensor at image.
	void setFlight(Flight* newFlight); // Set image association. Generally one sensor only.
	Flight* getFlight();  // Get associated sensor at image.

	void setIO(InteriorOrientation* newIO); // Set image association. Generally zero or one IO.
	InteriorOrientation* getIO(); // Get associated IO at image if there.
	void setEO(ExteriorOrientation* newEO); // Set image association. Generally zero or one EO.
	ExteriorOrientation* getEO(); // Get associated EO at image if there.

	void putPoint(Point* newPointAssociation); // Set image association. Generally from zero to many points.
	void clearPoints(); // Remove associations.
	void removePoint(int id); // Remove one association;
	Point* getPoint(int pointId); // Get associated point at image by id.
    int countPoints() const; // Inform the number of points associated the image.
	Point* getPointAt(unsigned int index); // Get associated point at image by index in deque for iterations.

	void sortPoints(); // sort points with ControlPoints before PhotogrammetricPoints //Paulo 27/09/2011

	// XML methods
	//
	void xmlSetData(std::string xml);
	std::string xmlGetData();

	//Other methods
	//
    static int load(); // Try load the image using filePath and fileName sets in the image.

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGE_H
