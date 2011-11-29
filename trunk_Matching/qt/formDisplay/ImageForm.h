#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include "ui_ImageLayout.h"
#include "ui_EfotoViewer.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"
#include <QMainWindow>
#include "SingleDisplay.h"

class ProjectUserInterface_Qt;

class EFotoViewer : public QMainWindow, public Ui::EVMainWindow
{
    Q_OBJECT

    SingleDisplay* sd;
    SingleToolsBar* tool;
    void closeEvent(QCloseEvent *);

public:
    EFotoViewer(QWidget *parent=0);

public slots:
    void loadImage(QString filename);
    void blockOpen();
    void blockSave();
    void blockMark();
};

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
    void startEFotoView();
    void metadataVisibleChanged(QString);

private:
  string tagXml;
  QString fileImagePath; QString fileImageName;
  QString lastPath;
  bool activeGNSS; bool activeINS;

};

#endif // IMAGEFORM_H
