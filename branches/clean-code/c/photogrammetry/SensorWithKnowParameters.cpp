/**************************************************************************
	  SensorWithKnowParameters.cpp
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
#include "SensorWithKnowParameters.h"

#include <sstream>
#include <iostream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors
//


SensorWithKnowParameters::SensorWithKnowParameters():FrameSensor()
{

}

SensorWithKnowParameters::SensorWithKnowParameters(const Sensor& sensor):FrameSensor(sensor)
{

}

SensorWithKnowParameters::SensorWithKnowParameters(int myId):FrameSensor(myId)
{

}

SensorWithKnowParameters::SensorWithKnowParameters(int myId, const Matrix& myXa):FrameSensor(myId)
{
	setXa(myXa);
}

SensorWithKnowParameters::SensorWithKnowParameters(int myId, const Matrix& myXa, const Matrix& mySigmaXa):FrameSensor(myId)
{
	setXa(myXa, mySigmaXa);
}



// Private attribute accessor methods
//

void SensorWithKnowParameters::setXa(const Matrix& newXa)
{
	if (newXa.getRows() == 6 && newXa.getCols() == 1)
	{
		Xa = newXa;
		SigmaXa.resize(newXa.getRows(), newXa.getCols()).ones();
	}
	else
        std::cerr << "Xa is not in the expected format.";
}

void SensorWithKnowParameters::setXa(const Matrix& newXa, const Matrix& newSigmaXa)
{
	if (newXa.getRows() == 6 && newXa.getCols() == 1)
	{
		Xa = newXa;
		if (newSigmaXa.getRows() == newXa.getRows() && newSigmaXa.getCols() == newXa.getCols())
			SigmaXa = newSigmaXa;
		else if (newSigmaXa.getRows() == newXa.getRows() && newSigmaXa.getCols() == newXa.getRows())
			SigmaXa = newSigmaXa;
		else
		{
			SigmaXa.resize(newXa.getRows(), newXa.getCols()).ones();
            std::cerr << "SigmaXa is not in the expected format.";
		}
	}
	else
        std::cerr << "Xa is not in the expected format.";
}

Matrix SensorWithKnowParameters::getXa() const
{
	return Xa;
}

Matrix SensorWithKnowParameters::getSigmaXa() const
{
	return SigmaXa;
}



// Composed objects accessor methods
//

// EObject methods
bool SensorWithKnowParameters::is(std::string s)
{
	return (s == "SensorWithKnowParameters" ? true : false);
}

// XML methods
//

void SensorWithKnowParameters::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	id_ = Conversion::stringToInt(root.attribute("key"));
	sensorId_ = root.elementByTagName("sensorId").toString();
	geometry_ = root.elementByTagName("geometry").toString();
	detector_ = root.elementByTagName("detector").toString();
	energySource_ = root.elementByTagName("energySource").toString();
	calculationMode_ = root.elementByTagName("calculationMode").toString();

	spectralRangesUnit_ = root.elementByTagName("spectralRanges").attribute("uom");
    std::deque<EDomElement> xmlSpectralRanges = root.elementsByTagName("spectralRange");
	spectralRanges_.clear();
	for (unsigned int i = 0; i < xmlSpectralRanges.size(); i++)
	{
		SpectralRange* spec = new SpectralRange;
		spec->band = Conversion::stringToInt(xmlSpectralRanges.at(i).attribute("band"));
		spec->inferiorLimit = xmlSpectralRanges.at(i).elementByTagName("inferiorLimit").toDouble();
		spec->superiorLimit = xmlSpectralRanges.at(i).elementByTagName("superiorLimit").toDouble();
		spectralRanges_.push_back(*spec);
	}

	description_ = root.elementByTagName("description").toString();
	calibrationCertificateNumber_ = root.elementByTagName("number").toString();
	calibrationCertificateNumber_ = root.elementByTagName("dispatch").toString();
	calibrationCertificateExpiration_ = root.elementByTagName("expiration").toString();

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

	EDomElement xa =  root.elementByTagName("Xa");
	Matrix newXa(6,1);
	newXa.set(1,1,xa.elementByTagName("a0").toDouble());
	newXa.set(2,1,xa.elementByTagName("a1").toDouble());
	newXa.set(3,1,xa.elementByTagName("a2").toDouble());
	newXa.set(4,1,xa.elementByTagName("b0").toDouble());
	newXa.set(5,1,xa.elementByTagName("b1").toDouble());
	newXa.set(6,1,xa.elementByTagName("b2").toDouble());
	setXa(newXa);
}

std::string SensorWithKnowParameters::xmlGetData()
{
    std::stringstream result;
	result << "<sensor key=\"" << Conversion::intToString(id_) << "\">\n";
	result << "<sensorId>" << sensorId_ << "</sensorId>\n";
	result << "<type>\n";
	result << "<geometry>" << geometry_ << "</geometry>\n";
	result << "<platform>aerial</platform>\n";
	result << "<detector>" << detector_ << "</detector>\n";
	result << "<energySource>" << energySource_ << "</energySource>\n";
	result << "<calculationMode>" << calculationMode_ << "</calculationMode>\n";
	result << "<spectralRanges uom=\"" << spectralRangesUnit_ << "\">\n";
	for (unsigned int i = 0; i < spectralRanges_.size(); i++)
	{
		result << "<spectralRange band=\"" << Conversion::intToString(spectralRanges_.at(i).band) << "\">\n";
		result << "<inferiorLimit>" << Conversion::doubleToString(spectralRanges_.at(i).inferiorLimit) << "</inferiorLimit>\n";
		result << "<superiorLimit>" << Conversion::doubleToString(spectralRanges_.at(i).superiorLimit) << "</superiorLimit>\n";
		result << "</spectralRange>\n";
	}
	result << "</spectralRanges>\n";
	result << "</type>\n";
	result << "<description>" << description_ << "</description>\n";
	result << "<calibrationCertificate>\n";
	result << "<number>" << calibrationCertificateNumber_ << "</number>\n";
	result << "<expiration>" << calibrationCertificateExpiration_ << "</expiration>\n";
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
	result << "<Xa>\n";
	Matrix xa = getXa();
	result << "<a0>" << xa.get(1,1) << "</a0>\n";
	result << "<a1>" << xa.get(2,1) << "</a1>\n";
	result << "<a2>" << xa.get(3,1) << "</a2>\n";
	result << "<b0>" << xa.get(4,1) << "</b0>\n";
	result << "<b1>" << xa.get(5,1) << "</b1>\n";
	result << "<b2>" << xa.get(6,1) << "</b2>\n";
	result << "</Xa>\n";
	result << "</sensor>\n";
	return result.str();
}

// Other methods
//

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
