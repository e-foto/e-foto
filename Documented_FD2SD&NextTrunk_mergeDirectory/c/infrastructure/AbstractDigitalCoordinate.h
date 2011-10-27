#ifndef ABSTRACTDIGITALCOORDINATE_H
#define ABSTRACTDIGITALCOORDINATE_H

#include "Coordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file AbstracDigitalCoordinate.h
* \class AbstractDigitalCoordinate
* \brief Base para as classes de coordenadas digitais 2D
* \abstract A classe de Coordenadas Digitais 2D foi definida como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class AbstractDigitalCoordinate : public Coordinate
{

protected:
	int lin;
	int col;
	string unit;

public:

	/**
	* \brief Destrutor virtual padrão.
	*/
	virtual ~AbstractDigitalCoordinate();

	/**
	* \brief Método para alterar o valor Linha num sistema de coordenadas (Coluna, Linha).
	* \param newLin	Novo valor de linha.
	*/
	void setLin(int newLin);

	/**
	* \brief Método para alterar o valor Coluna num sistema de coordenadas (Coluna, Linha).
	* \param newCol	Novo valor de coluna.
	*/
	void setCol(int newCol);

	/**
	* \brief Método para alterar a unidade em que a coordenada é considerada.
	* \param newUnit	Texto descritor da nova unidade de medidas considerada para a coordenada.
	*/
	void setUnit(string newUnit);

	/**
	* \brief Método para retornar o valor de Linha num sistema de coordenadas (Coluna, Linha).
	* \return double	Valor da linha.
	*/
	int getLin();

	/**
	* \brief Método para retornar o valor de Coluna num sistema de coordenadas (Coluna, Linha).
	* \return double	Valor da coluna.
	*/
	int getCol();

	/**
	* \brief Método para retornar a unidade em que a coordenada é considerada.
	* \return string	Texto descritor da unidade de medidas considerada para a coordenada.
	*/
	string getUnit();

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

	/**
	* \brief Método para emitir o nome de classe.
	* \return string	Retorna o nome de classe do objeto.
	*/
	string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //ABSTRACTDIGITALCOORDINATE_H
