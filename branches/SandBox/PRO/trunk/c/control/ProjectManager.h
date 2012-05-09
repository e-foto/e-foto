/**************************************************************************
							ProjectManager.h
**************************************************************************/


#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "EObject.h"
#include "ETreeModel.h"
#include "PositionMatrix.h"
#include "ProjectUserInterface.h"
#include "XmlUpdater.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectUserInterface;
class EFotoManager;

/**
* \file ProjectManager.h
* \class ProjectManager
* \brief Esta classe controla a Project Manager UI.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ProjectManager
{

	EFotoManager* manager;
	ProjectUserInterface* myInterface;
	//FILE* xmlFile; // attribute banned for lack of use
	XmlUpdater* updater;
	ETreeModel* treeModel;

public:
	/**
	* \brief Construtor vazio.
	*/
	ProjectManager();
	/**
	* \brief Construtor padrão.
	* \param manager : Controlador geral.
	*/
	ProjectManager(EFotoManager* manager);
	/**
	* \brief Destrutor padrão.
	*/
	~ProjectManager();
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
	* \brief Método que retorna um inteiro informando que tipo de erro tem o arquivo.
	* \return  int Código de erro que tem o arquivo.
	*/
	int informFileVersionError();
	/**
	* \brief Método que adiciona um nó no xml do projeto.
	* \param data : String com os dados a serem inseridos no XML.
	* \param parent : String com a tag na qual sera inseridos os dados
	* \return bool Retorna verdadeiro se a adição foi realizada corretamente. Retorna falso, caso contrário.
	*/
	bool addComponent(string data, string parent);
	/**
	* \brief Método que remove um nó xml do projeto.
	* \param type : Texto contendo o tipo, a saber: Sensor, Flight,Image, Point, IO, EO.
	* \param  id : Identificacao do componente no XML.
	* \return bool Retorna verdadeiro se a remoção foi realizada corretamente. Retorna falso, caso contrário.
	*/
	bool removeComponent(string type, int id);
	/**
	* \brief Método que edita um nó no xml do projeto.
	* \param type : Texto contendo o tipo, a saber: Header, Terrain.
	* \param  data : Dados novos a serem inseridos no lugar dos antigos.
	* \return bool Retorna verdadeiro se a edição foi realizada corretamente. Retorna falso, caso contrário.
	*/
	bool editComponent(string type, string data);
	/**
	* \brief Método que edita um nó no xml do projeto.
	* \param type : Texto contendo o tipo, a saber: Sensor, Flight,Image, Point, IO, EO.
	* \param  id : Identificacao do componente no XML.
	* \param  data : Dados novos a serem inseridos no lugar dos antigos.
	* \return bool Retorna verdadeiro se a edição foi realizada corretamente. Retorna falso, caso contrário.
	*/
	bool editComponent(string type, int id, string data);
	/**
	* \brief Método que retorna uma instância solicitada.
	* \param type Tipo da instância.
	* \param  id Identificador da instância.
	* \return EObject Retorna a instância solicitada.
	*/
	EObject* viewComponent(string type, int id);
	/**
	* \brief Método que retorna o atual modelo de dados do projeto..
	* \return ETreeModel Modelo de dados do projeto.
	*/
	ETreeModel* getTreeModel();
	/**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
	/**Image Keys Image Keys Image Keys Image Keys Image Keys Image Keys Image Keys Image Keys Image Keys Image Keys Image Keys */
	/**DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS DAR NOME AOS BOIS */
	/**
	* \brief Método que retorna o deque de Image Keys.
	* \return deque<int> O deque de Image Keys.
	*/
	deque<int> listImageKeys();
	/**
	* \brief Método que retorna o deque de imagens.
	* \return deque<string> O deque de imagens.
	*/
	deque<string> listImages();
	/**
	* \brief Método que retorna o identificador de uma imagem.
	* \param imageName O nome da imagem.
	* \return int Identificador solicitado da imagem.
	*/
	int getImageId(string imageName);
	/**
	* \brief Método que retorna um identificador de imagens que ainda não foi utilizado.
	* \return int Identificador da imagem que ainda não foi utilizado.
	*/
	int getFreeImageId();
	/**
	* \brief Método que retorna uma id de pontos que ainda não foi utilizado.
	* \return int Identificador de ponto  que ainda não foi utilizado.
	*/
	int getFreePointId();
	/**
	* \brief Método que inicia um módulo de Orientação Interior ou Resseção Espacial a partir de um identificador de imagem.
	* \param module Módulo a ser iniciado.
	* \param image Identificador da imagem.
	* \return bool Retorna verdadeiro se o módulo foi iniciado corretamente. Retorna falso, caso contrário.
	*/
	bool startModule(string module, int image);
	/**
	* \brief Método que carrega o módulo ProjectManager.
	* \return bool Retorna verdadeiro se o módulo ProjectManager já tiver terminado de ser carregado. Retorna falso, caso contrário.
	*/
	bool exec(string filename = "");
	/**
	* \brief Método que recarrega o módulo ProjectManager.
	* \return bool Retorna verdadeiro se o módulo ProjectManager tiver recarregado com sucesso. Retorna falso, caso contrário.
	*/
	bool reload();
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
	* \todo Método que será implementado futuramente.
	*/
	bool getSavedState();
	/**
	* \brief Método que cria arquivos a serem usados no módulo de Stereoplotter.
	* \param filename Nome do arquivo que será salvo.
	* \param image1 Identificador da primeira imagem.
	* \param image2 Identificador da segunda imagem.
	* \return bool Retorna verdadeiro se os arquivos foram criados com sucesso. Retorna falso, caso contrário.
	*/
	bool makeSPFile(string filename, int image1, int image2);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //PROJECTMANAGER_H
