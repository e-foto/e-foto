#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>
#include "LoadingScreen.h"
#include "EFotoManager.h"
#include "ReportManager.h"
#include "Project.h"


int main( int argc, char ** argv )
{

    QApplication efoto(argc, argv);
	efoto.setStyle("plastique");
    efoto.quitOnLastWindowClosed();

    qApp->processEvents();

    br::uerj::eng::efoto::EFotoManager manager;
    manager.setInterfaceType("Qt");
    manager.exec();

	return efoto.exec();
}
