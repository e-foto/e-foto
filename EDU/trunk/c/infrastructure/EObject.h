#ifndef EOBJECT_H
#define EOBJECT_H

#include "EDom.h"

#include <string>
#include <sstream>
#include <fstream>
#include <deque>

using namespace std;

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
	virtual string objectType(void) = 0;

	/**
 * \brief Método para emitir as associações de uma instância.
 * \return string	Nome de todas as classes associadas a este objeto.
 * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
 * \todo A verificação de associações existentes no projeto deverá receber um novo tratamento no futuro para que possamos saber a quais classes um objeto pode se ligar e a quais instancias ele está ligado.
 */
	virtual string objectAssociations(void) = 0;

	/**
 * \brief Método de teste para o nome/tipo de instância.
 * \param s	Texto com o nome da classe que é esperado.
 * \return bool	Retorna verdadeiro caso o nome da classe seja aquele passado. Retorna falso no caso contrário.
 */
	virtual bool is(string s) = 0;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //EOBJECT_H
