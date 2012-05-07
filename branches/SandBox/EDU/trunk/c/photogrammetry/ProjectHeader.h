#ifndef PROJECTHEADER_H
#define PROJECTHEADER_H

#include "EObject.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file ProjectHeader.h
* \class ProjectHeader
* \brief Classe que registra os atributos de cabeçalho do projeto.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class ProjectHeader : public EObject
{

private:
	string name;
	string description;
	string filePath;
	string fileName;
	string creation;
	string modification;
	string owner;
	string aims;
	string context;

public:

	/**
 * \brief Destrutor padrão.
 */
	virtual ~ProjectHeader();

	/**
 * \brief Método que define o nome do Projeto.
 * \return newName	Novo nome.
 */
	void setName(string newName);

	/**
 * \brief Método que define a descrição do Projeto.
 * \return newDescription	Nova descrição.
 */
	void setDescription(string newDescription);

	/**
 * \brief Método que define o caminho onde o projeto será salvo.
 * \return newFilePath	Novo caminho de arquivo.
 */
	void setFilePath(string newFilePath);

	/**
 * \brief Método que define o nome do arquivo que será salvo.
 * \return newFileName	Novo nome de arquivo.
 */
	void setFileName(string newFileName);

	/**
 * \brief Método que define a data e hora de criação do Projeto.
 * \return newCreation	Nova data de criação.
 */
	void setCreation(string newCreation);

	/**
 * \brief Método que define a data e hora de modificação do Projeto.
 * \return newModification	Nova data de modificação.
 */
	void setModification(string newModification);

	/**
 * \brief Método que define o dono do Projeto.
 * \return newOwner	Novo dono.
 */
	void setOwner(string newOwner);

	/**
 * \brief Método que define o objetivo do Projeto.
 * \return newAims	Novo objetivo.
 */
	void setAims(string newAims);

	/**
 * \brief Método que define o contexto do Projeto.
 * \return newContext	Novo contexto.
 */
	void setContext(string newContext);

	/**
 * \brief Método que retorna o nome do Projeto.
 * \return string	Nome do Projeto.
 */
	string getName();

	/**
 * \brief Método que retorna a descrição do Projeto.
 * \return string	Descrição do Projeto.
 */
	string getDescription();

	/**
 * \brief Método que retorna o caminho onde o Projeto foi salvo.
 * \return string	Caminho onde o projeto foi salvo.
 */
	string getFilePath();

	/**
 * \brief Método que retorna o nome do arquivo que o Projeto foi salvo.
 * \return string	Nome do arquivo do projeto.
 */
	string getFileName();

	/**
 * \brief Método que retorna a data e hora de criação do Projeto.
 * \return string	Data e hora de criação.
 */
	string getCreation();

	/**
 * \brief Método que retorna a data e hora da modificação do Projeto.
 * \return string	Data e hora de modificação.
 */
	string getModification();

	/**
 * \brief Método que retorna o dono do Projeto.
 * \return string	Dono do projeto.
 */
	string getOwner();

	/**
 * \brief Método que retorna o objetivo do Projeto.
 * \return string	Objetivo do projeto.
 */
	string getAims();

	/**
 * \brief Método que retorna o contexto do Projeto.
 * \return string	Contexto do projeto.
 */
	string getContext();

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
 * \return bool	Retorna verdadeiro caso o nome passado seja ProjectHeader. Retorna falso no caso contrário.
 */
	bool is(string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe ProjectHeader.
 */
	void xmlSetData(string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return string	Retorna o string contendo o xml para uma instância da classeprojectHeader.
 */
	string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //PROJECTHEADER_H
