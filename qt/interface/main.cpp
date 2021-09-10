#include <iostream>

#include <QSurfaceFormat>
#include <QApplication>
#include <QTranslator>
#include "LoadingScreen.h"
#include "EFotoManager.h"

int main( int argc, char ** argv )
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication efoto(argc, argv);
    //efoto.setStyle("plastique"); Excluído o código em Setembro de 2021
    efoto.setStyleSheet("QToolTip { color: #000000; background-color: #ffffdc; border: none; }");
    efoto.quitOnLastWindowClosed();
    QSurfaceFormat format;
    //format.setDepthBufferSize(24); Excluído o código em Setembro de 2021
    //QSurfaceFormat::setDefaultFormat(format); Excluído o código em Setembro de 2021

    qApp->processEvents();
    br::uerj::eng::efoto::EFotoManager manager;
    manager.setInterfaceType("Qt");
    if (argc==2){
        ++(argv);
        manager.exec(*argv);
    }
    else
    {
        manager.exec();
    }

    return efoto.exec();

}
