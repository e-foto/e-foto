#ifndef POINTFORM_H
#define POINTFORM_H

#include "ui_PointLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"

class PointForm : public AbstractForm , public Ui::Ponto
{
    Q_OBJECT

public:
    PointForm(QWidget *parent = 0);

	void setImageList(deque<int> myKeyList, deque<string> myNameList);
    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
	bool isForm(string formName);
	SigmaFormController* sigmaController;

private:
  string key;
  deque<string> imageNameList;
  deque<int> imageKeyList;
  void setType(string type);
  string getType();
};

#endif // POINTFORM_H
