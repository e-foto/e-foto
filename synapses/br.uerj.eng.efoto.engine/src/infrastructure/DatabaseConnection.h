/**************************************************************************
														   DatabaseConnection.h
**************************************************************************/


#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H


#include "EObject.h"
#include "Te3DPersistenceService.h"
#include "Te3DGeometry.h"

//#include "Features.h"
//#include <deque>

//#ifdef INTEGRATED_EFOTO
//#include "Project.h"
//#endif //INTEGRATED_EFOTO
//#ifdef SYNAPSE_EFOTO
//#include <IProject.h>
//#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

//class SPUserInterface;
//class EFotoManager;

class DatabaseConnection// : public EObject
{
private:
    bool conected;
    Te3DPersistenceService pService;
    TeDatabaseFactoryParams params;
	// Private Attributes
	//
	//int rad_cor, match_method, rgx, rgy, lsm_temp, lsm_it, lsm_dist, ncc_temp, ncc_sw;
	//double lsm_th, lsm_std, lsm_shift, lsm_shear, lsm_scale, ncc_th, ncc_std, downsample;
	//bool started;
	//bool status;
	//bool over_it;
	//double over_it_dist;
	//Features spFeatures;
	//int leftKey, rightKey;
        //std::string currentLayer;

        //void get

	//void getImagesId(int pos, int &left, int &right);
	//void addPairsToInterface();
	//int getPairs();
	//bool checkAnglesAlligned(double angle1, double angle2, double tolerance);
	//double getAngleBetweenImages(double X1, double Y1, double X2, double Y2);
	//double fixAngle(double angle);
	//deque<int> listPairs;

	//SPUserInterface* myInterface;
	//EFotoManager* manager;

	//Image* leftImage;
	//Image* rightImage;
    //ProjectiveRay* prL;
    //ProjectiveRay* prR;

//#ifdef INTEGRATED_EFOTO
//    Project* project;
//#endif //INTEGRATED_EFOTO
//#ifdef SYNAPSE_EFOTO
//    engine::IProjectPtr project;
//#endif //SYNAPSE_EFOTO

public:

	// Constructors and Destructors
	//
    DatabaseConnection();
    //DatabaseManager(EFotoManager* manager);
    ~DatabaseConnection();

    void existo();
    bool isConected();
    bool connect(std::string dbms_name_, std::string host_, std::string user_, std::string password_, int port_, std::string database_, int databaseMode);
    void disconnect();

    Te3DPersistenceService* getPService(){return &pService;};
    //int addLayerOnDatabase(std::string nameLayer, int typeLayer);
    //int addFeatureOnDatabase(int fid,int ftype);
    //void getLowerLeftUpperRightPointsLeftImage(double &x1, double &y1, double &x2, double &y2);//pode ser private
    //void getLowerLeftUpperRightPointsRightImage(double &x1, double &y1, double &x2, double &y2);//pode ser private
    //int loadFeaturesFromDatabase();//aqui passar os dados da conexão ao banco
    //int loadFeaturesFromDatabase(vector<std::string> vecLayers);
    //void errorMessage(int flag);

    //ObjectSpaceCoordinate getBoundingBoxCenter();
    //ObjectSpaceCoordinate getCentralPoint();
    //ImageSpaceCoordinate getLeftPoint(ObjectSpaceCoordinate coord);
    //ImageSpaceCoordinate getRightPoint(ObjectSpaceCoordinate coord);

	// Association Methods
	//
	//void setInterface(SPUserInterface* newInterface);
	//SPUserInterface* getInterface();

	// Other Methods
	//
	//bool exec();
	//void returnProject();
	//int loadFeatures(char *filename);
	//void saveFeatures(char *filename);
	//void addFeature(string name, int feattype, int featclass);
	//int removeFeature();
	//void removeAllFeatures();
	//int removePoint();
	//void setSelected(int feat_id, int pt_id);
    //void getSelected(int &fid, int &pid) { fid = spFeatures.selectedFeature(); pid = spFeatures.selectedPoint(); }
    //void getCurrentLayer(std::string &layer){layer=currentLayer;}
    //void setCurrentLayer(std::string layer){currentLayer=layer;}
	//Features* getFeaturesLink() { return &spFeatures; }
	//string getFeaturesList() { return spFeatures.getFeaturesList(); }
	//void updateProjections();
	//void computeIntersection(double xl, double yl, double xr, double yr, double& X, double& Y, double& Z);
	//string getFullImagePath(int imagekey);
	//void getFeatureData(string &fname, int &ftype, int &fclass);
        //void getFeatureData(string &flayer, string &fname, int &ftype, int &fclass);
        //void getFeatureData(int &ftype);
	//void changePair(int pair, int &lk, int &rk);
	//void addPoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z);
	//void updatePoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z);
    //void setSelectedXYZ(double X, double Y, double Z);
    //double getBoundingBoxIdealZoom(int width, int height);



};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //DATABASECONNECTION_H
