#ifndef __3D_GEOMETRY__
#define __3D_GEOMETRY__

#include <terralib/kernel/TeCoord2D.h>
#include <terralib/kernel/TeGeometry.h>

//namespace br {
//namespace uerj {
//namespace eng {
//namespace efoto {

class TeCoord3D : public TeCoord2D
{
public:

	TeCoord3D(const double& x = 0., const double& y = 0., const double& z = 0.);

	virtual ~TeCoord3D();

	bool operator== (const TeCoord3D& cd) const;

	double z() const;

	void z(const double& z);	

protected:

	double	z_;
};

class TePoint3D : public TeGeomSingle<TeCoord3D>
{
public:

	TePoint3D(const double& x = 0., const double& y = 0., const double& z = 0.);

	TePoint3D(const TeCoord3D& c);

	TeGeomRep elemType() const;
};


class TeLine3D : public TeGeomComposite<TeCoord3D>
{
public:
	
	TeLine3D();

	bool isRing() const;	

	TeGeomRep elemType() const;
};

class TePolygon3D : public TeGeomComposite<TeLine3D>  
{
public:
	
	TePolygon3D();

	TeGeomRep elemType() const;

	void copyElements ( const TePolygon3D& other );
};

//} // namespace efoto
//} // namespace eng
//} // namespace uerj
//} // namespace br

#endif //__3D_GEOMETRY__
