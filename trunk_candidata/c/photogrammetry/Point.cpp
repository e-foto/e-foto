/*******************************************************************************
	  Point.cpp
*******************************************************************************/

#include "Point.h"
#include "Image.h"

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
void Point::setPointId(string newPointId)
{
	pointId = newPointId;
}

/**
 * Set the value of description
 * @param newId the new value of description
 */
void Point::setDescription(string newDescription)
{
	description = newDescription;
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
string Point::getPointId()
{
	return pointId;
}

/**
 * Get the value of description
 * @return the value of description
 */
string Point::getDescription()
{
	return description;
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
 * Set all the values of digitalCoordinates deque at once
 * @param newDigitalCoordinates the new values
 */
void Point::setDigitalCoordinates(deque<DigitalImageSpaceCoordinate> newDigitalCoordinates)
{
	digitalCoordinates = newDigitalCoordinates;
}

/**
 * Set all the values of analogCoordinates deque at once
 * @param newAnalogCoordinates the new values
 */
void Point::setAnalogCoordinates(deque<AnalogImageSpaceCoordinate> newAnalogCoordinates)
{
	analogCoordinates = newAnalogCoordinates;
}

/**
 * Get the value of digitalCoordinates deque
 * @return the values of digitalCoordinates deque
 */
deque<DigitalImageSpaceCoordinate> Point::getDigitalCoordinates()
{
	return digitalCoordinates;
}

/**
 * Get the value of analogCoordinates deque
 * @return the values of analogCoordinates deque
 */
deque<AnalogImageSpaceCoordinate> Point::getAnalogCoordinates()
{
	return analogCoordinates;
}


/**
 * Put one value in digitalCoordinates deque
 * @param newDigitalSpace the new value in digitalCoordinates
 */
int Point::putDigitalCoordinate(DigitalImageSpaceCoordinate newDigitalCoordinate)
{
	if (digitalCoordinates.empty())
	{
		digitalCoordinates.push_back(newDigitalCoordinate);
	}
	else
	{
		if (digitalCoordinates.back().getImageId() < newDigitalCoordinate.getImageId())
		{
			digitalCoordinates.push_back(newDigitalCoordinate);
		}
		else
		{
			for (unsigned int i = 0; i < digitalCoordinates.size(); i++)
			{
				if (digitalCoordinates.at(i).getImageId() > newDigitalCoordinate.getImageId())
				{
					digitalCoordinates.insert(digitalCoordinates.begin()+i,newDigitalCoordinate);
					break;
				}
				else if (digitalCoordinates.at(i).getImageId() == newDigitalCoordinate.getImageId())
				{
					digitalCoordinates.erase(digitalCoordinates.begin()+i);
					digitalCoordinates.insert(digitalCoordinates.begin()+i,newDigitalCoordinate);
					return 1;
				}
			}
		}
	}
	return 0;
}

/**
 * Put one value in analogCoordinates deque
 * @param newAnalogSpace the new value in analogCoordinates
 */
int Point::putAnalogCoordinate(AnalogImageSpaceCoordinate newAnalogCoordinate)
{

	int imageId = newAnalogCoordinate.getImageId();
	if (hasAnalogCoordinate(imageId))
		return 1;
	analogCoordinates.push_back(newAnalogCoordinate);
	return 0;
}

/**
 * Get the value of the digitalCoordinate with a specific imageId
 * @param imageId the specified imageId
 * @return the value of digitalCoordinate
 */
DigitalImageSpaceCoordinate Point::getDigitalCoordinate(int imageId)
{

	for (unsigned int i = 0; i < digitalCoordinates.size(); i++)
		if (digitalCoordinates.at(i).getImageId() == imageId)
			return digitalCoordinates.at(i);
	return DigitalImageSpaceCoordinate();
}

/**
 * Get the value of analogCoordinates with a specific imageId
 * @param imageId the specified imageId
 * @return the value of analogCoordinates
 */
AnalogImageSpaceCoordinate Point::getAnalogCoordinate(int imageId)
{

	for (unsigned int i = 0; i < analogCoordinates.size(); i++)
		if (analogCoordinates.at(i).getImageId() == imageId)
			return analogCoordinates.at(i);
	return AnalogImageSpaceCoordinate();
}

/**
 *
 */
DigitalImageSpaceCoordinate Point::getDigitalCoordinateAt(unsigned int index)
{
	if (index < digitalCoordinates.size())
		return digitalCoordinates.at(index);
	return DigitalImageSpaceCoordinate();
}

/**
 *
 */
AnalogImageSpaceCoordinate Point::getAnalogCoordinateAt(unsigned int index)
{
	if (index < analogCoordinates.size())
		return analogCoordinates.at(index);
	return AnalogImageSpaceCoordinate();
}

/**
 *
 */
unsigned int Point::countDigitalCoordinates()
{
	return digitalCoordinates.size();
}

/**
 *
 */
unsigned int Point::countAnalogCoordinates()
{
	return analogCoordinates.size();
}

/**
 * Clear all the values of digitalCoordinates deque
 */
void Point::clearDigitalCoordinates()
{
	digitalCoordinates.clear();
}

/**
 * Clear all the values of analogCoordinates deque
 */
void Point::clearAnalogCoordinates()
{
	analogCoordinates.clear();
}

/**
 * Delete the value of digitalCoordinate with a specific imageId
 * @param imageId the specified imageId
 */
void Point::deleteDigitalCoordinate(int imageId)
{

	for (unsigned int i = 0; i < digitalCoordinates.size(); i++)
		if (digitalCoordinates.at(i).getImageId() == imageId)
		{
			digitalCoordinates.erase(digitalCoordinates.begin()+i);
			break;
		}
}

/**
 * Delete the value of analogCoordinate with a specific imageId
 * @param imageId the specified imageId
 */
void Point::deleteAnalogCoordinate(int imageId)
{

	for (unsigned int i = 0; i < analogCoordinates.size(); i++)
		if (analogCoordinates.at(i).getImageId() == imageId)
		{
			analogCoordinates.erase(analogCoordinates.begin()+i);
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
 * Check if digitalCoordinates has a member with a specific imageId
 * @param imageId the specified imageId
 */
bool Point::hasDigitalCoordinate(int imageId)
{

	for (unsigned int i = 0; i < digitalCoordinates.size(); i++)
		if (digitalCoordinates.at(i).getImageId() == imageId)
			return true;
	return false;
}

/**
 * Check if analogCoordinates has a member with a specific imageId
 * @param imageId the specified imageId
 */
bool Point::hasAnalogCoordinate(int imageId)
{

	for (unsigned int i = 0; i < analogCoordinates.size(); i++)
		if (analogCoordinates.at(i).getImageId() == imageId)
			return true;
	return false;

}

// EObject methods
//

/**
 *
 */
string Point::objectType(void)
{
	stringstream result;
	result << "Point " << id;
	return result.str();
}

/**
 *
 */
string Point::objectAssociations(void)
{
	stringstream result;
	if (countImages() != 0)
		result << "Image";
	for (int i = 0; i < countImages(); i++)
		result << " " << getImageAt(i)->getId();
	return result.str();
}

/**
 *
 */
bool Point::is(string s)
{
	return (s == "Point" ? true : false);
}

// Other methods
//

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
