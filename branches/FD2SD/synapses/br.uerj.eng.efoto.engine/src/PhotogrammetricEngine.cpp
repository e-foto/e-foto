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

IProjectPtr PhotogrammetricEngine::getProject()
{
    IProjectPtr proj = IProjectPtr((IProject*) new Project());
    project.push_front(proj);
    return proj;
}

// IPhotogrammetricEngine interface:

Q_EXPORT_PLUGIN2(br.uerj.eng.efoto.engine, PhotogrammetricEngine)

}
}
}
}
}	//namespace
