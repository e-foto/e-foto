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
	//polygon= new QPolygon();
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

QString EPolygon::getLabel()
{
	return label;
}

void EPolygon::addPointF(QPointF *point)
{
	listPointF << point;
}

void EPolygon::addPointsF(QList<QPointF*> list)
{
	listPointF << list;
}

QPointF * EPolygon::getPointF(int index)
{
	if (index<=listPointF.size() && !listPointF.isEmpty())
		return listPointF.at(index);
	else
		return NULL;
}

QList<QPointF*> EPolygon::getListPoint()
{
	return listPointF;
}

/*
EPolygon& EPolygon::operator <<(QPointF *point)
{
 listPointF << point;
}
*/

bool EPolygon::isIntercepted(EPolygon pol2, double &xi, double &yi)
{
	//for (int i=0;i<4;i++)
	{
		//printf("[%d]: %d ; %d\n",i,this->point(i).x(),this->point(i).y());

	}
	//for (int i=0;i<4;i++)
	{
		//printf("[%d]: %d ; %d\n",i,pol2.point(i).x(),pol2.point(i).y());
	}

	for (int i=0; i<this->getListPoint().size()-1; i++)
	{
		double x1 = this->getPointF(i)->x();
		double x2 = this->getPointF(i+1)->x();

		double y1 = this->getPointF(i)->y();
		double y2 = this->getPointF(i+1)->y();

		double a = y1-y2;
		double b = x2-x1;
		double c = x1*y2-x2*y1;

		//qDebug("Pol1: %s \t%.3fx + %.3fy + %.3f",this->getLabel().toStdString().c_str(),a,b,c);

		for (int j=0; j<pol2.getListPoint().size()-1; j++)
		{
			double x1t = pol2.getPointF(j)->x();
			double x2t = pol2.getPointF(j+1)->x();

			double y1t = pol2.getPointF(j)->y();
			double y2t = pol2.getPointF(j+1)->y();

			double at = y1t-y2t;
			double bt = x2t-x1t;
			double ct = x1t*y2t-x2t*y1t;

			//qDebug("Pol1: %s \t%.3fx + %.3fy + %.3f",pol2.getLabel().toStdString().c_str(),at,bt,ct);

			xi = ( bt*c - ct*b )/( b*at - bt*a );

			yi = (-c-a*xi)/b;
			//qDebug()<< "Xcalculado = "<< ( bt*c - ct*b )/( b*at - bt*a );

			// se um valor de x é valido então obviamente está entre os extremos de qualquer uma das retas
			double xmin1 = (x1 <= x2 ? x1 : x2);
			double xmax1 = (x1 > x2 ? x1 : x2);

			double ymin2 = (y1t <= y2t ? y1t : y2t);
			double ymax2 = (y1t > y2t ? y1t : y2t);

			//qDebug("Xmin = %.3f Xmax = %.3f",xmin,xmax);
			if (xi>= xmin1 && xi<= xmax1 && yi>=ymin2 && yi<=ymax2)
				return true;
		}
	}
	return false;
}

bool EPolygon::isIntercepted(EPolygon pol1, EPolygon pol2, double &xi, double &yi)
{
	for (int i=0;i<4;i++)
	{
		//printf("[%d]: %d ; %d\n",i,pol1.point(i).x(),pol1.point(i).y());

	}
	for (int i=0;i<4;i++)
	{
		//printf("[%d]: %d ; %d\n",i,pol2.point(i).x(),pol2.point(i).y());
	}

	for (int i=0; i<pol1.getListPoint().size()-1; i++)
	{
		double x1 = pol1.getPointF(i)->x();
		double x2 = pol1.getPointF(i+1)->x();

		double y1 = pol1.getPointF(i)->y();
		double y2 = pol1.getPointF(i+1)->y();

		double a = y1-y2;
		double b = x2-x1;
		double c = x1*y2-x2*y1;

		//qDebug()<< a <<"x+" << b <<"y+"<< c ;

		for (int j=0; j<pol2.getListPoint().size()-1; j++)
		{
			double x1t = pol2.getPointF(j)->x();
			double x2t = pol2.getPointF(j+1)->x();

			double y1t = pol2.getPointF(j)->y();
			double y2t = pol2.getPointF(j+1)->y();

			double at = y1t-y2t;
			double bt = x2t-x1t;
			double ct = x1t*y2t-x2t*y1t;

			//qDebug()<< at <<"x+" << bt <<"y+"<< ct ;

			xi = ( bt*c - ct*b )/( b*at - bt*a );
			yi = -(c+a*xi)/b;
			double xmin = (x1 <= x2 ? x1 : x2);
			double xmax = (x1 > x2 ? x1 : x2);
			double ymin = (y1 <= y2 ? y1 : y2);
			double ymax = (y1 > y2 ? y1 : y2);

			//qDebug("Xmin = %.3f Xmax = %.3f",xmin,xmax);
			if (xi>= xmin && xi<= xmax && yi>=ymin && yi<=ymax)
				return true;
		}
	}
	return false;
}






/*
int main(int argc,char ** argv)
{
 QPolygon pol1;
 QPolygon pol2;
 pol1.putPoints(0,4, 1,0, 0,4, 4,4, 4,0 );
 pol2.putPoints(0,4, 1,3, 2,5, 3,3, 5,5 );
 int x=0;
 qDebug()<<  isIntercepted(pol1,pol2,x);
 qDebug()<< "X = "<< x;

}
*/

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
