#ifndef BR_UERJ_ENG_EFOTO_APPV1_TEST_TEST_PROXY
#define BR_UERJ_ENG_EFOTO_APPV1_TEST_TEST_PROXY

#include "ISynapseTest.h"
#include <QObject>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {
namespace appV1 {
namespace test {

class Q_DECL_EXPORT TestProxy: public QObject, public cortex::ISynapseTest
{
	Q_OBJECT
	Q_INTERFACES(cortex::ISynapseTest)

public:
	TestProxy();
	virtual ~TestProxy();

	// ISynapseTest interface:
	virtual int run( const QStringList& arguments );
};

}
}
}
}
}
} //namespace

#endif	//BR_UERJ_ENG_EFOTO_APPV1_TEST_TEST_PROXY
