#ifndef SENSOR_H
#define SENSOR_H
/*******************************************************************************
		Sensor.h
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

#include "EObject.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class FrameSensor;
class SensorWithFiducialMarks;
class SensorWithKnowDimensions;
class SensorWithKnowParameters;
class Image;
class Flight;
class DetectorSpaceCoordinate;

class SpectralRange {
  int band;
  double inferiorLimit;
  double superiorLimit;

public:
  friend class Sensor;
  friend class FrameSensor;
  friend class SensorWithFiducialMarks;
  friend class SensorWithKnowDimensions;
  friend class SensorWithKnowParameters;
};


/**
  * class Sensor
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 07/02/2011
  * @version 1.4 - Rafael Alves de Aguiar & Irving da Silva Badolato.
  *
  */

class Sensor : public EObject {

protected:
  int id;
  std::string sensorId;
  std::string description;
  std::string geometry;
  std::string detector;
  std::string energySource;
  std::string calculationMode;
  std::string calibrationCertificateNumber;
  std::string calibrationCertificateDispatch;
  std::string calibrationCertificateExpiration;
  std::string spectralRangesUnit;
  std::deque<SpectralRange> spectralRanges;
  std::deque<Image*> myImages;
  std::deque<Flight*> myFlights;

public:
  Sensor();
  explicit Sensor(const Sensor& sensor);
  explicit Sensor(int myId);
  virtual ~Sensor();

  int getId();
  std::string getSensorId();
  virtual double getFocalDistance() = 0;
  virtual double getFocalDistanceSigma() = 0;
  virtual DetectorSpaceCoordinate getPrincipalPointCoordinates() = 0;
  std::string getDescription();
  std::string getCalculationMode();
  void putImage(Image* newImageAssociation); // Set association sensor. Generally from zero to many images and flights.
  void putFlight(Flight* newFligthAssociation);
  void clearImages()
  {
    myImages.clear();
  }
  void clearFlights()
  {
    myFlights.clear();
  }
  virtual std::string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSOR_H
