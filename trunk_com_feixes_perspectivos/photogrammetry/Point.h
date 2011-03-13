/*******************************************************************************
                        Point.h
*******************************************************************************/

#ifndef POINT_H
#define POINT_H

#include "EObject.h"
#include "ObjectSpaceCoordinate.h"
#include "DigitalImageSpaceCoordinate.h"
#include "AnalogImageSpaceCoordinate.h"

class Image;

/******************************* Abstract Class ********************************
Point does not have any pure virtual methods, but its author
  defined it as an abstract class, so you should not use it directly.
  Inherit from it instead and create only objects from the derived classes
*******************************************************************************/

/**
* class Point
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 06/05/2009
* @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato.
*/

class Point : public EObject
{
    //EOBJECT
protected:

    // Private attributes
    //
    int id;
	string pointId;
    string description;

    // Aggregated Objects
    //
    ObjectSpaceCoordinate objectCoordinate;
    deque<DigitalImageSpaceCoordinate> digitalCoordinates;
    deque<AnalogImageSpaceCoordinate> analogCoordinates;

    // Associated Objects
    //
    deque<Image*> myImages;

public:

    // Constructors
    //
    Point();
    Point(int myId);
    virtual ~Point();

    // Private attribute accessor methods
    //
    void setId(int newId);
	void setPointId(string newPointId);
    void setDescription(string newDescription);
    int getId();
	string getPointId();
    string getDescription();

    // Composed Objects accessor methods
    //
    void setObjectCoordinate(ObjectSpaceCoordinate newObject);
    ObjectSpaceCoordinate getObjectCoordinate();

    void setDigitalCoordinates(deque<DigitalImageSpaceCoordinate> newDigitalCoordinates);
    void setAnalogCoordinates(deque<AnalogImageSpaceCoordinate> newAnalogCoordinates);
    deque<DigitalImageSpaceCoordinate> getDigitalCoordinates();
    deque<AnalogImageSpaceCoordinate> getAnalogCoordinates();

    int putDigitalCoordinate(DigitalImageSpaceCoordinate newDigitalCoordinate);
    int putAnalogCoordinate(AnalogImageSpaceCoordinate newAnalogCoordinate);
    DigitalImageSpaceCoordinate getDigitalCoordinate(int imageId);
    AnalogImageSpaceCoordinate getAnalogCoordinate(int imageId);
    DigitalImageSpaceCoordinate getDigitalCoordinateAt(unsigned int index);
    AnalogImageSpaceCoordinate getAnalogCoordinateAt(unsigned int index);
    unsigned int countDigitalCoordinates();
    unsigned int countAnalogCoordinates();

    void clearDigitalCoordinates();
    void clearAnalogCoordinates();
    void deleteDigitalCoordinate(int imageId);
    void deleteAnalogCoordinate(int imageId);

    // Associated object accessor methods
    //
    void putImage(Image* newImageAssociation); // Set point associations. Generally from zero to many images.
    Image* getImage(int imageId); // Get associated point at image by id.
    int countImages(); // Inform the number of points associated the image.
    Image* getImageAt(unsigned int index); // Get associated point at image by index in deque for iterations.

    // Check methods
    //
    bool hasDigitalCoordinate(int imageId);
    bool hasAnalogCoordinate(int imageId);

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // Other methods
    //
};

#endif // POINT_H
