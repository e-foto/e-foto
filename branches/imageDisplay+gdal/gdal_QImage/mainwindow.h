#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

#include "gdal.h"
#include <gdal_priv.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

QImage* img;
GDALDataset* _dataset;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_load_clicked();

    void on_show_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
