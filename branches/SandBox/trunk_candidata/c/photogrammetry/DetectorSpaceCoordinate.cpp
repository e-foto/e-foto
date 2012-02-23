/************************************************************************
   DetectorSpaceCoordinate.cpp
**************************************************************************/

#include "DetectorSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DetectorSpaceCoordinate::DetectorSpaceCoordinate()
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

DetectorSpaceCoordinate::DetectorSpaceCoordinate(int myImageId)
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

DetectorSpaceCoordinate::DetectorSpaceCoordinate(int myImageId, string myUnit, double myXi, double myEta, double mySigmaXi, double mySigmaEta, double mySigmaXiEta)
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

DetectorSpaceCoordinate::DetectorSpaceCoordinate(int myImageId, const PositionMatrix& myPosition)
{
	imageId = myImageId;
	setPosition(myPosition);
	sigmaAvailable = false;
}

DetectorSpaceCoordinate::DetectorSpaceCoordinate(int myImageId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas)
{
	imageId = myImageId;
	setPosition(myPosition);
	setPositionSigmas(myPositionSigmas);
}

DetectorSpaceCoordinate::~DetectorSpaceCoordinate()
{

}

void DetectorSpaceCoordinate::setImageId(int newImageId)
{
	imageId = newImageId;
}

int DetectorSpaceCoordinate::getImageId()
{
	return imageId;
}

string DetectorSpaceCoordinate::objectType(void)
{
	return "DetectorSpaceCoordinate";
}

string DetectorSpaceCoordinate::objectAssociations(void)
{
	return "";
}

bool DetectorSpaceCoordinate::is(string s)
{
	return (s == "DetectorSpaceCoordinate" ? true : false);
}

void DetectorSpaceCoordinate::xmlSetData(string xml)
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

string DetectorSpaceCoordinate::xmlGetData()
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
