#include "Photogrammetry.h"
#include <QApplication>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace app {


Photogrammetry::Photogrammetry()
{
}

Photogrammetry::~Photogrammetry()
{
}

bool Photogrammetry::start()
{
    qApp->setStyle("plastique");
    manager.setInterfaceType("Qt");
    manager.exec();

	//TODO Automatically generated implementation
	return true;
}

bool Photogrammetry::stop()
{
	//TODO Automatically generated implementation
	return true;
}

// IPhotogrammetry interface:
#ifdef EFOTO_APP
Q_EXPORT_PLUGIN2(br.uerj.eng.efoto.app, Photogrammetry)
#endif //EFOTO_APP

}
}
}
}
}	//namespace
