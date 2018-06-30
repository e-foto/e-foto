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
class Flight : public EDom {

private:

    int id_;
    int sensorId_;
    std::string flightId;
    std::string description;
    std::string execution;
    std::string producerName;
    std::string scale;
    double scaleDen{0.0};
    double height{0.0};
    double longitudinalOverlap{0.0};
    double transversalOverlap{0.0};

    Terrain* myTerrain{nullptr};
    Sensor* mySensor{nullptr};
    std::deque<Image*> myImages{{}};


public:

    /**
    * \brief Construtor vazio.
    */
    Flight();

    /**
    * \brief Construtor padrão.
    * \param myId   Define o idenficador do voo no projeto.
    * \param mySensorId Define o idenficador do sensor associado a este voo.
    */
    Flight(int myId, int mySensorId);

    /**
    * \brief Destrutor padrão.
    */
    virtual ~Flight();

    /**
    * \brief Método de acesso que retorna a id do vôo.
    * \return int  Identificador do voo.
    */
    int getId() const;

    /**
    * \brief Método de acesso que retorna o identificador do vôo.
    * \return std::string Identificador do voo.
    */

    std::string getFlightId() const;

    /**
    * \brief Método de acesso que retorna a id do sensor associado ao vôo.
    * \return int   Identificador do sensor associado ao vôo.
    */
    int getSensorId() const;

    /**
    * \brief Método de acesso que retorna a data de execução do vôo.
    * \return std::string   Data de execução do vôo.
    */
    std::string getExecution() const;

    /**
    * \brief Método de acesso que retorna a escala nominal do vôo.
    * \return std::string   Escala nominal do vôo.
    */
    std::string getScale() const;

    /**
    * \brief Método de acesso que retorna o denominador da escala de vôo.
    * \return double    Denominador da escala de vôo.
    */
    double getScaleDen() const;

    /**
    * \brief Método de acesso que retorna a altura do vôo.
    * \return double    Altura do vôo.
    */
    double getHeight() const;

    /**
    * \brief Método de acesso que retorna a sobreposição longitudinal.
    * \return double    Sobreposição longitudinal.
    */
    double getLongitudinalOverlap() const;

    /**
    * \brief Método de acesso que retorna a sobreposição transversal.
    * \return double    Sobreposição transversal.
    */
    double getTransversalOverlap() const;

    /**
    * \brief Método de acesso que seta o ponteiro para a instância da classe Terrain associada ao voo.
    * \param newTerrain Ponteiro para o terreno.
    */
    void setTerrain(Terrain* newTerrain);

    /**
    * \brief Método de acesso que retorna o ponteiro para a instância da classe Terrain associada ao voo.
    * \return Terrain*  Ponteiro para o terreno.
    */
    Terrain* getTerrain();

    /**
    * \brief Método de acesso que seta o ponteiro para a instância da classe Sensor associada ao voo.
    * \param newSensor  Ponteiro para o sensor.
    */
    void setSensor(Sensor* newSensor);

    /**
    * \brief Método que associa uma imagem ao voo.
    * \param newImageAssociation    Ponteiro para a imagem.
    */
    void putImage(Image* newImageAssociation);

    void clearImages()
    {
        myImages.clear();
    }

    /**
    * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
    * \param xml    std::string contendo o xml com todos os valores de atributos adequados a uma instância da classe Flight.
    */
    void xmlSetData(std::string xml);

    /**
    * \brief Método para extrair o equivalente em dados xml de uma instância.
    * \return std::string   Retorna o std::string contendo o xml para uma instância da classe Flight.
    */
    std::string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //FLIGHT_H
