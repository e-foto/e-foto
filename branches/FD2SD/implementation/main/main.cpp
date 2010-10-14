#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>
#include "../../interface/io/IOUserInterface_Qt.h"
#include "../../control/IOManager.h"
#include "../../control/ProjectManager.h"
#include "../../control/EFotoManager.h"

int main( int argc, char ** argv )
{	
    QApplication efoto(argc, argv);
    QApplication::setStyle("plastique");

    //Code for translations
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("FormProject_") + locale);
    efoto.installTranslator(&translator);
    
    efoto.connect(&efoto, SIGNAL(lastWindowClosed()), &efoto, SLOT(quit()));

    EFotoManager manager;
    manager.setInterfaceType("Qt");
    manager.exec();
    
    return 0;
}
