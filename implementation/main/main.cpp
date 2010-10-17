#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>
#include "ProjectManager.h"
#include "EFotoManager.h"

int main( int argc, char ** argv )
{	
    QApplication efoto(argc, argv);
    QApplication::setStyle("plastique");

    efoto.connect(&efoto, SIGNAL(lastWindowClosed()), &efoto, SLOT(quit()));

    EFotoManager manager;
    manager.setInterfaceType("Qt");
    manager.exec();
    
    return 0;
}
