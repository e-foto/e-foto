#ifndef TERRAIN_H
#define TERRAIN_H

#include "EObject.h"

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
class Terrain : public EObject
{

private:
	double meanAltitude;
	string meanAltitudeUnit;
	double minAltitude;
	string minAltitudeUnit;
	double maxAltitude;
	string maxAltitudeUnit;
	string GRS;
	string CPS;
	string workAreaCentralCoordinateLat;
	string workAreaCentralCoordinateLong;
	int utmFuse;

public:

	/**
 * \brief Destrutor padrão.
 */
	virtual ~Terrain();

	/**
 * \brief Método que seta a altura média do Terreno.
 * \param newMeanAltitude	Nova altura média.
 */
	void setMeanAltitude(double newMeanAltitude);

	/**
 * \brief Método que seta a unidade da altura média do Terreno.
 * \param  newMeanAltitudeUnit	Nova unidade.
 */
	void setMeanAltitudeUnit(string newMeanAltitudeUnit);

	/**
 * \brief Método que seta a menor altura do Terreno.
 * \param newMinAltitude	Nova altura mínima.
 */
	void setMinAltitude(double newMinAltitude);

	/**
 * \brief Método que seta a unidade da menor altura do Terreno.
 * \param newMinAltitudeUnit	Nova unidade.
 */
	void setMinAltitudeUnit(string newMinAltitudeUnit);

	/**
 * \brief Método que seta a maior altura do Terreno.
 * \param newMaxAltitude	Nova altura máxima.
 */
	void setMaxAltitude(double newMaxAltitude);

	/**
 * \brief Método que seta a unidade da maior altura do Terreno.
 * \param newMaxAltitudeUnit	Nova unidade.
 */
	void setMaxAltitudeUnit(string newMaxAltitudeUnit);

	/**
 * \brief Método que seta a Geodesic Reference System (GRS) do Terreno.
 * \param newGRS	Nova GRS.
 */
	void setGRS(string newGRS);

	/**
 * \brief Método que seta a Cartographic Projection System (CPS) do Terreno.
 * \param newCPS	Nova CPS.
 */
	void setCPS(string newCPS);

	/**
 * \brief Método que seta a latitude da coodernada central do Terreno.
 * \param newCentralCoordLat	Nova latitude descrita e texto equivalente a "graus minutos segundos direção".
 */
	void setCentralCoordLat(string newCentralCoordLat);

	/**
 * \brief Método que seta a longitude da coodernada central do Terreno.
 * \param newCentralCoordLong	Nova longitude descrita e texto equivalente a "graus minutos segundos direção".
 */
	void setCentralCoordLong(string newCentralCoordLong);

	/**
 * \brief Método que seta em qual fuso do terreno no sistema de coordenadas Universal Transversa de Mercator (UTM).
 * \param newUtmFuse	Novo fuso.
 */
	void setUtmFuse(int newUtmFuse);

	/**
 * \brief Método que retorna a altura média do Terreno.
 * \return double	Retorna a altura média do Terreno.
 */
	double getMeanAltitude();

	/**
 * \brief Método que retorna a unidade da altura média do Terreno.
 * \return string	A unidade da altura média do Terreno.
 */
	string getMeanAltitudeUnit();

	/**
 * \brief Método que retorna a menor altura do Terreno.
 * \return double	Retorna a menor altura do Terreno.
 */
	double getMinAltitude();

	/**
 * \brief Método que retorna a unidade da menor altura do Terreno.
 * \return string	A unidade da menor altura do Terreno.
 */
	string getMinAltitudeUnit();

	/**
 * \brief Método que retorna a maior altura do Terreno.
 * \return double	Retorna a maior altura do Terreno.
 */
	double getMaxAltitude();

	/**
 * \brief Método que retorna a unidade da maior altura do Terreno.
 * \return string	A unidade da maior altura do Terreno.
 */
	string getMaxAltitudeUnit();

	/**
 * \brief Método que retorna a GRS do Terreno.
 * \return string	Retorna a GRS do Terreno.
 */
	string getGRS();

	/**
 * \brief Método que retorna a CPS do Terreno.
 * \return string	Retorna a CPS do Terreno.
 */
	string getCPS();

	/**
 * \brief Método que retorna a latitude da coodernada central do Terreno.
 * \return string	A latitude da coodernada central do Terreno.
 */
	string getCentralCoordLat();

	/**
 * \brief Método que retorna a longitude da coodernada central do Terreno.
 * \return string	A longitude da coodernada central do Terreno.
 */
	string getCentralCoordLong();

	/**
 * \brief Método que retorna a qual fuso UTM o Terreno pertence.
 * \return int	O fuso UTM do terreno.
 */
	int getUtmFuse();

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
 * \return bool	Retorna verdadeiro caso o nome passado seja Terrain. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Terrain.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classe Terrain.
 */
	string xmlGetData();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // TERRAIN_H
