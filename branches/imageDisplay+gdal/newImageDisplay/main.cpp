#include <iostream>
#include <gdal/gdal.h>
#include <QApplication>
#include <QTranslator>
#include "ImageViewers.h"

int main( int argc, char ** argv )
{
    QApplication efoto(argc, argv);
    efoto.setStyle("plastique");
    efoto.setStyleSheet("QToolTip { color: #000000; background-color: #ffffdc; border: none; }");
    efoto.quitOnLastWindowClosed();

    br::uerj::eng::efoto::SingleViewer viewer;

    //br::uerj::eng::efoto::StereoViewer sviewer;

    //br::uerj::eng::efoto::SeparatedStereoViewer ssviewer;

    viewer.show();
    //sviewer.show();
    //ssviewer.show();

    return efoto.exec();

}
