/**************************************************************************
						IOQuality.cpp
**************************************************************************/

#include "IOQuality.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "InteriorOrientation.h"

// Constructors and Destructor
//

/**
 *
 */
IOQuality::~IOQuality()
{
}

/**
 * @name Private attribute accessors of IOQuality objects.
 */

/** @{ */

/**
 * Get the value of V.
 * @return the value of V
 */
Matrix IOQuality::getV()
{
	return V;
}

/**
 * Get the value of sigma0Squared.
 * @return the value of sigma0Squared
 */
double IOQuality::getsigma0Squared()
{
	return sigma0Squared;
}

/**
 * Get the value of SigmaXa.
 * @return the value of SigmaXa
 */
Matrix IOQuality::getSigmaXa()
{
	return SigmaXa;
}

/**
 * Get the value of SigmaLa.
 * @return the value of SigmaLa
 */
Matrix IOQuality::getSigmaLa()
{
	return SigmaLa;
}

/** @} */

// EObject methods
//

/**
 *
 */
string IOQuality::objectType(void)
{
	return "IOQuality";
}

/**
 *
 */
string IOQuality::objectAssociations(void)
{
	return "";
}

/**
 *
 */
bool IOQuality::is(string s)
{
	return (s == "IOQuality" ? true : false);
}

// XML methods
//

/**
 * 
 */
void IOQuality::xmlSetData(string xml)
{
	EDomElement root(xml);
	V.xmlSetData(root.elementByTagName("V").elementByTagName("mml:matrix").getContent());
	if (root.elementByTagName("sigma0Squared").isAvailable())
		sigma0Squared = root.elementByTagName("sigma0Squared").toDouble();
	else
		sigma0Squared = 1.0;
	if (root.elementByTagName("SigmaXa").isAvailable())
		SigmaXa.xmlSetData(root.elementByTagName("SigmaXa").elementByTagName("mml:matrix").getContent());
	else
		SigmaXa.identity(6);
	if (root.elementByTagName("SigmaLa").isAvailable())
		SigmaLa.xmlSetData(root.elementByTagName("SigmaLa").elementByTagName("mml:matrix").getContent());
	else
		SigmaLa.identity(1);
}

/**
 * 
 */
string IOQuality::xmlGetData()
{
	stringstream result;
	result << "<quality>\n";
	result << "<V>\n";
	result << V.xmlGetData();
	result << "</V>\n";
	if (sigma0Squared == 1.0)
		result << "<sigma0Squared>Not Available</sigma0Squared>\n";
	else
		result << "<sigma0Squared>" << doubleToString(sigma0Squared) << "</sigma0Squared>\n";
	if (SigmaXa.isIdentity())
		result << "<SigmaXa>Not Available</SigmaXa>\n";
	else
	{
		result << "<SigmaXa>\n";
		result << SigmaXa.xmlGetData();
		result << "</SigmaXa>\n";
	}
	if (SigmaLa.isIdentity())
		result << "<SigmaLa>Not Available</SigmaLa>\n";
	else
	{
		result << "<SigmaLa>\n";
		result << SigmaLa.xmlGetData();
		result << "</SigmaLa>\n";
	}
	result << "</quality>\n";
	return result.str();
}

/**
 * @name Other methods
 */ 

/** @{ */

/**
 * This method calculates the values of an IOQuality's attributes based on its Orientation's values.
 * 
 * Formulae used:
 * \F$ V = A * Xa - Lb \F$
 * \F$ \sigma_{0}^{2} = frac{V^{T} * P * V}{n - m}\F$, where n is the # of observations and m the # of unknowns
 * \F$ \SigmaXa = \sigma_{0}^{2} * (A^{T} * P * A)^{-1} \F$
 * \F$ \SigmaLa = \sigma_{0}^{2} * A (A^{T} * P * A)^{-1} * A^{T} \F$
 * 
 * Reference: Coelho & Brito, Fotogrametria Digital. Rio de Janeiro, 2007. 
 * @param *myIO, *mySensorWithFiducialMarks
 */
void IOQuality::calculate(InteriorOrientation* myIO, Sensor* mySensor)
{
	if (mySensor->is("SensorWithFiducialMarks"))
	{
		SensorWithFiducialMarks* mySensorWithFiducialMarks = (SensorWithFiducialMarks*) mySensor;
		V = (myIO->getA() * myIO->getXa()) - mySensorWithFiducialMarks->getLb();
		sigma0Squared = (((V.transpose() * myIO->getP()) * V) / (mySensorWithFiducialMarks->getLb().getRows() - myIO->getXa().getRows())).get(1,1);
		SigmaXa = ((myIO->getA().transpose() * myIO->getP()) * myIO->getA()).inverse() * sigma0Squared;
		SigmaLa = ((myIO->getA() * ((myIO->getA().transpose() * myIO->getP()) * myIO->getA()).inverse()) * myIO->getA().transpose()) * sigma0Squared;
	}
	else if (mySensor->is("SensorWithKnowDimensions"))
	{
		SensorWithKnowDimensions* mySensorWithKnowDimensions = (SensorWithKnowDimensions*) mySensor;
		V = (myIO->getA() * myIO->getXa()) - mySensorWithKnowDimensions->forgeLb();
		sigma0Squared = (((V.transpose() * myIO->getP()) * V) / (mySensorWithKnowDimensions->forgeLb().getRows() - myIO->getXa().getRows())).get(1,1);
		SigmaXa = ((myIO->getA().transpose() * myIO->getP()) * myIO->getA()).inverse() * sigma0Squared;
		SigmaLa = ((myIO->getA() * ((myIO->getA().transpose() * myIO->getP()) * myIO->getA()).inverse()) * myIO->getA().transpose()) * sigma0Squared;
	}
}

/** @} */
