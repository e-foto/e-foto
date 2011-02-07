#ifndef XMLUPDATER_H
#define XMLUPDATER_H

#include <string>
#include "EDom.h"
//"..\infrastructure\EDom.h"

class XmlUpdater
{

private:
    deque <string> builds;
    string xmlBuild; // this is a version descriptor of loaded xml
    string referenceBuild; // this is a version descriptor of reference
    EDomElement allXml; // this is a xml loaded under constructor
    EDomElement header;
    EDomElement terrain;
    EDomElement sensor;
    EDomElement flight;
    EDomElement images;
    EDomElement points;

    int compareBuilds(string buildOne, string buildTwo);
    void executeUpdate();

public:
    XmlUpdater(string allXml, string referenceBuild = ""); //1

    EDomElement getAllXml(); //4
    EDomElement getFlightXml();
    EDomElement getSensorXml();
    EDomElement getTerrainXml();
    EDomElement getHeaderXml();
    EDomElement getImagesXml();
    EDomElement getPointsXml();

    string getXmlBuild();
    string getReferenceBuild();
    bool buildIsValid(string build);
    bool updateBuild(int *error); //2

    void updateToBuild1_0_42();
};

#endif // XMLUPDATER_H
