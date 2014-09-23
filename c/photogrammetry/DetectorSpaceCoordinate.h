#ifndef DETECTORSPACECOORDINATE_H
#define DETECTORSPACECOORDINATE_H
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

#include "AbstractDetectorCoordinate.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file DetectorSpaceCoordinate.h
* \class DetectorSpaceCoordinate
* \brief Classes de coordenada analógica sobre o espaço imagem (espaço discreto 2D). Usada quando transformamos uma coordenada digital de imagem (por transformação afim) para o plano do sensor onde ela foi obtida.
* \attention Atenção, pois essa classe foi construída para representar as coordenadas da matriz da imagem digital transformada sobre o plano de obtenção da imagem no sensor.
* \attention Não use esta classe como se ela fosse uma classe para subpixels.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class DetectorSpaceCoordinate : public AbstractDetectorCoordinate
{

private:
	int imageId;

public:

	/**
 * \brief Construtor vazio.
 */
	DetectorSpaceCoordinate();

	/**
 * \brief Construtor que cria uma coordenada analógica identificada sobre o espaço imagem.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 */
	DetectorSpaceCoordinate(int myImageId);

	/**
 * \brief Construtor que cria uma coordenada analógica identificada sobre o espaço imagem com todos os seus valores.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myUnit	Texto descritor da unidade de medidas considerada para a coordenada.
 * \param myXi	Valor Xi.
 * \param myEta	Valor Eta.
 * \param mySigmaXi	Valor do erro na direção Xi.
 * \param mySigmaEta	Valor do erro na direção Eta.
 * \param mySigmaXiEta	Valor de erro conjunto para as direções Xi e Eta.
 */
    DetectorSpaceCoordinate(int myImageId, std::string myUnit, double myXi, double myEta, double mySigmaXi = 1, double mySigmaEta = 1, double mySigmaXiEta = 0);

	/**
 * \brief Construtor que cria uma coordenada analógica identificada sobre o espaço imagem sem erro conhecido usando matrizes.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 */
	DetectorSpaceCoordinate(int myImageId, const PositionMatrix& myPosition);

	/**
 * \brief Construtor que cria uma coordenada analógica identificada sobre o espaço imagem com todos os seus valores usando matrizes.
 * \param myImageId	Identificador da imagem à qual a coordenada pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da coordenada.
 * \param myPositionSigmas	Matriz sigma com os valores de erro para o posicionamento da coordenada.
 */
	DetectorSpaceCoordinate(int myImageId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas);

	/**
 * \brief Destrutor padrão.
 */
	~DetectorSpaceCoordinate();

	/**
 * \brief Método de acesso que seta o valor de id da imagem à qual a coordenada pertence.
 * \param newSensorId	Novo identificador da imagem à qual a coordenada pertence.
 */
	void setImageId(int newImageId);

	/**
 * \brief Método de acesso que retorna o valor de id da imagem à qual a coordenada pertence.
 * \return int	int	Identificador da imagem à qual a coordenada pertence.
 */
	int getImageId();

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
 * \return bool	Retorna verdadeiro caso o nome passado seja DetectorSpaceCoordinate. Retorna falso no caso contrário.
 */
    bool is(std::string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	std::string contendo o xml com todos os valores de atributos adequados a uma instância da classe DetectorSpaceCoordinate.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o string contendo o xml para uma instância da classe DetectorSpaceCoordinate.
 */
    std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DETECTORSPACECOORDINATE_H
