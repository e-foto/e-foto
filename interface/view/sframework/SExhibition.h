#ifndef SEXHIBITION_H
#define SEXHIBITION_H

#include "GLMethods.h"

namespace S {class Image; class Viewport; class Point;}

namespace sclass
{

	class SImageInViewport
	{
	public:
		SImageInViewport();
		SImageInViewport(S::Image* image, S::Viewport* viewport);
		~SImageInViewport();

		string getImageNickname();
		string getViewportNickname();
		double getX();
		double getY();
		double getImageLeft();
		double getImageRight();
		double getImageTop();
		double getImageBottom();
		int getImageWidth();
		int getImageHeight();
		double getScale();
		void setScale(double scale);
		void moveTo(double x, double y, bool autoCorrect = true);
		void pan(double dx, double dy, bool autoCorrect = true);

		void draw();
		double getEfectiveZoom();

	protected:
		void calculateMargins();
		void correctXMovement();
		void correctYMovement();

		S::Image* image;
		S::Viewport* viewport;
		double x, y, scale;
		double left, right, top, bottom;
		double xMax, xMin, yMax, yMin;
		bool limitXMovement, limitYMovement;
	};

	class SMarkInImage
	{
	public:
		SMarkInImage();
		SMarkInImage(S::Point* point, S::Image* image);
		SMarkInImage(S::Point* point, S::Image* image, double x, double y, double scale = 1.0);
		~SMarkInImage();

		string getMarkNickname();
		string getImageNickname();
		double getX();
		double getY();
		double getScale();
		void setScale(double scale);
		void moveTo(double x, double y);
		void pan(double dx, double dy);

		void draw(double x, double y, double zoom);
		double getEfectiveZoom();

	protected:
		void correctMovement();

		S::Point* point;
		S::Image* image;
		double x, y, scale;
	};
/*
	class SMarkInViewport
	{
	public:
		SMarkInViewport();
		SMarkInViewport(S::Point* point, S::Viewport* viewport);
		~SMarkInViewport();

		string getMarkNickname();
		string getViewportNickname();
		double getX();
		double getY();
		double getImageLeft();
		double getImageRight();
		double getImageTop();
		double getImageBottom();
		double getScale();
		void setScale(double scale);
		void moveTo(double x, double y);
		void pan(double dx, double dy);

		void draw();
		double getEfectiveZoom();

	protected:
		void calculateMargins();
		void correctXMovement();
		void correctYMovement();

		S::Point* point;
		S::Viewport* viewport;
		double x, y, scale;
		double left, right, top, bottom;
		double xMax, xMin, yMax, yMin;
		bool limitXMovement, limitYMovement;
	};
*/
} // namespace sclass


#endif // SEXHIBITION_H
