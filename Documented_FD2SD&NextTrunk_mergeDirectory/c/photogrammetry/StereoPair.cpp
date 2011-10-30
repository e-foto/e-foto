/*******************************************************************************
							  StereoPair.cpp
*******************************************************************************/

#include "StereoPair.h"

// Constructors and destructors

StereoPair::StereoPair()
{
	leftImage = NULL;
	rightImage = NULL;
}

StereoPair::StereoPair(Image* myLeftImage, Image* myRightImage)
{
	leftImage = myLeftImage;
	rightImage = myRightImage;
}

StereoPair::~StereoPair()
{

}

// Aggregation modifiers

void StereoPair::setLeftImage(Image* newImage)
{
	leftImage = newImage;
}

void StereoPair::setRightImage(Image* newImage)
{
	rightImage = newImage;
}

Image* StereoPair::getLeftImage()
{
	return leftImage;
}

Image* StereoPair::getRightImage()
{
	return rightImage;
}

// Consistency testers

bool StereoPair::leftHasIO()
{
	if (leftImage->getIO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::leftHasEO()
{
	if (leftImage->getEO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::rightHasIO()
{
	if (rightImage->getIO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::rightHasEO()
{
	if (rightImage->getEO() == NULL)
		return false;
	else
		return true;
}

bool StereoPair::leftOriented()
{
	if (leftHasIO() && leftHasEO())
		return true;
	else
		return false;
}

bool StereoPair::rightOriented()
{
	if (rightHasIO() && rightHasEO())
		return true;
	else
		return false;
}

bool StereoPair::hasOrientations()
{
	if (leftOriented() && rightOriented())
		return true;
	else
		return false;
}

// EObject methods
//
string StereoPair::objectType(void)
{
	stringstream result;
	result << "StereoPair " << leftImage->getId() << " " << rightImage->getId();
	return result.str();
}

string StereoPair::objectAssociations(void)
{
	return "";
}

bool StereoPair::is(string s)
{
	return (s == "StereoPair" ? true : false);
}

// XML methods
//
void StereoPair::xmlSetData(string xml)
{

}

string StereoPair::xmlGetData()
{
	return "";
}
