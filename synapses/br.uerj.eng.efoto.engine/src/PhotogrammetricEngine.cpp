#include "PhotogrammetricEngine.h"

#include "Project.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace engine {


PhotogrammetricEngine::PhotogrammetricEngine()
{
}

PhotogrammetricEngine::~PhotogrammetricEngine()
{
}

bool PhotogrammetricEngine::start()
{
	//TODO Automatically generated implementation
	return true;
}

bool PhotogrammetricEngine::stop()
{
	//TODO Automatically generated implementation
	return true;
}

IProject* PhotogrammetricEngine::getProject()
{
    project.push_front((IProject*) new Project());
    return project.at(0);
}

// IPhotogrammetricEngine interface:

Q_EXPORT_PLUGIN2(br.uerj.eng.efoto.engine, PhotogrammetricEngine)

}
}
}
}
}	//namespace
