/************************************************************************
   AnalogImageSpaceCoordinate.cpp
**************************************************************************/

#include "AnalogImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

AnalogImageSpaceCoordinate::AnalogImageSpaceCoordinate()
{
	imageId = 0;
	unit = "";
	xi = 0;
	eta = 0;
	sigmaXi = 0;
	sigmaEta = 0;
	sigmaXiEta = 0;
	available = false;
	sigmaAvailable = false;
}

AnalogImageSpaceCoordinate::AnalogImageSpaceCoordinate(int myImageId)
{
	imageId = myImageId;
	unit = "";
	xi = 0;
	eta = 0;
	sigmaXi = 0;
	sigmaEta = 0;
	sigmaXiEta = 0;
	available = false;
	sigmaAvailable = false;
}

AnalogImageSpaceCoordinate::AnalogImageSpaceCoordinate(int myImageId, string myUnit, double myXi, double myEta, double mySigmaXi, double mySigmaEta, double mySigmaXiEta)
{
	imageId = myImageId;
	unit = myUnit;
	xi = myXi;
	eta = myEta;
	sigmaXi = mySigmaXi;
	sigmaEta = mySigmaEta;
	sigmaXiEta = mySigmaXiEta;
	available = true;
	sigmaAvailable = true;
}

AnalogImageSpaceCoordinate::AnalogImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition)
{
	imageId = myImageId;
	setPosition(myPosition);
	sigmaAvailable = false;
}

AnalogImageSpaceCoordinate::AnalogImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas)
{
	imageId = myImageId;
	setPosition(myPosition);
	setPositionSigmas(myPositionSigmas);
}

AnalogImageSpaceCoordinate::~AnalogImageSpaceCoordinate()
{

}

void AnalogImageSpaceCoordinate::setImageId(int newImageId)
{
	imageId = newImageId;
}

int AnalogImageSpaceCoordinate::getImageId()
{
	return imageId;
}

string AnalogImageSpaceCoordinate::objectType(void)
{
	return "AnalogImageSpaceCoordinate";
}

string AnalogImageSpaceCoordinate::objectAssociations(void)
{
	return "";
}

bool AnalogImageSpaceCoordinate::is(string s)
{
	return (s == "AnalogImageSpaceCoordinate" ? true : false);
}

void AnalogImageSpaceCoordinate::xmlSetData(string xml)
{
	EDomElement root(xml);
	imageId = Conversion::stringToInt(root.attribute("image_key"));
	unit = root.attribute("uom");
	EDomElement xmlPos = root.elementByTagName("gml:pos");
	if (xmlPos.isAvailable())
	{
		deque<double> gmlPosValues = xmlPos.toGmlPos();
		xi = gmlPosValues.at(0);
		eta = gmlPosValues.at(1);
		available = true;
	}
	else
	{
		available = false;
	}
	Matrix sigmaMatrix;
	EDomElement xmlSigma = root.elementByTagName("sigma");
	if (xmlSigma.isAvailable())
	{
		sigmaMatrix.xmlSetData(root.elementByTagName("sigma").elementByTagName("mml:matrix").getContent());
		sigmaAvailable = true;
	}
	else
	{
		sigmaMatrix.identity(2);
		sigmaAvailable = false;
	}
	setPositionSigmas(sigmaMatrix);
}

string AnalogImageSpaceCoordinate::xmlGetData()
{
	stringstream result;

	if (available)
		result << "<gml:pos>" << Conversion::doubleToString(xi) << " " << Conversion::doubleToString(eta) << "</gml:pos>\n";
	else
		result << "<gml:pos>Not Available</gml:pos>\n";
	if (sigmaAvailable)
		result << "<sigma>\n" << getPositionSigmas().xmlGetData() << "</sigma>\n";
	else
		result << "<sigma>Not Available</sigma>\n";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
