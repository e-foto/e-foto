#ifndef PROJECT_H
#define PROJECT_H

#include "EObject.h"
#include "Terrain.h"
#include "ProjectHeader.h"
#include "SensorWithFiducialMarks.h"
#include "SensorWithKnowDimensions.h"
#include "SensorWithKnowParameters.h"
#include "Flight.h"
#include "Image.h"
#include "Point.h"
#include "InteriorOrientation.h"
#include "SpatialRessection.h"
#include "Dummies.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Project
{

	string xmlData;
	string xmlState;

	ProjectHeader* theHeader;
	Terrain* theTerrain;
	deque<Sensor*> sensors;
	deque<Flight*> flights;
        deque<Image*> images;
        deque<Point*> points;
        deque<DEM*> DEMs;
        deque<OrthoImage*> orthos;
        deque<featuresFile*> feats;
        string processStates;
	deque<InteriorOrientation*> IOs;
	deque<ExteriorOrientation*> EOs;

protected:

	ProjectHeader* instanceHeader();

	/**
	* \brief Método que cria uma instância da classe Terrain.
	* \return Terrain Retorna a instância criada.
	*/
	Terrain* instanceTerrain();

	/**
	* \brief Método que cria uma instância da classe Sensor.
	* \param id Identificador do Sensor.
	* \return Sensor Retorna a instância criada.
	*/
	Sensor* instanceSensor(int id);

	/**
	* \brief Método que cria uma instância da classe Flight.
	* \param id Identificador do Vôo.
	* \return Flight Retorna a instância criada.
	*/
	Flight* instanceFlight(int id);

	/**
	* \brief Método que cria uma instância da classe Image.
	* \param id Identificador da Imagem.
	* \return Image Retorna a instância criada.
	*/
	Image* instanceImage(int id);


	/**
	* \brief Método que cria instâncias de classes para todos os pontos existentes do projeto.
	*/
	void instanceAllPoints();

	/**
	* \brief Método que cria instâncias de classes para todas as orientações interiores existentes do projeto.
	*/
	void instanceAllIOs();

	/**
	* \brief Método que cria instâncias de classes para todas as orientações exteriores existentes do projeto.
	*/
	void instanceAllEOs();

	/**
	* \brief Método que cria instâncias de classes para todas as imagens existentes do projeto.
	*/
	void instanceAllImages();

	/**
	* \brief Método que cria uma instância da classe Point.
	* \param id Identificador do Ponto.
	* \return Point Retorna a instância criada.
	*/
	Point* instancePoint(int id);

	/**
	* \brief Método que cria uma instância da classe InteriorOrientation.
	* \param imageId Identificador da imagem a ser utilizada na Orientação Interior.
	* \return InteriorOrientation Retorna a instância criada.
	*/
	InteriorOrientation* instanceIO(int imageId);

	/**
	* \brief Método que cria uma instância da classe ExteriorOrientation.
	* \param imageId Identificador da imagem a ser utilizada na Orientação Exterior.
	* \return ExteriorOrientation Retorna a instância criada.
	*/
	ExteriorOrientation* instanceEO(int imageId);

	/**
	* \brief Método que apaga uma instância da classe ProjectHeader.
	*/
	void deleteHeader(bool makeReconnections = true);

	/**
	* \brief Método que apaga uma instância da classe Terrain.
	*/
	void deleteTerrain(bool makeReconnections = true);

	void linkAll();

public:

	/**
	* \brief Construtor padrão.
	*/
	Project();

	/**
	* \brief Destrutor padrão.
	*/
	~Project();

	ProjectHeader* header();

	/**
	* \brief Método que retorna uma instância existente da classe Terrain.
	* \return Terrain Instância da classe Terrain.
	 */
	Terrain* terrain();

	/**
	* \brief Método que retorna uma instância existente da classe Sensor.
	* \param id Identificador do Sensor.
	* \return Sensor Instância da classe Sensor.
	*/
	Sensor* sensor(int id);

	/**
	* \brief Método que retorna uma instância existente da classe Flight.
	* \param id Identificador do Vôo.
	* \return Flight Instância da classe Flight.
	*/
	Flight* flight(int id);

	/**
	* \brief Método que retorna uma instância existente da classe Image.
	* \param id Identificador da Imagem.
	* \return Image Instância da classe Image.
	*/
	Image* image(int id);

	/**
	* \brief Método que retorna todas as instâncias existente da classe Image.
	* \return deque<Image*> deque com ponteiros para as instâncias da classe Image.
	*/
	deque<Image*> allImages() {return images;}

	/**
	* \brief Método que retorna uma instância existente da classe Point.
	* \param id Identificador do Ponto.
	* \return Point Instância da classe Point.
	*/
	Point* point(int id);

	/**
	* \brief Método que retorna todas as instâncias existente da classe Point.
	* \return deque<Point*> deque com ponteiros para as instâncias da classe Point.
	*/
	deque<Point*> allPoints() {return points;}

	/**
	* \brief Método que retorna uma instância existente da classe InteriorOrientation.
	* \param id Identificador da Orientação Interior.
	* \return InteriorOrientation Instância da classe InteriorOrientation.
    */
    InteriorOrientation* IO(int id);
    deque<InteriorOrientation*> allIOs() {return IOs;}

	/**
	* \brief Método que retorna uma instância existente da classe ExteriorOrientation.
	* \param id Identificador da Orientação Exterior.
	* \return ExteriorOrientation Instância da classe ExteriorOrientation.
    */
    ExteriorOrientation* EO(int id);
    deque<ExteriorOrientation*> allEOs() {return EOs;}

	/**
	* \brief Método que retorna os valores de um nó do XML.
	* \param tagname Nome da tag do XML.
	* \return string Valores do Nó do XML que foi requisitado.
	*/
	string getXml(string tagname);

	/**
	* \brief Método que retorna os valores de um nó do XML.
	* \param tagname Nome da tag do XML.
	* \param att Valor do atributo da tag do XML.
	* \param value Valor do nó da tag do XML.
	* \return string Valores do Nó do XML que foi requisitado.
	*/
	string getXml(string tagname, string att, string value);

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
	* \return bool	Retorna verdadeiro caso o nome passado seja EFotoManager. Retorna falso no caso contrário.
	*/
	bool is(string s);

	/**
    * \brief Método para setar os valores de atributos de uma instância de Projeto utilizando sua descrição em xml.
    * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Project.
	*/
    void setXml(string xml);

	/**
    * \brief Método para extrair o equivalente em xml de uma instância de Projeto.
    * \return string	Retorna o string contendo o xml para uma instância da classe Project.
	*/
    string getXml();

    /**
    * \brief Método que retorna um identificador de sensor que ainda não foi utilizado.
    * \return int Identificador da sensor.
    */
    int getFreeSensorId();

    /**
    * \brief Método que retorna um identificador de voo que ainda não foi utilizado.
    * \return int Identificador do voo.
    */
    int getFreeFlightId();

    /**
    * \brief Método que retorna um identificador de imagem que ainda não foi utilizada.
    * \return int Identificador da Imagem.
    */
    int getFreeImageId();

	/**
    * \brief Método que retorna um identificador de ponto que ainda não foi utilizado.
	* \return int Identificador do Ponto.
	*/
	int getFreePointId();


    /**
    * \brief Método que checa se a estrutura foi alterada em relação a sua versão salva.
    * \return bool Retorno verdadeiro se a estrutura instanciada equivale ao xml de permanencia para essa estrutura.
    */
	bool getSaveState();

	/**
	* \brief Método que apaga uma instância da classe Sensor.
	* \param id Identificador do Sensor.
	*/
	void deleteSensor(int id, bool makeReconnections = true);

	/**
	* \brief Método que apaga uma instância da classe Flight.
	* \param id Identificador do Vôo.
	*/
	void deleteFlight(int id, bool makeReconnections = true);

	/**
	* \brief Método que apaga uma instância da classe Image.
	* \param id Identificador da Imagem.
	*/
	void deleteImage(int id, bool makeReconnections = true);

	/**
	* \brief Método que apaga uma instância da classe Point.
	* \param id Identificador do Ponto.
	*/
	void deletePoint(int id, bool makeReconnections = true);

	/**
	* \brief Método que apaga uma instância da classe InteriorOrientation.
	* \param id Identificador da Orientação Interior.
	*/
	void deleteIO(int id, bool makeReconnections = true);

	/**
	* \brief Método que apaga uma instância da classe ExteriorOrientation.
	* \param id Identificador da Orientação Exterior.
	*/
	void deleteEO(int id, bool makeReconnections = true);

	void addSensor(string data = "", bool makeReconnections = true);

	void addFlight(string data = "", bool makeReconnections = true);

	void addImage(string data = "", bool makeReconnections = true);

	void addPoint(string data = "", bool makeReconnections = true);

    void addIO(string data, bool makeReconnections = true);

    void addEO(string data, bool makeReconnections = true);

    string getProcessStates() {return processStates;}

    void setProcessStates(string state) {processStates = state;}

    void closeProject();

    // Rever e adicionar aqui os métodos de new, load e save para uso de permanencia de disco
    // Rever e adicionar aqui os métodos de add, instance e delete dos itens Dem, EOI e Feat.
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECT_H

