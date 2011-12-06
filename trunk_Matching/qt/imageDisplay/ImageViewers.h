#ifndef IMAGEVIEWERS_H
#define IMAGEVIEWERS_H

#include <QMainWindow>
#include "ui_EfotoViewer.h"
#include "SingleDisplay.h"

class SingleViewer : public QMainWindow, public Ui::EVMainWindow
{
    Q_OBJECT

    SingleDisplay* sd;
    SingleToolsBar* tool;
    void closeEvent(QCloseEvent *);

public:
    SingleViewer(QWidget *parent=0);

public slots:
    void loadImage(QString filename);
    void loadImage(QImage* image);
    void loadImage(Matrix* image, bool isGrayscale = true);
    void blockOpen();
    void blockSave();
    void blockMark();
};


#endif // IMAGEVIEWERS_H
