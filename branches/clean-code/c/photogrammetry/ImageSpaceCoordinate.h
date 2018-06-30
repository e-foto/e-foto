#ifndef IMAGESPACECOORDINATE_H
#define IMAGESPACECOORDINATE_H
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
#include "AbstractImageCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file ImageSpaceCoordinate.h
* \class ImageSpaceCoordinate
* \brief Classes de coordenada digital sobre o espaço imagem (espaço discreto 2D).
* \todo Em breve poderão surgir novas classes denominadas DiscreetDigitalCoordinate para manter extamente a implementação desta interface e SubPixelatedDigitalCoordinate para extender esta classe em ocasiões onde for importante a medição em subpixels.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ImageSpaceCoordinate : public AbstractImageCoordinate
{

private:
	int imageId;

public:

	/**
 * \brief Construtor vazio.
 */
	ImageSpaceCoordinate();

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 */
    explicit ImageSpaceCoordinate(int myImageId);

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem com todos os seus valores.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myCol	Valor da coluna.
 * \param myLin	Valor da linha.
 */
    explicit ImageSpaceCoordinate(int myImageId, double myCol, double myLin);

	/**
 * \brief Construtor que cria uma coordenada digital identificada sobre o espaço imagem com todos os seus valores usando matrizes.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 */
    explicit ImageSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);

	/**
 * \brief Destrutor padrão.
 */
	~ImageSpaceCoordinate();

	/**
 * \brief Método de acesso que seta o valor de id da imagem à qual a coordenada pertence.
 * \param newSensorId	Novo identificador da imagem à qual a coordenada pertence.
 */
	void setImageId(int newImageId);

	/**
 * \brief Método de acesso que retorna o valor de id da imagem à qual a coordenada pertence.
 * \return int	int	Identificador da imagem à qual a coordenada pertence.
 */
    int getImageId() const;

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe ImageSpaceCoordinate.
 */
	void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o string contendo o xml para uma instância da classe ImageSpaceCoordinate.
 */
	std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGESPACECOORDINATE_H
