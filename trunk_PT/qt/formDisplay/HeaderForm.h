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
	* \brief Metodo que esvazia ou zera todos os campos do formul�rio
	*/
	void cleanForm();

	/**
	* \brief Metodo que altera todos os objetos do formulario para conven��o de data, hora etc. para o local especificado
	* \param locale : Objeto que informa o local
	*/
	void setFormLocale(QLocale locale);

	/**
	* \brief Metodo que informa se o formulario � o mesmo do parametro formName
	* \param formName : nome do formulario
	* \return bool : Verdadeiro se formName � esse formulario, falso caso contr�rio
	*/
	bool isForm(string formName);

private slots:
	/**
	* \brief Metodo que mostra ou n�o os metadados baseado no campo correspondente ao nome do arquivo.
	* \param metadata : Conteudo do campo correspondente ao nome do arquivo
	* \deprecated Metodo em desuso atualmente
	*/
	void metadataVisibleChanged(QString metadata);

	/**
	* \brief Metodo coloca o foco no campo correspondente ao nome do Projeto toda vez que o formulario ganha o foco
	* \param evento: Parametro contendo informa�oes sobre o evento de Foco
	*/
	void focusInEvent(QFocusEvent *evento);

};

#endif // HEADERFORM_H
