#ifndef FLIGHTFORM_H
#define FLIGHTFORM_H

#include "ui_FlightLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

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
	void fillvalues(string values);

	/**
  * \brief Metodo que cria uma string, padronizada em xml, com os valores dos campos do formulario
  * \return string : Texto em xml contendo tags com os campos do formulario
  */
	string getvalues();

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
	bool isForm(string formName);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
#endif //FLIGHTFORM_H
