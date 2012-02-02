#include <iostream>

using namespace std;

#include <QApplication>
#include <QTranslator>

#include <QTreeView>

#include "LoadingScreen.h"
#include "EFotoManager.h"
#include "testTable.h"
#include "DemFeatures.h"
#include "ImageViewers.h"

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

    QTreeView *treeViewer = new QTreeView;
    br::uerj::eng::efoto::TreeFeatures* tree = new br::uerj::eng::efoto::TreeFeatures(txt);
    treeViewer->setModel(tree);
    treeViewer->show();

    br::uerj::eng::efoto::SeparatedStereoViewer* imageViewer = new br::uerj::eng::efoto::SeparatedStereoViewer();
    imageViewer->loadLeftImage("/home/marts/EFOTO/Develop/data_and_images/1997_016_300dpi.bmp");
    imageViewer->loadRightImage("/home/marts/EFOTO/Develop/data_and_images/1997_017_300dpi.bmp");
    imageViewer->setFeatures(&myFeatures);
    return efoto.exec();
}
