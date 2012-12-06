#ifndef __3D_PERSISTENCE_SERVICE__
#define __3D_PERSISTENCE_SERVICE__

#include <string>
#include <terralib/kernel/TeAttribute.h>
#include <terralib/kernel/TeTable.h>
#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeDatabaseFactory.h>
//#include "terralib/kernel/TeGeometry.h"
#include <Te3DGeometry.h>

//namespace br {
//namespace uerj {
//namespace eng {
//namespace efoto {


class TePoint;


enum Te3DGeometryType
{
	TePOINTS3D, TeLINES3D, TePOLYGONS3D
};

class Te3DPersistenceService
{

public:

	Te3DPersistenceService();

	virtual ~Te3DPersistenceService();

	virtual bool createDatabase(const TeDatabaseFactoryParams& params);

	virtual bool connect(const TeDatabaseFactoryParams& params);

	virtual void disconnect();

	virtual bool layerExists(const std::string& layerName);

	virtual bool createLayer3D(const std::string& layerName, const TeProjectionParams& params, const Te3DGeometryType& geometryType);

	virtual bool addPoint(const std::string& layerName, const TePoint3D& point3D, const std::string& className);
	
	virtual bool addLine(const std::string& layerName, const TeLine3D& line3D, const std::string& className);

	virtual bool addPolygon(const std::string& layerName, const TePolygon3D& polygon3D, const std::string& className);

protected:

	virtual TeLayer* getLayer(const std::string& layerName) const;

	virtual bool createLayer(const std::string& layerName, const TeProjectionParams& params, const TeTable& attrTable);

	virtual bool addPoint2D(TeLayer* layer, TePoint& point2D);

	virtual TeAttributeList getPointsAttrList(const std::string& linkName) const;
	virtual TeAttributeList getLinesAttrList(const std::string& linkName) const;
	virtual TeAttributeList getPolygonsAttrList(const std::string& linkName) const;
	virtual TeAttributeList getPointsAuxiliaryAttrList(const std::string& linkName) const;

protected:

	TeDatabase*		conn_;
	std::string		errorMessage_;
	std::string		pointsTableSufix_;
};

//} // namespace efoto
//} // namespace eng
//} // namespace uerj
//} // namespace br

#endif //__3D_PERSISTENCE_SERVICE__
