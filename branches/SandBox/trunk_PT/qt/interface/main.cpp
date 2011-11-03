#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>
#include "LoadingScreen.h"
#include "EFotoManager.h"

#include "Matrix.h"
#include <sys/time.h>
//for tests
//#include "../../control/XmlUpdater.h"

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
/*
	Matrix mat(3,3);
	for (int i=1;i<4;i++)
	{
		for (int j=1;j<4;j++)
		mat.set(i,j,rand()*255);
	}
	Matrix iden;
	iden.identity(3);
	(mat&iden).triang().reduction().show('g',8,"teste");
*/
	qApp->processEvents();
    EFotoManager manager;
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
