#include "DmsEdit.h"
#include <QApplication>

#include "tester.h"

int main(int argc, char **argv)
{
    QApplication aplic(argc,argv);

    Tester *test= new Tester();

    test->show();

    return aplic.exec();

}
