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

#include "EDom.h"

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

// Classes/Structs used in Sensor only.
class SpectralRange
{
    int band{0};
    double inferiorLimit{0.0};
    double superiorLimit{0.0};

public:
    SpectralRange(){};

	friend class Sensor;
	friend class FrameSensor;
	friend class SensorWithFiducialMarks;
	friend class SensorWithKnowDimensions;
	friend class SensorWithKnowParameters;
};



// The Sensor class
//

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

class Sensor : public EDom
{
	//EOBJECT

protected:

	// Private attributes
	//
    int id_{-1};
    std::string sensorId_{""};
    std::string description_{""};
    std::string geometry_{""};
    std::string detector_{""};
    std::string energySource_{""};
    std::string calculationMode_{""};
    std::string calibrationCertificateNumber_{""};
    std::string calibrationCertificateDispatch_{""};
    std::string calibrationCertificateExpiration_{""};
    std::string spectralRangesUnit_{""};
    std::deque<SpectralRange> spectralRanges_{};

	// Associated Objects
	//
    std::deque<Image*> myImages_{};
    std::deque<Flight*> myFlights_{};

public:

	// Constructors and destructors
	//
	Sensor();
    explicit Sensor(const Sensor& sensor);
    explicit Sensor(int myId);
	virtual ~Sensor();

	// Private attributes accessor methods
    //
        void setId(int newId);
        void setSensorId(std::string nickname);
    void setDescription(std::string newDescription);
    int getId() const;
        std::string getSensorId() const;
	virtual double getFocalDistance() = 0;
	virtual double getFocalDistanceSigma() = 0;
	virtual DetectorSpaceCoordinate getPrincipalPointCoordinates() = 0;
    std::string getDescription() const;
    std::string getCalculationMode() const;

	// Associated object accessor methods
	//
	void putImage(Image* newImageAssociation); // Set association sensor. Generally from zero to many images and flights.
	void putFlight(Flight* newFligthAssociation);
        void clearImages() {myImages_.clear();} // Remove associations.
        void clearFlights() {myFlights_.clear();}
	Image* getImage(int imageId); // Get associated image or flight at sensor by id.
	Flight* getFlight(int flightId);
    int countImages() const; // Inform the number of images or flights associated the sensor.
	Image* getImageAt(unsigned int index); // Get associated image or flight at sensor by index in deque for iterations.

    /**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome da classe seja aquele passado. Retorna falso no caso contrário.
 */
    virtual bool is(std::string s) = 0;


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSOR_H
