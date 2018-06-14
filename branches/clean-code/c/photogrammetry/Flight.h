#ifndef FLIGHT_H
#define FLIGHT_H
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

class Terrain;
class Sensor;
class Image;

/**
* \file Flight.h
* \class Flight
* \brief Classe que registra os atributos do voo sobre o terreno em estudo num projeto fotogramétrico.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class Flight : public EDom
{

private:

	int id;
	int sensorId;
    std::string flightId;
    std::string description;
    std::string execution;
    std::string producerName;
    std::string scale;
	double scaleDen;
	double height;
	double longitudinalOverlap;
	double transversalOverlap;

	Terrain* myTerrain;
	Sensor* mySensor;
    std::deque<Image*> myImages;


public:

	/**
 * \brief Construtor vazio.
 */
	Flight();

	/**
 * \brief Construtor padrão.
 * \param myId	Define o idenficador do voo no projeto.
 * \param mySensorId	Define o idenficador do sensor associado a este voo.
 */
	Flight(int myId, int mySensorId);

	/**
 * \brief Destrutor padrão.
 */
	virtual ~Flight();

	/**
 * \brief Método de acesso que seta a id do vôo.
 * \param newId	Novo identificador.
 */
	void setId(int newId);

        /**
         * \brief Método de acesso que seta o identificador do vôo.
         * \param nickname Novo identificador.
        */
        void setFlightId(std::string nickname);

	/**
 * \brief Método de acesso que seta a id do sensor associado ao vôo.
 * \param newSensorId	Novo identificador de sensor associado.
 */
	void setSensorId(int newSensorId);

	/**
 * \brief Método de acesso que seta a descrição do vôo.
 * \param newDescription	Nova descrição.
 */
    void setDescription(std::string newDescription);

	/**
 * \brief Método de acesso que seta a data de execução do vôo.
 * \param newExecution	Nova data de execução do vôo.
 */
    void setExecution(std::string newExecution);

	/**
 * \brief Método de acesso que seta o nome do produtor do vôo.
 * \param newProducerName	Novo nome do produtor do vôo.
 */
    void setProducerName(std::string newProducerName);

	/**
 * \brief Método de acesso que seta a escala nominal do vôo.
 * \param newScaleNova escala nominal de voo.
 */
    void setScale(std::string newScale);

	/**
 * \brief Método de acesso que seta a altura do vôo.
 * \param newHeight	Nova altura de voo.
 */
	void setHeight(double newHeight);

	/**
 * \brief Método de acesso que seta a sobreposição longitudinal.
 * \param newLongitudinalOverlap	Nova sobreposição longitudinal.
 */
	void setLongitudinalOverlap(double newLongitudinalOverlap);

	/**
 * \brief Método de acesso que seta a sobreposição transversal.
 * \param newTransversalOverlap	Nova sobreposição transversal.
 */
	void setTransversalOverlap(double newTransversalOverlap);

	/**
 * \brief Método de acesso que retorna a id do vôo.
 * \return int	Identificador do voo.
 */
	int getId();

        /**
        * \brief Método de acesso que retorna o identificador do vôo.
        * \return std::string Identificador do voo.
        */

        std::string getFlightId();

	/**
 * \brief Método de acesso que retorna a id do sensor associado ao vôo.
 * \return int	Identificador do sensor associado ao vôo.
 */
	int getSensorId();

	/**
 * \brief Método de acesso que retorna a descrição do vôo.
 * \return std::string	Descrição do vôo.
 */
    std::string getDescription();

	/**
 * \brief Método de acesso que retorna a data de execução do vôo.
 * \return std::string	Data de execução do vôo.
 */
    std::string getExecution();

	/**
 * \brief Método de acesso que retorna o nome do produtor do vôo.
 * \return std::string	Nome do produtor do vôo.
 */
    std::string getProducerName();

	/**
 * \brief Método de acesso que retorna a escala nominal do vôo.
 * \return std::string	Escala nominal do vôo.
 */
    std::string getScale();

	/**
 * \brief Método de acesso que retorna o denominador da escala de vôo.
 * \return double	Denominador da escala de vôo.
 */
	double getScaleDen();

	/**
 * \brief Método de acesso que retorna a altura do vôo.
 * \return double	Altura do vôo.
 */
	double getHeight();

	/**
 * \brief Método de acesso que retorna a sobreposição longitudinal.
 * \return double	Sobreposição longitudinal.
 */
	double getLongitudinalOverlap();

	/**
 * \brief Método de acesso que retorna a sobreposição transversal.
 * \return double	Sobreposição transversal.
 */
	double getTransversalOverlap();

	/**
 * \brief Método de acesso que seta o ponteiro para a instância da classe Terrain associada ao voo.
 * \param newTerrain	Ponteiro para o terreno.
 */
	void setTerrain(Terrain* newTerrain);

	/**
 * \brief Método de acesso que retorna o ponteiro para a instância da classe Terrain associada ao voo.
 * \return Terrain*	Ponteiro para o terreno.
 */
	Terrain* getTerrain();

	/**
 * \brief Método de acesso que seta o ponteiro para a instância da classe Sensor associada ao voo.
 * \param newSensor	Ponteiro para o sensor.
 */
	void setSensor(Sensor* newSensor);

	/**
 * \brief Método de acesso que retorna o ponteiro para a instância da classe Sensor associada ao voo.
 * \return Sensor*	Ponteiro para o sensor.
 */
	Sensor* getSensor();

	/**
 * \brief Método que associa uma imagem ao voo.
 * \param newImageAssociation	Ponteiro para a imagem.
 */
    void putImage(Image* newImageAssociation);

    void clearImages() {myImages.clear();}

	/**
 * \brief Método de acesso que retorna o ponteiro para a instância da classe Image associada ao voo.
 * \param ImageId	Identificador da imagem no projeto.
 * \return Image*	Ponteiro para a imagem.
 */
	Image* getImage(int imageId);

	/**
 * \brief Método de acesso que retorna o número de instâncias da classe Image associadas ao voo.
 * \return int	Número de imagens associadas ao voo.
 */
	int countImages();

	/**
 * \brief Método de acesso que retorna o ponteiro para a instância da classe Image associada ao voo.
 * \param Index	Índice da associação na lista de associações conhecidas pelo voo.
 * \return Image*	Ponteiro para a imagem.
 */
	Image* getImageAt(unsigned int index);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	std::string contendo o xml com todos os valores de atributos adequados a uma instância da classe Flight.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o std::string contendo o xml para uma instância da classe Flight.
 */
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //FLIGHT_H
