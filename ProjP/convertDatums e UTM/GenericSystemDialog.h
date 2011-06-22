#ifndef GENERICSYSTEMDIALOG_H
#define GENERICSYSTEMDIALOG_H

#include "ui_GenericSystemDialog.h"
#include <QDialog>

class GenericSystemDialog: public QDialog, public Ui::genericDialog
{
    Q_OBJECT
public:
    GenericSystemDialog(QWidget *parent=0);

    //~GenericSystemDialog();

public slots:
    void okClicked();
    void cancelClicked();

signals:
    void ok(double a, double f);
    void cancel();
};

#endif // GENERICSYSTEMDIALOG_H
