#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>
#include "LoadingScreen.h"
#include "EFotoManager.h"


int main( int argc, char ** argv )
{

    QApplication efoto(argc, argv);
	efoto.setStyle("plastique");
	efoto.quitOnLastWindowClosed();
/*
    //Code for translations
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("FormProject_") + locale);
    efoto.installTranslator(&translator);

    efoto.connect(&efoto, SIGNAL(lastWindowClosed()), &efoto, SLOT(quit()));
*/
	//LoadingScreen::instance().show();
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
