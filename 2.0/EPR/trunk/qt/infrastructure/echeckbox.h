#ifndef ECHECKBOX_H
#define ECHECKBOX_H

#include<QCheckBox>

class ECheckBox : public QCheckBox
{
public:
	ECheckBox(QWidget* parent);
	QList <QCheckBox*> listaCheckBox;

	void controlCheckBox(bool status = true);
};

#endif // ECHECKBOX_H
