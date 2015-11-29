#ifndef IMAGEFIDUCIALMARK_H
#define IMAGEFIDUCIALMARK_H
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
* \file ImageFiducialMark.h
* \class ImageFiducialMark
* \brief Classes de marca fiducial digital. É a referência impressa sobre uma imagem por um sensor métrico.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ImageFiducialMark : public AbstractImageCoordinate
{

private:
	int id;
	int imageId;

public:

	/**
 * \brief Construtor vazio.
 */
	ImageFiducialMark();

	/**
 * \brief Construtor que cria uma marca fiducial digital com seus identificadores.
 * \param myId	Identificador da marca fiducial digital.
 * \param myImageId	Identificador da imagem à qual a marca pertence.
 */
	ImageFiducialMark(int myId, int myImageId);

	/**
 * \brief Construtor que cria uma marca fiducial digital com todos os seus valores.
 * \param myId	Identificador da marca fiducial digital.
 * \param myImageId	Identificador da imagem à qual a marca pertence.
 * \param myCol	Valor da coluna.
 * \param myLin	Valor da linha.
 */
	ImageFiducialMark(int myId, int myImageId, double myCol, double myLin);

	/**
 * \brief Construtor que cria uma marca fiducial digital com todos os seus valores usando matrizes.
 * \param myId	Identificador da marca fiducial digital.
 * \param myImageId	Identificador da imagem à qual a marca pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da marca.
 */
	ImageFiducialMark(int myId, int myImageId, const PositionMatrix& myPosition);

	/**
 * \brief Destrutor padrão.
 */
	~ImageFiducialMark();

	/**
 * \brief Método de acesso que seta o valor de id da marca fiducial digital.
 * \param newId	Novo identificador da marca fiducial digital.
 */
	void setId(int newId);

	/**
 * \brief Método de acesso que seta o valor de id da imagem à qual a marca pertence.
 * \param newSensorId	Novo identificador da imagem à qual a marca pertence.
 */
	void setImageId(int newImageId);

	/**
 * \brief Método de acesso que retorna o valor de id da marca fiducial digital.
 * \return int	Identificador da marca fiducial digital.
 */
	int getId();

	/**
 * \brief Método de acesso que retorna o valor de id da imagem à qual a marca pertence.
 * \return int	int	Identificador da imagem à qual a marca pertence.
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
 * \return bool	Retorna verdadeiro caso o nome passado seja ImageFiducialMark. Retorna falso no caso contrário.
 */
    bool is(std::string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe ImageFiducialMark.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o string contendo o xml para uma instância da classe ImageFiducialMark.
 */
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGEFIDUCIALMARK_H
