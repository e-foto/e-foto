#ifndef POINTSFORM_H
#define POINTSFORM_H

#include "ui_PointsLayout.h"
#include "AbstractForm.h"
#include "EDom.h"
#include <QStringList>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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
 * \brief Metodo que informa se o formulario é o mesmo do parametro formName
 * \param formName : nome do formulario
 * \return bool : Verdadeiro se formName é esse formulario, falso caso contrário
 */
	bool isForm(string formName);

private slots:

	/**
 * \brief Metodo que envia um sinal toda vez que uma coluna/campo de um ponto é clicada
 * \param i : Valor numérico da linha da tabela contendo os pontos
 */
	void emitSignal(int i);

signals:

	/**
 * \brief Sinal emitido informando qual a linha em que a tabela foi clicada
 * \param index : Valor numérico da linha em que a tabela de pontos foi clicado
 */
	void clicked(int pointKey);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //POINTSFORM_H
