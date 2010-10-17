/*******************************************************************************
                                 Terrain.h
*******************************************************************************/


#ifndef TERRAIN_H
#define TERRAIN_H

#include "EObject.h"

class Terrain : public EObject
{
    //EOBJECT

    //Private attributes
    //
    double meanAltitude;
    string meanAltitudeUnit;
    double minAltitude;
    string minAltitudeUnit;
    double maxAltitude;
    string maxAltitudeUnit;
    string GRS;
    string CPS;
    string workAreaCentralCoordinateLat;
    string workAreaCentralCoordinateLong;
    int utmFuse;

public:

    // Constructors and Destructor
    //
    virtual ~Terrain();

    // Private attribute accessor methods
    //
    void setMeanAltitude(double newMeanAltitude);
    void setMeanAltitudeUnit(string newMeanAltitudeUnit);
    void setMinAltitude(double newMinAltitude);
    void setMinAltitudeUnit(string newMinAltitudeUnit);
    void setMaxAltitude(double newMaxAltitude);
    void setMaxAltitudeUnit(string newMaxAltitudeUnit);
    void setGRS(string newGRS);
    void setCPS(string newCPS);
    void setCentralCoordLat(string newCentralCoordLat);
    void setCentralCoordLong(string newCentralCoordLong);
    void setUtmFuse(int newUtmFuse);
    double getMeanAltitude();
    string getMeanAltitudeUnit();
    double getMinAltitude();
    string getMinAltitudeUnit();
    double getMaxAltitude();
    string getMaxAltitudeUnit();
    string getGRS();
    string getCPS();
    string getCentralCoordLat();
    string getCentralCoordLong();
    int getUtmFuse();

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    void xmlSetData(string xml);
    string xmlGetData();
};

#endif // TERRAIN_H
