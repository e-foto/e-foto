#ifndef XMLFLIGHT_H
#define XMLFLIGHT_H

#include "EObject.h"
#include "Terrain.h"
#include "Sensor.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class XMLFlight
{
protected:
	virtual void setId(int newId) = 0;
	virtual void setSensorId(int newSensorId) =  0;
	virtual void setDescription(string newDescription) = 0;
	virtual void setExecution(string newExecution) = 0;
	virtual void setProducerName(string newProducerName) = 0;
	virtual void setScale(string newScale) = 0;
	virtual void setHeight(double newHeight) = 0;
	virtual void setLongitudinalOverlap(double newLongitudinalOverlap) = 0;
	virtual void setTransversalOverlap(double newTransversalOverlap) = 0;
	virtual double setTerrain(Terrain* newTerrain) = 0;
	virtual double setSensor(Sensor* newSensor) = 0;

	virtual int getId() = 0;
	virtual int getSensorId() = 0;
	virtual string getDescription() = 0;
	virtual string getExecution() = 0;
	virtual string getProducerName() = 0;
	virtual string getScale() = 0;
	virtual double getHeight() = 0;
	virtual double getLongitudinalOverlap() = 0;
	virtual double getTransversalOverlap() = 0;
	virtual Terrain* getTerrain() = 0;
	virtual Sensor* getSensor() = 0;
	virtual double putImage(Image* newImageAssociation) = 0;
	virtual Image* getImage(int imageId) = 0;
	virtual int countImages() = 0;
	virtual Image* getImageAt(unsigned int index) = 0;
	virtual string objectType(void) = 0;
	virtual string objectAssociations(void) = 0;
	virtual bool is(string s) = 0;

public:
	XMLFlight();

	// XML methods
	void xmlSetData(string xml);
	string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // XMLFLIGHT_H
