#ifndef SENSORFORM_H
#define SENSORFORM_H

#include "ui_SensorLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"

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
	SensorForm(QWidget *parent =0);

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

	/**
	* \brief Metodo que esvazia ou zera todos os campos do formul�rio
	*/
	void clearForm();

	/**
	* \brief Metodo que altera todos os objetos do formulario para conven��o de data, hora etc. para o local especificado
	* \param locale : Objeto que informa o local
	*/
	void setFormLocale(QLocale locale);

	/**
	* \brief Metodo que mostra ou n�o mais quatro marcas fiduciais no formulario
	* \param state : booleana que controla a exibi�ao ou n�o de quatro marcas fiduciais extra
	*/
	void showLast4Fiductial(bool state);
public slots:

	/**
	* \brief Metodo que controla a exibi�ao de campos para 4 ou 8 marcas fiduciais de acordo com a sele��o do usuario
	* \param marks : codigo para exibi�ao de 4 ou 8 marcas, marks=0 para 4 marcas fiduciais e marks=1 para 8 marcas fiduciais
	*/
	void fiductialGroup(int marks);

	/**
	* \brief Metodo que altera o metodo de calculo da orienta��o interior e atualiza o formulario para tal
	* \param mode : codigo que informa o modo de calculo.
	*/
	void calculationMode(int mode); //newSensor

	/**
	* \brief Metodo auxiliar para calcular e exibir automaticamente, baseado nos valores entrados pelo usuario, no formulario as dimens�es do sensor
	*/
	void updateSensorDiagonal();
};

#endif // SENSORFORM_H
