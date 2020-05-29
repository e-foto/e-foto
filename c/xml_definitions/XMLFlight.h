#ifndef XMLFLIGHT_H
#define XMLFLIGHT_H
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

#include <string>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Terrain;
class Sensor;
class Image;

class XMLFlight
{
protected:
	virtual void setId(int newId) = 0;
	virtual void setSensorId(int newSensorId) =  0;
    virtual void setDescription(std::string newDescription) = 0;
    virtual void setExecution(std::string newExecution) = 0;
    virtual void setProducerName(std::string newProducerName) = 0;
    virtual void setScale(std::string newScale) = 0;
	virtual void setHeight(double newHeight) = 0;
	virtual void setLongitudinalOverlap(double newLongitudinalOverlap) = 0;
	virtual void setTransversalOverlap(double newTransversalOverlap) = 0;
	virtual double setTerrain(Terrain* newTerrain) = 0;
	virtual double setSensor(Sensor* newSensor) = 0;

	virtual int getId() = 0;
	virtual int getSensorId() = 0;
    virtual std::string getDescription() = 0;
    virtual std::string getExecution() = 0;
    virtual std::string getProducerName() = 0;
    virtual std::string getScale() = 0;
	virtual double getHeight() = 0;
	virtual double getLongitudinalOverlap() = 0;
	virtual double getTransversalOverlap() = 0;
	virtual Terrain* getTerrain() = 0;
	virtual Sensor* getSensor() = 0;
	virtual double putImage(Image* newImageAssociation) = 0;
	virtual Image* getImage(int imageId) = 0;
	virtual int countImages() = 0;
	virtual Image* getImageAt(unsigned int index) = 0;
    virtual std::string objectType(void) = 0;
    virtual std::string objectAssociations(void) = 0;
    virtual bool is(std::string s) = 0;

public:
	XMLFlight();

	// XML methods
    void xmlSetData(std::string xml);
    std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // XMLFLIGHT_H
