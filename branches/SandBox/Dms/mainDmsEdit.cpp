#include "DmsEdit.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

int main(int argc, char **argv)
{
    QApplication aplic(argc,argv);

    DmsEdit *dmsedit1= new DmsEdit();
    DmsEdit *dmsedit2= new DmsEdit();
    DmsEdit *dmsedit3= new DmsEdit();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(dmsedit1);
    layout->addWidget(dmsedit2);
    layout->addWidget(dmsedit3);

    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    widget->show();


    return aplic.exec();

}
