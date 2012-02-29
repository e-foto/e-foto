/**************************************************************************
														   SPManager.cpp
**************************************************************************/

#include "SPManager.h"
#include "EFotoManager.h"
#include "SPUserInterface_Qt.h"
#include "ProjectiveRay.h"


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

// Constructors and destructors
//

SPManager::SPManager()
{
	started = false;
	status = false;
}

SPManager::SPManager(EFotoManager* manager, deque<Image*>images, deque<ExteriorOrientation*> eos)
{
	this->manager = manager;
	started = false;
	status = false;
	listAllImages = images;
	listEOs = eos;
	spFeatures.createClassesFromSp165();
	leftKey = 1;
	rightKey = 2;
}

SPManager::~SPManager()
{

}

// Association Methods
//

void SPManager::setInterface(SPUserInterface* newInterface)
{
	myInterface = newInterface;
}

SPUserInterface* SPManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

bool SPManager::exec()
{
	if (manager != NULL)
	{
		if (manager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = SPUserInterface_Qt::instance(this);
		}
		started = true;
		if (myInterface != NULL)
		{
			myInterface->exec();
			getPairs();
		}
	}
	return status;
}

void SPManager::returnProject()
{
	manager->reloadProject();
}

int SPManager::loadFeatures(char *filename)
{
	return spFeatures.loadFeatures(filename,0,false);
}

void SPManager::saveFeatures(char *filename)
{
	spFeatures.saveFeatures(filename,0,false);
}

void SPManager::addFeature(string name, int feattype, int featclass)
{
	int fclass = spFeatures.convClassFromSp165(feattype, featclass);

	spFeatures.addNewFeature(name, "", fclass, feattype, 1);

	setSelected(spFeatures.getNumFeatures(),-1);
}

int SPManager::removeFeature()
{
	int sel_feat = spFeatures.selectedFeature();

	if (sel_feat == -1)
		return 0;

	spFeatures.deleteFeature(sel_feat);

	return 1;
}

void SPManager::removeAllFeatures()
{
	spFeatures.deleteAllFeatures();
}

int SPManager::removePoint()
{
	int sel_feat = spFeatures.selectedFeature();

	if (sel_feat == -1)
		return 0;

	int sel_pt = spFeatures.selectedPoint();

	if (sel_pt == -1)
		return 0;

	spFeatures.deletePoint(sel_feat, sel_pt);

	return 1;
}

void SPManager::setSelected(int feat_id, int pt_id)
{
	if (feat_id < -1 || pt_id < -1 || feat_id > spFeatures.getNumFeatures())
		return;

	if (feat_id > 0)
	{
		if (pt_id > spFeatures.getNumPoints(feat_id))
			return;
	}

	if (feat_id == 0 || pt_id == 0)
		return;

	spFeatures.setSelectedFeature(feat_id);
	spFeatures.setSelectedPoint(pt_id);
}

void SPManager::getFeatureData(string &fname, int &ftype, int &fclass)
{
	ftype = -1;
	fclass = -1;
	fname = "";

	int sel_feat = spFeatures.selectedFeature();

	if (sel_feat < 1)
		return;

	fname = spFeatures.getFeature(sel_feat).name;
	ftype = spFeatures.getFeature(sel_feat).feature_type;
	fclass = spFeatures.getClassIdToSp165(spFeatures.getFeature(sel_feat).feature_class);
}

void SPManager::updateProjections()
{
	Image* leftImage = listAllImages.at(leftKey-1);
	Image* rightImage = listAllImages.at(rightKey-1);
	Sensor* sensor = leftImage->getSensor();
	InteriorOrientation* lio = leftImage->getIO();
	InteriorOrientation* rio = rightImage->getIO();
	SpatialRessection* lsr = (SpatialRessection*)leftImage->getEO();
	SpatialRessection* rsr = (SpatialRessection*)rightImage->getEO();
	leftImage->setSensor(sensor); leftImage->setIO(lio); leftImage->setEO(lsr);
	rightImage->setSensor(sensor); rightImage->setIO(rio); rightImage->setEO(rsr);
	if (leftImage == NULL || rightImage == NULL)
		return;
	ProjectiveRay prL(leftImage);
	ProjectiveRay prR(rightImage);

	// Aqui enquanto não definimos uma estrutura para as geometrias 2d eu vou coordenar a tradução para o que eu já possuia no display para pontos e retas.
	for (int i = 0; i < spFeatures.getNumFeatures(); i++)
	{
		DemFeature* df = spFeatures.getFeatureLink(i+1);
		for (int j = 0; j < df->points.size(); j++)
		{
			// Isso vai ficar um pouco ruim, mas vai melhorar em breve quando a classe DigitalImageCoordinates for substituida por algo equivalente usando double (completando o suporte a subpixels)
			ImageSpaceCoordinate pL0 = prL.objectToImage(df->points.at(j).X, df->points.at(j).Y, df->points.at(j).Z,false);
			ImageSpaceCoordinate pR0 = prR.objectToImage(df->points.at(j).X, df->points.at(j).Y, df->points.at(j).Z,false);
			df->points.at(j).left_x = pL0.getCol();
			df->points.at(j).left_y = pL0.getLin();
			df->points.at(j).right_x = pR0.getCol();
			df->points.at(j).right_y = pR0.getLin();
		}
	}
}

void SPManager::computeIntersection(double xl, double yl, double xr, double yr, double& X, double& Y, double& Z)
{
	Image* leftImage = listAllImages.at(leftKey-1);
	Image* rightImage = listAllImages.at(rightKey-1);
	Sensor* sensor = leftImage->getSensor();
	InteriorOrientation* lio = leftImage->getIO();
	InteriorOrientation* rio = rightImage->getIO();
	SpatialRessection* lsr = (SpatialRessection*)leftImage->getEO();
	SpatialRessection* rsr = (SpatialRessection*)rightImage->getEO();
	leftImage->setSensor(sensor); leftImage->setIO(lio); leftImage->setEO(lsr);
	rightImage->setSensor(sensor); rightImage->setIO(rio); rightImage->setEO(rsr);
	if (leftImage == NULL || rightImage == NULL)
		return;

	SpatialIntersection si(new StereoPair(leftImage, rightImage));
	ObjectSpaceCoordinate obj = si.calculateIntersectionSubPixel(xl,yl,xr,yr);
	X = obj.getX();
	Y = obj.getY();
	Z = obj.getZ();
}

string SPManager::getFullImagePath(int imagekey)
{
	Image* img = listAllImages.at(imagekey-1);
	string result;
	result.append(img->getFilepath());
	result.append("/");
	result.append(img->getFilename());
	return result;
}

/*
 *
 * Pair detection algorithm - Copied from DEM Manager
 *
 */

/*
 * Pair detection / Angle functions
 */
double SPManager::fixAngle(double angle)
{
	bool negative = angle < 0.0;

	// Positive signal
	angle = fabs(angle);

	// Bring angle to the first lap
	if (angle > 2*M_PI)
	{
		int laps = floor(angle/(2*M_PI));
		angle = angle - laps*(2*M_PI);
	}

	if (negative)
		angle = 2*M_PI - angle;

	return angle;
}

double SPManager::getAngleBetweenImages(double X1, double Y1, double X2, double Y2)
{
	double delta_X = X2 - X1;
	double delta_Y = Y2 - Y1;

		if (fabs(delta_X - 0.0) < 0.0000000000001)
	{
		if (delta_Y > 0.0)
			return M_PI/2.0;
		else
			return 3*M_PI/2.0;
	}

		return fixAngle(atan(delta_Y/delta_X));
}

bool SPManager::checkAnglesAlligned(double angle1, double angle2, double tolerance)
{
	angle1 = fixAngle(angle1);
	angle2 = fixAngle(angle2);

	double distance = fabs(angle1 - angle2);

	if (distance > M_PI)
		distance = 2*M_PI - distance;

	return (distance < tolerance || fabs(M_PI - distance) < tolerance);
}

int SPManager::getPairs()
{
	//
	// List Pairs description (0 - N-1):
	//
	// num = left + no_imgs*right // Encoding
	// left = num % no_imgs // Decoding
	// right =  num / no_imgs // Decoding
	//  Image ID ranges from 1-N

	// Clear list
	listPairs.clear();

	Image *img;
	double X1, Y1, X2, Y2, R, dist, best_dist, bX2, bY2, overlap, align_ang, kappa;
	int p1, p2, best_img, img_code;
	Matrix Xa;

	// Calculate Image Radius
	ObjectSpaceCoordinate osc;
	img = listAllImages.at(0);
	ProjectiveRay pr(img);
	Xa = img->getEO()->getXa();
	X1 = Xa.get(1,1);
	Y1 = Xa.get(2,1);
	p1 = img->getWidth();
	p2 = img->getHeight()/2;
	osc = pr.imageToObject(p1,p2,img->getHeight(),false);
	X2 = osc.getX();
	Y2 = osc.getY();
	R = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

	for (int i=0; i<listAllImages.size()-1; i++)
	{
		// Get base image center
		img = listAllImages.at(i);
		Xa = img->getEO()->getXa();
		X1 = Xa.get(1,1);
		Y1 = Xa.get(2,1);
		kappa = fabs(Xa.get(6,1));

		best_dist = 10e100;

		// Calculate the shortest image center
		for (int j=i+1; j<listAllImages.size(); j++)
		{
			if (i==j)
				continue;

			img = listAllImages.at(j);
			Xa = img->getEO()->getXa();
			X2 = Xa.get(1,1);
			Y2 = Xa.get(2,1);

			// Calculate dist
			dist = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

			if (dist < best_dist)
			{
				best_dist = dist;
				best_img = j;
				bX2 = X2;
				bY2 = Y2;
			}
		}

		// Check images overlapping
		overlap = 100*(2*R-best_dist)/(2*R);

		if (overlap < 60.0 || overlap > 100.0)
			continue;

		// Check images alignment and kappa
		align_ang = getAngleBetweenImages(X1, Y1, bX2, bY2);

		if (!checkAnglesAlligned(align_ang, kappa, 0.174532925)) // 10 Degrees
			continue;

		// Add images to list
		img_code = i + listAllImages.size()*best_img;
		listPairs.push_back(img_code);
	}

	addPairsToInterface();

	return 1;
}

void SPManager::addPairsToInterface()
{
	// Add pairs to the interface
	int left_id, right_id;
	string str_left, str_right;
	stringstream txt;
	SPUserInterface_Qt *spui = (SPUserInterface_Qt *)myInterface;

	for (int i=0; i<listPairs.size(); i++)
	{
		getImagesId(i,left_id,right_id);
		str_left = listAllImages.at(left_id-1)->getImageId();
		str_right = listAllImages.at(right_id-1)->getImageId();
		txt.str(""); // Clear stream
		txt << "Images " << str_left << " and " << str_right;
		spui->addImagePair((char *)txt.str().c_str());
	}
}

void SPManager::addPoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z)
{
	if (fid < 1 || fid > spFeatures.getNumFeatures())
		return;

	if (pid < 1 || pid > spFeatures.getFeature(fid).points.size()+1)
		return;

	spFeatures.addNewPoint(fid, pid, X, Y, Z);
	spFeatures.update2DPoint(fid, pid, lx, ly, rx, ry);
	spFeatures.setSelectedFeature(fid);
	spFeatures.setSelectedPoint(pid);
}

void SPManager::updatePoint(int fid, int pid, double lx, double ly, double rx, double ry, double X, double Y, double Z)
{
	if (fid < 1 || fid > spFeatures.getNumFeatures())
		return;

	if (pid < 1 || pid > spFeatures.getFeature(fid).points.size())
		return;

	spFeatures.updatePoint(fid, pid, X, Y, Z);
	spFeatures.update2DPoint(fid, pid, lx, ly, rx, ry);
}

void SPManager::setSelectedXYZ(double X, double Y, double Z)
{
	int fid, pid;

	spFeatures.getNearestPoint(X,Y,Z,fid,pid);
	spFeatures.setSelectedFeature(fid);
	spFeatures.setSelectedPoint(pid);
}

// Internal function. Pos from 0 - N-1.
void SPManager::getImagesId(int pos, int &left, int &right)
{
	// Check pos
	left = -1;
	right = -1;
	if (pos < 0 || pos > listPairs.size()-1)
		return;

	// Decode
	int no_imgs = listAllImages.size();
	left = 1 + (listPairs.at(pos) % no_imgs);
	right = 1 + (listPairs.at(pos) / no_imgs);
}

void SPManager::changePair(int pos, int &lk, int &rk)
{
	getImagesId(pos, leftKey, rightKey);
	lk = leftKey;
	rk = rightKey;

	updateProjections();
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
