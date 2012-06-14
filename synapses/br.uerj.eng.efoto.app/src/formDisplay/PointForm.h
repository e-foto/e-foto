#ifndef POINTFORM_H
#define POINTFORM_H

#include "ui_PointLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"
#include <QTranslator>


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * \file PointForm.h
 * \class PointForm
 * \brief Classe que controla a interface do formulario de Ponto
 * \author
 */
class PointForm : public AbstractForm , public Ui::Ponto
{
	Q_OBJECT
	string key;
	deque<string> imageNameList;
	deque<int> imageKeyList;

public:
	SigmaFormController* sigmaController;

	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
	PointForm(QWidget *parent = 0);

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
 * \brief Altera a lista de keys e nome de arquivos de imagens em que o ponto mostrado pelo formulario está
 * \param myKeyList : Lista de keys das imagens em que o ponto aparece
 * \param myNameList : Lista de nomes dos arquivos das imagens em que o ponto aparece
 */
	void setImageList(deque<int> myKeyList, deque<string> myNameList);

	/**
 * \brief Altera o objeto(QComboBox) que indica o tipo do ponto cadastrado
 * \param type : Texto informando o tipo do ponto, podendo ser: "control", "photogrammetric" ou "checking"
 */
	void setType(string type);

	/**
 * \brief Metodo que retorna o tipo do ponto do formulario
 * \return string : Contem o texto informando o tipo do ponto
 */
	string getType();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // POINTFORM_H
