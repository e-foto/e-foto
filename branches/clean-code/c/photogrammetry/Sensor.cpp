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

Sensor::Sensor(int myId):
    id_{myId}
{
}

Sensor::~Sensor()
{
}

// Private attributes accessor methods
void Sensor::setId(int newId)
{
    id_ = newId;
}

void Sensor::setSensorId(std::string nickname) {
        sensorId_ = nickname;
}

void Sensor::setDescription(std::string newDescription)
{
    description_ = newDescription;
}

int Sensor::getId() const
{
    return id_;
}
std::string Sensor::getSensorId() const
{
        return sensorId_;
}

std::string Sensor::getDescription() const
{
    return description_;
}

std::string Sensor::getCalculationMode() const
{
    return calculationMode_;
}



// Associated object accessor methods
//

void Sensor::putImage(Image* newImageAssociation)
{
	bool insert = true;
	// Eliminamos primeiro a possibilidade duplicar uma associação.
    for (unsigned int i = 0; i < myImages_.size(); i++)
        if (myImages_.at(i) == newImageAssociation)
			insert = false;
	// Fazemos a nova associação.
	if (insert)
        myImages_.push_back(newImageAssociation);
}

void Sensor::putFlight(Flight* newFlightAssociation)
{
	// Eliminamos primeiro a possibilidade duplicar uma associação.
	bool insert = true;
    for (unsigned int i = 0; i < myFlights_.size(); i++)
        if (myFlights_.at(i) == newFlightAssociation)
			insert = false;
	// Fazemos a nova associação.
	if (insert)
        myFlights_.push_back(newFlightAssociation);
}

Image* Sensor::getImage(int imageId)
{
    for (unsigned int i = 0; i < myImages_.size(); i++)
        if (myImages_.at(i)->getId() == imageId)
            return myImages_.at(i);
	return NULL;
}

Flight* Sensor::getFlight(int flightId)
{
    for (unsigned int i = 0; i < myFlights_.size(); i++)
        if (myFlights_.at(i)->getId() == flightId)
            return myFlights_.at(i);
	return NULL;
}

int Sensor::countImages() const
{
    return myImages_.size();
}

Image* Sensor::getImageAt(unsigned int index)
{
    if (index < myImages_.size())
        return myImages_.at(index);
	return NULL;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
