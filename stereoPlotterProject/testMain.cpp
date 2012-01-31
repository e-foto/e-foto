#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>

#include <QTreeView>

#include "LoadingScreen.h"
#include "EFotoManager.h"
#include "testTable.h"
#include "DemFeatures.h"

int main( int argc, char ** argv )
{

    QApplication efoto(argc, argv);
        efoto.setStyle("plastique");
        efoto.quitOnLastWindowClosed();

    qApp->processEvents();

    //br::uerj::eng::efoto::EFotoManager manager;
    //manager.setInterfaceType("Qt");

    br::uerj::eng::efoto::DemFeatures myFeatures;
    myFeatures.loadFeatures("/home/marts/Documentos/e-foto/eppsHomePage/FEICOES_UERJ_3.txt",1,false);
    QString txt = QString::fromStdString(myFeatures.getFeaturesList());

    //txt = ("Getting Started\tHow to familiarize yourself with Qt Designer\n\t\tLaunching Designer\tRunning the Qt Designer application\n\t\tThe User Interface\tHow to interact with Qt Designer\n\tConnection Editing Mode\tConnecting widgets together with signals and slots\n\t\tConnecting Objects\tMaking connections in Qt Designer\n\t\tEditing Connections\tChanging existing connections");
    QTreeView *treeViewer = new QTreeView;
    br::uerj::eng::efoto::TreeFeatures* tree = new br::uerj::eng::efoto::TreeFeatures(txt);
    treeViewer->setModel(tree);
    treeViewer->show();
    return efoto.exec();
}
