#include "GenericSystemDialog.h"

GenericSystemDialog::GenericSystemDialog(QWidget *parent)//:
        //QDialog(parent)
{
    setupUi(this);
    connect(okButton,SIGNAL(clicked()),SLOT(okClicked()));
    connect(cancelButton,SIGNAL(clicked()),SLOT(cancelClicked()));

}

void GenericSystemDialog::okClicked()
{
    emit ok(aDoubleSpin->value(),fDoubleSpin->value());

    close();
    delete this;
}

void GenericSystemDialog::cancelClicked()
{
    close();//emit cancel();
    delete this;
}

