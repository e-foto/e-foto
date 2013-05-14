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

SPManager::SPManager() :
    prL(0), prR(0)
{
	started = false;
	status = false;
}

SPManager::SPManager(EFotoManager* manager, deque<Image*>images, deque<ExteriorOrientation*> eos) :
    prL(0), prR(0)
{
	this->manager = manager;
	started = false;
	status = false;
	listAllImages = images;
	listEOs = eos;
	spFeatures.createClassesFromSp165();
	leftKey = 1;
	rightKey = 2;
        setListPoint();
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
                    // Test if theere are at least two images
                    if (listAllImages.size() < 2)
                    {
                            SPUserInterface_Qt *spui = (SPUserInterface_Qt *)myInterface;
                            spui->showErrorMessage("There are no enough images to run this application");
                            returnProject();
                    }

                    myInterface->exec();

                    // Check if pairs available
                    if (!getPairs())
                    {
                            SPUserInterface_Qt *spui = (SPUserInterface_Qt *)myInterface;
                            spui->showErrorMessage("Could not find any pair. Please, check your Exterior Orientation.");
                            returnProject();
                            //return status;
                    }

                    myInterface->centerImages(getCentralPoint(), 1.0);
		}
	}
	return status;
}

void SPManager::returnProject()
{
	manager->reloadProject();
}

void SPManager::setListPoint()
{
        EDomElement xmlPoints(manager->getXml("points"));
        deque<EDomElement> allPoints = xmlPoints.elementsByTagName("point");
        for(int i=0;i<allPoints.size();i++)
        {
                Point* point= manager->instancePoint(Conversion::stringToInt(allPoints.at(i).attribute("key")));
                if (point != NULL)
                        listAllPoints.push_back(point);
        }
}

double SPManager::getTerrainMeanAltitude()
{
    double Z=0.0;
    int num_points = listAllPoints.size();
    Point *p;

    for (int i=0; i<num_points; i++)
    {
        p = listAllPoints.at(i);
        Z += p->getObjectCoordinate().getZ();
    }

    return Z / double(num_points);
}

int SPManager::loadFeatures(char *filename)
{
	return spFeatures.loadFeatures(filename,0,false);
}

void SPManager::saveFeatures(char *filename)
{
	spFeatures.saveFeatures(filename,0,false);
    // Expanção do XML
    addGeometryToXML(string(filename));
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
	spFeatures.setSelectedFeature(-1);

	return 1;
}

void SPManager::removeAllFeatures()
{
	spFeatures.deleteAllFeatures();
	spFeatures.setSelectedFeature(-1);
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
    if (sel_pt > spFeatures.getFeature(spFeatures.selectedFeature()).points.size() && sel_pt != 1)
        spFeatures.setSelectedPoint(sel_pt - 1);
    else if (sel_pt == 1 && spFeatures.getFeature(spFeatures.selectedFeature()).points.size() == 0)
        spFeatures.setSelectedPoint(-1);

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
    leftImage = listAllImages.at(leftKey-1);
    rightImage = listAllImages.at(rightKey-1);
    Sensor* sensor = leftImage->getSensor();
    InteriorOrientation* lio = leftImage->getIO();
    InteriorOrientation* rio = rightImage->getIO();
    SpatialRessection* lsr = (SpatialRessection*)leftImage->getEO();
    SpatialRessection* rsr = (SpatialRessection*)rightImage->getEO();
    leftImage->setSensor(sensor); leftImage->setIO(lio); leftImage->setEO(lsr);
    rightImage->setSensor(sensor); rightImage->setIO(rio); rightImage->setEO(rsr);
    if (leftImage == NULL || rightImage == NULL)
        return;
    prL = ProjectiveRay(leftImage);
    prR = ProjectiveRay(rightImage);

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
        double X1, Y1, X2, Y2, R, dist, bX2, bY2, overlap, align_ang, kappa, terrain_mean_Z;
        int p1, p2, img_code, id1, id2;
	Matrix Xa;

        // Calculate Images Radius, using the first image as reference
	ObjectSpaceCoordinate osc;
	img = listAllImages.at(0);
	ProjectiveRay pr(img);
	Xa = img->getEO()->getXa();
	X1 = Xa.get(1,1);
	Y1 = Xa.get(2,1);
	p1 = img->getWidth();
	p2 = img->getHeight()/2;
        terrain_mean_Z = getTerrainMeanAltitude();
        osc = pr.imageToObject(p1,p2,terrain_mean_Z,false);
	X2 = osc.getX();
	Y2 = osc.getY();
	R = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

        for (int i=0; i<listAllImages.size(); i++)
	{
                // Get reference image data
		img = listAllImages.at(i);
		Xa = img->getEO()->getXa();
		X1 = Xa.get(1,1);
		Y1 = Xa.get(2,1);
		kappa = fabs(Xa.get(6,1));

		// Calculate the shortest image center
                for (int j=0; j<listAllImages.size(); j++)
		{
                        // Skip same image check
			if (i==j)
				continue;

                        // Get test image data
			img = listAllImages.at(j);
			Xa = img->getEO()->getXa();
			X2 = Xa.get(1,1);
			Y2 = Xa.get(2,1);

			// Calculate dist
			dist = sqrt(pow(X1-X2,2) + pow(Y1-Y2,2));

                        // Check images overlapping
                        overlap = 100*(2*R - dist)/(2*R);

                        if (overlap < 60.0 || overlap > 100.0)
                                continue;

/*                        // Check images alignment and kappa
                        align_ang = getAngleBetweenImages(X1, Y1, bX2, bY2);

                        if (!checkAnglesAlligned(align_ang, kappa, 0.174532925)) // 10 Degrees
                                continue;
*/
                        // Assign image ids (in this case, the verctor number - image id ranges from 1-N
                        id1 = i+1;
                        id2 = j+1;

                        // Check if pair exists
                        if (!existPair(id1, id2))
                        {
                            // Add images to list
                            img_code = (id1-1) + listAllImages.size()*(id2-1);
                            listPairs.push_back(img_code);
                        }
		}
	}

	addPairsToInterface();

        return (listPairs.size() > 0);
}

// Check if pair already exists and sort ids
bool SPManager::existPair(int &id1, int &id2)
{
    int aux, pair_id1, pair_id2;

    if (listPairs.size() == 0)
        return false;

    // Sort id
    if (id1 > id2)
    {
        aux = id1;
        id1 = id2;
        id2 = aux;
    }

    for (int i=0; i<listPairs.size(); i++)
    {
        // Decode image list
        getImagesId(i, pair_id1, pair_id2);
        if (pair_id1 == id1 && pair_id2 == id2)
            return true;
    }

    return false;
}

/*
 *  End of pair automatic identification
 */

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

ObjectSpaceCoordinate SPManager::getBoundingBoxCenter()
{
    double x[8], y[8];
    double xmax, ymax, xmin, ymin;
    double zm = manager->instanceTerrain()->getMeanAltitude();

    x[0] = prL.imageToObject(0,0,zm,false).getPosition().get(1);
    x[1] = prL.imageToObject(0,leftImage->getHeight(),zm,false).getPosition().get(1);
    x[2] = prL.imageToObject(leftImage->getWidth(),0,zm,false).getPosition().get(1);
    x[3] = prL.imageToObject(leftImage->getWidth(),leftImage->getHeight(),zm,false).getPosition().get(1);
    x[4] = prR.imageToObject(0,0,zm,false).getPosition().get(1);
    x[5] = prR.imageToObject(0,rightImage->getHeight(),zm,false).getPosition().get(1);
    x[6] = prR.imageToObject(rightImage->getWidth(),0,zm,false).getPosition().get(1);
    x[7] = prR.imageToObject(rightImage->getWidth(),rightImage->getHeight(),zm,false).getPosition().get(1);

    y[0] = prL.imageToObject(0,0,zm,false).getPosition().get(2);
    y[1] = prL.imageToObject(0,leftImage->getHeight(),zm,false).getPosition().get(2);
    y[2] = prL.imageToObject(leftImage->getWidth(),0,zm,false).getPosition().get(2);
    y[3] = prL.imageToObject(leftImage->getWidth(),leftImage->getHeight(),zm,false).getPosition().get(2);
    y[4] = prR.imageToObject(0,0,zm,false).getPosition().get(2);
    y[5] = prR.imageToObject(0,rightImage->getHeight(),zm,false).getPosition().get(2);
    y[6] = prR.imageToObject(rightImage->getWidth(),0,zm,false).getPosition().get(2);
    y[7] = prR.imageToObject(rightImage->getWidth(),rightImage->getHeight(),zm,false).getPosition().get(2);

    xmax = xmin = x[0];
    ymax = ymin = y[0];
    for (int i = 1; i < 8; i++)
    {
        if (x[i] > xmax)
            xmax = x[i];
        if (x[i] < xmin)
            xmin = x[i];
        if (y[i] > ymax)
            ymax = y[i];
        if (y[i] < ymin)
            ymin = y[i];
    }
    double xm = (xmax + xmin) / 2.0;
    double ym = (ymax + ymin) / 2.0;
    return ObjectSpaceCoordinate("m", xm, ym, zm);
}

double SPManager::getBoundingBoxIdealZoom(int width, int height)
{
    double zoom = 1.0;

    ImageSpaceCoordinate leftCenter = getLeftPoint(getBoundingBoxCenter());
    ImageSpaceCoordinate rightCenter = getRightPoint(getBoundingBoxCenter());

    double dx, dy;
    double xl = leftCenter.getPosition().get(1);
    double yl = leftCenter.getPosition().get(2);
    double xr = rightCenter.getPosition().get(1);
    double yr = rightCenter.getPosition().get(2);
    double xmax, ymax;

    if (leftCenter.getPosition().get(1) < rightCenter.getPosition().get(1))
    {
        dx = rightCenter.getPosition().get(1) - leftCenter.getPosition().get(1);
        if (leftImage->getWidth() + dx > rightImage->getWidth())
            xmax = leftImage->getWidth() + dx;
        else
            xmax = rightImage->getWidth();
    }
    else
    {
        dx = leftCenter.getPosition().get(1) - rightCenter.getPosition().get(1);
        if (leftImage->getWidth() > rightImage->getWidth() + dx)
            xmax = leftImage->getWidth();
        else
            xmax = rightImage->getWidth() + dx;
    }

    if (leftCenter.getPosition().get(2) < rightCenter.getPosition().get(2))
    {
        dy = rightCenter.getPosition().get(2) - leftCenter.getPosition().get(2);
        if (leftImage->getHeight() + dy > rightImage->getHeight())
            ymax = leftImage->getHeight() + dy;
        else
            ymax = rightImage->getHeight();
    }
    else
    {
        dy = leftCenter.getPosition().get(2) - rightCenter.getPosition().get(2);
        if (leftImage->getHeight() > rightImage->getHeight() + dy)
            ymax = leftImage->getHeight();
        else
            ymax = rightImage->getHeight() + dy;
    }

    double wscale = width / xmax;
    double hscale = height / ymax;
    zoom = wscale < hscale ? wscale : hscale;

    return zoom;
}

// Esse método calcula o centro planimétrico na linha de voo entre duas imagens
ObjectSpaceCoordinate SPManager::getCentralPoint()
{
    double xl = leftImage->getEO()->getXa().get(1,1);
    double yl = leftImage->getEO()->getXa().get(2,1);
    double xr = rightImage->getEO()->getXa().get(1,1);
    double yr = rightImage->getEO()->getXa().get(2,1);
    double zm = manager->instanceTerrain()->getMeanAltitude();
    double xm = (xl + xr) / 2.0;
    double ym = (yl + yr) / 2.0;
    return ObjectSpaceCoordinate("m", xm, ym, zm);
}

ImageSpaceCoordinate SPManager::getLeftPoint(ObjectSpaceCoordinate coord)
{
    return ImageSpaceCoordinate(prL.objectToImage(coord,false));
}

ImageSpaceCoordinate SPManager::getRightPoint(ObjectSpaceCoordinate coord)
{
    return ImageSpaceCoordinate(prR.objectToImage(coord,false));
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

void SPManager::addGeometryToXML(string filename)
{
    stringstream add;
    add << "<featuresFilename>";
    add << filename;
    add << "</featuresFilename>";

    EDomElement newXml(manager->xmlGetData());

    if (newXml.elementByTagName("features").getContent() == "")
        newXml.addChildAtTagName("efotoPhotogrammetricProject","<features>\n</features>");
    newXml.addChildAtTagName("features", add.str());

    manager->xmlSetData(newXml.getContent());
    manager->setSavedState(false);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
