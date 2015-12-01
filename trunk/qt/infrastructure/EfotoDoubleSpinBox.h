#ifndef EFOTODOUBLESPINBOX_H
#define EFOTODOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QEvent>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/**
* \file EfotoDoubleSpinBox.h
* \class EfotoDoubleSpinBox
* \brief Classe reimplentada de QDoubleSpinBox ate o momento apenas para trocar o separador decimal para o separador decimal do seu local
* \copyright E-foto group
* \author Paulo Andre Batista Pupim e Irving Badolato
*/
class EfotoDoubleSpinBox: public QDoubleSpinBox
{

public:

	/**
	* \brief Construtor default
	* \param parent: Ponteiro para objeto QWidget pai
	*/
    explicit EfotoDoubleSpinBox(QWidget *parent=0);

	/**
	* \brief Evento reimplementado para trocar ',' por '.' e vice-versa dependendo do local em que o usuario esta.
	* \param Objeto de evento contendo as informaçoes sobre as teclas pressionadas
	*/
	void keyPressEvent(QKeyEvent *event);

	/**
	* \brief Metodo reimplementado para desviar os eventos de teclado
	* \param QObject : Objeto que gerou um evento
	* \param QEvent : Evento gerado pelo objeto
	*/
	bool eventFilter(QObject *, QEvent *);


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EFOTODOUBLESPINBOX_H
