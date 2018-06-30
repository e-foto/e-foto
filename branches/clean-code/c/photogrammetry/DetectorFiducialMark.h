#ifndef DETECTORFIDUCIALMARK_H
#define DETECTORFIDUCIALMARK_H
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
* \file DetectorFiducialMark.h
* \class DetectorFiducialMark
* \brief Classes de marca fiducial analógica. É uma referência do sensor métrico com posicionamento determinado em laboratório de calibração.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class DetectorFiducialMark : public AbstractDetectorCoordinate
{

private:
	int id;
	int sensorId;


public:

	/**
 * \brief Construtor vazio.
 */
	DetectorFiducialMark();

	/**
 * \brief Destrutor padrão.
 */
	~DetectorFiducialMark();

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe DetectorFiducialMark.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o string contendo o xml para uma instância da classe DetectorFiducialMark.
 */
    std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DETECTORFIDUCIALMARK_H
