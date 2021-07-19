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

#include "CommonDef.h"

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

class Image : public EObject {
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

    bool gnssAvailable;
    bool gnssSigmaAvailable;
    bool insAvailable;
    bool insSigmaAvailable;
    std::string gnssType;
    std::string insType;
    double gnssX0;
    double gnssY0;
    double gnssZ0;
    Matrix gnssSigma;
    double insOmega;
    double insPhi;
    double insKappa;
    Matrix insSigma;

    std::deque<ImageFiducialMark> digFidMarks;

    Sensor* mySensor;
    Flight* myFlight;
    InteriorOrientation* myIO;
    SpatialRessection* myEO;
    std::deque<Point*> myPoints;

public:
    Image();
    explicit Image(int myId, int mySensorId);
    virtual ~Image();

    int getId();
    int getSensorId();
    int getFlightId();
    unsigned int getResolution();
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
    void setFlightDirection(double radianAngle);
    double getFlightDirection();
    std::deque<ImageFiducialMark> getDigFidMarks();
    void putDigFidMark(ImageFiducialMark newDigFidMark);
    ImageFiducialMark getDigFidMark(int id);
    ImageFiducialMark getDigFidMarkAt(unsigned int index);
    int countDigFidMarks();
    void deleteDigFidMark(int id);
    void clearDigFidMarks();
    void setSensor(Sensor* newSensor);
    Sensor* getSensor();
    void setFlight(Flight* newFlight);
    Flight* getFlight();
    void setIO(InteriorOrientation* newIO);
    InteriorOrientation* getIO();
    void setEO(SpatialRessection* newEO);
    SpatialRessection* getEO();
    void putPoint(Point* newPointAssociation);
    void clearPoints();
    void removePoint(int id);
    Point* getPoint(int pointId);
    int countPoints();
    Point* getPointAt(unsigned int index);
    void sortPoints();
    std::string objectType(void);
    std::string objectAssociations(void);
    bool is(std::string s);
    void xmlSetData(std::string xml);
    std::string xmlGetData();
    int load();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGE_H
