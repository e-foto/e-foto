#include "TestProxy.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace app {
namespace test {


TestProxy::TestProxy()
{
}

TestProxy::~TestProxy()
{
}

int TestProxy::run( const QStringList& arguments )
{
	//TODO Automatically generated implementation
	//You probably want to create some test and then do:
	//	SomeTest someTestObject;
	//	int result QTest::qExec(someTestObject, arguments);
	//	...
	//	return result;
	return true;
}

Q_EXPORT_PLUGIN2(br.uerj.eng.efoto.app.test, TestProxy)

}
}
}
}
}
}	//namespace
