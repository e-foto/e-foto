#include "Te3DPersistenceService.h"

#include <terralib/kernel/TeDatabase.h>
#include <terralib/kernel/TeDatabaseFactory.h>
#include <terralib/kernel/TeUtils.h>
#include "terralib/kernel/TeGeometry.h"
#include "Te3DGeometry.h"

#include <terralib/teConfig.h>

#include <terralib/kernel/TeInitDatabases.h>
#ifdef ADO_FOUND
	#include <TeInitAdoDatabase.h>
#endif

//namespace br {
//namespace uerj {
//namespace eng {
//namespace efoto {

Te3DPersistenceService::Te3DPersistenceService()
	: conn_(0)
	, pointsTableSufix_("_p3d")
{

	TeInitDatabases();

#ifdef ADO_FOUND
	TeInitAdoDatabase();
#endif

}

Te3DPersistenceService::~Te3DPersistenceService()
{
	disconnect();
}

bool Te3DPersistenceService::createDatabase(const TeDatabaseFactoryParams& params)
{
	TeDatabase* conn = 0;
	try
	{
		conn = TeDatabaseFactory::make(params);
		if(conn->newDatabase(params.database_, params.user_, params.password_, params.host_, params.port_) == false)
		{
			delete conn;
			return false;
		}
	}
	catch(...)
	{
		delete conn;
		return false;
	}

	delete conn;
	return true;
}

bool Te3DPersistenceService::connect(const TeDatabaseFactoryParams& params)
{
	try
	{
		conn_ = TeDatabaseFactory::make(params);
	}
	catch(...)
	{
		
	}
	if(conn_ == 0)
	{
		return false;
	}
	if(conn_->isConnected() == false)
	{
		delete conn_;
		conn_ = 0;
		return false;
	}

	if(conn_->loadLayerSet() == false)
	{
		delete conn_;
		conn_ = 0;
		return false;
	}

	return true;
}

void Te3DPersistenceService::disconnect()
{
	if(conn_ == 0)
	{
		return;
	}

	if(conn_->isConnected() == true)
	{
		conn_->close();
	}
		
	delete conn_;
	conn_ = 0;
}

bool Te3DPersistenceService::layerExists(const std::string& layerName)
{
	if(conn_ == 0)
	{
		return false;
	}

	bool exists = conn_->layerExist(layerName);
		
	return exists;
}

bool Te3DPersistenceService::createLayer3D(const std::string& layerName, const TeProjectionParams& params, const Te3DGeometryType& geometryType)
{
	if(conn_ == 0)
	{
		return false;
	}

	if(layerExists(layerName) == true)
	{
		return false;
	}

	std::string linkcolumn = "linkcolumn";	

	conn_->beginTransaction();

	if(geometryType == TePOINTS3D)
	{
		//here we create a layer to store the 3D points
		TeTable attrTable(layerName, getPointsAttrList(linkcolumn), linkcolumn, linkcolumn);
		if(createLayer(layerName, params,  attrTable) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}
	}
	else if(geometryType == TeLINES3D)
	{
		//here we create two layers. The first to store the the lines and the second to store the 3D vertexes of the line
		TeTable linesAttrTable(layerName, getLinesAttrList(linkcolumn), linkcolumn, linkcolumn);
		if(createLayer(layerName, params,  linesAttrTable) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}

		TeTable pointsAttrTable(layerName + pointsTableSufix_, getPointsAuxiliaryAttrList(linkcolumn), linkcolumn, linkcolumn);
		if(createLayer(layerName + pointsTableSufix_, params,  pointsAttrTable) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}
	}
	else if(geometryType == TePOLYGONS3D)
	{
		//here we create two layers. The first to store the the polygons and the second to store the 3D vertexes of the line
		TeTable polygonsAttrTable(layerName, getPolygonsAttrList(linkcolumn), linkcolumn, linkcolumn);
		if(createLayer(layerName, params,  polygonsAttrTable) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}

		TeTable pointsAttrTable(layerName + pointsTableSufix_, getPointsAuxiliaryAttrList(linkcolumn), linkcolumn, linkcolumn);
		if(createLayer(layerName + pointsTableSufix_, params,  pointsAttrTable) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}
	}

	if(conn_->commitTransaction() == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	return true;
}

bool Te3DPersistenceService::addPoint(const std::string& layerName, const TePoint3D& point3D, const std::string& className)
{
	if(conn_ == 0)
	{
		return false;
	}

	if(layerExists(layerName) == false)
	{
		return false;
	}

	TeLayer* layer = getLayer(layerName);
	if(layer == 0)
	{
		return false;
	}

	TePoint point2D(point3D.location().x(), point3D.location().y());
	const double zValue = point3D.location().z();

	TeTable& table = layer->attrTables()[0];
	table.clear();

	conn_->beginTransaction();

	if(addPoint2D(layer, point2D) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	TeTableRow row;
	row.push_back(Te2String(point2D.geomId()));
	row.push_back(className);
	row.push_back(Te2String(zValue, 10));

	table.add(row);

	if(layer->saveAttributeTable(table) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	table.clear();

	if(conn_->commitTransaction() == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	return true;
}

bool Te3DPersistenceService::addLine(const std::string& layerName, const TeLine3D& line3D, const std::string& className)
{
	if(conn_ == 0)
	{
		return false;
	}

	if(layerExists(layerName) == false)
	{
		return false;
	}

	TeLayer* layer = getLayer(layerName);
	if(layer == 0)
	{
		return false;
	}

	TeLayer* layerPoints = getLayer(layerName + pointsTableSufix_);
	if(layerPoints == 0)
	{
		return false;
	}

	TeTable& table = layer->attrTables()[0];
	table.clear();

	TeTable& tablePoints = layerPoints->attrTables()[0];
	tablePoints.clear();

	TeLine2D line2D;
	std::vector<TeCoord2D> vecCoords;
	std::vector<double> vecZ;
	for(unsigned int i = 0; i < line3D.size(); ++i)
	{
		TeCoord2D coord2D(line3D[i].x(), line3D[i].y());
		const double zValue = line3D[i].z();

		line2D.add(coord2D);
		vecCoords.push_back(coord2D);
		vecZ.push_back(zValue);
	}	

//we first store the 2D part
	conn_->beginTransaction();

	TeLineSet ls;
	ls.add(line2D);
	if(layer->addLines(ls) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	line2D.geomId(ls[0].geomId());
	line2D.objectId(Te2String(ls[0].geomId()));

	const std::string updateLinkColumn = "UPDATE " + layer->tableName(TeLINES) + " SET object_id = '" + Te2String(line2D.geomId()) +"' WHERE geom_id = " + Te2String(line2D.geomId());
	if(conn_->execute(updateLinkColumn) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	TeTableRow row;
	row.push_back(Te2String(line2D.geomId()));
	row.push_back(className);

	table.add(row);

	if(layer->saveAttributeTable(table) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	table.clear();

//then we store the 3d part
	for(unsigned int i = 0; i < vecCoords.size(); ++i)
	{
		const TeCoord2D& coord2D = vecCoords[i];
		const double zValue = vecZ[i];

		TePoint point2D(coord2D);
		if(addPoint2D(layerPoints, point2D) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}

		TeTableRow row;
		row.push_back(Te2String(point2D.geomId()));
		row.push_back(Te2String(line2D.geomId()));
		row.push_back(Te2String(zValue, 10));

		tablePoints.add(row);		
	}

	if(layerPoints->saveAttributeTable(tablePoints) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	tablePoints.clear();
	
	if(conn_->commitTransaction() == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	return true;
}

bool Te3DPersistenceService::addPolygon(const std::string& layerName, const TePolygon3D& polygon3D, const std::string& className)
{
	if(conn_ == 0)
	{
		return false;
	}

	if(layerExists(layerName) == false)
	{
		return false;
	}

	TeLayer* layer = getLayer(layerName);
	if(layer == 0)
	{
		return false;
	}

	TeLayer* layerPoints = getLayer(layerName + pointsTableSufix_);
	if(layerPoints == 0)
	{
		return false;
	}

	TeTable& table = layer->attrTables()[0];
	table.clear();

	TeTable& tablePoints = layerPoints->attrTables()[0];
	tablePoints.clear();

	TePolygon polygon2D;
	std::vector<TeCoord2D> vecCoords;
	std::vector<double> vecZ;
	for(unsigned int i = 0; i < polygon3D.size(); ++i)
	{
		TeLine2D line2D;
		const TeLine3D& line3D = polygon3D[i];
		for(unsigned int j = 0; j < line3D.size() - 1; ++j) //nao gravamos novamente o ultimo ponto uma vez que ele eh igual ao primeiro
		{			
			TeCoord2D coord2D(line3D[j].x(), line3D[j].y());
			const double zValue = line3D[j].z();

			line2D.add(coord2D);

			vecCoords.push_back(coord2D);
			vecZ.push_back(zValue);
		}

		polygon2D.add(line2D);
	}	

//we first store the 2D part
	conn_->beginTransaction();

	TePolygonSet ps;
	ps.add(polygon2D);
	if(layer->addPolygons(ps) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	polygon2D.geomId(ps[0].geomId());
	polygon2D.objectId(Te2String(ps[0].geomId()));

	const std::string updateLinkColumn = "UPDATE " + layer->tableName(TePOLYGONS) + " SET object_id = '" + Te2String(polygon2D.geomId()) +"' WHERE geom_id = " + Te2String(polygon2D.geomId());
	if(conn_->execute(updateLinkColumn) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	TeTableRow row;
	row.push_back(Te2String(polygon2D.geomId()));
	row.push_back(className);

	table.add(row);

	if(layer->saveAttributeTable(table) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	table.clear();

//then we store the 3d part
	for(unsigned int i = 0; i < vecCoords.size(); ++i)
	{
		const TeCoord2D& coord2D = vecCoords[i];
		const double zValue = vecZ[i];

		TePoint point2D(coord2D);
		if(addPoint2D(layerPoints, point2D) == false)
		{
			conn_->rollbackTransaction();
			return false;
		}

		TeTableRow row;
		row.push_back(Te2String(point2D.geomId()));
		row.push_back(Te2String(polygon2D.geomId()));
		row.push_back(Te2String(zValue, 10));

		tablePoints.add(row);		
	}

	if(layerPoints->saveAttributeTable(tablePoints) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	tablePoints.clear();
	
	if(conn_->commitTransaction() == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	return true;
}

TeLayer* Te3DPersistenceService::getLayer(const std::string& layerName) const
{
	if(conn_ == 0)
	{
		return 0;
	}

	const std::string upLayerName = TeConvertToUpperCase(layerName);

	const TeLayerMap& layerMap = conn_->layerMap();
	TeLayerMap::const_iterator it = layerMap.begin();
	while(it != layerMap.end())
	{
		if(upLayerName == TeConvertToUpperCase(it->second->name()) == true)
		{
			return it->second;
		}

		++it;
	}

	return 0;
}

bool Te3DPersistenceService::createLayer(const std::string& layerName, const TeProjectionParams& params, const TeTable& attrTable)
{
	if(conn_ == 0)
	{
		return false;
	}

	if(layerExists(layerName) == true)
	{
		return false;
	}

	TeTable table(attrTable);

	conn_->beginTransaction();

	TeProjection* proj = TeProjectionFactory::make(params);
	if(proj == 0)
	{
		conn_->rollbackTransaction();
		return false;
	}

	TeLayer* layer = new TeLayer(layerName, conn_, proj);
	
	delete proj;

	if(layer->createAttributeTable(table) == false)
	{
		conn_->deleteLayer(layer->id());
		conn_->rollbackTransaction();
		return false;
	}

	if(conn_->commitTransaction() == false)
	{
		conn_->deleteLayer(layer->id());
		conn_->rollbackTransaction();
		return false;
	}

	return true;
}

bool Te3DPersistenceService::addPoint2D(TeLayer* layer, TePoint& point2D)
{
	if(layer == 0)
	{
		return false;
	}

	conn_->beginTransaction();

	TePointSet ps;
	ps.add(point2D);
	if(layer->addPoints(ps) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}
	point2D.geomId(ps[0].geomId());
	point2D.objectId(Te2String(ps[0].geomId()));

	const std::string updateLinkColumn = "UPDATE " + layer->tableName(TePOINTS) + " SET object_id = '" + Te2String(point2D.geomId()) +"' WHERE geom_id = " + Te2String(point2D.geomId());
	if(conn_->execute(updateLinkColumn) == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	if(conn_->commitTransaction() == false)
	{
		conn_->rollbackTransaction();
		return false;
	}

	return true;

}

TeAttributeList Te3DPersistenceService::getPointsAttrList(const std::string& linkName) const
{
	TeAttributeList attrList;
	{
		//the primary key and link column
		TeAttribute attr;
		attr.rep_.name_ = linkName;
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = true;

		attrList.push_back(attr);
	}
	{
		//the class of the geometry
		TeAttribute attr;
		attr.rep_.name_ = "class_name";
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = false;

		attrList.push_back(attr);
	}
	{
		//the Z attribute
		TeAttribute attr;
		attr.rep_.name_ = "z_value";
		attr.rep_.type_ = TeREAL;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = false;

		attrList.push_back(attr);
	}
	return attrList;
}

TeAttributeList Te3DPersistenceService::getLinesAttrList(const std::string& linkName) const
{
	TeAttributeList attrList;
	{
		//the primary key and link column
		TeAttribute attr;
		attr.rep_.name_ = linkName;
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = true;

		attrList.push_back(attr);
	}
	{
		//the class of the geometry
		TeAttribute attr;
		attr.rep_.name_ = "class_name";
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = false;

		attrList.push_back(attr);
	}

	return attrList;
}

TeAttributeList Te3DPersistenceService::getPolygonsAttrList(const std::string& linkName) const
{
	TeAttributeList attrList;
	{
		//the primary key and link column
		TeAttribute attr;
		attr.rep_.name_ = linkName;
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = true;

		attrList.push_back(attr);
	}
	{
		//the class of the geometry
		TeAttribute attr;
		attr.rep_.name_ = "class_name";
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = false;

		attrList.push_back(attr);
	}

	return attrList;
}

TeAttributeList Te3DPersistenceService::getPointsAuxiliaryAttrList(const std::string& linkName) const
{
	TeAttributeList attrList;
	{
		//the primary key and link column
		TeAttribute attr;
		attr.rep_.name_ = linkName;
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = true;

		attrList.push_back(attr);
	}
	{
		//the id of the geometry that contains this 3d point
		TeAttribute attr;
		attr.rep_.name_ = "parent_oid";
		attr.rep_.type_ = TeSTRING;
		attr.rep_.numChar_ = 255;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = false;

		attrList.push_back(attr);
	}
	{
		//the Z attribute
		TeAttribute attr;
		attr.rep_.name_ = "z_value";
		attr.rep_.type_ = TeREAL;
		attr.rep_.isAutoNumber_ = false;
		attr.rep_.isPrimaryKey_ = false;

		attrList.push_back(attr);
	}

	return attrList;
}

//} // namespace efoto
//} // namespace eng
//} // namespace uerj
//} // namespace br
