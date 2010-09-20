#ifndef IMAGESFORM_H
#define IMAGESFORM_H


#include "ui_ImagesLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

class ImagesForm : public AbstractForm, public Ui::ImagesView
{
    Q_OBJECT

public:
    ImagesForm(QWidget *parent=0);

    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
	bool isForm(string formName);

private slots:
    void emitSignal(int i);

signals:
    //emite um sinal com o valor do atributo key da tag image
    void clicked(int imageKey);

};

#endif // IMAGESFORM_H
