#ifndef SENSORFORM_H
#define SENSORFORM_H
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
#include "ui_SensorLayout.h"
#include "AbstractForm.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
 * \file SensorForm.h
 * \class SensorForm
 * \brief Classe que controla a interface do formulario de Sensor
 * \author
 */
class SensorForm : public AbstractForm, public Ui::SensorView
{
	Q_OBJECT

public:
	EDomElement ede;
	bool radialAvailable,decenteredAvailable;
	SigmaFormController* calibratedSigmaController;
	SigmaFormController* fid0SigmaController;
	SigmaFormController* fid1SigmaController;
	SigmaFormController* fid2SigmaController;
	SigmaFormController* fid3SigmaController;
	SigmaFormController* fid4SigmaController;
	SigmaFormController* fid5SigmaController;
	SigmaFormController* fid6SigmaController;
	SigmaFormController* fid7SigmaController;
	SigmaFormController* radialSigmaController;
	SigmaFormController* decenteredSigmaController;
	SigmaFormController* principalSigmaController;

	/**
 * \brief Construtor do formulario
 * \param parent : QWidget pai do formulario
 */
    explicit SensorForm(QWidget *parent =0);

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
 * \brief Metodo que esvazia ou zera todos os campos do formulário
 */
	void clearForm();

	/**
 * \brief Metodo que altera todos os objetos do formulario para convenção de data, hora etc. para o local especificado
 * \param locale : Objeto que informa o local
 */
	void setFormLocale(QLocale locale);

	/**
 * \brief Metodo que mostra ou não mais quatro marcas fiduciais no formulario
 * \param state : booleana que controla a exibiçao ou não de quatro marcas fiduciais extra
 */
	void showLast4Fiducial(bool state);
public slots:

	/**
 * \brief Metodo que controla a exibiçao de campos para 4 ou 8 marcas fiduciais de acordo com a seleção do usuario
 * \param marks : codigo para exibiçao de 4 ou 8 marcas, marks=0 para 4 marcas fiduciais e marks=1 para 8 marcas fiduciais
 */
	void fiducialGroup(int marks);

	/**
 * \brief Metodo que altera o metodo de calculo da orientação interior e atualiza o formulario para tal
 * \param mode : codigo que informa o modo de calculo.
 */
	void calculationMode(int mode); //newSensor

	/**
 * \brief Metodo auxiliar para calcular e exibir automaticamente, baseado nos valores entrados pelo usuario, no formulario as dimensões do sensor
 */
	void updateSensorDiagonal();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SENSORFORM_H
