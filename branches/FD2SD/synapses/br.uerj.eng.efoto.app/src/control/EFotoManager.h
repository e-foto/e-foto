/*******************************************************************************
		EFotoManager.h
*******************************************************************************/


#ifndef EFOTOMANAGER_H
#define EFOTOMANAGER_H

#include "EObject.h"
//#include "Terrain.h"
//#include "ProjectHeader.h"
//#include "SensorWithFiducialMarks.h"
//#include "SensorWithKnowDimensions.h"
//#include "SensorWithKnowParameters.h"
//#include "Flight.h"
//#include "Image.h"
//#include "Point.h"
//#include "InteriorOrientation.h"
//#include "SpatialRessection.h"

#ifdef INTEGRATED_EFOTO
#include "Project.h"
#endif //INTEGRATED_EFOTO
#ifdef SYNAPSE_EFOTO
	// Aqui vai entrar uma referência para a Project na sinapse engine
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class IOManager;
class SRManager;
class PTManager;
class ProjectManager;
class DEMManager;
class OrthoManager;
class SPManager;

/**
* \file EFotoManager.h
* \class EFotoManager
* \brief Esta classe gerencia o fluxo de dados da aplicação.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class EFotoManager
{
	Project project;

	int nextModule;
	int nextImage;

	//bool savedState;
	//string xmlData;
	string interfaceType;

	ProjectManager* projectManager;
	IOManager* interiorOrientation;
	SRManager* spatialRessection;
	PTManager* fotoTri;
	DEMManager* dem;
	OrthoManager* ortho;
	SPManager* sp;

	//Terrain* theTerrain;
	//deque<Sensor*> sensors;
	//deque<Flight*> flights;
	//deque<Image*> images;
	//deque<Point*> points;
	//deque<InteriorOrientation*> IOs;
	//deque<ExteriorOrientation*> EOs;

public:

	/**
	* \brief Construtor padrão.
	*/
	EFotoManager();

	/**
	* \brief Destrutor padrão.
	*/
	~EFotoManager();

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
	* \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
	* \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe EFotoManager.
	*/
	void setXml(string xml);

	/**
	* \brief Método para extrair o equivalente em dados xml de uma instância.
	* \return string	Retorna o string contendo o xml para uma instância da classe EFotoManager.
	*/
	string getXml();

	/**
	* \brief Método usado após aceitar uma IO ou SR para setar como default que o projeto ainda não foi salvo.
	* \param state Estado do projeto se foi ou não salvo.
	*/
	void setSavedState(bool state);

	/**
	* \brief Método que retorna se o projeto foi salvo ou não.
	* \return bool Retorna verdadeiro caso o projeto tenha sido salvo. Retorna falso, caso contrário.
	*/
	bool getSavedState();

	/**
	* \brief Método que seta o tipo de interface em que o programa irá rodar.
	* \param newInterfaceType O tipo de interface.
	*/
	void setInterfaceType(string newInterfaceType);

	/**
	* \brief Método que retorna o tipo de interface corrente.
	* \return string O tipo de interface corrente.
	*/
	string getInterfaceType();

	/**
	* \brief Método que seta o próximo módulo a ser executado.
	* \param newModule  O próximo módulo a ser executado.
	*/
	void setNextModule(int newModule);

	/**
	* \brief Método que seta a próxima imagem a ser processada.
	* \param newImage  A próxima imagem a ser executada.
	*/
	void setNextImage(int newImage);

	/**
	* \brief Método que carrega o módulo ProjectManager.
	* \return bool Retorna verdadeiro se o módulo ProjectManager já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execProject(string filename = "");

	/**
	* \brief Método que recarrega o módulo ProjectManager.
	* \return bool Retorna verdadeiro se o módulo ProjectManager já tiver terminado de ser recarregado. Retorna falso, caso contrário.
	*/
	bool reloadProject();

	/**
	* \brief Método que carrega o módulo de InteriorOrientation.
	* \param id Identificador da Imagem.
	* \return bool Retorna verdadeiro se o módulo InteriorOrientation já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execIO(int id);

	/**
	* \brief Método que encerra o módulo de InteriorOrientation.
	* \param id Identificador da Imagem.
	*/
	void stopIO(int id);

	/**
	* \brief Método que carrega o módulo de ExteriorOrientation.
	* \param id Identificador da Imagem.
	* \return bool Retorna verdadeiro se o módulo ExteriorOrientation já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execSR(int id);

	/**
	* \brief Método que encerra o módulo de ExteriorOrientation.
	* \param id Identificador da Imagem.
	*/
	void stopSR(int id);

	/**
	* \brief Método que carrega o módulo de DEM.
	* \return bool Retorna verdadeiro se o módulo DEM já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execDEM();

	/**
	* \brief Método que encerra o módulo de DEM.
	*/
	void stopDEM();

	/**
	* \brief Método que carrega o módulo de ortoretificação de imagens.
	* \return bool Retorna verdadeiro se o módulo Ortho já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execOrtho();

	/**
	* \brief Método que encerra o módulo de ortoretificação de imagens.
	*/
	void stopOrtho();

	/**
	* \brief Método que carrega o módulo de estereoploter.
	* \return bool Retorna verdadeiro se o módulo estereoploter já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execSP();

	/**
	* \brief Método que encerra o módulo de estereoploter.
	*/
	void stopSP();

	/**
	* \brief Método que executa um loop de execuções principais.
	* \return bool Retorna verdadeiro se as execuções principais estejam sendo executadas corretamente. Retorna falso, caso contrário.
	*/
	bool exec(string filename= "");
	/**
	* \brief Método que carrega o módulo de FotoTri.
	* \return bool Retorna verdadeiro se o módulo FotoTri já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execPT();
	/**
	* \brief Método que encerra o módulo de FotoTri.
	*/
	void stopPT();

	Project* getProject() {return &project;}

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECT_H
