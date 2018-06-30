#ifndef TERRAIN_H
#define TERRAIN_H
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
#include "EDom.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file Terrain.h
* \class Terrain
* \brief Classe que registra os atributos do terreno em estudo num projeto fotogramétrico e o sistema de coordenadas adotado.
* \todo Após iniciarmos o uso da classe GeoSystem poderá surgir uma classe denominada System para registrar o sistema de cada grupo de coordenadas que entre no projeto. Deste modo o projeto poderá ser feito com sistemas de coordenadas variados.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class Terrain : public EDom
{

private:
    double meanAltitude{0.0};
    std::string meanAltitudeUnit{""};
    double minAltitude{0.0};
    std::string minAltitudeUnit{""};
    double maxAltitude{0.0};
    std::string maxAltitudeUnit{""};
    std::string GRS{""};
    std::string CPS{""};
    std::string workAreaCentralCoordinateLat{""};
    std::string workAreaCentralCoordinateLong{""};
    int utmFuse{0};

public:
    Terrain(){};
	virtual ~Terrain();

	/**
 * \brief Método que retorna a altura média do Terreno.
 * \return double	Retorna a altura média do Terreno.
 */
    double getMeanAltitude() const;

	/**
 * \brief Método que retorna a menor altura do Terreno.
 * \return double	Retorna a menor altura do Terreno.
 */
    double getMinAltitude() const;

	/**
 * \brief Método que retorna a maior altura do Terreno.
 * \return double	Retorna a maior altura do Terreno.
 */
    double getMaxAltitude() const;

/**
 * \brief Método que retorna a GRS do Terreno.
 * \return std::string	Retorna a GRS do Terreno.
 */
    std::string getGRS() const;

	/**
 * \brief Método que retorna a CPS do Terreno.
 * \return std::string	Retorna a CPS do Terreno.
 */
    std::string getCPS() const;

	/**
 * \brief Método que retorna a latitude da coodernada central do Terreno.
 * \return std::string	A latitude da coodernada central do Terreno.
 */
    std::string getCentralCoordLat() const;

	/**
 * \brief Método que retorna a longitude da coodernada central do Terreno.
 * \return std::string	A longitude da coodernada central do Terreno.
 */
    std::string getCentralCoordLong() const;

	/**
 * \brief Método que retorna a qual fuso UTM o Terreno pertence.
 * \return int	O fuso UTM do terreno.
 */
    int getUtmFuse() const;

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Terrain.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o string contendo o xml para uma instância da classe Terrain.
 */
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // TERRAIN_H
