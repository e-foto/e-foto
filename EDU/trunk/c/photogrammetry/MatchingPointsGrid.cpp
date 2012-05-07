#include "MatchingPointsGrid.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

MatchingPointsGrid::MatchingPointsGrid(MatchingPointsList *plist, double rx, double ry)
{
	point_list = plist;
	createNewGrid(rx, ry);
}

void MatchingPointsGrid::createNewGrid(double rx, double ry)
{
	double Zi, Zf;
	point_list->XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);

	res_x = rx;
	res_y = ry;

	width = 2 + (Xf - Xi)/res_x;
	height = 2 + (Yf - Yi)/res_y;

	// Create empty structure
	printf("Map size: %d x %d\n",width,height);
	printf("Creating empty structure ...\n");
	MPGCell cell;
	gridMap.clear();
	for (unsigned int i=0; i<width*height; i++)
		gridMap.push_back(cell);

	createStructure();
}

void MatchingPointsGrid::createStructure()
{
	double X,Y;
	int col,row;

	no_inside_points = no_outside_points = 0;

	MatchingPoints *mp;

	if (point_list == NULL)
	{
		printf("Error! No point list assigned.\n");
		return;
	}

	printf("Creating grid map for %d points ...\n",point_list->size());
	for (unsigned int i=1; i<=point_list->size(); i++)
	{
		mp = point_list->get(i);

		X = mp->X;
		Y = mp->Y;

		// Calculate grid position
		col = round((X - Xi)/res_x);
		row = round((Y - Yi)/res_y);

		if (col<0 || row<0 || col>=width || row>=height)
		{
			no_outside_points++;
			continue;
		}

		no_inside_points++;

		unsigned int pos = col + (row * width);

		gridMap.at(pos).points.push_back(i);
	}

	//	printf("Number of inside points: %d\n",no_inside_points);
	//	printf("Number of outside points: %d\n",no_outside_points);
	//	printf("Total number of points: %d\n",no_inside_points+no_outside_points);
}

/*
 * Calculate nearest point
 * Return: id of point
 */
int MatchingPointsGrid::getNearestPoint(double X, double Y)
{
	// Calculate grid position
	int col = round((X - Xi)/res_x);
	int row = round((Y - Yi)/res_y);

	// If out of gridMap, find nearest point of grid
	if (col<0) col=0;
	if (row<0) row=0;
	if (col>=width) col=width-1;
	if (row>=height) row=height-1;

	// Calculate window by step left and right (col-w to col+w and row-h to row+h)
	// Must take in account one additional cell for all sides
	int w, h, pos, point_id, best_point_id=-1;
	w = 1;
	h = 1;

	double distance, min_dist=-1.0, Xc, Yc;
	MatchingPoints *mp;
	while (best_point_id < 0)
	{
		for (int y=row-h; y<row+h; y++)
		{
			for (int x=col-w; x<col+w; x++)
			{
				// Check if inside gridMap
				if (x<0 || y<0 || x>=width || y>=height)
					continue;

				pos = x + (y * width);

				for (int p=0; p<gridMap.at(pos).points.size(); p++)
				{
					point_id = gridMap.at(pos).points.at(p);

					// Get current point at list and its coordinates
					mp = point_list->get(point_id);
					Xc = mp->X;
					Yc = mp->Y;

					// Check distance
					distance = sqrt(pow(X - Xc,2) + pow(Y - Yc,2));

					// If distance is shorter
					if (distance <= min_dist || min_dist<0.0)
					{
						min_dist = distance;
						best_point_id = point_id;
					}
				}
			}
		}

		// Enlarge window
		w++;
		h++;

		// Check if window is too large
		if (w > width || h > height)
			break;
	}

	return best_point_id;

	/*
 // Comparisson between this approach and full list distance checking
 printf("Best point: %d\n",best_point_id);

 // Full test
 min_dist = -1;
 best_point_id = -1;
 for (int i=0; i<point_list->size(); i++)
 {
  mp = point_list->get(i+1);
  Xc = mp->X;
  Yc = mp->Y;
  distance = sqrt(pow(X - Xc,2) + pow(Y - Yc,2));
  if (distance <= min_dist || min_dist<0.0)
  {
   min_dist = distance;
   best_point_id = i+1;
  }
 }
 printf("Best point: %d\n",best_point_id);
*/
}

/*
 * Select list of points that matches a certain distance
 */
void MatchingPointsGrid::getPointsClose(double X, double Y, double dist)
{
	// Calculate grid position
	int col = round((X - Xi)/res_x);
	int row = round((Y - Yi)/res_y);

	// If out of gridMap, find nearest point of grid
	if (col<0) col=0;
	if (row<0) row=0;
	if (col>=width) col=width-1;
	if (row>=height) row=height-1;

	// Calculate window by step left and right (col-w to col+w and row-h to row+h)
	// Must take in account one additional cell for all sides
	int w, h, pos, point_id;
	w = 1 + round(dist/res_x);
	h = 1 + round(dist/res_y);

	// Clear selected points
	selected_points.clear();

	// Add points near
	double distance, Xc, Yc;
	MatchingPoints *mp;
	for (int y=row-h; y<row+h; y++)
	{
		for (int x=col-w; x<col+w; x++)
		{
			// Check if inside gridMap
			if (x<0 || y<0 || x>=width || y>=height)
				continue;

			pos = x + (y * width);

			for (int p=0; p<gridMap.at(pos).points.size(); p++)
			{
				point_id = gridMap.at(pos).points.at(p);

				// Get current point at list and its coordinates
				mp = point_list->get(point_id);
				Xc = mp->X;
				Yc = mp->Y;

				// Check distance
				distance = sqrt(pow(X - Xc,2) + pow(Y - Yc,2));

				// If distance is shorter than given distance, add to list
				if (distance <= dist)
					selected_points.push_back(point_id);
			}
		}
	}

	/*
 // Comparisson between this approach and full list distance checking
 printf("Found: %d points.\n",(int)selected_points.size());

 for (int i=0; i<selected_points.size(); i++)
  printf("%d\n",selected_points.at(i));

 printf("\n");

 // Full test
 for (int i=0; i<point_list->size(); i++)
 {
  mp = point_list->get(i+1);
  Xc = mp->X;
  Yc = mp->Y;
  distance = sqrt(pow(X - Xc,2) + pow(Y - Yc,2));
  if (distance <= dist)
   printf("%d\n",i+1);
 }
*/
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
