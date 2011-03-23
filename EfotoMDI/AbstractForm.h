#ifndef ABSTRACTFORM_H
#define ABSTRACTFORM_H

#include <QWidget>
#include <string>
using namespace std;

class AbstractForm : public QWidget
{
	Q_OBJECT

public:
	AbstractForm(QWidget *parent = 0) : QWidget(parent) {}

	//virtual void fillvalues(string values) = 0;
	//virtual string getvalues() = 0;
	virtual void setReadOnly(bool state) = 0;
	//virtual bool isForm(string formName) = 0;
};



#endif // ABSTRACTFORM_H
