#include "Photogrammetry.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace teste {
namespace app {


Photogrammetry::Photogrammetry()
{
}

Photogrammetry::~Photogrammetry()
{
}

bool Photogrammetry::start()
{
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

Q_EXPORT_PLUGIN2(br.uerj.eng.efoto.teste.app, Photogrammetry)

}
}
}
}
}
}	//namespace
