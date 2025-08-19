#ifndef OBJECTSPACECOORDINATE_H
#define OBJECTSPACECOORDINATE_H
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

#include "Coordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file ObjectSpaceCoordinate.h
* \class ObjectSpaceCoordinate
* \brief Classes de coordenada sobre o espaço objecto (espaço contínuo 3D).
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ObjectSpaceCoordinate : public Coordinate
{

private:
	double x;
	double y;
	double z;
	double sigmaX;
	double sigmaY;
	double sigmaZ;
	double sigmaXY;
	double sigmaXZ;
	double sigmaYZ;
    std::string unit;

public:

	/**
 * \brief Construtor vazio.
 */
	ObjectSpaceCoordinate();

	/**
 * \brief Construtor que cria uma coordenada de objeto com todos os seus valores.
 * \param myUnit	Texto descritor da unidade de medidas considerada para a coordenada.
 * \param myX	Valor X.
 * \param myY	Valor Y.
 * \param myZ	Valor Z.
 * \param mySigmaX	Valor do erro na direção X.
 * \param mySigmaY	Valor do erro na direção Y.
 * \param mySigmaZ	Valor do erro na direção Z.
 * \param mySigmaXY	Valor de erro conjunto para as direções X e Y.
 * \param mySigmaXZ	Valor de erro conjunto para as direções X e Z.
 * \param mySigmaYZ	Valor de erro conjunto para as direções Y e Z.
 */
    explicit ObjectSpaceCoordinate(std::string myUnit, double myX, double myY, double myZ, double mySigmaX = 1, double mySigmaY = 1, double mySigmaZ = 1, double mySigmaXY = 0, double mySigmaXZ = 0, double mySigmaYZ = 0);

	/**
 * \brief Construtor que cria uma coordenada de objeto sem erro conhecido usando matrizes.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 */
    explicit ObjectSpaceCoordinate(const PositionMatrix& myPosition);

	/**
 * \brief Construtor que cria uma coordenada de objeto com todos os seus valores usando matrizes.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 * \param myPositionSigmas	Matriz sigma com os valores de erro para o posicionamento da coordenada.
 */
    explicit ObjectSpaceCoordinate(const PositionMatrix& myPosition, const Matrix& myPositionSigmas);

	/**
 * \brief Método para alterar o valor X num sistema de coordenadas (X, Y, Z).
 * \param newX	Novo valor X.
 */
	void setX(double newX);

	/**
 * \brief Método para alterar o valor Y num sistema de coordenadas (X, Y, Z).
 * \param newY	Novo valor Y.
 */
	void setY(double newY);

	/**
 * \brief Método para alterar o valor Z num sistema de coordenadas (X, Y, Z).
 * \param newZ	Novo valor Z.
 */
	void setZ(double newZ);

	/**
 * \brief Método para alterar o valor de erro (variância) na direção X.
 * \param newSigmaX	Novo valor erro para X.
 */
	void setSigmaX(double newSigmaX);

	/**
 * \brief Método para alterar o valor de erro (variância) na direção Y.
 * \param newSigmaY	Novo valor erro para Y.
 */
	void setSigmaY(double newSigmaY);

	/**
 * \brief Método para alterar o valor de erro (variância) na direção Z.
 * \param newSimgaZ	Novo valor erro para Z.
 */
	void setSigmaZ(double newSigmaZ);

	/**
 * \brief Método para alterar o valor de erro (covariância) conjunto nas direções X e Y.
 * \param newSigmaXY	Novo valor erro para X e Y.
 */
	void setSigmaXY(double newSigmaXY);

	/**
 * \brief Método para alterar o valor de erro (covariância) conjunto nas direções X e Z.
 * \param newSigmaXZ	Novo valor erro para X e Z.
 */
	void setSigmaXZ(double newSigmaXZ);

	/**
 * \brief Método para alterar o valor de erro (covariância) conjunto nas direções Y e Z.
 * \param newSigmaYZ	Novo valor erro para Y e Z.
 */
	void setSigmaYZ(double newSigmaYZ);

	/**
 * \brief Método para alterar a unidade em que a coordenada é considerada.
 * \param newUnit	Texto descritor da nova unidade de medidas considerada para a coordenada.
 */
    void setUnit(std::string newUnit);

	/**
 * \brief Método para retornar o valor X num sistema de coordenadas (X, Y, Z).
 * \return double	Valor X.
 */
	double getX();

	/**
 * \brief Método para retornar o valor Y num sistema de coordenadas (X, Y, Z).
 * \return double	Valor Y.
 */
	double getY();

	/**
 * \brief Método para retornar o valor Z num sistema de coordenadas (X, Y, Z).
 * \return double	Valor Z.
 */
	double getZ();

	/**
 * \brief Método para retornar o valor de erro na direção X.
 * \return double	Valor do erro na direção X.
 */
	double getSigmaX();

	/**
 * \brief Método para retornar o valor de erro na direção Y.
 * \return double	Valor do erro na direção Y.
 */
	double getSigmaY();

	/**
 * \brief Método para retornar o valor de erro na direção Z.
 * \return double	Valor do erro na direção Z.
 */
	double getSigmaZ();

	/**
 * \brief Método para retornar o valor de erro conjunto para as direções X e Y.
 * \return double	Valor de erro conjunto para as direções X e Y.
 */
	double getSigmaXY();

	/**
 * \brief Método para retornar o valor de erro conjunto para as direções X e Z.
 * \return double	Valor de erro conjunto para as direções X e Z.
 */
	double getSigmaXZ();

	/**
 * \brief Método para retornar o valor de erro conjunto para as direções Y e Z.
 * \return double	Valor de erro conjunto para as direções Y e Z.
 */
	double getSigmaYZ();

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
 * \brief Método para alterar o erro (sigma) conhecido para a posição de uma coordenada.
 * \param newPositionSigmas	Matriz com os novos valores de erro para o posicionamento da coordenada.
 */
	void setPositionSigmas(const Matrix& newPositionSigmas);

	/**
 * \brief Método para retornar a coordenada em uma matriz coluna.
 * \return PositionMatrix	Matriz coluna com os valores de posicionamento da coordenada.
 */
	PositionMatrix getPosition();

	/**
 * \brief Método para retornar o erro (sigma) conhecido para a coordenada em uma matriz.
 * \return Matrix	Matriz sigma com os valores de erro para o posicionamento da coordenada.
 */
	Matrix getPositionSigmas();

	/**
 * \brief Método para emitir o nome de classe.
 * \return std::string	Retorna o nome de classe do objeto.
 */
    std::string objectType(void);

	/**
 * \brief Método para emitir as associações de uma instância.
 * \return std::string	Retorna vazio para esta classe.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 */
    std::string objectAssociations(void);

	/**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome passado seja ObjectSpaceCoordinate. Retorna falso no caso contrário.
 */
    bool is(std::string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	std::string contendo o xml com todos os valores de atributos adequados a uma instância da classe ObjectSpaceCoordinate.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o string contendo o xml para uma instância da classe ObjectSpaceCoordinate.
 */
    std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // OBJECTSPACECOORDINATE_H
