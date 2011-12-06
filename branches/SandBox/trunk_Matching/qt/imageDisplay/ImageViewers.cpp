#include "ImageViewers.h"

SingleViewer::SingleViewer(QWidget* parent) : QMainWindow(parent)
{
    setupUi(this);
    sd = new SingleDisplay(this);
    setCentralWidget(sd);
    tool = new SingleToolsBar(sd, this);
    addToolBar(Qt::TopToolBarArea,tool);
    statusBar()->addWidget(tool->getInfo());
    this->showMaximized();
}

void SingleViewer::closeEvent(QCloseEvent *)
{
    delete tool;
    delete sd;
}

void SingleViewer::loadImage(QString filename)
{
    SingleScene* ss = (SingleScene*) sd->getCurrentScene();
    ss->loadImage(filename);
    sd->fitView();
}

void SingleViewer::loadImage(QImage *image)
{
    SingleScene* ss = (SingleScene*) sd->getCurrentScene();
    ss->loadImage(*image);
    sd->fitView();
}

void SingleViewer::loadImage(Matrix *image, bool isGrayscale)
{
    SingleScene* ss = (SingleScene*) sd->getCurrentScene();
    ss->loadImage(image, isGrayscale);
    sd->fitView();
}

void SingleViewer::blockOpen()
{
    tool->setOpenVisible(false);
}

void SingleViewer::blockSave()
{
    tool->setSaveVisible(false);
}

void SingleViewer::blockMark()
{
    tool->setMarkVisible(false);
}
