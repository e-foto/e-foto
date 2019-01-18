#ifndef XMLSENSOR_H
#define XMLSENSOR_H
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

#include "DetectorSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Image;
class Flight;


class XMLSensor
{
protected:
	virtual void setId(int newId) = 0;
	virtual void setFocalDistance(double newFocalDistance) = 0;
	virtual void setFocalDistanceSigma(double newFocalDistanceSigma) = 0;
	virtual void setPrincipalPointCoordinates(DetectorSpaceCoordinate newCoordinates) = 0;
    virtual void setDescription(std::string newDescription) = 0;

	virtual int getId() = 0;
	virtual double getFocalDistance() = 0;
	virtual double getFocalDistanceSigma() = 0;
	virtual DetectorSpaceCoordinate getPrincipalPointCoordinates() = 0;
    virtual std::string getDescription() = 0;
	virtual void putImage(Image* newImageAssociation) = 0;
	virtual void putFlight(Flight* newFlightAssociation) = 0;
	virtual Image* getImage(int imageId) = 0;
	virtual Flight* getFlight(int flightId) = 0;
	virtual int countImages() = 0;
	virtual int countFlights() = 0;
	virtual Image* getImageAt(unsigned int index) = 0;
	virtual Flight* getFlightAt(unsigned int index) = 0;
    virtual std::string objectType(void) = 0;

public:
	XMLSensor();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
#endif // XMLSENSOR_H
