/*******************************************************************************
		Sensor.cpp
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
#include "Sensor.h"
#include "Image.h"
#include "Flight.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructors
//

Sensor::Sensor()
{

}

Sensor::Sensor(const Sensor& sensor)
{
	*this = sensor;
}

Sensor::Sensor(int myId)
{
	id = myId;
}

Sensor::~Sensor()
{

}



// Private attributes accessor methods
//

void Sensor::setId(int newId)
{
	id = newId;
}

void Sensor::setSensorId(std::string nickname) {
        sensorId = nickname;
}
/*
void Sensor::setFocalDistance(double newFocalDistance)
{
 focalDistance = newFocalDistance;
}

void Sensor::setFocalDistanceSigma(double newFocalDistanceSigma)
{
 focalDistanceSigma = newFocalDistanceSigma;
}

void Sensor::setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates)
{
 principalPointCoordinates = newCoordinates;
}
*/
void Sensor::setDescription(std::string newDescription)
{
	description = newDescription;
}

void Sensor::setCalculationMode(std::string newCalculationMode)
{
	calculationMode = newCalculationMode;
}

int Sensor::getId()
{
	return id;
}
std::string Sensor::getSensorId()
{
        return sensorId;
}
/*
double Sensor::getFocalDistance()
{
 return focalDistance;
}

double Sensor::getFocalDistanceSigma()
{
 return focalDistanceSigma;
}

DetectorSpaceCoordinate Sensor::getPrincipalPointCoordinates()
{
 return principalPointCoordinates;
}
*/
std::string Sensor::getDescription()
{
	return description;
}

std::string Sensor::getCalculationMode()
{
	return calculationMode;
}



// Associated object accessor methods
//

void Sensor::putImage(Image* newImageAssociation)
{
	bool insert = true;
	// Eliminamos primeiro a possibilidade duplicar uma associação.
	for (unsigned int i = 0; i < myImages.size(); i++)
		if (myImages.at(i) == newImageAssociation)
			insert = false;
	// Fazemos a nova associação.
	if (insert)
		myImages.push_back(newImageAssociation);
}

void Sensor::putFlight(Flight* newFlightAssociation)
{
	// Eliminamos primeiro a possibilidade duplicar uma associação.
	bool insert = true;
	for (unsigned int i = 0; i < myFlights.size(); i++)
		if (myFlights.at(i) == newFlightAssociation)
			insert = false;
	// Fazemos a nova associação.
	if (insert)
		myFlights.push_back(newFlightAssociation);
}

Image* Sensor::getImage(int imageId)
{
	for (unsigned int i = 0; i < myImages.size(); i++)
		if (myImages.at(i)->getId() == imageId)
			return myImages.at(i);
	return NULL;
}

Flight* Sensor::getFlight(int flightId)
{
	for (unsigned int i = 0; i < myFlights.size(); i++)
		if (myFlights.at(i)->getId() == flightId)
			return myFlights.at(i);
	return NULL;
}

int Sensor::countImages()
{
	return myImages.size();
}

int Sensor::countFlights()
{
	return myFlights.size();
}

Image* Sensor::getImageAt(unsigned int index)
{
	if (index < myImages.size())
		return myImages.at(index);
	return NULL;
}

Flight* Sensor::getFlightAt(unsigned int index)
{
	if (index < myFlights.size())
		return myFlights.at(index);
	return NULL;
}

// EObject methods
//

std::string Sensor::objectType(void)
{
	return "Sensor";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
