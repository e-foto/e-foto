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

	Matrix N1(780,5790);
	Matrix N2(5790,780);

	timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	for(int i=0;i<4;i++)
		N2*N1;
	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("%.6lf seconds elapsed\n", t2-t1);


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
