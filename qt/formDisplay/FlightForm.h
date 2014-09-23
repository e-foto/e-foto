#ifndef FLIGHTFORM_H
#define FLIGHTFORM_H
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

#include "ui_FlightLayout.h"
#include "AbstractForm.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * \file FlightForm.h
 * \class FlightForm
 * \brief Classe que controla a interface do formulario de Flight
 * \author
 */

class FlightForm : public AbstractForm, public Ui::FlightForm
{
	Q_OBJECT

public:
	/**
  * \brief Construtor do formulario
  * \param parent : QWidget pai do formulario
  */
	FlightForm(QWidget *parent = 0);

	/**
  * \brief Metodo que preenche um formulário previamente salvo em um arquivo *.epp
  * \param values : string que contém o xml com os valores que preencherão o formulario
  */
    void fillvalues(std::string values);

	/**
  * \brief Metodo que cria uma string, padronizada em xml, com os valores dos campos do formulario
  * \return std::string : Texto em xml contendo tags com os campos do formulario
  */
    std::string getvalues();

	/**
  * \brief Metodo que altera todos os objetos editaveis do formulario no modo de somente leitura
  * \param state : Diz se objetos devem ficar no modo somente leitura ou não
  */
	void setReadOnly(bool state);

	/**
  * \brief Metodo que altera todos os objetos do formulario para convenção de data, hora etc. para o local especificado
  * \param locale : Objeto que informa o local
  */
	void setFormLocale(QLocale locale);

	/**
  * \brief Metodo que esvazia ou zera todos os campos do formulário
  */
	void cleanForm();

	/**
  * \brief Metodo que informa se o formulario é o mesmo do parametro formName
  * \param formName : nome do formulario
  * \return bool : Verdadeiro se formName é esse formulario, falso caso contrário
  */
    bool isForm(std::string formName);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
#endif //FLIGHTFORM_H
