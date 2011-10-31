#ifndef FLIGHTFORM_H
#define FLIGHTFORM_H

#include "ui_FlightLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

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
  * \brief Metodo que preenche um formul�rio previamente salvo em um arquivo *.epp
  * \param values : string que cont�m o xml com os valores que preencher�o o formulario
  */
	void fillvalues(string values);

	/**
  * \brief Metodo que cria uma string, padronizada em xml, com os valores dos campos do formulario
  * \return string : Texto em xml contendo tags com os campos do formulario
  */
	string getvalues();

	/**
  * \brief Metodo que altera todos os objetos editaveis do formulario no modo de somente leitura
  * \param state : Diz se objetos devem ficar no modo somente leitura ou n�o
  */
	void setReadOnly(bool state);

	/**
  * \brief Metodo que altera todos os objetos do formulario para conven��o de data, hora etc. para o local especificado
  * \param locale : Objeto que informa o local
  */
	void setFormLocale(QLocale locale);

	/**
  * \brief Metodo que esvazia ou zera todos os campos do formul�rio
  */
	void cleanForm();

	/**
  * \brief Metodo que informa se o formulario � o mesmo do parametro formName
  * \param formName : nome do formulario
  * \return bool : Verdadeiro se formName � esse formulario, falso caso contr�rio
  */
	bool isForm(string formName);

};

#endif //FLIGHTFORM_H
