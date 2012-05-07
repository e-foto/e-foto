#ifndef EFOTODOUBLESPINBOX_H
#define EFOTODOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QEvent>

class EfotoDoubleSpinBox: public QDoubleSpinBox
{
public:
	EfotoDoubleSpinBox(QWidget *parent=0);
	void keyPressEvent(QKeyEvent *event);

	bool eventFilter(QObject *, QEvent *);


};

#endif // EFOTODOUBLESPINBOX_H
