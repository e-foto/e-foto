#include "DemFeatures.h"

/*
 * Dem features
 **/

DemFeatures::DemFeatures()
{
	// No feature and point selected
	selected_feat = -1;
	selected_pt = -1;
}


//
// Common functions
//

FeatureClass * DemFeatures::getFeatureClass(int classid)
{
	if (classid < 1 || classid > feature_classes.size())
		return NULL;

	return &feature_classes.at(classid-1);
}

/*
 * Feature type:
 * 1- Point
 * 2- Line
 * 3- Polygon
 */
string DemFeatures::getFeatureTypeName(int ftype)
{
	switch (ftype)
	{
		case 1: return "Point"; break;
		case 2: return "Line"; break;
		case 3: return "Polygon"; break;
		default: return "Unkown";
	}
}

int DemFeatures::loadFeatures(char *filename, int mode, bool append=false)
{
	// 0- SP 1.65
	// 1- This version
	return loadFeatSp165(filename, append);
}

// Feature id ranges from 1-N
// Return just a copy for reading
DemFeature DemFeatures::getFeature(int feat)
{
	// Create empty object
	DemFeature df;
	if (feat < 1 || feat > features.size())
		return df;

	return features.at(feat-1);
}

// Returns feature id
int DemFeatures::addNewFeature(string name, string fdesc, int fclass, int ftype, int layer) 
{
	DemFeature df;
	df.name = name;
	df.description = fdesc;
	df.feature_class = fclass;
	df.feature_type = ftype;
	df.layer = layer;

	features.push_back(df);

	return features.size();
}

int DemFeatures::deleteFeature(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return 0;

	features.erase(features.begin()+featid-1);

	return 1;
}

int DemFeatures::copyFeature(int featid, double shift=0.0)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return 0;

	DemFeature df;
	df = features.at(featid-1);

	// Add shift to NE direction
	for (int i=0; i<df.points.size(); i++)
	{
		df.points.at(i).X +=  shift;
		df.points.at(i).Y +=  shift;
	}

	features.push_back(df);

	return features.size();
}

// Returns point id - 0, if feature_id is invalid
void DemFeatures::addNewPoint(int featid, int pointid, double X, double Y, double Z)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	// If feature is point, no more than 1 point is allowed
	if (features.at(featid-1).points.size() > 0 && features.at(featid-1).feature_type == 1)
		return;

	// If point id is invalid, return
	if (pointid < 1)
		return;

	DemFeaturePoints dfp;
	dfp.X = X;
	dfp.Y = Y;
	dfp.Z = Z;

	// Insert point
	if (pointid > features.at(featid-1).points.size())
		features.at(featid-1).points.push_back(dfp);
	else
		features.at(featid-1).points.insert(features.at(featid-1).points.begin()+pointid-1, dfp);

	calculateFeatureAttributes(featid);
}

void DemFeatures::updatePoint(int featid, int pointid, double X, double Y, double Z)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	// check if point is valid
	if (pointid < 1 || pointid > features.at(featid-1).points.size())
		return;

	features.at(featid-1).points.at(pointid-1).X = X;
	features.at(featid-1).points.at(pointid-1).Y = Y;
	features.at(featid-1).points.at(pointid-1).Z = Z;

	calculateFeatureAttributes(featid);
}

int DemFeatures::deletePoint(int featid, int pointid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return 0;

	// check if point is valid
	if (pointid < 1 || pointid > features.at(featid-1).points.size())
		return 0;

	features.at(featid-1).points.erase(features.at(featid-1).points.begin()+pointid-1);

	calculateFeatureAttributes(featid);
}

void DemFeatures::setName(int featid, string _name)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	features.at(featid-1).name = _name;
}

string DemFeatures::getName(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return "";

	return features.at(featid-1).name;
}

void DemFeatures::setDescription(int featid, string _desc)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	features.at(featid-1).description = _desc;
}

string DemFeatures::getDescription(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return "";

	return features.at(featid-1).description;
}

void DemFeatures::setFeatureClassId(int featid, int fclass)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	features.at(featid-1).feature_class = fclass;
}

int DemFeatures::getFeatureClassId(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return -1;

	return features.at(featid-1).feature_class;
}

void DemFeatures::setFeatureType(int featid, int ftype)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	features.at(featid-1).feature_type = ftype;
}

int DemFeatures::getFeatureType(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return -1;

	return features.at(featid-1).feature_type;
}

void DemFeatures::setLayer(int featid, int layer)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return;

	features.at(featid-1).layer = layer;
}

int DemFeatures::getLayer(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return -1;

	return features.at(featid-1).layer;
}

int DemFeatures::getNumPoints(int featid)
{
	// Check if feature is valid
	if (featid < 1 || featid > features.size())
		return -1;

	return features.at(featid-1).points.size();
}

// Return class id
int DemFeatures::addFeatureClass(string name, string description, int ccolor, int fcolor)
{
	FeatureClass fc;
	fc.name = name;
	fc.description = description;
	fc.outline_color = ccolor;
	fc.fill_color = fcolor;

	return addFeatureClass(fc);
}

// Return class id
int DemFeatures::addFeatureClass(FeatureClass fc)
{
	feature_classes.push_back(fc);

	return feature_classes.size();
}

void DemFeatures::setFeatureClass(int classid, FeatureClass fc)
{
	if (classid < 1 || classid > feature_classes.size())
		return;

}


//
// Arithmetic functions
//

void DemFeatures::calculateFeaturesAttributes()
{
	for (int i=0; i<features.size(); i++)
	{
		// Check if centroid was already calculated
		if (features.at(i).flag_calc_centroid)
			continue;

		calculateCentroid(i+1);
		calculatePerimeter(i+1);
		calculateArea(i+1);
	}
}

void DemFeatures::calculateFeatureAttributes(int featid)
{
	if (featid < 1 || featid > features.size())
		return;

	calculateCentroid(featid);
	calculatePerimeter(featid);
	calculateArea(featid);
}

// Calculate the average
void DemFeatures::calculateCentroid(int featid)
{
	if (featid < 1 || featid > features.size())
		return;

	DemFeature *df =  &features.at(featid-1);
	double CX, CY, CZ;
	CX = CY = CZ = 0;

	for (int k=0; k<df->points.size(); k++)
	{
		CX += df->points.at(k).X;
		CY += df->points.at(k).Y;
		CZ += df->points.at(k).Z;
	}

	df->centroid.X = CX/double(df->points.size());
	df->centroid.Y = CY/double(df->points.size());
	df->centroid.Z = CZ/double(df->points.size());
	df->flag_calc_centroid = true;
}

void DemFeatures::calculatePerimeter(int featid)
{
	if (featid < 1 || featid > features.size())
		return;

	DemFeature *df =  &features.at(featid-1);
	double RX, RY, RZ, perimeter = 0.0;

	for (int k=0; k<df->points.size()-1; k++)
	{
		RX = df->points.at(k+1).X - df->points.at(k).X;
		RY = df->points.at(k+1).Y - df->points.at(k).Y;
		RZ = df->points.at(k+1).Z - df->points.at(k).Z;
		perimeter += sqrt(pow(RX,2) + pow(RY,2) + pow(RZ,2)); 
	}

	// Add the closing line to the polygon
	int last_pt = df->points.size() - 1;
	if (df->feature_type == 3 && df->points.size() > 2)
	{
		RX = df->points.at(0).X - df->points.at(last_pt).X;
		RY = df->points.at(0).Y - df->points.at(last_pt).Y;
		RZ = df->points.at(0).Z - df->points.at(last_pt).Z;
		perimeter += sqrt(pow(RX,2) + pow(RY,2) + pow(RZ,2)); 
	}

	df->perimeter = perimeter;
}

void DemFeatures::calculateArea(int featid)
{
	if (featid < 1 || featid > features.size())
		return;

	DemFeature *df =  &features.at(featid-1);

	// Calculate area only for polygons
	if (df->feature_type < 3)
	{
		df->area = 0.0;
		return;
	}

	// Calculate normal vector
	double NX, NY, NZ, X1, Y1, Z1, X2, Y2, Z2, VX, VY, VZ;
	int num_points = df->points.size();
	NX = NY = NZ = VX = VY = VZ = 0.0;
	for (int i=0; i<num_points; i++)
	{
		X1 = df->points.at(i).X;
		Y1 = df->points.at(i).Y;
		Z1 = df->points.at(i).Z;
		X2 = df->points.at((i+1) % num_points).X;
		Y2 = df->points.at((i+1) % num_points).Y;
		Z2 = df->points.at((i+1) % num_points).Z;
		NX += Y1*Z2 - Y2*Z1;
		NY += - (Z2*X1 - Z1*X2);
		NZ += X1*Y2 - X2*Y1;
	}

	// Calculate area
	for (int i=0; i<num_points; i++)
	{
		X1 = df->points.at(i).X;
		Y1 = df->points.at(i).Y;
		Z1 = df->points.at(i).Z;
		X2 = df->points.at((i+1) % num_points).X;
		Y2 = df->points.at((i+1) % num_points).Y;
		Z2 = df->points.at((i+1) % num_points).Z;
		VX += X1-X2;
		VY += Y1-Y2;
		VZ += Z1-Z2;
	}

	df->area = sqrt(pow(VX-NX,2)+pow(VY-NY,2)+pow(VZ-NZ,2));
}

// Return 0, if feature list is empty
int DemFeatures::getNearestFeature(double X, double Y, double Z)
{
	if (features.size() < 1)
		return 0;

	int best_feat = 1;
	double dist, best_dist = sqrt(pow(X - features.at(0).centroid.X, 2) + pow(Y - features.at(0).centroid.Y, 2) + pow(Z - features.at(0).centroid.Z, 2));

	for (int i=1; i<features.size(); i++)
	{
		dist = sqrt(pow(X - features.at(i).centroid.X, 2) + pow(Y - features.at(i).centroid.Y, 2) + pow(Z - features.at(i).centroid.Z, 2));

		if (dist < best_dist)
		{
			best_dist = dist;
			best_feat = i+1;	
		}
	}

	return best_feat;
}

//
// Stereoplotter 1.65 and DEM 2.5 compatibility
//

void DemFeatures::convertClassesIdsFomSp165()
{
	int fc, ft, new_fc;
	for (int i=0; i<features.size(); i++)
	{
		ft = features.at(i).feature_type;
		fc = features.at(i).feature_class;

		if (ft == 1)
		{
			features.at(i).feature_class = 2;
			continue;
		}

		if (fc == 0)
		{
			features.at(i).feature_class = 1;
			continue;
		}

		if (ft == 2)
		{
			switch (fc)
			{
				case 1 : new_fc = 3; break;
				case 2 : new_fc = 4; break;
				case 3 : new_fc = 5; break;
				case 4 : new_fc = 6; break;
				case 5 : new_fc = 7; break;
				case 6 : new_fc = 8; break;
				default: new_fc = 1;
			}
		}

		if (ft == 3)
		{
			switch (fc)
			{
				case 1 : new_fc = 9; break;
				case 2 : new_fc = 10; break;
				case 3 : new_fc = 11; break;
				case 4 : new_fc = 12; break;
				case 5 : new_fc = 13; break;
				case 6 : new_fc = 14; break;
				case 7 : new_fc = 15; break;
				case 8 : new_fc = 16; break;
				case 9 : new_fc = 17; break;
				case 10 : new_fc = 18; break;
				case 11 : new_fc = 19; break;
				default: new_fc = 1;
			}
		}

		features.at(i).feature_class = new_fc;
	}
}

void DemFeatures::createClassesFromSp165()
{
	int p_class = feature_classes.size();

	FeatureClass fc;

	// Undefined
	fc.type = 0;
	fc.description = "";
	fc.fill_transparency = 0xFFFFFF;
	fc.line_type = 1;
	fc.outline_color = 0;
	fc.fill_color = 0;
	fc.name = "Undefined";
	feature_classes.push_back(fc);

	// Point
	fc.type = 1;
	fc.outline_color = 0xFF0000;
	fc.name = "Point";
	feature_classes.push_back(fc);

	// Line
	fc.type = 2;
	fc.outline_color = 0xFF0000;
	fc.fill_color = 0;
	fc.name = "Paved street";
	feature_classes.push_back(fc);
	fc.outline_color = 0xFF808000;
	"Unpaved street";
	feature_classes.push_back(fc);
	fc.outline_color = 0xFF00FF00 ;
	fc.name = "Trail";
	feature_classes.push_back(fc);
	fc.outline_color = 0xFFFF00FF;
	fc.name = "Railway";
	feature_classes.push_back(fc);
	fc.outline_color = 0xFF00FFFF;
	fc.name = "River";
	feature_classes.push_back(fc);
	fc.outline_color = 0xFF0000FF;
	fc.name = "Bridge";
	feature_classes.push_back(fc);
	
	// Polygon
	fc.type = 3;
	fc.outline_color = 0xFFFF8080;
	fc.fill_color = 0xFFFF8000;
	fc.name = "House";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFFC0C0C0;
	fc.name = "Building";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFF808080;
	fc.name = "Industrial";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFFFFFF80;
	fc.name = "Club";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFFFF00FF;
	fc.name = "Station";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFF804000;
	fc.name = "Wasteland";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFFC0FFC0;
	fc.name = "Square";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFF00FF00;
	fc.name = "Park";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFF00FF00;
	fc.name = "Forest";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFF0000FF;
	fc.name = "Lagoon";
	feature_classes.push_back(fc);
	fc.fill_color = 0xFF00FFFF;
	fc.name = "Pool";
	feature_classes.push_back(fc);
}

int DemFeatures::loadFeatSp165(char *filename, bool append=false)
{
	// Clear list, if not append
	if (!append)
		features.clear();

	// Get pointer to the last feature
	int p_feat = features.size();

	// Start to read file info
	ifstream arq(filename); // open the emf file

	if (arq.fail())
	{
		printf("Error while opening file !\n");
		return 0;
	}
	
	// Search for features (search the <EFOTO_FEATURES> tag in the first 20 lines)
	string tag;
	bool flag = false;
	int count=0;
	while ((!flag) && (!arq.fail()) && (count < 20))
	{
		getline(arq,tag);
		tag = tag.substr(0,16);
		if (tag.compare("<EFOTO_FEATURES>")==0)
			flag = true;
		count++;
	}
	
	if (!flag)
	{
		printf("This is not a E-FOTO Feature file %d !\n",count);
		arq.close();
		return 0;
	}

	// Start to read the features
	// Create empty features
//	arq.seekg (0, ios::beg);
	DemFeature df;
	df.layer = 1;
	df.description = "";
	df.flag_calc_centroid = false;
	while (!arq.fail())
	{
		// Features EOF
		getline(arq,tag); // 1st line id#
		tag = tag.substr(0,17);
 		if ((tag.compare("</EFOTO_FEATURES>")==0) || (arq.fail()))
			break;
		
		// Else, insert element
		getline(arq,tag); // 2nd line class
		df.feature_class = atoi(tag.c_str());

		getline(arq,tag); // 3rd line type of feature
		if (tag.substr(0,5).compare("Point")==0)
			df.feature_type = 1;
		if (tag.substr(0,4).compare("Line")==0)
			df.feature_type = 2;
		if (tag.substr(0,7).compare("Polygon")==0)
			df.feature_type = 3;

		getline(arq,tag); // 4th line description
		df.name = tag;

		features.push_back(df);
	}

	// Search for points (search the <EFOTO_POINTS> tag in the first 20 lines)
	flag = false;
	count=0;
	while ((!flag) && (!arq.fail()) && (count < 20))
	{
		getline(arq,tag);
		tag = tag.substr(0,14);
		if (tag.compare("<EFOTO_POINTS>")==0)
			flag = true;
		count++;
	}

	if (!flag)
	{
		printf("Could not find the points. This file may be corrupted !\n");
		arq.close();
		return 0;
	}
	
	// Start to read the points
	int feature_id, point_id;
	DemFeaturePoints dfp;
	while (!arq.fail())
	{
		// Points EOF
		getline(arq,tag); // 1st line feature_id
		tag = tag.substr(0,15);
		if ((tag.compare("</EFOTO_POINTS>")==0) || (arq.fail()))
			break;

		feature_id = atoi(tag.c_str());
		feature_id += p_feat;
		getline(arq,tag); // 2nd line point_id
		point_id = atoi(tag.c_str());
		
		// Check if point is not a closing point (string "C1")
		if (point_id != 0)
		{
			// These coords are not used in this version		
			getline(arq,tag); // 3rd line left_column
			getline(arq,tag); // 4th line left_row
			getline(arq,tag); // 5th line right_column
			getline(arq,tag); // 6th line right_row

			// Only X, Y, Z
			getline(arq,tag); // 7th line X
			dfp.X = atof(tag.c_str());
			getline(arq,tag); // 8th line Y
			dfp.Y = atof(tag.c_str());
			getline(arq,tag); // 9th line Z
			dfp.Z = atof(tag.c_str());

			features.at(feature_id-1).points.push_back(dfp);
		}
		else
		{
			// Jump to another point (read file blindly)
			for (unsigned int i=0; i<7; i++)
				getline(arq,tag);
		}
	}

	arq.close();

	// Calculate centroids
	calculateFeaturesAttributes();

	// Create classes from SP 1.65
	createClassesFromSp165();
	convertClassesIdsFomSp165();

	return 1;
}


//
// Output
//

void DemFeatures::showFeatures(bool full=false)
{
	printf("Total features: %d\n",(int)features.size());

	DemFeature df;

	for (int i=0; i<features.size(); i++)
	{
		df = features.at(i);
		printf("Feature #%d:\n",i+1);
		printf(" Type: %s\n",getFeatureTypeName(df.feature_type).c_str());
		printf(" Class: %s\n",getFeatureClass(df.feature_class)->name.c_str());
		printf(" Name: %s\n",df.name.c_str());		
		printf(" Description: %s\n",df.description.c_str());
		printf(" Points: %d\n",(int)df.points.size());
		if (full)
		{
			for (int k=0; k<df.points.size(); k++)
				printf("  %f, %f, %f\n",df.points.at(k).X, df.points.at(k).Y, df.points.at(k).Z);
		}
		printf(" Centroid: %f, %f, %f\n",df.centroid.X, df.centroid.Y, df.centroid.Z);
		printf(" Perimeter: %f\n",df.perimeter);
		printf(" Area: %f\n",df.area);
	}
}

void DemFeatures::showClasses()
{
	printf("Total classes: %d\n",(int)feature_classes.size());

	for (int i=0; i<feature_classes.size(); i++)
	{
		printf("Class #%d\n",i+1);
		printf(" Name: %s\n",feature_classes.at(i).name.c_str());
		printf(" Description: %s\n",feature_classes.at(i).description.c_str());
		printf(" Outline color: %06X\n",feature_classes.at(i).outline_color);
		printf(" Fill color: %06X\n",feature_classes.at(i).fill_color);
	}
}

/*
 * This function converts points and lines to pair list
 * usePolygons enable using polygons too
 * The pair list is not cleared !
 */
void DemFeatures::addFeaturesToPairList(MatchingPointsList *mpl, bool usePolygons = false)
{
	// Search for features
	for (int f=0; f<features.size(); f++)
	{
		DemFeature *df =  &features.at(f);

		if (!usePolygons && df->feature_type == 3)
			continue;

		// Add point
		for (int k=0; k<df->points.size(); k++)
			mpl->add(df->points.at(k).X, df->points.at(k).Y, df->points.at(k).Z);

	}
}
