#include "XmlUpdater.h"
#include <QApplication>
//#include <QDebug>

#include <sstream>


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

XmlUpdater::XmlUpdater(std::string xml,std::string referenceBuild)
{
	builds.push_back("1.0.20");
	builds.push_back("1.0.42");
	builds.push_back("1.0.266");
	builds.push_back("1.0.325");

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

std::string XmlUpdater::getReferenceBuild()
{
	return referenceBuild;
}

std::string XmlUpdater::getXmlBuild()
{
	return xmlBuild = getAllXml().elementByTagName("efotoPhotogrammetricProject").attribute("version");
}

int XmlUpdater::compareBuilds(std::string buildOne, std::string buildTwo)
{
	if (!buildIsValid(buildOne))
	{
		return -2; //buildOne (referenceBuil) is invalid
	}
	if (!buildIsValid(buildTwo))
	{
		return -3; //buildTwo (thisXmlBuild) is invalid
	}
	int iniOne=Conversion::stringToInt(buildOne.substr(0,buildOne.find_first_of('.')).c_str());
	int meioOne=Conversion::stringToInt(buildOne.substr(buildOne.find_first_of('.')+1,(buildOne.find_last_of('.')-buildOne.find_first_of('.')-1)).c_str());
	int ultOne=Conversion::stringToInt(buildOne.substr(buildOne.find_last_of('.')).c_str()+1);

	int iniTwo=Conversion::stringToInt(buildTwo.substr(0,buildTwo.find_first_of('.')).c_str());
	int meioTwo=Conversion::stringToInt(buildTwo.substr(buildTwo.find_first_of('.')+1,buildTwo.find_last_of('.')-2).c_str());
	int ultTwo=Conversion::stringToInt(buildTwo.substr(buildTwo.find_last_of('.')).c_str()+1);

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
	if (getXmlBuild().compare("1.0.42") == 0)
	{
		updateToBuild1_0_266();
	}
	if (getXmlBuild().compare("1.0.266") == 0)
	{
		updateToBuild1_0_325();
	}
}

bool XmlUpdater::buildIsValid(std::string build)
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
    std::string type = edeType.elementByTagName("detector").toString();
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

void XmlUpdater::updateToBuild1_0_266()
{
    std::string all= allXml.getContent();
	while(replacer(all,"fiductial","fiducial"));
	allXml.setContent(all);

    std::deque<EDomElement> eosXml = allXml.elementsByTagName("imageEO");

    allXml.addChildAtTagName("efotoPhotogrammetricProject","<spatialRessections>\n</spatialRessections>");
    for(int i=0;i<eosXml.size();i++)
	{
        std::string total="";
		EDomElement eoXml;
		eoXml.setContent(eosXml.at(i).getContent());
		if (eoXml.hasTagName("iterations"))
		{
			EDomElement xaXml =	eoXml.elementByTagName("Xa");
			xaXml.replaceAttributeByTagAtt("phi","uom","#m","uom","#rad");
			xaXml.replaceAttributeByTagAtt("omega","uom","#m","uom","#rad");
			xaXml.replaceAttributeByTagAtt("kappa","uom","#m","uom","#rad");

            std::stringstream aux;
			aux << "<imageEO type=\""<< eoXml.attribute("type") <<"\" image_key=\""<< eoXml.attribute("image_key") << "\">\n";
			aux << xaXml.getContent();
			aux << "\n</imageEO>";

			eoXml.replaceChildByTagName("Xa","");

            std::stringstream newXmlSpatialRessection;
            newXmlSpatialRessection << "<imageSR image_key=\""<< eoXml.attribute("image_key") << "\">\n";
			newXmlSpatialRessection << eoXml.elementByTagName("iterations").getContent() << "\n";
			newXmlSpatialRessection << eoXml.elementByTagName("converged").getContent() << "\n";
			newXmlSpatialRessection << eoXml.elementByTagName("parameters").getContent() << "\n";
			newXmlSpatialRessection << eoXml.elementByTagName("quality").getContent() << "\n";
            newXmlSpatialRessection << "</imageSR>\n";

            allXml.replaceChildByTagAtt("imageEO","image_key",eoXml.attribute("image_key"),aux.str());

            allXml.addChildAtTagName("spatialRessections",newXmlSpatialRessection.str());
		}
    }

    allXml.replaceAttributeByTagName("efotoPhotogrammetricProject","version","1.0.266");
}

void XmlUpdater::updateToBuild1_0_325()
{
    std::string all= allXml.getContent();
    while(replacer(all,"1.0.266","1.0.325"));
    while(replacer(all,"Ressection","Resection"));
    while(replacer(all,"<spatialResection ","<imageSR "));
    while(replacer(all,"</spatialResection>","</imageSR>"));
    while(replacer(all,"gml:sigma","sigma"));
	allXml.setContent(all);
	allXml.replaceAttributeByTagName("efotoPhotogrammetricProject","version","1.0.325");
}

bool XmlUpdater::replacer(std::string &text,std::string oldWord,std::string newWord)
{
	size_t searched;
	searched = text.rfind(oldWord);
    if (searched != std::string::npos)
	{
		text.replace(searched,oldWord.length(),newWord);
		return true;
	}
	return false;
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
