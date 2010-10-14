#ifndef POINTSFORM_H
#define POINTSFORM_H

#include "ui_PointsLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

class PointsForm : public AbstractForm, public Ui::PointsForm
{
    Q_OBJECT

public:
    PointsForm(QWidget *parent=0);

    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
        bool isForm(string formName);

private slots:
    void emitSignal(int i);

signals:
    //emite um sinal com o valor do atributo key da tag image
    void clicked(int pointKey);

};
#endif //POINTSFORM_H
