#ifndef SMARK_H
#define SMARK_H

#include <string>
#include <math.h>
#include <deque>
using namespace std;

#include "GLMethods.h"
#include "SExhibition.h"
#include "SMarker.h"

class SWidget;

namespace S {class Point {public: virtual void panInImage(string imagenickname, double x, double y)=0; virtual double getXInImage(string imageNickname)=0; virtual double getYInImage(string imageNickname)=0; virtual void setNewMarker(S::Pin* markedWith)=0; virtual void draw(sclass::SMarkInImage *link, double x, double y, double zoom)=0; virtual void removeExhibition(sclass::SMarkInImage* exhibition)=0; virtual void addExhibition(sclass::SMarkInImage* exhibition)=0; virtual string getNickname()=0;};}

namespace sclass
{

	class SPoint : public S::Point
	{
	public:
		SPoint();
		SPoint(SWidget* parent, string nickname);
		SPoint(SWidget* parent, string nickname, S::Pin* markedWith);
		~SPoint();

		string getNickname();

		void setNewMarker(S::Pin* markedWith);
		void addExhibition(sclass::SMarkInImage* exhibition);
		void removeExhibition(sclass::SMarkInImage* exhibition);
		void removeExhibitions();
		bool isShowedInImage(string imageNickname);
		double getXInImage(string imageNickname);
		double getYInImage(string imageNickname);
		void panInImage(string imagenickname, double x, double y);

		void draw(sclass::SMarkInImage *link, double x, double y, double zoom);

	protected:
		SWidget* parent;
		string nickname;
		S::Pin* markedWith;
		deque<sclass::SMarkInImage*> exposedInImage;
	};

} // namespace sclass

#endif // SMARK_H
