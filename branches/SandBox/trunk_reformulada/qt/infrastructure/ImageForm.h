#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include "ui_ImageLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"

class ProjectUserInterface_Qt;

class ImageForm : public AbstractForm, public Ui::ImageView
{
    Q_OBJECT

public:
    ProjectUserInterface_Qt* proj;
    ImageForm(QWidget *parent=0);
    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
    void setFormLocale(QLocale locale);
    void cleanForm();

    string getFileImagePath();
    string getFileImageName();
    bool isForm(string formName);
    SigmaFormController* gnssSigmaController;
    SigmaFormController* insSigmaController;
    void setIOAvailable(bool state);
    void setEOAvailable(bool state);


private slots:
    QString loadImageFile();
	void metadataVisibleChanged(QString);

private:
  string tagXml;
  QString fileImagePath, fileImageName;
  QString lastPath;
  bool activeGNSS, activeINS;

};

#endif // IMAGEFORM_H
