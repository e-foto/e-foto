#ifndef EDOMVALIDATOR_H
#define EDOMVALIDATOR_H

#include "EDom.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class EDomValidator
{
public:
	static bool validateProject();
	static bool validateHeader(string xml);
	static bool validateTerrain(string xml);
	static bool validateFlight(string xml);
	static bool validateSensor(string xml);
	static bool validateImage(string xml);
	static bool validatePoint(string xml);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EDOMVALIDATOR_H
