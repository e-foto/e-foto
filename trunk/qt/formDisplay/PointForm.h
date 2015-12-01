#ifndef POINTFORM_H
#define POINTFORM_H
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

#include "ui_PointLayout.h"
#include "AbstractForm.h"
//#include "SigmaForm.h"


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
    std::string key;
    std::deque<std::string> imageNameList;
    std::deque<int> imageKeyList;

public:
	SigmaFormController* sigmaController;

	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
    explicit PointForm(QWidget *parent = 0);

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
 * \brief Metodo que informa se o formulario é o mesmo do parametro formName
 * \param formName : nome do formulario
 * \return bool : Verdadeiro se formName é esse formulario, falso caso contrário
 */
    bool isForm(std::string formName);

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
    void setImageList(std::deque<int> myKeyList, std::deque<std::string> myNameList);

	/**
 * \brief Altera o objeto(QComboBox) que indica o tipo do ponto cadastrado
 * \param type : Texto informando o tipo do ponto, podendo ser: "control", "photogrammetric" ou "checking"
 */
    void setType(std::string type);

	/**
 * \brief Metodo que retorna o tipo do ponto do formulario
 * \return std::string : Contem o texto informando o tipo do ponto
 */
    std::string getType();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // POINTFORM_H
