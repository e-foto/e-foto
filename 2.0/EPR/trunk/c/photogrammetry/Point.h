#ifndef POINT_H
#define POINT_H

#include "EObject.h"
#include "ObjectSpaceCoordinate.h"
#include "ImageSpaceCoordinate.h"
#include "DetectorSpaceCoordinate.h"

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
public:

	enum PointType { PHOTOGRAMMETRIC, CHECKING, CONTROL, UNKNOWN};

protected:
	int id;
	string pointId;
	string description;
	PointType type;

	ObjectSpaceCoordinate objectCoordinate;
	deque<ImageSpaceCoordinate> imageCoordinates;
	deque<DetectorSpaceCoordinate> detectorCoordinates;

	deque<Image*> myImages;

	PointType readPointType(string type);
	string writePointType(PointType type);

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
	void setType(PointType type);
	int getId();
	string getPointId();
	string getDescription();
	PointType getType();

	// Composed Objects accessor methods
	//
	void setObjectCoordinate(ObjectSpaceCoordinate newObject);
	ObjectSpaceCoordinate getObjectCoordinate();

	void setImageCoordinates(deque<ImageSpaceCoordinate> newImageCoordinates);
	void setDetectorCoordinates(deque<DetectorSpaceCoordinate> newDetectorCoordinates);
	deque<ImageSpaceCoordinate> getImageCoordinates();
	deque<DetectorSpaceCoordinate> getDetectorCoordinates();

	int putImageCoordinate(ImageSpaceCoordinate newimageCoordinate);
	int putDetectorCoordinate(DetectorSpaceCoordinate newdetectorCoordinate);
	ImageSpaceCoordinate getImageCoordinate(int imageId);
	DetectorSpaceCoordinate getDetectorCoordinate(int imageId);
	ImageSpaceCoordinate getImageCoordinateAt(unsigned int index);
	DetectorSpaceCoordinate getDetectorCoordinateAt(unsigned int index);
	unsigned int countImageCoordinates();
	unsigned int countDetectorCoordinates();

	void clearImageCoordinates();
	void clearDetectorCoordinates();
	void deleteImageCoordinate(int imageId);
	void deleteDetectorCoordinate(int imageId);

	// Associated object accessor methods
	//
	void putImage(Image* newImageAssociation); // Set point associations. Generally from zero to many images.
    void clearImages(); // Remove associations.
    void removeImage(int id); // Remove one association;
	Image* getImage(int imageId); // Get associated point at image by id.
	int countImages(); // Inform the number of points associated the image.
	Image* getImageAt(unsigned int index); // Get associated point at image by index in deque for iterations.

	// Check methods
	//
	bool hasImageCoordinate(int imageId);
	bool hasDetectorCoordinate(int imageId);

	// EObject methods
	//
	string objectType(void);
	string objectAssociations(void);
	bool is(string s);

	// Other methods
	//
	void xmlSetData(string xml);
	string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // POINT_H
