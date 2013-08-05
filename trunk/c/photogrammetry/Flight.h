#ifndef FLIGHT_H
#define FLIGHT_H

#include "EObject.h"

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
class Flight : public EObject
{

private:

	int id;
	int sensorId;
	string flightId;
	string description;
	string execution;
	string producerName;
	string scale;
	double scaleDen;
	double height;
	double longitudinalOverlap;
	double transversalOverlap;

	Terrain* myTerrain;
	Sensor* mySensor;
	deque<Image*> myImages;


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
        void setFlightId(string nickname);

	/**
 * \brief Método de acesso que seta a id do sensor associado ao vôo.
 * \param newSensorId	Novo identificador de sensor associado.
 */
	void setSensorId(int newSensorId);

	/**
 * \brief Método de acesso que seta a descrição do vôo.
 * \param newDescription	Nova descrição.
 */
	void setDescription(string newDescription);

	/**
 * \brief Método de acesso que seta a data de execução do vôo.
 * \param newExecution	Nova data de execução do vôo.
 */
	void setExecution(string newExecution);

	/**
 * \brief Método de acesso que seta o nome do produtor do vôo.
 * \param newProducerName	Novo nome do produtor do vôo.
 */
	void setProducerName(string newProducerName);

	/**
 * \brief Método de acesso que seta a escala nominal do vôo.
 * \param newScaleNova escala nominal de voo.
 */
	void setScale(string newScale);

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
        * \return string Identificador do voo.
        */

        string getFlightId();

	/**
 * \brief Método de acesso que retorna a id do sensor associado ao vôo.
 * \return int	Identificador do sensor associado ao vôo.
 */
	int getSensorId();

	/**
 * \brief Método de acesso que retorna a descrição do vôo.
 * \return string	Descrição do vôo.
 */
	string getDescription();

	/**
 * \brief Método de acesso que retorna a data de execução do vôo.
 * \return string	Data de execução do vôo.
 */
	string getExecution();

	/**
 * \brief Método de acesso que retorna o nome do produtor do vôo.
 * \return string	Nome do produtor do vôo.
 */
	string getProducerName();

	/**
 * \brief Método de acesso que retorna a escala nominal do vôo.
 * \return string	Escala nominal do vôo.
 */
	string getScale();

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
 * \brief Método para emitir o nome de classe.
 * \return string	Retorna o nome de classe do objeto.
 */
	string objectType(void);

	/**
 * \brief Método para emitir as associações de uma instância.
 * \return string	Retorna Sensor e o id deste sensor no projeto se ele estiver conectado a um sensor.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 */
	string objectAssociations(void);

	/**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome passado seja Flight. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Flight.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classe Flight.
 */
	string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //FLIGHT_H
