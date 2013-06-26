#include <sys/time.h>
#include <time.h>
#include "DemGrid.h"
#include "DEMManager.h"

/**
* class DemGrid
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 11/10/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DemGrid::DemGrid(double _Xi, double _Yi, double _Xf, double _Yf, double _res_x, double _res_y)
{
	point_list = NULL;
	mpg = NULL;
	createNewGrid(_Xi, _Yi, _Xf, _Yf, _res_x, _res_y);
	manager = NULL;
}

void DemGrid::createNewGrid(double _Xi, double _Yi, double _Xf, double _Yf, double _res_x, double _res_y)
{
	// Update data
	Xi = _Xi;
	Yi = _Yi;
	Xf = _Xf;
	Yf = _Yf;
	res_x = _res_x;
	res_y = _res_y;

	// Calculate DEM size
	dem_width = int(1.0 + floor((Xf - Xi) / res_x));
	dem_height = int(1.0 + floor((Yf - Yi) / res_y));

	// Create DEM
	DEM.resize(dem_height, dem_width);
}

void DemGrid::changeGridResolution(double _res_x, double _res_y)
{
	createNewGrid(Xi, Yi, Xf, Yf, _res_x, _res_y);
}

void DemGrid::setPointList(MatchingPointsList *mpl)
{
	// Link
	point_list = mpl;
}

void DemGrid::getMinMax(double &min, double &max)
{
        min = 1e100;
        max = -1e100;
	double Z;

        for (unsigned int i=1; i<=dem_height; i++)
	{
                for (unsigned int j=1; j<=dem_width; j++)
		{
			Z = DEM.get(i,j);

                        if (fabs(Z - 0.0) < 0.00000000001)
                            continue;

			if (Z < min) min = Z;
			if (Z > max) max = Z;
		}
	}
}

double DemGrid::getMeanZ()
{
	double meanZ = 0.0, Z;
	unsigned int count=0;

        for (unsigned int i=1; i<=dem_height; i++)
	{
                for (unsigned int j=1; j<=dem_width; j++)
		{
			Z = DEM.get(i,j);

			// If hole, continue
                        if (fabs(Z - 0.0) < 0.000000000000000001)
				continue;

			meanZ += Z;
			count++;
		}
	}

	return meanZ/double(count);
}

double DemGrid::getHeightXY(double X, double Y)
{
	// Calculate col,row - double
	double col,row;
	col = 1.0 + (X-Xi)/res_x;
	row = 1.0 + (Y-Yi)/res_y;

        return getHeight(row, col);
}

double DemGrid::getHeight(double row, double col)
{
	// Check limits
	if (col<1.0) col = 1.0;
	if (row<1.0) row = 1.0;
	if (col>=double(dem_width)) col = double(dem_width) - 1;
	if (row>=double(dem_height)) row = double(dem_height) - 1;

	// Calculate Z using bilinear interpolation
	double Z;
	int col_i = floor(col), row_i = floor(row);
	double delta_x = col - col_i, delta_y = row - row_i;

	Z = DEM.get(row_i,col_i)*(1-delta_x)*(1-delta_y) + DEM.get(row_i,col_i+1)*delta_x*(1-delta_y) + DEM.get(row_i+1,col_i)*(1-delta_x)*delta_y + DEM.get(row_i+1,col_i+1)*delta_x*delta_y;

	return Z;
}

Matrix * DemGrid::getDemImage(double min, double max)
{
	if (int(min) == 0 && int(max) == 0)
		getMinMax(min,max);

	if (min >= max)
	{
		printf("Error: min value is greater or equal max.\n");
		return NULL;
	}

	Matrix *img = new Matrix(dem_height, dem_width);

	// Convert DEM to image - (0.0 to 1.0)
	double gray, Z, deltaZ = max-min;
	double p1 = 1.0/255.0, p2 = 1-p1;
	for (unsigned int i=1; i<=dem_height; i++)
	{
		for (unsigned int j=1; j<=dem_width; j++)
		{
			Z = DEM.get(i,j);

			// If values inside Zmin and Zmax
			if (fabs(Z - 0.0) < 0.000000000000001)
				gray = 0.0;
			else
				gray = p2*((Z-min)/deltaZ) + p1;

			// Else
			if (Z < min) gray = p1;
			if (Z > max) gray = 1.0;

			// Change 180 degrees from spatial coordinate system to image
			img->set(dem_height-i+1,j,gray);
		}
	}

	return img;
}

// Return a copy of the DEM
Matrix & DemGrid::getDem()
{
	Matrix *m = new Matrix(DEM.getRows(),DEM.getCols());
	*m = DEM;
	return *m;
}

void DemGrid::getXYAt(int col, int row, double &X, double &Y)
{
	X = res_x * (double(col) - 1.0) + Xi;
	Y = res_y * (double(row) - 1.0) + Yi;
}

void DemGrid::getXYAt(double col, double row, double &X, double &Y)
{
        X = res_x * (double(col) - 1.0) + Xi;
        Y = res_y * (double(row) - 1.0) + Yi;
}

void DemGrid::getColRowAt(double X, double Y, double &col, double &row)
{
        col = 1.0 + (X - Xi) / res_x;
        row = 1.0 + (Y - Yi) / res_y;
}

void DemGrid::getColRowAt(double X, double Y, int &col, int &row)
{
        col = int(1.0 + (X - Xi) / res_x);
        row = int(1.0 + (Y - Yi) / res_y);
}

/*
 * INTERPOOLATION DECISION METHOD
 */

void DemGrid::interpolateNearestPoint()
{
        // nf set to 1.0 is the size of the matrix grid cell (not terrain)
        (chooseBestInterpolationMethod(1.0) == 0) ? interpolateNearestPointNormal() : interpolateNearestPointFast();
}

void DemGrid::interpolateTrendSurface(int mode)
{
	// No difference between fast and normal methods
	interpolateTrendSurfaceFast(mode);
}

void DemGrid::interpolateMovingAverage(double n, double D0, int mode)
{
	double nf = (n/res_x)*(n/res_y);
        (chooseBestInterpolationMethod(nf) == 0) ? interpolateMovingAverageNormal(n, D0, mode) : interpolateMovingAverageFast(n, D0, mode);
}

void DemGrid::interpolateMovingSurface(double n, double D0, int mode, int mode2)
{
	double nf = (n/res_x)*(n/res_y);
        (chooseBestInterpolationMethod(nf) == 0) ? interpolateMovingSurfaceNormal(n, D0, mode, mode2) : interpolateMovingSurfaceFast(n, D0, mode, mode2);
}

/*
 * Input:
 * nf- Structure matrix area in cells (new subdivision of the grid)
 *
 * Output:
 * 0- Use normal interpolation
 * 1- Use fast interpolation
 */
int DemGrid::chooseBestInterpolationMethod(double nf)
{
	int no_points = point_list->size();

	if (no_points < 1000)
		return 0;

        // May have very large numbers - double prefered !!
        double area = double(dem_width) * double (dem_height); // Calculate DEM matrix area - not terrain area
        double grid_density = double(no_points)/double(area); // Points per area

        // If density is less than a cell size
        if (grid_density < 1.0)
                grid_density = 1.0;

        // Let's calculate the computational effort for each grid cell
        // For not overfloating values, we consider the sqtr for both equations
        double no_its_normal = no_points;           // no_points^2
        double no_its_fast = sqrt(area*grid_density*nf); // area*density*nf

	return (no_its_normal > no_its_fast);
}


/***********************************************
 *                                             *
 * 3D Point INTERPOLATION using fast structure *
 *                                             *
 ***********************************************/

/*
 * These 4 3D points algorithms were based on ILWIS open source: http://www.ilwis.org:
 * Nearest Point
 * Moving Average
 * Moving Surface
 * Trend Surface
 */

// Best performance
// Time function found at: http://forum.clubedohardware.com.br/milisegundos-c/282275?s=bbcb5f6ff12025b9a8227a5a37e4ccfc&amp;
void DemGrid::interpolateNearestPointFast()
{
        cancel_flag = false;

	if (point_list == NULL)
	{
		printf("Error! No point list linked.\n");
		return;
	}

	// If empty list of points
	if (point_list->size() < 1)
		return;

	// Create faster structure to deal with the cloud of points
	// As NP must find a point, even in a hole.
	// Holes decreases performance. So, we increase the sctructure reolution
	// to solve this problem.
	mpg = new MatchingPointsGrid(point_list, res_x*10, res_y*10);

	double Px, Py, Z;
	int best_point;
	int total = DEM.getCols()*DEM.getRows();
	MatchingPoints *mp;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

        printf("Interpolating using Nearest Point Fast ...\n");
	gettimeofday(&begin,NULL);
	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			Px = Xi + double(x-1)*res_x;
			Py = Yi + double(y-1)*res_y;

			best_point = mpg->getNearestPoint(Px, Py);

			Z = 0.0;
			if (best_point > 0)
			{
				mp = point_list->get(best_point);
				Z = mp->Z;
			}

			DEM.set(y, x, Z);

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}
	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);

	delete mpg;
}

// This interpolation method does not use the fast structure
void DemGrid::interpolateTrendSurfaceFast(int mode)
{
        cancel_flag = false;

	if (point_list == NULL)
	{
		printf("Error! No point list linked.\n");
		return;
	}

	// If empty list of points
	if (point_list->size() < 1)
		return;

	// Mode:
	// 0 - Plane
	// 1 - 2nd Degree Linear
	// 2 - 2nd Degree Parabolic
	// 3 - 2nd Degree
	// 4 - 3rd Degree
	double Px, Py, Cx, Cy;
	int no_points = point_list->size();
	int total = DEM.getCols()*DEM.getRows();

	Matrix X,A,L;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

        printf("Interpolating using Trend Surface Fast ...\n");
	gettimeofday(&begin,NULL);

	// Size of the matrices
	switch (mode)
	{
	case 0 : A.resize(no_points,3); break;
	case 1 : A.resize(no_points,4); break;
	case 2 : A.resize(no_points,5); break;
	case 3 : A.resize(no_points,6); break;
	default : A.resize(no_points,10); break;
	}
	L.resize(no_points,1);

	// Calculate the cendroid of the mass of data
	Cx=0.0;
	Cy=0.0;
	MatchingPoints *mp;
	for (unsigned int i=1; i<=no_points; i++)
	{
		mp = point_list->get(i);
		Cx += mp->X;
		Cy += mp->Y;
	}
	Cx = Cx/double(no_points);
	Cy = Cy/double(no_points);

	// Fill matrices A and L
	for (unsigned int i=1; i<=no_points; i++)
	{
		mp = point_list->get(i);
		A.set(i,1,1.0);
		A.set(i,2,mp->X-Cx);
		A.set(i,3,mp->Y-Cy);
		if ((mode>0) && (mode!=2))
			A.set(i,4,(mp->X-Cx) * (mp->Y-Cy));
		if ((mode==2) || (mode>2))
		{
			A.set(i,4+(mode>2),pow(mp->X-Cx,2));
			A.set(i,5+(mode>2),pow(mp->Y-Cy,2));
		}
		if (mode==4)
		{
			A.set(i,7,pow(mp->X-Cx,3));
			A.set(i,8,pow(mp->X-Cx,2) * (mp->Y-Cy));
			A.set(i,9,pow(mp->Y-Cy,2) * (mp->X-Cx));
			A.set(i,10,pow(mp->Y-Cy,3));
		}
		L.set(i,1,mp->Z);

                if (cancel_flag)
                    return;
	}

	// Least-squares
	X = (A.transpose()*A).inverse()*A.transpose()*L;

	// Interpolate
	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			Px = Xi + double(x-1)*res_x - Cx;
			Py = Yi + double(y-1)*res_y - Cy;
			switch (mode)
			{
                            case 0 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py); break;
                            case 1 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py); break;
                            case 2 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Px + X.get(5,1)*Py*Py); break;
                            case 3 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py + X.get(5,1)*Px*Px + X.get(6,1)*Py*Py); break;
                            case 4 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py + X.get(5,1)*Px*Px + X.get(6,1)*Py*Py + X.get(7,1)*Px*Px*Px + X.get(8,1)*Px*Px*Py + X.get(9,1)*Px*Py*Py + X.get(10,1)*Py*Py*Py); break;
			}

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}

	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);
}

// Using fast structure
void DemGrid::interpolateMovingAverageFast(double n, double D0, int mode)
{
        cancel_flag = false;

	if (point_list == NULL)
	{
		printf("Error! No point list linked.\n");
		return;
	}

	// If empty list of points
	if (point_list->size() < 1)
		return;

	// Mode 0 = inverse distance
	// Mode 1 = linear decrease
	double Px, Py, d, D, sum1, sum2, weight;
	int total = DEM.getCols()*DEM.getRows(), point_id;

	// Create fast structure
	mpg = new MatchingPointsGrid(point_list, res_x, res_y);

	MatchingPoints *mp;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

        printf("Interpolating using Moving Average Fast ...\n");
	gettimeofday(&begin,NULL);

	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			Px = Xi + double(x-1)*res_x;
			Py = Yi + double(y-1)*res_y;

			// Calculate weights
			sum1=0.0;
			sum2=0.0;

			// Returns a list with points inside the circle with center PX,PY and radius D0
			mpg->getPointsClose(Px, Py, D0);

			for (unsigned int i=0; i < mpg->selected_points.size(); i++)
			{
				point_id = mpg->selected_points.at(i);
				mp = point_list->get(point_id);

				D = sqrt(pow(Px-mp->X,2)+pow(Py-mp->Y,2));
				if (D < D0)
				{
					d = D/D0;
					if (mode==0)
						weight = (1/pow(d,n))-1;
					else
						weight = 1 - pow(d,n);
					sum1 += weight*mp->Z;
					sum2 += weight;
				}
			}
			if (sum2 > 0.0)
				DEM.set(y,x,sum1/sum2);
			else
				DEM.set(y,x,0.0);

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}

	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);

	delete mpg;
}

// Using fast structure
void DemGrid::interpolateMovingSurfaceFast(double n, double D0, int mode, int mode2)
{
        cancel_flag = false;

	if (point_list == NULL)
	{
		printf("Error! No point list linked.\n");
		return;
	}

	// If empty list of points
	if (point_list->size() < 1)
		return;

	// Mode - For moving Average
	// 0 = inverse distance
	// 1 = linear decrease
	//
	// Mode2 - For Trend Surface
	// 0 - Plane
	// 1 - 2nd Degree Linear
	// 2 - 2nd Degree Parabolic
	// 3 - 2nd Degree
	// 4 - 3rd Degree

	double Px, Py, d, D, weight, Cx, Cy, Pxi, Pyi;
	int no_points = point_list->size(), no_valid_points, point_id;
	int total = DEM.getCols()*DEM.getRows();
	Matrix X,A,L;

	// Create fast structure
	mpg = new MatchingPointsGrid(point_list, res_x, res_y);

	MatchingPoints *mp;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

        printf("Interpolating using Moving Surface Fast ...\n");
	gettimeofday(&begin,NULL);

	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			// Calculate Px and Py
			Px = Xi + double(x-1)*res_x;
			Py = Yi + double(y-1)*res_y;

			//
			// Moving Average Part
			//

			// Returns a list with points inside the circle with center PX,PY and radius D0
			mpg->getPointsClose(Px, Py, D0);
			no_valid_points = mpg->selected_points.size();

			if (no_valid_points == 0)
				continue;

			//
			// Trend Surface Part
			//

			// Size the matrices
			switch (mode2)
			{
			case 0 : A.resize(no_valid_points,3); break;
			case 1 : A.resize(no_valid_points,4); break;
			case 2 : A.resize(no_valid_points,5); break;
			case 3 : A.resize(no_valid_points,6); break;
			default : A.resize(no_valid_points,10); break;
			}
			L.resize(no_valid_points,1);

			// Calculate the cendroid of the mass of data
			Cx=0.0;
			Cy=0.0;
			for (unsigned int i=0; i<no_valid_points; i++)
			{
				point_id = mpg->selected_points.at(i);
				mp = point_list->get(point_id);
				Cx += mp->X;
				Cy += mp->Y;
			}
			Cx = Cx/double(no_valid_points);
			Cy = Cy/double(no_valid_points);

			// Calculate Px and Py with centroids
			Pxi = Px;
			Pyi = Py;
			Px = Px - Cx;
			Py = Py - Cy;

			// Fill matrices A and L
			for (unsigned int i=1; i<=no_valid_points; i++)
			{
				point_id = mpg->selected_points.at(i-1);
				mp = point_list->get(point_id);

				// Calculate weight
				D = sqrt(pow(Pxi-mp->X,2)+pow(Pyi-mp->Y,2));
				d = D/D0;
				if (mode==0)
					weight = (1/pow(d,n))-1;
				else
					weight = 1 - pow(d,n);

				A.set(i,1,1.0 * weight);
				A.set(i,2,(mp->X-Cx) * weight);
				A.set(i,3,(mp->Y-Cy) * weight);

				if ((mode2>0) && (mode2!=2))
					A.set(i,4,(mp->X-Cx) * (mp->Y-Cy) * weight);
				if ((mode2==2) || (mode2>2))
				{
					A.set(i,4+(mode2>2),pow(mp->X-Cx,2) * weight);
					A.set(i,5+(mode2>2),pow(mp->Y-Cy,2) * weight);
				}
				if (mode2==4)
				{
					A.set(i,7,pow(mp->X-Cx,3) * weight);
					A.set(i,8,pow(mp->X-Cx,2) * (mp->Y-Cy) * weight);
					A.set(i,9,pow(mp->Y-Cy,2) * (mp->X-Cx) * weight);
					A.set(i,10,pow(mp->Y-Cy,3) * weight);
				}
				L.set(i,1,mp->Z * weight);
			}

			// Least-squares
			X = (A.transpose()*A).inverse()*A.transpose()*L;

			switch (mode2)
			{
                            case 0 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py); break;
                            case 1 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py); break;
                            case 2 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Px + X.get(5,1)*Py*Py); break;
                            case 3 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py + X.get(5,1)*Px*Px + X.get(6,1)*Py*Py); break;
                            case 4 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py + X.get(5,1)*Px*Px + X.get(6,1)*Py*Py + X.get(7,1)*Px*Px*Px + X.get(8,1)*Px*Px*Py + X.get(9,1)*Px*Py*Py + X.get(10,1)*Py*Py*Py); break;
			}

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}

	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);

	delete mpg;
}

void DemGrid::cutGrid(double min, double max, bool fromList=true)
{
	double Xi, Xf, Yi, Yf, Zi, Zf;

	if (fromList)
	{
		point_list->XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
		min = Zi;
		max = Zf;
	}

	if (min >= max)
	{
		printf("Error: min is greater or equal than max\n");
		return;
	}

	printf("Cutting DEM: accept from %f to %f\n",min,max);

	for (int i=1; i<=DEM.getRows(); i++)
	{
		for (int j=1; j<=DEM.getCols(); j++)
		{
			if (DEM.get(i,j) < min || DEM.get(i,j) > max)
				DEM.set(i,j,0.0);
		}
	}
}

/************************
 *                      *
 * Read/Write functions *
 *                      *
 ************************/

// This is an internal function used to debug loading functions
void DemGrid::printData()
{
	printf("DEM header:\n");
	printf(" Xi: %f\n Yi: %f\n Xf: %f\n Yf: %f\n",Xi,Yi,Xf,Yf);
	printf(" Resolution X: %f\n Resolution Y: %f\n",res_x,res_y);
	printf(" GRID width: %d\n GRID height: %d\n",dem_width,dem_height);
	printf("Sample DEM data:\n");

	int w,h;
	(dem_width < 10) ? w = dem_width : w = 10;
	(dem_height < 10) ? h = dem_height : h = 10;

	for (unsigned int i=1; i<=h; i++)
	{
		for (unsigned int j=1; j<=w; j++)
			printf("%.2f  ",DEM.get(i,j));
		printf("\n");
	}
}

void DemGrid::saveDem(char * filename, int mode)
{

	switch (mode)
	{
	case 0 : saveDemEfoto(filename); break;
	default : saveDemAscii(filename); break;
	}
}

void DemGrid::loadDem(char * filename, int mode)
{

	switch (mode)
	{
	case 0 : loadDemEfoto(filename); break;
	default : loadDemAscii(filename); break;
	}
}

void DemGrid::saveDemEfoto(char * filename)
{
	FILE *fp;

	fp = fopen(filename,"wb");

	// Write header
	double header[8];
	header[0] = Xi;
	header[1] = Yi;
	header[2] = Xf;
	header[3] = Yf;
	header[4] = res_x;
	header[5] = res_y;
	header[6] = double(dem_width);
	header[7] = double(dem_height);

	fwrite(&header, 1, 8*8, fp);

	// Write DEM
	int p=0;
	unsigned int file_size = dem_width*dem_height;
	double *data = new double[file_size];

	for (int i=1; i<=dem_height; i++)
	{
		for (int j=1; j<=dem_width; j++)
		{
			data[p] = DEM.get(i,j);
			p++;
		}
	}

	fwrite(data, 1, file_size*8, fp);

	fclose(fp);

	delete data;
}

void DemGrid::loadDemEfoto(char * filename)
{
	FILE *fp;

	fp = fopen(filename,"rb");

	// Read header
	double header[8];
	fread(&header, 1, sizeof(double)*8, fp);

	Xi = header[0];
	Yi = header[1];
	Xf = header[2];
	Yf = header[3];
	res_x = header[4];
        res_y = header[5];
	dem_width = int(header[6]);
	dem_height = int(header[7]);

	// Read DEM
	DEM.resize(dem_height, dem_width);
	unsigned int file_size = dem_width*dem_height;
	double *data = new double[file_size];
	int p=0;
	fread(data, 1, dem_width*dem_height*8, fp);

	for (int i=1; i<=dem_height; i++)
	{
		for (int j=1; j<=dem_width; j++)
		{
			DEM.set(i,j,data[p]);
			p++;
		}
	}

	fclose(fp);

	delete data;

	//	printData();
}

void DemGrid::saveDemAscii(char * filename)
{
	ofstream outfile(filename);

	// Write header
	outfile << "-= EFOTO DEM GRID DATA =-\n\n";
	outfile << "Header info:\n";
	outfile << fixed << setprecision(5);
	outfile << "Xi=" << Xi << "\nYi=" << Yi << "\nXf=" << Xf << "\nYf=" << Yf << "\n";
	outfile << "Res_X=" << res_x << "\nRes_Y=" << res_y << "\n";
	outfile << "Width=" << dem_width << "\nHeight=" << dem_height << "\n";
	outfile << "\nGRID data:\n";

	double X,Y;

	for (int i=1; i<=dem_height; i++)
	{
		for (int j=1; j<=dem_width; j++)
		{
			X = Xi + (j-1)*res_x;
			Y = Yi + (i-1)*res_y;
			outfile << X << "\t" << Y << "\t" << DEM.get(i,j) << "\n";
		}
	}

	outfile.close();
}

// For later conversion to stream
double DemGrid::getAsciiParameter(ifstream *file, string tag)
{
	char line[256];
	string sline;

	int line_count = 0, p;
	// find -1 = not found
	file->seekg (0, ios::beg);
	while (!file->eof() && line_count<50)
	{
		file->getline(line,256);
		sline = (string) line;
		p = sline.find(tag);
		if (p>=0)
		{
			sline = sline.substr(p+tag.length());
			return atof(sline.c_str());
		}
		line_count++;
	}

	return -1.0;
}

void DemGrid::loadDemAscii(char * filename)
{
	ifstream arq(filename);
	if (arq.fail())
	{
		cout << "Error while opening file!\n";
		return;
	}

	// Read header
	Xi = getAsciiParameter(&arq, "Xi=");
	Yi = getAsciiParameter(&arq, "Yi=");
	Xf = getAsciiParameter(&arq, "Xf=");
	Yf = getAsciiParameter(&arq, "Yf=");
	res_x = getAsciiParameter(&arq, "Res_X=");
	res_y = getAsciiParameter(&arq, "Res_Y=");
	dem_width = getAsciiParameter(&arq, "Width=");
	dem_height = getAsciiParameter(&arq, "Height=");

	// Check header
	if (Xi<0 || Yi<0 || Xf<0 || Yf<0 || res_x<0 || res_y<0 || dem_width<0 || dem_height<0)
	{
		arq.close();
		printf("Error while reading file header!\n");
		return;
	}

	// Read data
	DEM.resize(dem_height, dem_width);
	// Set pointer to first line of the GRID
	getAsciiParameter(&arq, "GRID data:");
	double X, Y, Z;

	for (int i=1; i<=dem_height; i++)
	{
		for (int j=1; j<=dem_width; j++)
		{
			arq >> X >> Y >> Z;
			DEM.set(i,j,Z);
			if (arq.eof())
				printf("Error: missing data!\n");
		}
	}

	arq.close();

	//	printData();
}


/********************************************************
 * Interpolation functions without using fast structure *
 *                                                      *
 ********************************************************/

void DemGrid::interpolateNearestPointNormal()
{
        cancel_flag = false;

	if (point_list == NULL)
	{
		printf("Error! No point list linked.\n");
		return;
	}

	// If empty list of points
	if (point_list->size() < 1)
		return;

	double Px, Py, dist, best_dist, Z;
	int total = DEM.getCols()*DEM.getRows();
	MatchingPoints *mp;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

	printf("Interpolating using Nearest Point ...\n");
	gettimeofday(&begin,NULL);
	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			Px = Xi + double(x-1)*res_x;
			Py = Yi + double(y-1)*res_y;

			mp = point_list->get(1);
			best_dist = sqrt(pow(Px - mp->X,2)+pow(Py - mp->Y,2));
			Z = 0.0;
			for (unsigned int i=2; i<=point_list->size(); i++)
			{
				mp = point_list->get(i);
				dist = sqrt(pow(Px - mp->X,2)+pow(Py - mp->Y,2));
				if (dist < best_dist)
				{
					best_dist = dist;
					Z = mp->Z;
				}
			}
			DEM.set(y, x, Z);

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}
	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);
}

void DemGrid::interpolateMovingAverageNormal(double n, double D0, int mode)
{
        cancel_flag = false;

	// If empty list of points
	if (point_list->size() < 1)
		return;

	// Mode 0 = inverse distance
	// Mode 1 = linear decrease
	double Px, Py, d, D, sum1, sum2, weight;
	int total = DEM.getCols()*DEM.getRows();

	MatchingPoints *mp;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

	printf("Interpolating using Moving Average ...\n");
	gettimeofday(&begin,NULL);

	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			Px = Xi + double(x-1)*res_x;
			Py = Yi + double(y-1)*res_y;
			// Calculate weights
			sum1=0.0;
			sum2=0.0;
			for (unsigned int i=1; i <= point_list->size(); i++)
			{
				mp = point_list->get(i);
				D = sqrt(pow(Px-mp->X,2)+pow(Py-mp->Y,2));
				if (D < D0)
				{
					d = D/D0;
					if (mode==0)
						weight = (1/pow(d,n))-1;
					else
						weight = 1 - pow(d,n);
					sum1 += weight*mp->Z;
					sum2 += weight;
				}
			}
			if (sum2 > 0.0)
				DEM.set(y,x,sum1/sum2);
			else
				DEM.set(y,x,0.0);

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}

	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);
}

void DemGrid::interpolateMovingSurfaceNormal(double n, double D0, int mode, int mode2)
{
        cancel_flag = false;

	// If empty list of points
	if (point_list->size() < 1)
		return;

	// Mode - For moving Average
	// 0 = inverse distance
	// 1 = linear decrease
	//
	// Mode2 - For Trend Surface
	// 0 - Plane
	// 1 - 2nd Degree Linear
	// 2 - 2nd Degree Parabolic
	// 3 - 2nd Degree
	// 4 - 3rd Degree

	vector <int> validPointList;
	vector <double> validPointListWeight;
	double Px, Py, d, D, weight, Cx, Cy;
	int no_points = point_list->size(), no_valid_points;
	int total = DEM.getCols()*DEM.getRows();
	Matrix X,A,L;

	MatchingPoints *mp;

	struct timeval begin;
	struct timeval end;
	int MICRO_PER_SECOND = 1000000;

	printf("Interpolating using Moving Surface ...\n");
	gettimeofday(&begin,NULL);

	for (unsigned int y=1; y<=DEM.getRows(); y++)
	{
		for (unsigned int x=1; x<=DEM.getCols(); x++)
		{
			// Calculate Px and Py
			Px = Xi + double(x-1)*res_x;
			Py = Yi + double(y-1)*res_y;

			//
			// Moving Average Part
			//

			// Check for valid points
			validPointList.clear();
			for (unsigned int i=1; i <= point_list->size(); i++)
			{
				mp = point_list->get(i);

				D = sqrt(pow(Px-mp->X,2)+pow(Py-mp->Y,2));
				if (D < D0)
				{
					d = D/D0;
					if (mode==0)
						weight = (1/pow(d,n))-1;
					else
						weight = 1 - pow(d,n);

					validPointList.push_back(i);
					validPointListWeight.push_back(weight);
				}
			}
			no_valid_points = validPointList.size();

			if (no_valid_points == 0)
				continue;

			//
			// Trend Surface Part
			//

			// Size the matrices
			switch (mode2)
			{
			case 0 : A.resize(no_valid_points,3); break;
			case 1 : A.resize(no_valid_points,4); break;
			case 2 : A.resize(no_valid_points,5); break;
			case 3 : A.resize(no_valid_points,6); break;
			default : A.resize(no_valid_points,10); break;
			}
			L.resize(no_valid_points,1);

			// Calculate the cendroid of the mass of data
			Cx=0.0;
			Cy=0.0;
			for (unsigned int i=0; i<no_valid_points; i++)
			{
				mp = point_list->get(validPointList.at(i));
				Cx += mp->X;
				Cy += mp->Y;
			}
			Cx = Cx/double(no_valid_points);
			Cy = Cy/double(no_valid_points);

			// Calculate Px and Py with centroids
			Px = Px - Cx;
			Py = Py - Cy;

			// Fill matrices A and L
			for (unsigned int i=1; i<=no_valid_points; i++)
			{
				mp = point_list->get(validPointList.at(i-1));
				weight = validPointListWeight.at(i-1);

				A.set(i,1,1.0 * weight);
				A.set(i,2,(mp->X-Cx) * weight);
				A.set(i,3,(mp->Y-Cy) * weight);

				if ((mode2>0) && (mode2!=2))
					A.set(i,4,(mp->X-Cx) * (mp->Y-Cy) * weight);
				if ((mode2==2) || (mode2>2))
				{
					A.set(i,4+(mode2>2),pow(mp->X-Cx,2) * weight);
					A.set(i,5+(mode2>2),pow(mp->Y-Cy,2) * weight);
				}
				if (mode2==4)
				{
					A.set(i,7,pow(mp->X-Cx,3) * weight);
					A.set(i,8,pow(mp->X-Cx,2) * (mp->Y-Cy) * weight);
					A.set(i,9,pow(mp->Y-Cy,2) * (mp->X-Cx) * weight);
					A.set(i,10,pow(mp->Y-Cy,3) * weight);
				}
				L.set(i,1,mp->Z * weight);
			}

			// Least-squares
			X = (A.transpose()*A).inverse()*A.transpose()*L;

			switch (mode2)
			{
                            case 0 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py); break;
                            case 1 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py); break;
                            case 2 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Px + X.get(5,1)*Py*Py); break;
                            case 3 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py + X.get(5,1)*Px*Px + X.get(6,1)*Py*Py); break;
                            case 4 : DEM.set(y, x, X.get(1,1) + X.get(2,1)*Px + X.get(3,1)*Py + X.get(4,1)*Px*Py + X.get(5,1)*Px*Px + X.get(6,1)*Py*Py + X.get(7,1)*Px*Px*Px + X.get(8,1)*Px*Px*Py + X.get(9,1)*Px*Py*Py + X.get(10,1)*Py*Py*Py); break;
			}

                        if (cancel_flag)
                            return;
		}
		if (manager!=NULL)
			manager->setProgress((100*y)/DEM.getRows());
	}

	gettimeofday(&end,NULL);

	float etime = (float)(end.tv_sec - begin.tv_sec);
	etime += (end.tv_usec - begin.tv_usec)/(float)MICRO_PER_SECOND;
	elap_time = double(etime);

	printf("Elapsed time: %.6f\n",etime);
}


/***************
 *             *
 * DEM QUALITY *
 *             *
 ***************/

string DemGrid::calculateDemQuality(MatchingPointsList mpl)
{
	stringstream txt;

	MatchingPoints *mp;
	double Z, Zgrid;
	int list_size = mpl.size();
	Matrix Zerr(list_size,1);
	double Zerror;

	txt << "E-FOTO DEM Quality\n\n";
	txt << "Number of testing points: " << list_size << "\n\n";

	// Calculate whole errors
	txt << "Whole set Z error:\n";
	for (int i=0; i<list_size; i++)
	{
		mp = mpl.get(i+1);

		Z = mp->Z;
		Zgrid = getHeightXY(mp->X, mp->Y);

		// -1.0 is a DEM hole flag
		if (Zgrid - 0.0 < 0.0000000000000001)
		{
			Zerror = -1.0;
			txt << "Point " << i+1 << ": Not used\n";
		}
		else
		{
			Zerror = fabs(Z-Zgrid);
			txt << "Point " << i+1 << ": " << Zerror << "\n";
		}

		Zerr.set(i+1,1,Zerror);
	}

	bool flag=true;
	int pts_used = mpl.size();
	double Zmean, Zstd;

	while (flag && pts_used > 2)
	{
		// Calculate useful points mean
		Zmean = 0.0;
		pts_used = 0;
		for (int i=1; i <= list_size; i++)
		{
			if (int(Zerr.get(i,1)) == -1)
				continue;

			pts_used++;
			Zmean += Zerr.get(i,1);
		}
		Zmean /= double(pts_used);

		// Calculate STD
		Zstd = 0.0;
		for (int i=1; i <= list_size; i++)
		{
			if (int(Zerr.get(i,1)) == -1)
				continue;

			Zstd += pow(Zerr.get(i,1) - Zmean, 2);
		}
		Zstd = sqrt(Zstd/(pts_used));

		// Eliminate outliers
		flag = false;
		for (int i=1; i <= list_size; i++)
		{
			if (int(Zerr.get(i,1)) == -1)
				continue;

			if (Zerr.get(i,1) > Zmean + 3.0*Zstd || Zerr.get(i,1) < Zmean - 3.0*Zstd)
			{
				flag = true;
				Zerr.set(i,1,-1.0);
			}
		}
	}

	// Show final results
	txt << "\nFinal set Z error:\n";
	for (int i=1; i <= list_size; i++)
	{
		if (int(Zerr.get(i,1)) == -1)
			continue;

		txt << "Point " << i << ": " << Zerr.get(i,1) << "\n";
	}

	txt << "\nPoints used: " << pts_used << "\n";
	txt << "Z error average: " << Zmean << "\n";
	txt << "Z error standard deviation: " << Zstd << "\n\n";

	return txt.str();
}

void DemGrid::overlayMap(Matrix *map)
{
	if (map->getCols() != DEM.getCols() || map->getRows() != DEM.getRows())
		return;

	for (int i=1; i<=DEM.getRows(); i++)
	{
		for (int j=1; j<=DEM.getCols(); j++)
		{
			if (map->get(i,j) - 0.0 < 0.000000000001)
				continue;

			DEM.set(i,j,map->get(i,j));
		}
	}
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
