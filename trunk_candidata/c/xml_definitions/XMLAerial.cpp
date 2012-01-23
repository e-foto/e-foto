#include "XMLAerial.h"
/*
XMLAerial::XMLAerial()
{
}

void XMLAerial::xmlSetData(string xml)
{
	EDomElement root(xml);
	setId(Conversion::stringToInt(root.attribute("key")));
*/
/*Sem methodos ainda para preencher
		sensorId = root.elementByTagName("sensorId").toString();
	geometry = root.elementByTagName("geometry").toString();
	detector = root.elementByTagName("detector").toString();
	energySource = root.elementByTagName("energySource").toString();

	spectralRangesUnit = root.elementByTagName("spectralRanges").attribute("uom");
	deque<EDomElement> xmlSpectralRanges = root.elementsByTagName("spectralRange");
	spectralRanges.clear();
	for (unsigned int i = 0; i < xmlSpectralRanges.size(); i++)
	{
		SpectralRange* spec = new SpectralRange;
		spec->band = Conversion::stringToInt(xmlSpectralRanges.at(i).attribute("band"));
		spec->inferiorLimit = xmlSpectralRanges.at(i).elementByTagName("inferiorLimit").toDouble();
		spec->superiorLimit = xmlSpectralRanges.at(i).elementByTagName("superiorLimit").toDouble();
		spectralRanges.push_back(*spec);
	}
 */	/*
	setDescription(root.elementByTagName("description").toString());
	calibrationCertificateNumber = root.elementByTagName("number").toString();
	calibrationCertificateNumber = root.elementByTagName("dispatch").toString();
	calibrationCertificateExpiration = root.elementByTagName("expiration").toString();

	EDomElement xmlFocalDistance = root.elementByTagName("focalDistance");
	focalDistanceUnit = xmlFocalDistance.attribute("uom");

	setFocalDistance(xmlFocalDistance.elementByTagName("value").toDouble());
	if (xmlFocalDistance.elementByTagName("sigma").isAvailable())
		setFocalDistanceSigma(xmlFocalDistance.elementByTagName("sigma").toDouble());
	else
		setFocalDistanceSigma(1.0);

	deque<EDomElement> xmlRadialSymmetric = root.elementByTagName("radialSymmetric").children();
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

	deque<EDomElement> xmlDecentered = root.elementByTagName("decentered").children();
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

	deque<EDomElement> xmlFiductial = root.elementsByTagName("fiductialMark");
	anaFidMarks.clear();
	for (unsigned int i = 0; i < xmlFiductial.size(); i++)
	{
		AnalogFiductialMark* fiductial = new AnalogFiductialMark;
		fiductial->xmlSetData(xmlFiductial.at(i).getContent());
		anaFidMarks.push_back(*fiductial);
	}
}

string Aerial::xmlGetData()
{
	stringstream result;
	result << "<sensor key=\"" << Conversion::intToString(id) << "\">\n";
		result << "<sensorId>" << sensorId << "</sensorId>\n";
	result << "<type>\n";
	result << "<geometry>" << geometry << "</geometry>\n";
	result << "<platform>aerial</platform>\n";
	result << "<detector>" << detector << "</detector>\n";
	result << "<energySource>" << energySource << "</energySource>\n";
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
	result << "<fiductialMarks uom=\"" << anaFidMarks.at(1).getUnit() << "\">\n";
	for (unsigned int i = 0; i < anaFidMarks.size(); i++)
	{
		result << anaFidMarks.at(i).xmlGetData() << "\n";
	}
	result << "</fiductialMarks>\n";
	result << "</sensor>\n";
	return result.str();
}
*/
// Other methods
//

/**
 * Generate Lb from anaFidMarks
 */	/*
virtual voidmarksToLb()
{
	Lb.resize(anaFidMarks.size() * 2,1);
	SigmaLb.resize(anaFidMarks.size() * 2,anaFidMarks.size() * 2).zero();
	for(unsigned int i = 0; i < anaFidMarks.size(); i++)
	{
		Lb.set(i*2+1,1,anaFidMarks.at(i).getXi());
		Lb.set(i*2+2,1,anaFidMarks.at(i).getEta());
		SigmaLb.set(i*2+1,i*2+1,anaFidMarks.at(i).getSigmaXi());
		SigmaLb.set(i*2+2,i*2+2,anaFidMarks.at(i).getSigmaEta());
		SigmaLb.set(i*2+1,i*2+2,anaFidMarks.at(i).getSigmaXiEta());
		SigmaLb.set(i*2+2,i*2+1,anaFidMarks.at(i).getSigmaXiEta());
	}
}
*/
