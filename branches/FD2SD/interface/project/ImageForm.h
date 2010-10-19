#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include "ui_ImageLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
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
	SigmaFormController* gnssSigmaController;
	SigmaFormController* insSigmaController;

private slots:
    QString loadImageFile();
	void metadataVisibleChanged(QString);

private:
  string tagXml;
  QString fileImagePath, fileImageName;
  bool activeGNSS, activeINS;

};

#endif // IMAGEFORM_H