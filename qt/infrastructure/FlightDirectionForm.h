#ifndef FLIGHTDIRECTIONFORM_H
#define FLIGHTDIRECTIONFORM_H

#include "ui_FlightDirectionForm.h"
#include <QEvent>
#include <QCloseEvent>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file FlightDirectionForm.h
* \class FlightDirectionform
* \brief Classe que controla o widget usado para indicar direçao de voo
* \copyright E-foto group
* \author Paulo Andre Batista Pupim
*/
class FlightDirectionForm : public QWidget, public Ui::FlightDirectionForm
{
	Q_OBJECT

public:

	QList<int> markedImages;
	QIcon markedIcon, unmarkedIcon;
	bool autoPassMode;
	FlightDirectionForm(QStringList comboBoxItens,QList<int> newMarkedImages,QWidget *parent = 0);


	/**
	* \brief Construtor padrao
	* \param parent : ponteiro para o QWidget pai
	*/
	FlightDirectionForm(QWidget *parent = 0);

	bool getPassMode();
	void setPassMode(bool status);
	bool isMarked(int comboIndex);
	void setMarkedImages(QList<int> indexesCombo);
	QList<int> getMarkedImages();
	QStringList getStringMarkedImages();
	int getIndexCombo(QString comboText);

public slots:

	/**
	* brief Metodo que emite um sinal com o valor do angulo, em radianos, quando o botao accept e acionado, e o texto da QComboBox contendo o arquivo da imagem
	*/
	void acceptValues();

	/**
	* \brief Metodo que altera o valor do angulo contido na QSpinBox
	* \param value : Valor do angulo em graus
	*/
    void setSpinValue(/*int value*/);

	/**
	* \brief Metodo que altera o valor do angulo apontado no QDial
	* \param value : Valor do angulo em graus
	*/
    void setDialValue(/*int value*/);

	void updateMarkedLabel(int comboIndex);

    void updateItemCombo(/*int comboIndex*/);

protected slots:
	bool eventFilter(QObject * obj, QEvent * event);
	void closeEvent(QCloseEvent *event);

signals:

	/**
	* \brief Sinal contendo o texto com o arquivo de imagem e o valor do angulo da direçao de voo em radianos
	*/
	void valuesFlightDirectionForm(QString,double);

	void markedImagesList(QList<int>,QStringList);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // FLIGHTDIRECTIONFORM_H
