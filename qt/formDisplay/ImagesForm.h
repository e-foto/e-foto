#ifndef IMAGESFORM_H
#define IMAGESFORM_H
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

#include "ui_ImagesLayout.h"
#include "AbstractForm.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * \file ImagesForm.h
 * \class ImagesForm
 * \brief Classe que controla a interface do formulario de Imagens
 * \author
 */
class ImagesForm : public AbstractForm, public Ui::ImagesView
{
	Q_OBJECT

public:
	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
    explicit ImagesForm(QWidget *parent=0);

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
 * \brief Metodo que informa no formulario se a orientaçao interior foi feita ou não para cada imagem
 * \param xmlIOs : string informando se o estado da orientaçao interior de todas as imagens cadastradas no projeto
 */
    void setIOsAvailable(std::string xmlIOs);

	/**
 * \brief Metodo que informa no formulario se a orientaçao exterior foi feita ou não para cada imagem
 * \param xmlIOs : string informando se o estado da orientaçao exterior de todas as imagens cadastradas no projeto
 */
    void setEOsAvailable(std::string xmlEOs);

private slots:

	/**
 * \brief Metodo que envia um sinal toda vez que uma coluna/campo de uma imagem é clicada
 * \param i : Valor numérico da linha da tabela contendo as imagens
 */
	void emitSignal(int i);

signals:

	/**
 * \brief Sinal emitido informando qual a linha em que a tabela foi clicada
 * \param index : Valor numérico da linha em que a tabela de imagens foi clicado
 */
	void clicked(int index);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGESFORM_H
