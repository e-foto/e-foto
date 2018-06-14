#ifndef POINT_H
#define POINT_H
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
#include "EDom.h"
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
class Point : public EDom
{
public:

	enum PointType { PHOTOGRAMMETRIC, CHECKING, CONTROL, UNKNOWN};

protected:
	int id;
    std::string pointId;
    std::string description;
	PointType type;

	ObjectSpaceCoordinate objectCoordinate;
    std::deque<ImageSpaceCoordinate> imageCoordinates;
    std::deque<DetectorSpaceCoordinate> detectorCoordinates;

    std::deque<Image*> myImages;

    PointType readPointType(std::string type);
    std::string writePointType(PointType type);

public:

	// Constructors
	//
	Point();
    explicit Point(int myId);
	virtual ~Point();

	// Private attribute accessor methods
	//
	void setId(int newId);
    void setPointId(std::string newPointId);
    void setDescription(std::string newDescription);
	void setType(PointType type);
	int getId();
    std::string getPointId();
    std::string getDescription();
	PointType getType();

	// Composed Objects accessor methods
	//
	void setObjectCoordinate(ObjectSpaceCoordinate newObject);
	ObjectSpaceCoordinate getObjectCoordinate();

    void setImageCoordinates(std::deque<ImageSpaceCoordinate> newImageCoordinates);
    void setDetectorCoordinates(std::deque<DetectorSpaceCoordinate> newDetectorCoordinates);
    std::deque<ImageSpaceCoordinate> getImageCoordinates();
    std::deque<DetectorSpaceCoordinate> getDetectorCoordinates();

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
    Image* getImageAt(unsigned int index); // Get associated point at image by index in std::deque for iterations.

	// Check methods
	//
	bool hasImageCoordinate(int imageId);
	bool hasDetectorCoordinate(int imageId);

	// Other methods
	//
    void xmlSetData(std::string xml);
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // POINT_H
