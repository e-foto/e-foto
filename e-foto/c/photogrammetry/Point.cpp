/*******************************************************************************
	  Point.cpp
*******************************************************************************/
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
#include "Point.h"
#include "Image.h"

#include <sstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors

/**
 *
 */
Point::Point()
{
	setId(0);
}

/**
 *
 */
Point::Point(int myId)
{
	setId(myId);
}

/**
 *
 */
Point::~Point()
{
}

// Private attribute accessor methods
//

/**
 * Set the value of id
 * @param newId the new value of id
 */
void Point::setId(int newId)
{
	id = newId;
}

/**
 * Set the value of gcpId
 * @param newId the new value of gcpId
 */
void Point::setPointId(std::string newPointId)
{
	pointId = newPointId;
}

/**
 * Set the value of description
 * @param newId the new value of description
 */
void Point::setDescription(std::string newDescription)
{
	description = newDescription;
}

void Point::setType(PointType type)
{
	this->type = type;
}

/**
 * Get the value of id
 * @return the value of id
 */
int Point::getId()
{
	return id;
}

/**
 * Get the value of gcpId
 * @return the value of gcpId
 */
std::string Point::getPointId()
{
	return pointId;
}

/**
 * Get the value of description
 * @return the value of description
 */
std::string Point::getDescription()
{
	return description;
}

Point::PointType Point::getType()
{
	return type;
}

// Composed Objects accessor methods
//

/**
 * Set the value of objectCoordinate
 * @param newObjectCoordinate the new value
 */
void Point::setObjectCoordinate(ObjectSpaceCoordinate newObjectCoordinate)
{
	objectCoordinate = newObjectCoordinate;
}

/**
 * Get the value of objectCoordinate
 * @return the value of objectCoordinate
 */
ObjectSpaceCoordinate Point::getObjectCoordinate()
{
	return objectCoordinate;
}

/**
 * Set all the values of imageCoordinates deque at once
 * @param newimageCoordinates the new values
 */
void Point::setImageCoordinates(std::deque<ImageSpaceCoordinate> newimageCoordinates)
{
	imageCoordinates = newimageCoordinates;
}

/**
 * Set all the values of DetectorCoordinates deque at once
 * @param newDetectorCoordinates the new values
 */
void Point::setDetectorCoordinates(std::deque<DetectorSpaceCoordinate> newDetectorCoordinates)
{
	detectorCoordinates = newDetectorCoordinates;
}

/**
 * Get the value of imageCoordinates deque
 * @return the values of imageCoordinates deque
 */
std::deque<ImageSpaceCoordinate> Point::getImageCoordinates()
{
	return imageCoordinates;
}

/**
 * Get the value of DetectorCoordinates deque
 * @return the values of DetectorCoordinates deque
 */
std::deque<DetectorSpaceCoordinate> Point::getDetectorCoordinates()
{
	return detectorCoordinates;
}


/**
 * Put one value in imageCoordinates deque
 * @param newimageSpace the new value in imageCoordinates
 */
int Point::putImageCoordinate(ImageSpaceCoordinate newimageCoordinate)
{
	if (imageCoordinates.empty())
	{
		imageCoordinates.push_back(newimageCoordinate);
	}
	else
	{
		if (imageCoordinates.back().getImageId() < newimageCoordinate.getImageId())
		{
			imageCoordinates.push_back(newimageCoordinate);
		}
		else
		{
			for (unsigned int i = 0; i < imageCoordinates.size(); i++)
			{
				if (imageCoordinates.at(i).getImageId() > newimageCoordinate.getImageId())
				{
					imageCoordinates.insert(imageCoordinates.begin()+i,newimageCoordinate);
					break;
				}
				else if (imageCoordinates.at(i).getImageId() == newimageCoordinate.getImageId())
				{
					imageCoordinates.erase(imageCoordinates.begin()+i);
					imageCoordinates.insert(imageCoordinates.begin()+i,newimageCoordinate);
					return 1;
				}
			}
		}
	}
	return 0;
}

/**
 * Put one value in DetectorCoordinates deque
 * @param newDetectorSpace the new value in DetectorCoordinates
 */
int Point::putDetectorCoordinate(DetectorSpaceCoordinate newDetectorCoordinate)
{

	int imageId = newDetectorCoordinate.getImageId();
	if (hasDetectorCoordinate(imageId))
		return 1;
	detectorCoordinates.push_back(newDetectorCoordinate);
	return 0;
}

/**
 * Get the value of the imageCoordinate with a specific imageId
 * @param imageId the specified imageId
 * @return the value of imageCoordinate
 */
ImageSpaceCoordinate Point::getImageCoordinate(int imageId)
{

	for (unsigned int i = 0; i < imageCoordinates.size(); i++)
		if (imageCoordinates.at(i).getImageId() == imageId)
			return imageCoordinates.at(i);
	return ImageSpaceCoordinate();
}

/**
 * Get the value of DetectorCoordinates with a specific imageId
 * @param imageId the specified imageId
 * @return the value of DetectorCoordinates
 */
DetectorSpaceCoordinate Point::getDetectorCoordinate(int imageId)
{

	for (unsigned int i = 0; i < detectorCoordinates.size(); i++)
		if (detectorCoordinates.at(i).getImageId() == imageId)
			return detectorCoordinates.at(i);
	return DetectorSpaceCoordinate();
}

/**
 *
 */
ImageSpaceCoordinate Point::getImageCoordinateAt(unsigned int index)
{
	if (index < imageCoordinates.size())
		return imageCoordinates.at(index);
	return ImageSpaceCoordinate();
}

/**
 *
 */
DetectorSpaceCoordinate Point::getDetectorCoordinateAt(unsigned int index)
{
	if (index < detectorCoordinates.size())
		return detectorCoordinates.at(index);
	return DetectorSpaceCoordinate();
}

/**
 *
 */
unsigned int Point::countImageCoordinates()
{
	return imageCoordinates.size();
}

/**
 *
 */
unsigned int Point::countDetectorCoordinates()
{
	return detectorCoordinates.size();
}

/**
 * Clear all the values of imageCoordinates deque
 */
void Point::clearImageCoordinates()
{
	imageCoordinates.clear();
}

/**
 * Clear all the values of DetectorCoordinates deque
 */
void Point::clearDetectorCoordinates()
{
	detectorCoordinates.clear();
}

/**
 * Delete the value of imageCoordinate with a specific imageId
 * @param imageId the specified imageId
 */
void Point::deleteImageCoordinate(int imageId)
{

	for (unsigned int i = 0; i < imageCoordinates.size(); i++)
		if (imageCoordinates.at(i).getImageId() == imageId)
		{
			imageCoordinates.erase(imageCoordinates.begin()+i);
			break;
		}
}

/**
 * Delete the value of DetectorCoordinate with a specific imageId
 * @param imageId the specified imageId
 */
void Point::deleteDetectorCoordinate(int imageId)
{

	for (unsigned int i = 0; i < detectorCoordinates.size(); i++)
		if (detectorCoordinates.at(i).getImageId() == imageId)
		{
			detectorCoordinates.erase(detectorCoordinates.begin()+i);
			break;
		}
}



// Associated object accessor methods
//

/**
 *
 */
void Point::putImage(Image* newImageAssociation)
{
	bool insert = true;
	// Eliminamos primeiro a possibilidade duplicar uma associação.
	for (unsigned int i = 0; i < myImages.size(); i++)
		if (myImages.at(i) == newImageAssociation)
			insert = false;
	// Fazemos a nova associação.
	if (insert)
		myImages.push_back(newImageAssociation);
}

void Point::clearImages()
{
    myImages.clear();
}

void Point::removeImage(int id)
{
    for (int i = myImages.size()-1; i >= 0; i--)
        if (myImages.at(i)->getId() == id)
            myImages.erase(myImages.begin()+i);
}

/**
 *
 */
Image* Point::getImage(int imageId)
{
	for (unsigned int i = 0; i < myImages.size(); i++)
		if (myImages.at(i)->getId() == imageId)
			return myImages.at(i);
	return NULL;
}

/**
 *
 */
int Point::countImages()
{
	return myImages.size();
}

/**
 *
 */
Image* Point::getImageAt(unsigned int index)
{
	if (index < myImages.size())
		return myImages.at(index);
	return NULL;
}



// Cheking methods
//

/**
 * Check if imageCoordinates has a member with a specific imageId
 * @param imageId the specified imageId
 */
bool Point::hasImageCoordinate(int imageId)
{

	for (unsigned int i = 0; i < imageCoordinates.size(); i++)
		if (imageCoordinates.at(i).getImageId() == imageId)
			return true;
	return false;
}

/**
 * Check if DetectorCoordinates has a member with a specific imageId
 * @param imageId the specified imageId
 */
bool Point::hasDetectorCoordinate(int imageId)
{

	for (unsigned int i = 0; i < detectorCoordinates.size(); i++)
		if (detectorCoordinates.at(i).getImageId() == imageId)
			return true;
	return false;

}

// EObject methods
//

/**
 *
 */
std::string Point::objectType(void)
{
    std::stringstream result;
	result << "Point " << id;
	return result.str();
}

/**
 *
 */
std::string Point::objectAssociations(void)
{
    std::stringstream result;
	if (countImages() != 0)
		result << "Image";
	for (int i = 0; i < countImages(); i++)
		result << " " << getImageAt(i)->getId();
	return result.str();
}

/**
 *
 */
bool Point::is(std::string s)
{
	return (s == "Point" ? true : false);
}

void Point::xmlSetData(std::string xml)
{
	EDomElement root(xml);
	id = Conversion::stringToInt(root.attribute("key"));
	type = readPointType(root.attribute("type"));
	pointId = root.elementByTagName("pointId").toString();
	description = root.elementByTagName("description").toString();
	objectCoordinate.xmlSetData(root.elementByTagName("spatialCoordinates").getContent());
    std::deque<EDomElement> xmlimageCoordinates = root.elementsByTagName("imageCoordinates");
	imageCoordinates.clear();
	for (unsigned int i = 0; i < xmlimageCoordinates.size(); i++)
	{
		ImageSpaceCoordinate* disc = new ImageSpaceCoordinate;
		disc->xmlSetData(xmlimageCoordinates.at(i).getContent());
		imageCoordinates.push_back(*disc);
	}
}

std::string Point::xmlGetData()
{
    std::stringstream result;
	result << "<point key=\"" << Conversion::intToString(id) << "\" type=\"" << writePointType(type) << "\">\n";
	result << "<pointId>" << pointId << "</pointId>\n";
	result << "<description>" << description << "</description>\n";
	result << objectCoordinate.xmlGetData();
	result << "<imagesMeasurements>\n";
	for (unsigned int i = 0; i < imageCoordinates.size(); i++)
	{
		result << imageCoordinates.at(i).xmlGetData();
	}
	result << "</imagesMeasurements>\n";
	result << "</point>\n";
	return result.str();
}

Point::PointType Point::readPointType(std::string type)
{
	if (type == "control")
	{
		return CONTROL;
	}
	else if (type == "checking")
	{
		return CHECKING;
	}
	else if (type == "photogrammetric")
	{
		return PHOTOGRAMMETRIC;
	}
	else
	{
		return UNKNOWN;
	}
}

std::string Point::writePointType(PointType type)
{
	if (type == CONTROL)
	{
		return "control";
	}
	else if (type == CHECKING)
	{
		return "checking";
	}
	else if (type == PHOTOGRAMMETRIC)
	{
		return "photogrammetric";
	}
	else
	{
		return "unknown";
	}
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
