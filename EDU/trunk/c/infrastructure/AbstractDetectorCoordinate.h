#ifndef ABSTRACTDETECTORCOORDINATE_H
#define ABSTRACTDETECTORCOORDINATE_H

#include "Coordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file AbstractDetectorCoordinate.h
* \class AbstractDetectorCoordinate
* \brief Base para as classes de coordenadas analógicas 2D
* \abstract A classe de Coordenadas Analógicas 2D foi definida como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class AbstractDetectorCoordinate : public Coordinate
{

protected:
	double xi;
	double eta;
	double sigmaXi;
	double sigmaEta;
	double sigmaXiEta;
	string unit;

public:

	/**
 * \brief Destrutor virtual padrão.
 */
	virtual ~AbstractDetectorCoordinate();

	/**
 * \brief Método para alterar o valor Xi num sistema de coordenadas (Xi, Eta).
 * \param newXi	Novo valor Xi.
 */
	void setXi(double newXi);

	/**
 * \brief Método para alterar o valor Eta num sistema de coordenadas (Xi, Eta).
 * \param newEta	Novo valor Eta.
 */
	void setEta(double newEta);

	/**
 * \brief Método para alterar o valor de erro (variância) na direção Xi.
 * \param newSigmaXi	Novo valor erro para Xi.
 */
	void setSigmaXi(double newSigmaXi);

	/**
 * \brief Método para alterar o valor de erro (variância) na direção Eta.
 * \param newSigmaEta	Novo valor erro para Eta.
 */
	void setSigmaEta(double newSigmaEta);

	/**
 * \brief Método para alterar o valor de erro (covariância) conjunto nas direções Xi e Eta.
 * \param newSigmaXiEta	Novo valor erro para Xi e Eta.
 */
	void setSigmaXiEta(double newSigmaXiEta);

	/**
 * \brief Método para alterar a unidade em que a coordenada é considerada.
 * \param newUnit	Texto descritor da nova unidade de medidas considerada para a coordenada.
 */
	void setUnit(string newUnit);

	/**
 * \brief Método para retornar o valor Xi num sistema de coordenadas (Xi, Eta).
 * \return double	Valor Xi.
 */
	double getXi();

	/**
 * \brief Método para retornar o valor Eta num sistema de coordenadas (Xi, Eta).
 * \return double	Valor Eta.
 */
	double getEta();

	/**
 * \brief Método para retornar o valor de erro na direção Xi.
 * \return double	Valor do erro na direção Xi.
 */
	double getSigmaXi();

	/**
 * \brief Método para retornar o valor de erro na direção Eta.
 * \return double	Valor do erro na direção Eta.
 */
	double getSigmaEta();

	/**
 * \brief Método para retornar o valor de erro conjunto para as direções Xi e Eta.
 * \return double	Valor de erro conjunto para as direções Xi e Eta.
 */
	double getSigmaXiEta();

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
 * \return string	Retorna o nome de classe do objeto.
 */
	string objectType(void);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // ABSTRACTDETECTORCOORDINATE_H
