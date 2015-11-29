/*******************************************************************************
		EFotoManager.h
*******************************************************************************/
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


#ifndef EFOTOMANAGER_H
#define EFOTOMANAGER_H

#include "Project.h"
#include "XmlUpdater.h"

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
class ReportManager;

/**
* \file EFotoManager.h
* \class EFotoManager
* \brief Esta classe gerencia o fluxo de dados da aplicação.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class EFotoManager
{

public:
	enum nextModule_t {NEXT_NONE, NEXT_PROJECT, NEXT_RELOAD, NEXT_IO, 
	  NEXT_SR, NEXT_PT, NEXT_DEM, NEXT_ORTHO, NEXT_SP, NEXT_Report, NEXT_PTReport};
private:
	nextModule_t nextModule;
	int nextImage;

	bool savedState;
    std::string xmlData;
    std::string interfaceType;


    ProjectManager* project;
	IOManager* interiorOrientation;
	SRManager* spatialRessection;
	PTManager* fotoTri;
	DEMManager* dem;
	OrthoManager* ortho;
	SPManager* sp;
    ReportManager* report;
    XmlUpdater updater;
    Project report_project;

	Terrain* theTerrain;
    std::deque<Sensor*> sensors;
    std::deque<Flight*> flights;
    std::deque<Image*> images;
    std::deque<Point*> points;
    std::deque<InteriorOrientation*> IOs;
    std::deque<ExteriorOrientation*> EOs;

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
	* \brief Método que apaga uma instância da classe Terrain.
	*/
	void deleteTerrain();

	/**
	* \brief Método que apaga uma instância da classe Sensor.
	* \param id Identificador do Sensor.
	*/
	void deleteSensor(int id);

	/**
	* \brief Método que apaga uma instância da classe Flight.
	* \param id Identificador do Vôo.
	*/
	void deleteFlight(int id);

	/**
	* \brief Método que apaga uma instância da classe Image.
	* \param id Identificador da Imagem.
	*/
	void deleteImage(int id);

	/**
	* \brief Método que apaga uma instância da classe Point.
	* \param id Identificador do Ponto.
	*/
	void deletePoint(int id);

	/**
	* \brief Método que apaga uma instância da classe InteriorOrientation.
	* \param id Identificador da Orientação Interior.
	*/
	void deleteIO(int id);

	/**
	* \brief Método que apaga uma instância da classe ExteriorOrientation.
	* \param id Identificador da Orientação Exterior.
	*/
	void deleteEO(int id);

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
	* \brief Método que retorna uma instância existente da classe Point.
	* \param id Identificador do Ponto.
	* \return Point Instância da classe Point.
	*/
	Point* point(int id);

	/**
	* \brief Método que retorna uma instância existente da classe InteriorOrientation.
	* \param id Identificador da Orientação Interior.
	* \return InteriorOrientation Instância da classe InteriorOrientation.
	*/
	InteriorOrientation* IO(int id);

	/**
	* \brief Método que retorna uma instância existente da classe ExteriorOrientation.
	* \param id Identificador da Orientação Exterior.
	* \return ExteriorOrientation Instância da classe ExteriorOrientation.
	*/
	ExteriorOrientation* EO(int id);

	/**
	* \brief Método que retorna os valores de um nó do XML.
	* \param tagname Nome da tag do XML.
    * \return std::string Valores do Nó do XML que foi requisitado.
	*/
    std::string getXml(std::string tagname);

	/**
	* \brief Método que retorna os valores de um nó do XML.
	* \param tagname Nome da tag do XML.
	* \param att Valor do atributo da tag do XML.
	* \param value Valor do nó da tag do XML.
    * \return std::string Valores do Nó do XML que foi requisitado.
	*/
    std::string getXml(std::string tagname, std::string att, std::string value);

	/**
	* \brief Método para emitir o nome de classe.
    * \return std::string	Retorna o nome de classe do objeto.
	*/
    std::string objectType(void);

	/**
	* \brief Método para emitir as associações de uma instância.
    * \return std::string	Retorna vazio para esta classe.
	* \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
	*/
    std::string objectAssociations(void);

	/**
	* \brief Método de teste para o nome/tipo de instância.
	* \param s	Texto com o nome da classe que é esperado.
	* \return bool	Retorna verdadeiro caso o nome passado seja EFotoManager. Retorna falso no caso contrário.
	*/
    bool is(std::string s);

	/**
	* \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
	* \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe EFotoManager.
	*/
    void xmlSetData(std::string xml);

	/**
	* \brief Método para extrair o equivalente em dados xml de uma instância.
    * \return std::string	Retorna o string contendo o xml para uma instância da classe EFotoManager.
	*/
    std::string xmlGetData();

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
    void setInterfaceType(std::string newInterfaceType);

	/**
	* \brief Método que retorna o tipo de interface corrente.
    * \return std::string O tipo de interface corrente.
	*/
    std::string getInterfaceType();

	/**
	* \brief Método que seta o próximo módulo a ser executado.
	* \param newModule  O próximo módulo a ser executado.
	*/
    void setNextModule(nextModule_t newModule);

	/**
	* \brief Método que seta a próxima imagem a ser processada.
	* \param newImage  A próxima imagem a ser executada.
	*/
	void setNextImage(int newImage);

	/**
	* \brief Método que carrega o módulo ProjectManager.
	* \return bool Retorna verdadeiro se o módulo ProjectManager já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
    bool execProject(std::string filename = "");

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
    bool exec(std::string filename= "");
	/**
	* \brief Método que carrega o módulo de FotoTri.
	* \return bool Retorna verdadeiro se o módulo FotoTri já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool execPT();
	/**
	* \brief Método que encerra o módulo de FotoTri.
	*/
	void stopPT();
	/**
	* \brief Método que retorna uma imagem cadastrada que ainda não foi utilizada.
	* \return int Identificador da Imagem.
	*/
	int getFreeImageId();
	/**
	* \brief Método que retorna um ponto cadastrado que ainda não foi utilizado.
	* \return int Identificador do Ponto.
	*/
	int getFreePointId();

	/**
	* \brief Método que carrega o módulo de Report.
	* \return bool Retorna verdadeiro se o módulo FotoTri já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
        bool execEPR();

	/**
	* \brief Método que encerra o módulo de Report.
	*/
        void stopEPR();

        bool loadProject();

        Project* getProject() {return &report_project;}
        void execPTReport();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EFOTOMANAGER_H
