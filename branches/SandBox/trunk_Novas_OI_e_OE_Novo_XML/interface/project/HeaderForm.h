#ifndef HEADERFORM_H
#define HEADERFORM_H

#include "ui_HeaderLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

class HeaderForm : public AbstractForm, public Ui::Form
{
	Q_OBJECT

public:
	HeaderForm(QWidget *parent = 0);

	void fillvalues(string values);
	string getvalues();
	void setReadOnly(bool state);
	bool isForm(string formName);

private slots:
	void metadataVisibleChanged(QString);
	void focusInEvent(QFocusEvent *);

};

#endif // HEADERFORM_H
