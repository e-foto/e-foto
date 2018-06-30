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
#include "EDom.h"

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
class ProjectHeader : public EDom {

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
    * \brief Método que retorna o nome do Projeto.
    * \return std::string   Nome do Projeto.
    */
    std::string getName() const;

    /**
    * \brief Método que retorna a descrição do Projeto.
    * \return std::string   Descrição do Projeto.
    */
    std::string getDescription() const;

    /**
    * \brief Método que retorna a data e hora de criação do Projeto.
    * \return std::string   Data e hora de criação.
    */
    std::string getCreation() const;

    /**
    * \brief Método que retorna a data e hora da modificação do Projeto.
    * \return std::string   Data e hora de modificação.
    */
    std::string getModification() const;

    /**
    * \brief Método que retorna o dono do Projeto.
    * \return std::string   Dono do projeto.
    */
    std::string getOwner() const;

    /**
    * \brief Método para setar os valores de atributos de uma instância utilizando sua descrição em xml.
    * \param xml    std::string contendo o xml com todos os valores de atributos adequados a uma instância da classe ProjectHeader.
    */
    void xmlSetData(std::string xml);

    /**
    * \brief Método para extrair o equivalente em dados xml de uma instância.
    * \return std::string   Retorna o std::string contendo o xml para uma instância da classeprojectHeader.
    */
    std::string xmlGetData();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //PROJECTHEADER_H
