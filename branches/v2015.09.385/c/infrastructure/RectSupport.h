#ifndef RECTSUPPORT_H
#define RECTSUPPORT_H
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

#include "EObject.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file RectSupport.h
* \class RectSupport
* \brief Estrutura básica de definição das dimensões para uma matriz.
* \abstract Embora não possua métodos puramente virtuais a classe RectSupport é considerada pelos autores como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Guilherme Abelha, Rafael Aguiar & Irving Badolato.
*/
class RectSupport : public EObject
{

protected:
	unsigned int ncols;
	unsigned int nrows;

public:

	/**
 * \brief Construtor padrão.
 * \param parNcols	Número de colunas.
 * \param parNrows	Número de linhas.
 */
	RectSupport(unsigned int parNcols, unsigned int parNrows);

	/**
 * \brief Destrutor virtual padrão.
 */
	virtual ~RectSupport();

	/**
 * \brief Método para emitir o nome de classe.
 * \return string	Retorna o nome de classe do objeto.
 */
    std::string objectType(void);

	/**
 * \brief Método para retornar o número de colunas da matriz.
 * \return unsigned	Número de colunas.
 */
	unsigned int getCols() const;

	/**
 * \brief Método para retornar o número de linhas da matriz.
 * \return size_t	Número de linhas.
 */
    size_t getRows() const;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //RECTSUPPORT_H
