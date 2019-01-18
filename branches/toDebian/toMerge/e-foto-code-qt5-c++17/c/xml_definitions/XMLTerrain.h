#ifndef XMLTERRAIN_H
#define XMLTERRAIN_H
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

class XMLTerrain
{
protected:

	virtual void setMeanAltitude(double newMeanAltitude) = 0;
	virtual void setMinAltitude(double newMinAltitude) = 0;
	virtual void setMaxAltitude(double newMaxAltitude) = 0;
    virtual void setMeanAltitudeUnit(std::string newMeanAltitudeUnit) = 0;
    virtual void setMinAltitudeUnit(std::string newMinAltitudeUnit) = 0;
    virtual void setMaxAltitudeUnit(std::string newMaxAltitudeUnit) = 0;
    virtual void setGRS(std::string newGRS) = 0;
    virtual void setCPS(std::string newCPS) = 0;
    virtual void setCentralCoordLat(std::string newCentralCoordLat) = 0;
    virtual void setCentralCoordLong(std::string newCentralCoordLong) = 0;
	virtual void setUtmFuse(int newUtmFuse) = 0;

	virtual double getMeanAltitude() = 0;
    virtual std::string getMeanAltitudeUnit() = 0;
	virtual double getMaxAltitude() = 0;
    virtual std::string getMaxAltitudeUnit() = 0;
	virtual double getMinAltitude() = 0;
    virtual std::string getMinAltitudeUnit() = 0;
    virtual std::string getGRS() = 0;
    virtual std::string getCPS() = 0;
    virtual std::string getCentralCoordLat() = 0;
    virtual std::string getCentralCoordLong() = 0;
	virtual int getUtmFuse() = 0;

public:
	XMLTerrain();

	// XML methods
    void xmlSetData(std::string xml);
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // XMLTERRAIN_H
