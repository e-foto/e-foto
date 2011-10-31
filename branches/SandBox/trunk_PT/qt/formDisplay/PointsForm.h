#ifndef POINTSFORM_H
#define POINTSFORM_H

#include "ui_PointsLayout.h"
#include "AbstractForm.h"
#include "EDom.h"
#include <QStringList>

/**
 * \file PointsForm.h
 * \class PointsForm
 * \brief Classe que controla a interface do formulario de Pontos
 * \author
 */
class PointsForm : public AbstractForm, public Ui::PointsForm
{
    Q_OBJECT

public:

	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
    PointsForm(QWidget *parent=0);

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
	* \brief Metodo que informa se o formulario � o mesmo do parametro formName
	* \param formName : nome do formulario
	* \return bool : Verdadeiro se formName � esse formulario, falso caso contr�rio
	*/
	bool isForm(string formName);

private slots:

	/**
	* \brief Metodo que envia um sinal toda vez que uma coluna/campo de um ponto � clicada
	* \param i : Valor num�rico da linha da tabela contendo os pontos
	*/
	void emitSignal(int i);

signals:

	/**
	* \brief Sinal emitido informando qual a linha em que a tabela foi clicada
	* \param index : Valor num�rico da linha em que a tabela de pontos foi clicado
	*/
    void clicked(int pointKey);

};
#endif //POINTSFORM_H
