#ifndef TERRAINFORM_H
#define TERRAINFORM_H

#include "ui_TerrainLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

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
	void fillLatDir(string hemi);

	/**
 * \brief Metodo que altera o valor do objeto correspondente a escolha do hemisferio ditado pela longitude
 * \param hemi : Texto contendo a inicial do hemisferio, a saber, "E" ou "W"
 */
	void fillLongDir(string hemi);

	/**
 * \brief Metodo que retorna um texto com o valor do hemisferio ditado pela latitude
 * \return string : Texto contendo a inicial do hemisferio, a saber, "N" ou "S"
 */
	string getLatDir();

	/**
 * \brief Metodo que retorna um texto com o valor do hemisferio ditado pela longitude
 * \return string : Texto contendo a inicial do hemisferio, a saber, "E" ou "W"
 */
	string getLongDir();

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
 * \brief Metodo que altera todos os objetos do formulario para convenção de data, hora etc. para o local especificado
 * \param locale : Objeto que informa o local
 */
	bool isForm(string formName);

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
