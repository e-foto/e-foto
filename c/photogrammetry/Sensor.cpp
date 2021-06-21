/*******************************************************************************
		Sensor.cpp
*******************************************************************************/
/*Copyright 2002-2021 e-foto team (UERJ)
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

int Sensor::getId()
{
  return id;
}
std::string Sensor::getSensorId()
{
  return sensorId;
}

std::string Sensor::getDescription()
{
  return description;
}

std::string Sensor::getCalculationMode()
{
  return calculationMode;
}

void Sensor::putImage(Image* newImageAssociation)
{
  bool insert = true;

  // Eliminamos primeiro a possibilidade duplicar uma associação.
  for (unsigned int i = 0; i < myImages.size(); i++)
    if (myImages.at(i) == newImageAssociation) {
      insert = false;
    }

  // Fazemos a nova associação.
  if (insert) {
    myImages.push_back(newImageAssociation);
  }
}

void Sensor::putFlight(Flight* newFlightAssociation)
{
  // Eliminamos primeiro a possibilidade duplicar uma associação.
  bool insert = true;

  for (unsigned int i = 0; i < myFlights.size(); i++)
    if (myFlights.at(i) == newFlightAssociation) {
      insert = false;
    }

  // Fazemos a nova associação.
  if (insert) {
    myFlights.push_back(newFlightAssociation);
  }
}

std::string Sensor::objectType(void)
{
  return "Sensor";
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
