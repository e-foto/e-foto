#include "EPolygon.h"
#include <QPolygon>
#include <QDebug>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

EPolygon::EPolygon()//:QPolygonF()
{
	setLabel("Default");
}

EPolygon::EPolygon(QList<QPointF *>points)
{
	listPointF=points;
	setLabel("Default");
}

void EPolygon::setLabel(QString newLabel)
{
	label=newLabel;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
