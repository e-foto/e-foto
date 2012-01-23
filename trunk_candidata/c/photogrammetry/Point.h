#ifndef POINT_H
#define POINT_H

#include "EObject.h"
#include "ObjectSpaceCoordinate.h"
#include "DigitalImageSpaceCoordinate.h"
#include "AnalogImageSpaceCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Image;

/**
* \file Point.h
* \class Point
* \brief Classe que agrega todas as informações que possam ser pertinentes a um ponto. Sua identificação no projeto, sua coordenada no espaço objeto e suas coordenadas projetadas sobre as imagens onde ele possa ser observado.
* \abstract Essa é uma classe abstrata para permitir manipular os diversos tipos de pontos específicos de pontos.
* \attention Construa apenas objetos das classes derivadas.
* \todo Está em análise refatorar esta família de classes para permitir a transformação de tipo dos pontos sem alterar o endereço de memória destas instâncias.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class Point : public EObject
{

protected:
	int id;
	string pointId;
	string description;

	ObjectSpaceCoordinate objectCoordinate;
	deque<DigitalImageSpaceCoordinate> digitalCoordinates;
	deque<AnalogImageSpaceCoordinate> analogCoordinates;

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

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // POINT_H
