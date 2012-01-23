#ifndef XMLSENSOR_H
#define XMLSENSOR_H

#include "EObject.h"
#include "Sensor.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


class XMLSensor
{
protected:
	virtual void setId(int newId) = 0;
	virtual void setFocalDistance(double newFocalDistance) = 0;
	virtual void setFocalDistanceSigma(double newFocalDistanceSigma) = 0;
	virtual void setPrincipalPointCoordinates(AnalogImageSpaceCoordinate newCoordinates) = 0;
	virtual void setDescription(string newDescription) = 0;

	virtual int getId() = 0;
	virtual double getFocalDistance() = 0;
	virtual double getFocalDistanceSigma() = 0;
	virtual AnalogImageSpaceCoordinate getPrincipalPointCoordinates() = 0;
	virtual string getDescription() = 0;
	virtual void putImage(Image* newImageAssociation) = 0;
	virtual void putFlight(Flight* newFlightAssociation) = 0;
	virtual Image* getImage(int imageId) = 0;
	virtual Flight* getFlight(int flightId) = 0;
	virtual int countImages() = 0;
	virtual int countFlights() = 0;
	virtual Image* getImageAt(unsigned int index) = 0;
	virtual Flight* getFlightAt(unsigned int index) = 0;
	virtual string objectType(void) = 0;

public:
	XMLSensor();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
#endif // XMLSENSOR_H
