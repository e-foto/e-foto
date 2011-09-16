#ifndef TERRAINFORM_H
#define TERRAINFORM_H

#include "ui_TerrainLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

class TerrainForm : public AbstractForm, public Ui::Terreno
{
    Q_OBJECT
    void fillLatDir(string);
    void fillLongDir(string);
    string getLatDir();
    string getLongDir();
    void setFormLocale(QLocale locale);
    void cleanForm();


public:
        TerrainForm(QWidget *parent = 0);

        void fillvalues(string values);
        string getvalues();
        void setReadOnly(bool state);
        bool isForm(string formName);

    public slots:
        /*void validatorLat();
        void validatorLong();*/
        void validatorAlt(double);
};

#endif // TERRAINFORM_H
