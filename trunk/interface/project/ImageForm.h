#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include "ui_ImageLayout.h"
#include "AbstractForm.h"
#include "EDom.h"

class ImageForm : public AbstractForm, public Ui::ImageView
{
    Q_OBJECT

public:
    ImageForm(QWidget *parent=0);
    
    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
    string getFileImagePath();
    string getFileImageName();
	bool isForm(string formName);

private slots:
    QString loadImageFile();

private:
  string tagXml;
  QString fileImagePath, fileImageName;

};

#endif // IMAGEFORM_H
