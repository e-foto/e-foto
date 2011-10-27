/**************************************************************************
						ExteriorOrientation.cpp
**************************************************************************/

#include "ExteriorOrientation.h"
#include "Image.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Associated object accessor methods
//

void ExteriorOrientation::setImageId(int newImageId)
{
	imageId = newImageId;
}

/**
 * Sets the value of myImage
 * @param newImage the new value of myImage
 */
void ExteriorOrientation::setImage(Image* newImage)
{
	myImage = newImage;
}


int ExteriorOrientation::getImageId()
{
	return imageId;
}

/**
 * Gets the value of myImage
 * @return the value of myImage
 */
Image* ExteriorOrientation::getImage()
{
	return myImage;
}

// EObject methods
//

/**
 *
 */
string ExteriorOrientation::objectType(void)
{
	stringstream result;
	result << "ExteriorOrientation " << imageId;
	return result.str();
}

/**
 *
 */
string ExteriorOrientation::objectAssociations(void)
{
	return myImage->objectType();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
