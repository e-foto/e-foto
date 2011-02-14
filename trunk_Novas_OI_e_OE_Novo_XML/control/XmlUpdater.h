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

	int error;
	bool updated;
    int compareBuilds(string buildOne, string buildTwo);
    void executeUpdate();

public:
	XmlUpdater(string allXml, string referenceBuild = "");

	EDomElement getAllXml();
	int getError();
	bool isUpdated();

    string getXmlBuild();
    string getReferenceBuild();
    bool buildIsValid(string build);
	bool updateBuild(int *error);

    void updateToBuild1_0_42();
};

#endif // XMLUPDATER_H
