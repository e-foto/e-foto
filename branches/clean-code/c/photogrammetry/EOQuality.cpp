/**************************************************************************
	  EOQuality.cpp
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

#include "EOQuality.h"
#include "ExteriorOrientation.h"
#include "SpatialRessection.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and Destructor
//

/**
 *
 */
EOQuality::~EOQuality()
{
}

// Private attribute accessors
//

/**
 * Get the value of V.
 * @return the value of V
 */
Matrix EOQuality::getV() const
{
	return V;
}

/**
 * Get the value of sigma0Squared.
 * @return the value of sigma0Squared
 */
double EOQuality::getsigma0Squared() const
{
	return sigma0Squared;
}

/**
 * Get the value of SigmaXa.
 * @return the value of SigmaXa
 */
Matrix EOQuality::getSigmaXa() const
{
	return SigmaXa;
}

/**
 * Get the value of SigmaLa.
 * @return the value of SigmaLa
 */
Matrix EOQuality::getSigmaLa() const
{
	return SigmaLa;
}

// XML methods
//

/**
 *
 */
void EOQuality::xmlSetData(std::string xml)
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
std::string EOQuality::xmlGetData()
{
    std::stringstream result;
	result << "<quality>\n";
	result << "<V>\n";
	result << V.xmlGetData();
	result << "</V>\n";
	if (sigma0Squared == 1.0)
		result << "<sigma0Squared>Not Available</sigma0Squared>\n";
	else
		result << "<sigma0Squared>" << Conversion::doubleToString(sigma0Squared) << "</sigma0Squared>\n";
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

// Other methods
//

/**
 * This method calculates the values of an IOQuality's attributes based on its Orientation's values.
 */
void EOQuality::calculate(SpatialRessection* myEO)
{
	V = myEO->getLb() - myEO->getLastL0();
	sigma0Squared = (((V.transpose() * myEO->getP()) * V) / (myEO->getLb().getRows() - myEO->getXa().getRows())).get(1,1);
	SigmaXa = ((myEO->getA().transpose() * myEO->getP()) * myEO->getA()).inverse() * sigma0Squared;
	SigmaLa = ((myEO->getA() * ((myEO->getA().transpose() * myEO->getP()) * myEO->getA()).inverse()) * myEO->getA().transpose()) * sigma0Squared;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
