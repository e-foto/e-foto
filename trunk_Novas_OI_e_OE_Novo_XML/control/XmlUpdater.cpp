#include "XmlUpdater.h"

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
        cerr << "erro";
        return;
    }
    else
    {
        allXml.setContent(xml);
        header.setContent(allXml.elementByTagName("projectHeader").getContent());
        terrain.setContent(allXml.elementByTagName("terrain").getContent());
        sensor.setContent(allXml.elementByTagAtt("sensor","key","1").getContent());
        flight.setContent(allXml.elementByTagAtt("flight","key","1").getContent());
        images.setContent(allXml.elementByTagName("images").getContent());
        points.setContent(allXml.elementByTagName("points").getContent());
    }
}

EDomElement XmlUpdater::getAllXml()
{
    return allXml;
}

EDomElement XmlUpdater::getHeaderXml()
{
    return header;
}

EDomElement XmlUpdater::getTerrainXml()
{
    return terrain;
}

EDomElement XmlUpdater::getSensorXml()
{
    return sensor;
}

EDomElement XmlUpdater::getFlightXml()
{
    return flight;
}

EDomElement XmlUpdater::getImagesXml()
{
    return images;
}

EDomElement XmlUpdater::getPointsXml()
{
    return points;
}

string XmlUpdater::getReferenceBuild()
{
    return referenceBuild;
}

string XmlUpdater::getXmlBuild()
{
    return xmlBuild=getAllXml().elementByTagName("efotoPhotogrammetricProject").attribute("version");
}

int XmlUpdater::compareBuilds(string buildOne, string buildTwo)
{
    if (!buildIsValid(buildOne))
    {
        return -2;
    }
    if (!buildIsValid(buildTwo))
    {
        return -3;
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
        return -1;
    }
    else
    {
        if(meioOne>meioTwo)
        {
            return 1;
        }
        else if(meioOne<meioTwo)
        {
            return -1;
        }
        else
        {
            if(ultOne>ultTwo)
            {
                return 1;
            }
            else if (ultOne<ultTwo)
            {
                return -1;
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
        return true;
    }
    else if (op > 0)
    {
        executeUpdate();
        return true;
    }
    else
    {
        *error = op;
        return false;
    }
}

void XmlUpdater::executeUpdate()
{

  /*  switch ()
    {
        case :

    }
*/
}

bool XmlUpdater::buildIsValid(string build)
{
    for (int i=0;i<builds.size();i++)
    {
        if (builds.at(i).compare(build)==0)
            return true;
    }
    return false;
}

void XmlUpdater::updateToBuild1_0_42()
{
	//implementação do update aqui	
	getSensorXml();
}

