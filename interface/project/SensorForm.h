#ifndef SENSORFORM_H
#define SENSORFORM_H

#include "ui_SensorLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

class SensorForm : public AbstractForm, public Ui::SensorView
{
    Q_OBJECT

public:
    EDomElement ede;
    SensorForm(QWidget *parent =0);

    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
    bool isForm(string formName);
    void showLast4Fiductial(bool state);

    public slots:
            void detectorGroup(int);
            void fiductialGroup(int);
            void setSigmasVisible(bool);

};

#endif // SENSORFORM_H
