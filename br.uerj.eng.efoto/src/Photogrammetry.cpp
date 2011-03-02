#include "Photogrammetry.h"

#include <QApplication>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


Photogrammetry::Photogrammetry()
{
}

Photogrammetry::~Photogrammetry()
{
}

bool Photogrammetry::start()
{
	qDebug("br.uerj.eng.efoto::start");

	QApplication::setStyle("plastique");

        //connect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(stop()));
        //qApp->setQuitOnLastWindowClosed( false );

	manager.setInterfaceType("Qt");
	manager.exec();

	return true;
}

bool Photogrammetry::stop()
{
	qDebug("br.uerj.eng.efoto::stop");
        manager.stop();
	return true;
}

cortex::ISynapse* Photogrammetry::getImplementation()
{
	return this;
}


// IPhotogrammetry interface:


Q_EXPORT_PLUGIN2(br.uerj.eng.efoto, Photogrammetry)

}
}
}
}	//namespace
