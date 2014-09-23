#ifndef EOBJECT_H
#define EOBJECT_H
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
* \file EObject.h
* \class EObject
* \brief Interface básica para todos os componentes do projeto E-Foto.
* \attention A classe EObject foi definida como uma classe abstrata, então você não deve usá-la diretamente.
* \attention Contrua apenas objetos das classes derivadas.
* \copyright E-Foto group
* \authors Rafael Aguiar & Irving Badolato
*/
class EObject : public EDom
{

public:

	/**
 * \brief Método para emitir o tipo de um objeto (seu nome de classe) e seu identificador, caso exista.
 * \return string	Retorna o nome de classe do objeto e seu identificador, caso exista.
 */
    virtual std::string objectType(void) = 0;

	/**
 * \brief Método para emitir as associações de uma instância.
 * \return string	Nome de todas as classes associadas a este objeto.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 * \todo A verificação de associações existentes no projeto deverá receber um novo tratamento no futuro para que possamos saber a quais classes um objeto pode se ligar e a quais instancias ele está ligado.
 */
    virtual std::string objectAssociations(void) = 0;

	/**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome da classe seja aquele passado. Retorna falso no caso contrário.
 */
    virtual bool is(std::string s) = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //EOBJECT_H
