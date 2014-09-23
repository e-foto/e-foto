/**************************************************************************
	  SensorWithKnowDimensions.cpp
**************************************************************************/
/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "SensorWithKnowDimensions.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors
//


SensorWithKnowDimensions::SensorWithKnowDimensions():FrameSensor()
{

}

SensorWithKnowDimensions::SensorWithKnowDimensions(const Sensor& sensor):FrameSensor(sensor)
{

}

SensorWithKnowDimensions::SensorWithKnowDimensions(int myId):FrameSensor(myId)
{

}

SensorWithKnowDimensions::SensorWithKnowDimensions(int myId, double myPixelSize, int myFrameRows, int myFrameColumns):FrameSensor(myId)
{
	pixelSize = myPixelSize;
	frameRows = myFrameRows;
	frameColumns = myFrameColumns;
}



// Private attribute accessor methods
//

void SensorWithKnowDimensions::setPixelSize(double newPixelSize)
{
	pixelSize = newPixelSize;
}

void SensorWithKnowDimensions::setFrameDimensions(int newFrameRows, int newFrameColumns)
{
	frameRows = newFrameRows;
	frameColumns = newFrameColumns;
}

double SensorWithKnowDimensions::getPixelSize()
{
	return pixelSize;
}

int SensorWithKnowDimensions::getFrameRows()
{
	return frameRows;
}

int SensorWithKnowDimensions::getFrameColumns()
{
	return frameColumns;
}

Matrix SensorWithKnowDimensions::forgeLb()
{
	double rowsSize = frameRows * pixelSize / 1000.0;
	double columnsSize = frameColumns * pixelSize / 1000.0;
	Matrix Lb(8,1);
	/* This is wrong, I think... - Rafael on 11/02/2011
 Lb.set(1,1,-rowsSize/2);
 Lb.set(2,1,columnsSize/2);
 Lb.set(3,1,rowsSize/2);
 Lb.set(4,1,columnsSize/2);
 Lb.set(5,1,rowsSize/2);
 Lb.set(6,1,-columnsSize/2);
 Lb.set(7,1,-rowsSize/2);
 Lb.set(8,1,-columnsSize/2);*/
	Lb.set(1,1,-columnsSize/2);
	Lb.set(2,1,rowsSize/2);
	Lb.set(3,1,columnsSize/2);
	Lb.set(4,1,rowsSize/2);
	Lb.set(5,1,columnsSize/2);
	Lb.set(6,1,-rowsSize/2);
	Lb.set(7,1,-columnsSize/2);
	Lb.set(8,1,-rowsSize/2);
	return Lb;
}

// Composed objects accessor methods
//

// EObject methods
//

std::string SensorWithKnowDimensions::objectType(void)
{
    std::stringstream result;
	result << "SensorWithKnowDimensions " << id;
	return result.str();
}

std::string SensorWithKnowDimensions::objectAssociations(void)
{
	return "";
}

bool SensorWithKnowDimensions::is(std::string s)
{
	return (s == "SensorWithKnowDimensions" ? true : false);
}

// XML methods
//

void SensorWithKnowDimensions::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	sensorId = root.elementByTagName("sensorId").toString();
	geometry = root.elementByTagName("geometry").toString();
	detector = root.elementByTagName("detector").toString();
	energySource = root.elementByTagName("energySource").toString();
	calculationMode = root.elementByTagName("calculationMode").toString();

	spectralRangesUnit = root.elementByTagName("spectralRanges").attribute("uom");
    std::deque<EDomElement> xmlSpectralRanges = root.elementsByTagName("spectralRange");
	spectralRanges.clear();
	for (unsigned int i = 0; i < xmlSpectralRanges.size(); i++)
	{
		SpectralRange* spec = new SpectralRange;
		spec->band = Conversion::stringToInt(xmlSpectralRanges.at(i).attribute("band"));
		spec->inferiorLimit = xmlSpectralRanges.at(i).elementByTagName("inferiorLimit").toDouble();
		spec->superiorLimit = xmlSpectralRanges.at(i).elementByTagName("superiorLimit").toDouble();
		spectralRanges.push_back(*spec);
	}

	description = root.elementByTagName("description").toString();
	calibrationCertificateNumber = root.elementByTagName("number").toString();
	calibrationCertificateNumber = root.elementByTagName("dispatch").toString();
	calibrationCertificateExpiration = root.elementByTagName("expiration").toString();

	EDomElement xmlFocalDistance = root.elementByTagName("focalDistance");
	focalDistanceUnit = xmlFocalDistance.attribute("uom");
	focalDistance = xmlFocalDistance.elementByTagName("value").toDouble();
	if (xmlFocalDistance.elementByTagName("sigma").isAvailable())
		focalDistanceSigma = xmlFocalDistance.elementByTagName("sigma").toDouble();
	else
		focalDistanceSigma = 1.0;

    std::deque<EDomElement> xmlRadialSymmetric = root.elementByTagName("radialSymmetric").children();
	rsCoefficients.clear();
	for (unsigned int i = 0; i < xmlRadialSymmetric.size(); i++)
	{
		RadialSymmetricDistortionCoefficient* radial = new RadialSymmetricDistortionCoefficient;
		radial->value = xmlRadialSymmetric.at(i).elementByTagName("value").toDouble();
		if (xmlRadialSymmetric.at(i).elementByTagName("sigma").isAvailable())
			radial->sigma = xmlRadialSymmetric.at(i).elementByTagName("sigma").toDouble();
		else
			radial->sigma = 1.0;
		rsCoefficients.push_back(*radial);
	}

    std::deque<EDomElement> xmlDecentered = root.elementByTagName("decentered").children();
	dCoefficients.clear();
	for (unsigned int i = 0; i < xmlDecentered.size(); i++)
	{
		DecenteredDistortionCoefficient* decentered = new DecenteredDistortionCoefficient;
		decentered->value = xmlDecentered.at(i).elementByTagName("value").toDouble();
		if (xmlDecentered.at(i).elementByTagName("sigma").isAvailable())
			decentered->sigma = xmlDecentered.at(i).elementByTagName("sigma").toDouble();
		else
			decentered->sigma = 1.0;
		dCoefficients.push_back(*decentered);
	}

	principalPointCoordinates.xmlSetData(root.elementByTagName("principalPointCoordinates").getContent());

	EDomElement sensorDimensions = root.elementByTagName("sensorDimensions");
	pixelSize = sensorDimensions.elementByTagName("pixelSize").toDouble();
	frameRows = sensorDimensions.elementByTagName("rows").toInt();
	frameColumns = sensorDimensions.elementByTagName("columns").toInt();
}

std::string SensorWithKnowDimensions::xmlGetData()
{
    std::stringstream result;
	result << "<sensor key=\"" << Conversion::intToString(id) << "\">\n";
	result << "<sensorId>" << sensorId << "</sensorId>\n";
	result << "<type>\n";
	result << "<geometry>" << geometry << "</geometry>\n";
	result << "<platform>aerial</platform>\n";
	result << "<detector>" << detector << "</detector>\n";
	result << "<energySource>" << energySource << "</energySource>\n";
	result << "<calculationMode>" << calculationMode << "</calculationMode>\n";
	result << "<spectralRanges uom=\"" << spectralRangesUnit << "\">\n";
	for (unsigned int i = 0; i < spectralRanges.size(); i++)
	{
		result << "<spectralRange band=\"" << Conversion::intToString(spectralRanges.at(i).band) << "\">\n";
		result << "<inferiorLimit>" << Conversion::doubleToString(spectralRanges.at(i).inferiorLimit) << "</inferiorLimit>\n";
		result << "<superiorLimit>" << Conversion::doubleToString(spectralRanges.at(i).superiorLimit) << "</superiorLimit>\n";
		result << "</spectralRange>\n";
	}
	result << "</spectralRanges>\n";
	result << "</type>\n";
	result << "<description>" << description << "</description>\n";
	result << "<calibrationCertificate>\n";
	result << "<number>" << calibrationCertificateNumber << "</number>\n";
	result << "<expiration>" << calibrationCertificateExpiration << "</expiration>\n";
	result << "</calibrationCertificate>\n";
	result << "<focalDistance uom=\"" << focalDistanceUnit << "\">\n";
	result << "<value>" << Conversion::doubleToString(focalDistance) << "</value>\n";
	if (focalDistanceSigma == 1.0)
		result << "<sigma>Not Available</sigma>\n";
	else
		result << "<sigma>" << Conversion::doubleToString(focalDistanceSigma) << "</sigma>\n";
	result << "</focalDistance>\n";
	result << "<distortionCoefficients>\n";
	result << "<radialSymmetric>\n";
	for (unsigned int i = 0; i < rsCoefficients.size(); i++)
	{
		result << "<k" << i << ">\n";
		result << "<value>" << Conversion::doubleToString(rsCoefficients.at(i).value) << "</value>\n";
		if (rsCoefficients.at(i).sigma == 1.0)
			result << "<sigma>Not Available</sigma>\n";
		else
			result << "<sigma>" << Conversion::doubleToString(rsCoefficients.at(i).sigma) << "</sigma>\n";
		result << "</k" << i << ">\n";
	}
	result << "</radialSymmetric>\n";
	result << "<decentered>\n";
	for (unsigned int i = 0; i < dCoefficients.size(); i++)
	{
		result << "<P" << i+1 << ">\n";
		result << "<value>" << Conversion::doubleToString(dCoefficients.at(i).value) << "</value>\n";
		if (dCoefficients.at(i).sigma == 1.0)
			result << "<sigma>Not Available</sigma>\n";
		else
			result << "<sigma>" << Conversion::doubleToString(dCoefficients.at(i).sigma) << "</sigma>\n";
		result << "</P" << i+1 << ">\n";
	}
	result << "</decentered>\n";
	result << "</distortionCoefficients>\n";
	result << "<principalPointCoordinates uom=\"" << principalPointCoordinates.getUnit() << "\">\n";
	result << principalPointCoordinates.xmlGetData();
	result << "</principalPointCoordinates>\n";
	result << "<sensorDimensions>\n";
	result << "<pixelSize uom=\"#um\">"<< pixelSize << "</pixelSize>\n";
	result << "<sensorSize>\n";
	result << "<rows>"<< frameRows <<"</rows>\n";
	result << "<columns>"<< frameColumns <<"</columns>\n";
	result << "</sensorSize>\n";
	result << "</sensorDimensions>\n";
	result << "</sensor>\n";
	return result.str();
}

// Other methods
//

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
