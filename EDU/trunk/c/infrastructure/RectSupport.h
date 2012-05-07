#ifndef RECTSUPPORT_H
#define RECTSUPPORT_H

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
	string objectType(void);

	/**
 * \brief Método para retornar o número de colunas da matriz.
 * \return unsigned	Número de colunas.
 */
	unsigned int getCols() const;

	/**
 * \brief Método para retornar o número de linhas da matriz.
 * \return unsigned	Número de linhas.
 */
	unsigned int getRows() const;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //RECTSUPPORT_H
