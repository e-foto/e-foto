#ifndef COORDINATE_H
#define COORDINATE_H

#include "EObject.h"
#include "PositionMatrix.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file Coordinate.h
* \class Coordinate
* \brief Base para as classes de coordenadas
* \abstract A classe de Coordenadas foi definida como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Construa apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class Coordinate : public EObject
{

protected:
	bool available;
	bool sigmaAvailable;

public:

	/**
 * \brief Destrutor virtual padrão.
 */
	virtual ~Coordinate();

	/**
 * \brief Método para alterar o valor que diz se a coordenada é válida.
 * \param value	Valor verdadeiro ou falso para o julgamento sobre a validade da coordenada.
 */
	void setAvailable(bool value);

	/**
 * \brief Método para alterar o valor que diz se o erro (sigma) armazenado pela coordenada é válido.
 * \param value	Valor verdadeiro ou falso para o julgamento sobre a validade do erro armazenado pela classe de coordenada.
 */
	void setSigmaAvailable(bool value);

	/**
 * \brief Método para informar se a coordenada possui um valor válido.
 * \return PositionMatrix	Retorna verdadeiro se existir um valor válido armazenado para a coordenada e falso em caso contrário.
 */
	bool isAvailable();

	/**
 * \brief Método para informar se uma coordenada possui um erro sigma conhecido.
 * \return bool	Retorna verdadeiro se existir um erro sigma conhecido para aquela coordenada e falso em caso contrário.
 */
	bool isSigmaAvailable();

	/**
 * \brief Método para retornar a coordenada em uma matriz coluna.
 * \return PositionMatrix	Matriz coluna com os valores de posicionamento da coordenada.
 */
	virtual PositionMatrix getPosition() = 0;

	/**
 * \brief Método para retornar a coordenada em formato GML (Geography Markup Language).
 * \return string	Coordenada em formato GML.
 */
	virtual string getGmlPos();

	/**
 * \brief Método para alterar a posição de uma coordenada.
 * \param newPosition	Matriz coluna com os valores de posicionamento da nova coordenada.
 */
	virtual void setPosition(const PositionMatrix& newPosition) = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // COORDINATE_H
