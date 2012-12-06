#include "Te3DGeometry.h"
//#include <terralib/kernel/TePrecision.h>

//namespace br {
//namespace uerj {
//namespace eng {
//namespace efoto {

//TeCoord3D
TeCoord3D::TeCoord3D(const double& x, const double& y, const double& z)
	: TeCoord2D(x, y)
	, z_(z)
{
}

TeCoord3D::~TeCoord3D()
{
}

bool TeCoord3D::operator== (const TeCoord3D& cd) const
{
	bool xEqual = fabs (x_ - cd.x_) < TePrecision::instance().precision();
	bool yEqual = fabs (y_ - cd.y_) < TePrecision::instance().precision();
	bool zEqual = fabs (z_ - cd.z_) < TePrecision::instance().precision();

	return (xEqual && yEqual && zEqual); 
}

double TeCoord3D::z() const
{
	return z_;
}

void TeCoord3D::z(const double& z)
{
	z_ = z;
}

//TePoint3D
TePoint3D::TePoint3D(const double& x, const double& y, const double& z) 
	: TeGeomSingle<TeCoord3D> ()
{
		elem_ = TeCoord3D(x,y,z);
		setBox(TeBox(x,y,x,y));
}

TePoint3D::TePoint3D(const TeCoord3D& c) 
	: TeGeomSingle<TeCoord3D> ( )
{
		elem_ = c;
		setBox(TeBox(c.x(),c.y(),c.x(),c.y())); // the box of a point is the point itself
}

TeGeomRep TePoint3D::elemType() const 
{ 
	return TePOINTS; 
}

//TeLine3D
TeLine3D::TeLine3D()
{
}

bool TeLine3D::isRing() const
{
	if ( size() <= 3 )
	{
		return false;
	}
	
	return ( pImpl_->operator[] (0) == pImpl_->operator[] ( pImpl_->size() - 1 ) );
}

TeGeomRep TeLine3D::elemType() const 
{ 
	return TeLINES; 
}

//TePolygon3D
TePolygon3D::TePolygon3D()
{
}

TeGeomRep TePolygon3D::elemType() const 
{ 
	return TePOLYGONS; 
}

void TePolygon3D::copyElements( const TePolygon3D& other )
{
	geomId_ = other.geomId();
	objectId_ = other.objectId();

	for (unsigned int i = 0; i < other.size(); ++i) // for each linear ring
	{
		TeLine3D line; 
		line.copyElements(other[i]);
		this->add (line);
	}
}

//} // namespace efoto
//} // namespace eng
//} // namespace uerj
//} // namespace br
