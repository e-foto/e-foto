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

class EPolygon //: public QPolygonF
{
public:
	EPolygon();
	EPolygon(QList<QPointF *> points);

	//QPolygonF *polygon;

	bool isIntercepted(EPolygon pol1, EPolygon pol2, double &xi, double &yi);
	bool isIntercepted(EPolygon pol2, double &xi, double &yi);
	void setLabel(QString newLabel);
	void addPointF(QPointF *point);
	void addPointsF(QList<QPointF *> list);

	QString getLabel();
	QPointF * getPointF(int index);
	QList <QPointF*> getListPoint();

	//EPolygon& operator << (QPointF *point);

protected:
	QString label;
	QList<QPointF*> listPointF;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // EPOLYGON_H
