/************************************************************************
   DetectorFiducialMark.cpp
**************************************************************************/

#include "DetectorFiducialMark.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DetectorFiducialMark::DetectorFiducialMark()
{
	id = 0;
	sensorId = 0;
	unit = "";
	xi = 0;
	eta = 0;
	sigmaXi = 0;
	sigmaEta = 0;
	sigmaXiEta = 0;
	available = false;
	sigmaAvailable = false;
}

DetectorFiducialMark::DetectorFiducialMark(int myId, int mySensorId)
{
	id = myId;
	sensorId = mySensorId;
	unit = "";
	xi = 0;
	eta = 0;
	sigmaXi = 0;
	sigmaEta = 0;
	sigmaXiEta = 0;
	available = false;
	sigmaAvailable = false;
}

DetectorFiducialMark::DetectorFiducialMark(int myId, int mySensorId, string myUnit, double myXi, double myEta, double mySigmaXi, double mySigmaEta, double mySigmaXiEta)
{
	id = myId;
	sensorId = mySensorId;
	unit = myUnit;
	xi = myXi;
	eta = myEta;
	sigmaXi = mySigmaXi;
	sigmaEta = mySigmaEta;
	sigmaXiEta = mySigmaXiEta;
	available = true;
	sigmaAvailable = true;
}

DetectorFiducialMark::DetectorFiducialMark(int myId, int mySensorId, const PositionMatrix& myPosition)
{
	id = myId;
	sensorId = mySensorId;
	setPosition(myPosition);
	sigmaAvailable = false;
}

DetectorFiducialMark::DetectorFiducialMark(int myId, int mySensorId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas)
{
	id = myId;
	sensorId = mySensorId;
	setPosition(myPosition);
	setPositionSigmas(myPositionSigmas);
}

DetectorFiducialMark::~DetectorFiducialMark()
{

}

void DetectorFiducialMark::setId(int newId)
{
	id = newId;
}

void DetectorFiducialMark::setSensorId(int newSensorId)
{
	sensorId = newSensorId;
}

int DetectorFiducialMark::getId()
{
	return id;
}

int DetectorFiducialMark::getSensorId()
{
	return sensorId;
}

string DetectorFiducialMark::objectType(void)
{
	stringstream result;
	result << "DetectorFiducialMark " << id;
	return result.str();
}

string DetectorFiducialMark::objectAssociations(void)
{
	return "";
}

bool DetectorFiducialMark::is(string s)
{
	return (s == "DetectorFiducialMark" ? true : false);
}

void DetectorFiducialMark::xmlSetData(string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
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

string DetectorFiducialMark::xmlGetData()
{
	stringstream result;
    result << "<fiducialMark key=\"" << Conversion::intToString(id) << "\">\n";
	if (available)
		result << "<gml:pos>" << Conversion::doubleToString(xi) << " " << Conversion::doubleToString(eta) << "</gml:pos>\n";
	else
		result << "<gml:pos>Not Available</gml:pos>\n";
	if (sigmaAvailable)
		result << "<sigma>\n" << getPositionSigmas().xmlGetData() << "</sigma>\n";
	else
		result << "<sigma>Not Available</sigma>\n";
    result << "</fiducialMark>";
	return result.str();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
