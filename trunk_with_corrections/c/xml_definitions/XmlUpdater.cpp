#include "XmlUpdater.h"
#include <QApplication>

XmlUpdater::XmlUpdater(string xml,string referenceBuild)
{
    builds.push_back("1.0.20");
    builds.push_back("1.0.42");

    if(referenceBuild=="")
    {
        this->referenceBuild = builds.at(builds.size()-1);
    }
    else
    {
        this->referenceBuild = referenceBuild;
    }

    if(xml=="")
    {
		error = 4;
		updated = false;
        return;
    }
    else
    {
        allXml.setContent(xml);
	}
	updateBuild(&error);
}

EDomElement XmlUpdater::getAllXml()
{
    return allXml;
}

int XmlUpdater::getError()
{
	return error;
}

bool XmlUpdater::isUpdated()
{
	return updated;
}

string XmlUpdater::getReferenceBuild()
{
    return referenceBuild;
}

string XmlUpdater::getXmlBuild()
{
	return xmlBuild = getAllXml().elementByTagName("efotoPhotogrammetricProject").attribute("version");
}

int XmlUpdater::compareBuilds(string buildOne, string buildTwo)
{
    if (!buildIsValid(buildOne))
    {
		return -2; //buildOne (referenceBuil) is invalid
    }
    if (!buildIsValid(buildTwo))
    {
		return -3; //buildTwo (thisXmlBuild) is invalid
    }
    int iniOne=stringToInt(buildOne.substr(0,buildOne.find_first_of('.')).c_str());
    int meioOne=stringToInt(buildOne.substr(buildOne.find_first_of('.')+1,(buildOne.find_last_of('.')-buildOne.find_first_of('.')-1)).c_str());
    int ultOne=stringToInt(buildOne.substr(buildOne.find_last_of('.')).c_str()+1);

    int iniTwo=stringToInt(buildTwo.substr(0,buildTwo.find_first_of('.')).c_str());
    int meioTwo=stringToInt(buildTwo.substr(buildTwo.find_first_of('.')+1,buildTwo.find_last_of('.')-2).c_str());
    int ultTwo=stringToInt(buildTwo.substr(buildTwo.find_last_of('.')).c_str()+1);

    if(iniOne>iniTwo)
    {
        return 1;
    }
    else if (iniOne < iniTwo)
    {
		return -1; // (referenceBuild < thisXmlBuid) is not supported
    }
    else
    {
        if(meioOne>meioTwo)
        {
            return 1;
        }
        else if(meioOne<meioTwo)
        {
			return -1; // (referenceBuild < thisXmlBuid) is not supported
        }
        else
        {
            if(ultOne>ultTwo)
            {
                return 1;
            }
            else if (ultOne<ultTwo)
            {
				return -1; // (referenceBuild < thisXmlBuid) is not supported
            }
            else
                return 0;
        }
    }

}

bool XmlUpdater::updateBuild(int* error)
{
    int op = compareBuilds(getReferenceBuild(), getXmlBuild());
    *error = 0;
    if (op ==  0 )
    {
		return updated = true;
    }
    else if (op > 0)
    {
        executeUpdate();
		return updated = true;
    }
    else
    {
		*error = -op;
		return updated = false;
    }
}

void XmlUpdater::executeUpdate()
{
	if (getXmlBuild().compare("1.0.20") == 0)
	{
		updateToBuild1_0_42();
	}
}

bool XmlUpdater::buildIsValid(string build)
{
	for (unsigned int i=0;i<builds.size();i++)
    {
        if (builds.at(i).compare(build)==0)
            return true;
    }
    return false;
}

void XmlUpdater::updateToBuild1_0_42()
{
	EDomElement sensor = allXml.elementByTagAtt("sensor","key","1");
	EDomElement edeType = sensor.elementByTagName("type");
	string type = edeType.elementByTagName("detector").toString();
	if(type=="ccd")
	{
		edeType.addChildAtTagName("type","<calculationMode>Fixed Parameters</calculationMode>");
	}
	else //if (type=="film")
	{
		edeType.addChildAtTagName("type","<calculationMode>With Fiducial Marks</calculationMode>");
	}
	sensor.replaceChildByTagName("type", edeType.getContent());

	sensor.addAttributeByTagName("radialSymmetric","considered", "false");

	sensor.addAttributeByTagName("decentered","considered", "false");

	allXml.replaceChildByTagAtt("sensor","key","1",sensor.getContent());

	allXml.replaceAttributeByTagName("efotoPhotogrammetricProject","version","1.0.42");
}

