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
#ifndef ABSTRACTIMAGECOORDINATE_H
#define ABSTRACTIMAGECOORDINATE_H

#include "Coordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file AbstracDigitalCoordinate.h
* \class AbstractImageCoordinate
* \brief Base para as classes de coordenadas digitais 2D
* \abstract A classe de Coordenadas Digitais 2D foi definida como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class AbstractImageCoordinate : public Coordinate
{

protected:
	double lin;
	double col;
    std::string unit;

public:

	/**
 * \brief Destrutor virtual padrão.
 */
	virtual ~AbstractImageCoordinate();

	/**
 * \brief Método para alterar o valor Linha num sistema de coordenadas (Coluna, Linha).
 * \param newLin	Novo valor de linha.
 */
	void setLin(double newLin);

	/**
 * \brief Método para alterar o valor Coluna num sistema de coordenadas (Coluna, Linha).
 * \param newCol	Novo valor de coluna.
 */
	void setCol(double newCol);

	/**
 * \brief Método para retornar o valor de Linha num sistema de coordenadas (Coluna, Linha).
 * \return double	Valor da linha.
 */
	double getLin();

	/**
 * \brief Método para retornar o valor de Coluna num sistema de coordenadas (Coluna, Linha).
 * \return double	Valor da coluna.
 */
	double getCol();

	/**
 * \brief Método para retornar a unidade em que a coordenada é considerada.
 * \return std::string	Texto descritor da unidade de medidas considerada para a coordenada.
 */
    std::string getUnit();

	/**
 * \brief Método para alterar a posição de uma coordenada.
 * \param newPosition	Matriz coluna com os valores de posicionamento da nova coordenada.
 */
	void setPosition(const PositionMatrix& newPosition);

	/**
 * \brief Método para retornar a coordenada em uma matriz coluna.
 * \return PositionMatrix	Matriz coluna com os valores de posicionamento da coordenada.
 */
	PositionMatrix getPosition();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //ABSTRACTIMAGECOORDINATE_H
