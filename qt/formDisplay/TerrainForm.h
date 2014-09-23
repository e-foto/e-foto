#ifndef TERRAINFORM_H
#define TERRAINFORM_H
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

#include "ui_TerrainLayout.h"
#include "AbstractForm.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class TerrainForm : public AbstractForm, public Ui::Terreno
{
	Q_OBJECT

	/**
 * \brief Metodo que altera o valor do objeto correspondente a escolha do hemisferio ditado pela latitude
 * \param hemi : Texto contendo a inicial do hemisferio, a saber, "N" ou "S"
 */
    void fillLatDir(std::string hemi);

	/**
 * \brief Metodo que altera o valor do objeto correspondente a escolha do hemisferio ditado pela longitude
 * \param hemi : Texto contendo a inicial do hemisferio, a saber, "E" ou "W"
 */
    void fillLongDir(std::string hemi);

	/**
 * \brief Metodo que retorna um texto com o valor do hemisferio ditado pela latitude
 * \return std::string : Texto contendo a inicial do hemisferio, a saber, "N" ou "S"
 */
    std::string getLatDir();

	/**
 * \brief Metodo que retorna um texto com o valor do hemisferio ditado pela longitude
 * \return std::string : Texto contendo a inicial do hemisferio, a saber, "E" ou "W"
 */
    std::string getLongDir();

	/**
 * \brief Metodo que altera todos os objetos do formulario para convenção de data, hora etc. para o local especificado
 * \param locale : Objeto que informa o local
 */
	void setFormLocale(QLocale locale);

	/**
 * \brief Metodo que esvazia ou zera todos os campos do formulário
 */
	void cleanForm();

public:

	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
	TerrainForm(QWidget *parent = 0);

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
    bool isForm(std::string formName);

public slots:
	/**
 * \brief Metodo auxiliar para não deixar o usuario colocar a altitude media menor que altitude minima ou maior que a altitude maxima
 */
	void validatorAlt();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // TERRAINFORM_H
