#ifndef PROJECTHEADER_H
#define PROJECTHEADER_H
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
    std::string name;
    std::string description;
    std::string filePath;
    std::string fileName;
    std::string creation;
    std::string modification;
    std::string owner;
    std::string aims;
    std::string context;

public:

	/**
 * \brief Destrutor padrão.
 */
	virtual ~ProjectHeader();

	/**
 * \brief Método que define o nome do Projeto.
 * \return newName	Novo nome.
 */
    void setName(std::string newName);

	/**
 * \brief Método que define a descrição do Projeto.
 * \return newDescription	Nova descrição.
 */
    void setDescription(std::string newDescription);

	/**
 * \brief Método que define o caminho onde o projeto será salvo.
 * \return newFilePath	Novo caminho de arquivo.
 */
    void setFilePath(std::string newFilePath);

	/**
 * \brief Método que define o nome do arquivo que será salvo.
 * \return newFileName	Novo nome de arquivo.
 */
    void setFileName(std::string newFileName);

	/**
 * \brief Método que define a data e hora de criação do Projeto.
 * \return newCreation	Nova data de criação.
 */
    void setCreation(std::string newCreation);

	/**
 * \brief Método que define a data e hora de modificação do Projeto.
 * \return newModification	Nova data de modificação.
 */
    void setModification(std::string newModification);

	/**
 * \brief Método que define o dono do Projeto.
 * \return newOwner	Novo dono.
 */
    void setOwner(std::string newOwner);

	/**
 * \brief Método que define o objetivo do Projeto.
 * \return newAims	Novo objetivo.
 */
    void setAims(std::string newAims);

	/**
 * \brief Método que define o contexto do Projeto.
 * \return newContext	Novo contexto.
 */
    void setContext(std::string newContext);

	/**
 * \brief Método que retorna o nome do Projeto.
 * \return std::string	Nome do Projeto.
 */
    std::string getName();

	/**
 * \brief Método que retorna a descrição do Projeto.
 * \return std::string	Descrição do Projeto.
 */
    std::string getDescription();

	/**
 * \brief Método que retorna o caminho onde o Projeto foi salvo.
 * \return std::string	Caminho onde o projeto foi salvo.
 */
    std::string getFilePath();

	/**
 * \brief Método que retorna o nome do arquivo que o Projeto foi salvo.
 * \return std::string	Nome do arquivo do projeto.
 */
    std::string getFileName();

	/**
 * \brief Método que retorna a data e hora de criação do Projeto.
 * \return std::string	Data e hora de criação.
 */
    std::string getCreation();

	/**
 * \brief Método que retorna a data e hora da modificação do Projeto.
 * \return std::string	Data e hora de modificação.
 */
    std::string getModification();

	/**
 * \brief Método que retorna o dono do Projeto.
 * \return std::string	Dono do projeto.
 */
    std::string getOwner();

	/**
 * \brief Método que retorna o objetivo do Projeto.
 * \return std::string	Objetivo do projeto.
 */
    std::string getAims();

	/**
 * \brief Método que retorna o contexto do Projeto.
 * \return std::string	Contexto do projeto.
 */
    std::string getContext();

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
 * \return bool	Retorna verdadeiro caso o nome passado seja ProjectHeader. Retorna falso no caso contrário.
 */
    bool is(std::string s);

	/**
 * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
 * \param xml	std::string contendo o xml com todos os valores de atributos adequados a uma instância da classe ProjectHeader.
 */
    void xmlSetData(std::string xml);

	/**
 * \brief Método para extrair o equivalente em dados xml de uma instância.
 * \return std::string	Retorna o std::string contendo o xml para uma instância da classeprojectHeader.
 */
    std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //PROJECTHEADER_H
