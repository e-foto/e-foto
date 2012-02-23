#ifndef DETECTORFIDUCIALMARK_H
#define DETECTORFIDUCIALMARK_H

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
 * \brief Construtor que cria uma marca fiducial analógica com seus identificadores.
 * \param myId	Identificador da marca fiducial analógica.
 * \param mySensorId	Identificador do sensor ao qual a marca pertence.
 */
	DetectorFiducialMark(int myId, int mySensorId);

	/**
 * \brief Construtor que cria uma marca fiducial analógica com todos os seus valores.
 * \param myId	Identificador da marca fiducial analógica.
 * \param mySensorId	Identificador do sensor ao qual a marca pertence.
 * \param myUnit	Texto descritor da unidade de medidas considerada para a marca.
 * \param myXi	Valor Xi.
 * \param myEta	Valor Eta.
 * \param mySigmaXi	Valor do erro na direção Xi.
 * \param mySigmaEta	Valor do erro na direção Eta.
 * \param mySigmaXiEta	Valor de erro conjunto para as direções Xi e Eta.
 */
	DetectorFiducialMark(int myId, int mySensorId, string myUnit, double myXi, double myEta, double mySigmaXi = 1, double mySigmaEta = 1, double mySigmaXiEta = 0);

	/**
 * \brief Construtor que cria uma marca fiducial analógica sem erro conhecido usando matrizes.
 * \param myId	Identificador da marca fiducial analógica.
 * \param mySensorId	Identificador do sensor ao qual a marca pertence.
 * \param myPosition	Matriz coluna com os valores de posicionamento da marca.
 */
	DetectorFiducialMark(int myId, int mySensorId, const PositionMatrix& myPosition);

	/**
 * \brief Construtor que cria uma marca fiducial analógica com todos os seus valores usando matrizes.
 * \param myId	Identificador da marca fiducial analógica.
 * \param mySensorId	Identificador do sensor ao qual a marca pertence.
 * \param myPositio	Matriz coluna com os valores de posicionamento da marca.
 * \param myPositionSigmas	Matriz sigma com os valores de erro para o posicionamento da marca.
 */
	DetectorFiducialMark(int myId, int mySensorId, const PositionMatrix& myPosition, const Matrix& myPositionSigmas);

	/**
 * \brief Destrutor padrão.
 */
	~DetectorFiducialMark();

	/**
 * \brief Método de acesso que seta o valor de id da marca fiducial analógica.
 * \param newId	Novo identificador da marca fiducial analógica.
 */
	void setId(int newId);

	/**
 * \brief Método de acesso que seta o valor de id do sensor ao qual a marca pertence.
 * \param newSensorId	Novo identificador do sensor ao qual a marca pertence.
 */
	void setSensorId(int newSensorId);

	/**
 * \brief Método de acesso que retorna o valor de id da marca fiducial analógica.
 * \return int	Identificador da marca fiducial analógica.
 */
	int getId();

	/**
 * \brief Método de acesso que retorna o valor de id do sensor ao qual a marca pertence.
 * \return int	Identificador do sensor ao qual a marca pertence.
 */
	int getSensorId();

	/**
 * \brief Método para emitir o nome de classe.
 * \return string	Retorna o nome de classe do objeto.
 */
	string objectType(void);

	/**
 * \brief Método para emitir as associações de uma instância.
 * \return string	Retorna vazio para esta classe.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 */
	string objectAssociations(void);

	/**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome passado seja DetectorFiducialMark. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe DetectorFiducialMark.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classe DetectorFiducialMark.
 */
	string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // DETECTORFIDUCIALMARK_H
