#ifndef XMLTERRAIN_H
#define XMLTERRAIN_H

#include "EObject.h"

class XMLTerrain
{
protected:

    virtual void setMeanAltitude(double newMeanAltitude) = 0;
    virtual void setMinAltitude(double newMinAltitude) = 0;
    virtual void setMaxAltitude(double newMaxAltitude) = 0;
    virtual void setMeanAltitudeUnit(string newMeanAltitudeUnit) = 0;
    virtual void setMinAltitudeUnit(string newMinAltitudeUnit) = 0;
    virtual void setMaxAltitudeUnit(string newMaxAltitudeUnit) = 0;
    virtual void setGRS(string newGRS) = 0;
    virtual void setCPS(string newCPS) = 0;
    virtual void setCentralCoordLat(string newCentralCoordLat) = 0;
    virtual void setCentralCoordLong(string newCentralCoordLong) = 0;
    virtual void setUtmFuse(int newUtmFuse) = 0;

    virtual double getMeanAltitude() = 0;
    virtual string getMeanAltitudeUnit() = 0;
    virtual double getMaxAltitude() = 0;
    virtual string getMaxAltitudeUnit() = 0;
    virtual double getMinAltitude() = 0;
    virtual string getMinAltitudeUnit() = 0;
    virtual string getGRS() = 0;
    virtual string getCPS() = 0;
    virtual string getCentralCoordLat() = 0;
    virtual string getCentralCoordLong() = 0;
    virtual int getUtmFuse() = 0;

public:
    XMLTerrain();

    // XML methods
    void xmlSetData(string xml);
    string xmlGetData();
};

#endif // XMLTERRAIN_H
