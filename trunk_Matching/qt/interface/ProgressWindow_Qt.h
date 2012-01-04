#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include "ui_ProgressWindow.h"

class ProgressWindow : public QWidget
{
	Q_OBJECT

public:
    ProgressWindow(QWidget *parent = 0);
    void setProgress(int prog);
    void setDescription(QString desc);
    void setAllowClose(bool sac) { allow_close = sac; };

private:
    Ui::ProgressWindow ui;
    bool allow_close;
    void closeEvent(QCloseEvent *e);

};

#endif
