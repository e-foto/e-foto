#ifndef EPOLYGON_H
#define EPOLYGON_H

#include <QPolygonF>
#include <QString>
#include <QPointF>
#include <QList>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class EPolygon
{
public:
	EPolygon();
    explicit EPolygon(QList<QPointF *> points);

	void setLabel(QString newLabel);

protected:
	QString label;
	QList<QPointF*> listPointF;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EPOLYGON_H
