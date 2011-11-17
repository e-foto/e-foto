#ifndef DEGREELINEEDIT_H
#define DEGREELINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include "Dms.h"
#include <QKeyEvent>
#include <QEvent>
#include <QValidator>
#include <QDoubleValidator>


enum PositionValue{ NONE = -1 ,DEGREES, MINUTES, SECONDS};
enum DmsEditMode { DMS, DEG, RAD };


/**
* \file DmsEdit.h
* \class DmsEdit
* \brief Classe que controla um widget para mostrar valores no formato de angulos
* \copyright E-Foto group
* \author Paulo André Batista Pupim e Irving Badolato
*/

class DmsEdit : public QLineEdit
{
    Q_OBJECT
protected:
    Dms *dmsMin;
    Dms *dmsMax;
    Dms *oldDegMinSecLine;
    double radMax,radMin,degMin,degMax;
	Dms *degMinSecLine;
	double radValue;
	double degValue;
	int decimals;
	QString previousValue;
	QDoubleValidator *degValidator;
	QDoubleValidator *radValidator;
	DmsEditMode mode;

public:

	/**
	* \brief Construtor da classe
	* \param parent : Widget pai
	*/
    DmsEdit(QWidget *parent=0);

	/**
	* \brief Metodo para alterar o valor de casas decimais do campo segundos quando no modo DMS
	* \param newDecimals : Numero de casas decimais
	*/
    void setDecimals(int newDecimals);

	/**
	* \brief Metodo para retornar o numero de casas decimais do campo segundos quando no modo DMS
	* \return int : Numero de casas decimais
	*/
    int getDecimals();

	/**
	* \brief Metodo para retornar o modo do widget, podendo ser DMS, grau decimal e radiano
	* \return DmsEditMode : Enumerador que representa o modo de operação do widget
	*/
    DmsEditMode getDmsEditMode();

	/**
	* \brief Metodo que retorna o valor do angulo em radiano
	* \return double : Valor do angulo em radiano
	*/
    double getRadianValue();

	/**
	* \brief Metodo que retorna o valor do angulo em grau decimal
	* \return double : Valor do angulo em grau decimal
	*/
    double getDegreeValue();

	/**
	* \brief Metodo que retorna um objeto Dms correspondente ao valor atual do widget
	* \return Dms : Objeto Dms
	*/
    Dms* getDmsValue();

	/**
	* \brief Metodo que altera o valor minimo aceito pelo widget
	* \param degree  : Valor minimo do grau
	* \param minutes : Valor minimo do minuto
	* \param seconds : Valor minimo do segundo
	* \param signal  : Booleano que informa se o valor é positivo ou negativo
	*/
    void setDmsMinimum(int degree,int minutes, double seconds, bool signal);

	/**
	* \brief Metodo que altera o valor maximo aceito pelo widget
	* \param degree  : Valor maximo do grau
	* \param minutes : Valor maximo do minuto
	* \param seconds : Valor maximo do segundo
	* \param signal  : Booleano que informa se o valor é positivo ou negativo
	*/
    void setDmsMaximum(int degree,int minutes, double seconds, bool signal);

	/**
	* \brief Metodo que retorna o valor minimo aceito pelo widget em graus decimais
	* \return double : Valor minimo
	*/
    double getMinimumDeg();

	/**
	* \brief Metodo que retorna o valor maximo aceito pelo widget em graus decimais
	* \return double : Valor maximo
	*/
    double getMaximumDeg();

	/**
	* \brief Metodo que retorna o valor minimo aceito pelo widget em radianos
	* \return double : Valor minimo
	*/
    double getMinimumRad();

	/**
	* \brief Metodo que retorna o valor maximo aceito pelo widget em radianos
	* \return double : Valor maximo
	*/
    double getMaximumRad();

	/**
	* \brief Metodo que retorna o valor minimo aceito pelo widget representado no objeto Dms
	* \return double : Valor minimo
	*/
    Dms * getMinimumDms();

	/**
	* \brief Metodo que retorna o valor maximo aceito pelo widget representado no objeto Dms
	* \return double : Valor maximo
	*/
    Dms * getMaximumDms();


protected:

	/**
	* \brief Metodo sobrescrito da QLineEdit para atender as necessidades da classe no que diz respeito a redirecionamento de eventos
	* \param objeto : Objeto do Qt
	* \param evento : Evento ocorrido no widget
	* \return bool  : informa se o evento foi tratado
	*/
	bool eventFilter(QObject* objeto, QEvent* evento);

	/**
	* \brief Metodo sobrescrito da QLineEdit para atender os eventos de pressionamento de tecla
	* \param evento : Evento de pressionamento de tecla a ser tratado
	*/
    void keyPressEvent(QKeyEvent *evento);

	/**
	* \brief Metodo sobrescrito da QLineEdit para atender os eventos de entrada de foco do widget
	* \param evento : Evento de obtenção de foco pelo widget
	*/
	void focusInEvent(QFocusEvent *evento);

	/**
	* \brief Metodo sobrescrito da QLineEdit para atender os eventos de saida de foco do widget
	* \param evento : Evento de perda de foco pelo widget
	*/
	void focusOutEvent(QFocusEvent *evento);

private:

	/**
	* \brief Metodo auxiliar que informa a posiçao atual do cursor do teclado no widget
	* \return PositionValue : Enumerador que indica em que campo esta o cursor do teclado
	*/
	PositionValue positionValue();

	/**
	* \brief Metodo auxiliar que informa a posiçao do cursor do teclado no widget
	* \param pos : Valor posiçao do cursor
	* \return PositionValue : Enumerador que indica em que campo esta o cursor do teclado
	*/
    PositionValue positionValue(int pos);

public slots:

	/**
	* \brief Metodo que valida o valor apresentado no widget
	*/
    void validate();

	/**
	* \brief Metodo que diminui o valor do campo em uma unidade, desde que o modo esteja em DMS
	*/
    void stepDown();

	/**
	* \brief Metodo que aumenta o valor do campo em uma unidade, desde que o modo esteja em DMS
	*/
    void stepUp();

	/**
	* \brief Metodo que aumenta ou diminui o valor do campo em uma unidade, desde que o modo esteja em DMS
	* \param steps : Valor a ser acrescentado, se steps>0, ou a ser decrementado, se steps <0
	*/
    void stepBy(int steps);

	/**
	* \brief Metodo que seleciona um campo segundo o valor informado no parametro pos
	* \param pos : Enumerador indicando o campo a ser selecionado
	*/
    void selectField(PositionValue pos);

	/**
	* \brief Metodo auxiliar que determina de que campo para qual campo o cursor do teclado mudou
	* \param oldPos : Valor inicial da posiçao do cursor
	* \param newPos : Valor final da posiçao do cursor
	*/
    void changedField(int oldPos, int newPos);

	/**
	* \brief Metodo que altera o modo em que o widget apresenta os angulos
	* \param newMode : Enumerador que representa o modo de apresentação dos valores
	*/
    void setDmsEditMode(DmsEditMode newMode);

	/**
	* \brief Metodo auxiliar para atualizar o valor apresentado pelo widget
	* \param newValue : Texto contendo o valor a ser mostrado no widget, se for vazio simplesmente mostra o valor armazenado pelo objeto.
	*/
    void updateValue(QString newValue="");

};

#endif // DEGREELINEEDIT_H
