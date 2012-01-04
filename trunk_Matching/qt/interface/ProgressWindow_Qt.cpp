#include <QtGui>
#include "ProgressWindow_Qt.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QWidget(parent)
{
     ui.setupUi(this);

     // Center window
     QDesktopWidget *desktop = QApplication::desktop();
     int Cx,Cy;
     QRect rect = geometry();
     Cx = (desktop->width() - rect.width())/2;
     Cy = (desktop->height() - rect.height())/2;
     move(Cx,Cy);

     allow_close = true;
}

void ProgressWindow::setProgress(int prog)
{
     ui.progressBar->setValue(prog);
}

void ProgressWindow::setDescription(QString desc)
{
     ui.label->setText(desc);
}

void ProgressWindow::closeEvent(QCloseEvent *e)
{
    if (!allow_close)
    {
        e->ignore();
        return;
    }
}
