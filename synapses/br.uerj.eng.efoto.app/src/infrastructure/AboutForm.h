#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include "ui_AboutLayout.h"
#include <QDialog>

class AboutForm: public QDialog, public Ui::AboutForm
{
public:
    AboutForm();
};

#endif // ABOUTFORM_H
