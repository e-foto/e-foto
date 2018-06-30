#ifndef IMAGEFORM_H
#define IMAGEFORM_H
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

#include "ui_ImageLayout.h"
#include "AbstractForm.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectUserInterface_Qt;

class ImageForm : public AbstractForm, public Ui::ImageView
{
	Q_OBJECT
    std::string tagXml;
	QString fileImagePath; QString fileImageName;
	QString lastPath;
	bool activeGNSS; bool activeINS;
    double flightDirection;

public:
	ProjectUserInterface_Qt* proj;
	SigmaFormController* gnssSigmaController;
	SigmaFormController* insSigmaController;


	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
    explicit ImageForm(QWidget *parent=0);

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
	void setFormLocale(QLocale locale);

	/**
 * \brief Metodo que esvazia ou zera todos os campos do formulário
 */
	void cleanForm();

private slots:
	/**
 * \brief Metodo que informa o nome do arquivo de imagem escolhido pelo usuario no formulario e obtem as dimensões das imagens
 * \return QString : Contem o nome do arquivo de imagem
 */
	QString loadImageFile();

	/**
 * \brief Metodo que dispara a exibição da imagem utilizando o visualizador de imagens simplificado.
 */
	void startSingleViewer();

	/**
 * \brief Metodo que mostra ou não os metadados baseado no campo correspondente ao nome do arquivo.
 * \param metadata : Conteudo do campo correspondente ao nome do arquivo
 * \deprecated Metodo em desuso atualmente
 */
	void metadataVisibleChanged(QString);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // IMAGEFORM_H
