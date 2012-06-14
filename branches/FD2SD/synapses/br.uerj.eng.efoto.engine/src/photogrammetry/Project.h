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
#include "XmlUpdater.h"

#ifdef SYNAPSE_EFOTO
#include "IProject.h"
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

#ifdef INTEGRATED_EFOTO
class Project
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
class Q_DECL_EXPORT Project : public engine::IProject
#endif //SYNAPSE_EFOTO
{
	string xmlData;
	string xmlState;
    XmlUpdater* updater;

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



    /**
    * \brief Método que será implementado futuramente.
    * \todo  Será usado para conectar a um futuro banco de dados.
    */
    bool connectDatabase();
    /**
    * \brief Método que será implementado futuramente.
    * \todo  Será usado para desconectar a um futuro banco de dados.
    */
    bool disconnectDatabase();
    /**
    * \brief Método que inicia um novo projeto, inicia um xml vazio para ser preenchido posteriormente ao longo do projeto.
    * \param filename Nome do arquivo.
    * \return bool Retorna verdadeiro se o novo projeto foi iniciado corretamente. Retorna falso, caso contrário.
    */
    bool newProject(string filename);
    /**
    * \brief Método que será implementado futuramente.
    * \todo  Será usado para carregar um projeto a partir de um banco de dados.
    */
    bool loadProject();
    /**
    * \brief Método que será implementado futuramente.
    * \todo  Será usado para salvar um projeto a partir de um banco de dados.
    */
    bool saveProject();
    /**
    * \brief Método que carrega um projeto a partir de um arquivo do tipo *.epp.
    * \param  filename Nome do arquivo.
    * \return  bool Retorna verdadeiro se o projeto foi carregado corretamente. Retorna falso, caso contrário.
    */
    bool loadFile(string filename);
    /**
    * \brief Método que salva um projeto em um arquivo do tipo *.epp.
    * \param  filename Nome do arquivo.
    * \return  bool Retorna verdadeiro se o projeto foi salvado corretamente. Retorna falso, caso contrário.
    */
    bool saveFile(string filename);

    /**
    * \brief Método que checa se a estrutura foi alterada em relação a sua versão salva.
    * \return bool Retorno verdadeiro se a estrutura instanciada equivale ao xml de permanencia para essa estrutura.
    */
    bool getSaveState();

    /**
    * \brief Método que fecha um projeto.
    * \return  bool Retorna verdadeiro se o projeto fechado corretamente. Retorna falso, caso contrário.
    */
    bool closeProject();

    /**
    * \brief Método que retorna o código de erro para o caso de um carregamento de arquivo falhar.
    */
    int getError();



    /**
    * \brief Método que retorna uma instância existente da classe ProjectHeader.
    * \return Terrain Instância da classe Terrain.
     */
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

    /**
    * \brief Método que retorna todas as instâncias existente da classe InteriorOrientation.
    * \return deque<InteriorOrientation*> deque com ponteiros para as instâncias da classe InteriorOrientation.
    */
    deque<InteriorOrientation*> allIOs() {return IOs;}

	/**
	* \brief Método que retorna uma instância existente da classe ExteriorOrientation.
	* \param id Identificador da Orientação Exterior.
	* \return ExteriorOrientation Instância da classe ExteriorOrientation.
	*/
	ExteriorOrientation* EO(int id);

    /**
    * \brief Método que retorna todas as instâncias existente da classe ExteriorOrientation.
    * \return deque<ExteriorOrientation*> deque com ponteiros para as instâncias da classe ExteriorOrientation.
    */
    deque<ExteriorOrientation*> allEOs() {return EOs;}



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



    /**
    * \brief Método adicionar um novo Sensor.
    * \param data	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Sensor.
    * \param makeReconnections	Booleano usado para realizar as devidas conexões da classe.
    */
	void addSensor(string data = "", bool makeReconnections = true);

    /**
    * \brief Método adicionar um novo Flight.
    * \param data	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Flight.
    * \param makeReconnections	Booleano usado para realizar as devidas conexões da classe.
    */
	void addFlight(string data = "", bool makeReconnections = true);

    /**
    * \brief Método adicionar um novo Image.
    * \param data	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Image.
    * \param makeReconnections	Booleano usado para realizar as devidas conexões da classe.
    */
	void addImage(string data = "", bool makeReconnections = true);

    /**
    * \brief Método adicionar um novo Point.
    * \param data	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Point.
    * \param makeReconnections	Booleano usado para realizar as devidas conexões da classe.
    */
	void addPoint(string data = "", bool makeReconnections = true);

    /**
    * \brief Método adicionar um novo InteriorOrientation.
    * \param data	String contendo o xml com todos os valores de atributos adequados a uma instância da classe InteriorOrientation.
    * \param makeReconnections	Booleano usado para realizar as devidas conexões da classe.
    */
    void addIO(string data, bool makeReconnections = true);

    /**
    * \brief Método adicionar um novo ExteriorOrientation.
    * \param data	String contendo o xml com todos os valores de atributos adequados a uma instância da classe ExteriorOrientation.
    * \param makeReconnections	Booleano usado para realizar as devidas conexões da classe.
    */
    void addEO(string data, bool makeReconnections = true);



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
    * \brief Método para extrair o equivalente em xml de uma instância de Projeto.
    * \return string	Retorna o string contendo o xml para uma instância da classe Project.
    */
    string getXml();

    /**
    * \brief Método para setar os valores de atributos de uma instância de Projeto utilizando sua descrição em xml.
    * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Project.
    */
    void setXml(string xml);



    string getProcessStates() {return processStates;}

    void setProcessStates(string state) {processStates = state;}

    // Rever! e adicionar aqui os métodos de add, instance e delete dos itens Dem, EOI e Feat.
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECT_H
