#ifndef HEADERFORM_H
#define HEADERFORM_H

#include "ui_HeaderLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

/**
 * \file HeaderForm.h
 * \class HeaderForm
 * \brief Classe que controla a interface do formulario de Header
 * \author Irving Badolato
 */

class HeaderForm : public AbstractForm, public Ui::Form
{
	Q_OBJECT

public:
	/**
	* \brief Construtor do formulario
	* \param parent : QWidget pai do formulario
	*/
	HeaderForm(QWidget *parent = 0);

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
	* \brief Metodo que esvazia ou zera todos os campos do formulário
	*/
	void cleanForm();

	/**
	* \brief Metodo que altera todos os objetos do formulario para convenção de data, hora etc. para o local especificado
	* \param locale : Objeto que informa o local
	*/
	void setFormLocale(QLocale locale);

	/**
	* \brief Metodo que informa se o formulario é o mesmo do parametro formName
	* \param formName : nome do formulario
	* \return bool : Verdadeiro se formName é esse formulario, falso caso contrário
	*/
	bool isForm(string formName);

private slots:
	/**
	* \brief Metodo que mostra ou não os metadados baseado no campo correspondente ao nome do arquivo.
	* \param metadata : Conteudo do campo correspondente ao nome do arquivo
	* \deprecated Metodo em desuso atualmente
	*/
	void metadataVisibleChanged(QString metadata);

	/**
	* \brief Metodo coloca o foco no campo correspondente ao nome do Projeto toda vez que o formulario ganha o foco
	* \param evento: Parametro contendo informaçoes sobre o evento de Foco
	*/
	void focusInEvent(QFocusEvent *evento);

};

#endif // HEADERFORM_H
