#include "MatchingPoints.h"

/*******************
 * Matching Points *
 *******************/

MatchingPoints::MatchingPoints()
{
	// Clear all variables
	matching_id = -1;
	left_image_id = right_image_id = -1;
	left_x = left_y = right_x = right_y = 0.0;
	X = Y = Z = 0.0;
	matching_accuracy = 0.0;
}


/************************
 * Matching Points List *
 ************************/

void MatchingPointsList::add(int left_id, int right_id, double lx, double ly, double rx, double ry, double acc)
{
	MatchingPoints mp;
	mp.left_image_id = left_id;
	mp.right_image_id = right_id;
	mp.left_x = lx;
	mp.left_y = ly;
	mp.right_x = rx;
	mp.right_y = ry;
	mp.matching_accuracy = acc;

	list.push_back(mp);
}

// List ranges from 1 to size
void MatchingPointsList::modify(int pos, int left_id, int right_id, double lx, double ly, double rx, double ry, double acc)
{
	if (pos < 1 || pos > list.size())
		return;

	MatchingPoints *mp = &list.at(pos-1);
	mp->left_image_id = left_id;
	mp->right_image_id = right_id;
	mp->left_x = lx;
	mp->left_y = ly;
	mp->right_x = rx;
	mp->right_y = ry;
	mp->matching_accuracy = acc;
}

void MatchingPointsList::modify(int pos, double X, double Y, double Z)
{
	if (pos < 1 || pos > list.size())
		return;

	MatchingPoints *mp = &list.at(pos-1);
	mp->X = X;
	mp->Y = Y;
	mp->Z = Z;
}

void MatchingPointsList::remove(int pos)
{
	if (pos < 1 || pos > list.size())
		return;

	list.erase(list.begin()+(pos-1));
}

MatchingPoints* MatchingPointsList::get(int pos)
{
	if (pos < 1 || pos > list.size())
		return NULL;

	return &list.at(pos-1);
}

void MatchingPointsList::filterBadPoints2D(double sigma_x, double sigma_y)
{
	double avg_x, avg_y, std_x, std_y;

	// Calculate average
	avg_x = avg_y = 0.0;
	for (int i=0; i<list.size(); i++)
	{
		avg_x += fabs(list.at(i).left_x - list.at(i).right_x);
		avg_y += fabs(list.at(i).left_y - list.at(i).right_y);
	}
	avg_x = avg_x / double(list.size());
	avg_y = avg_y / double(list.size());

	// Calculate standard deviation
	std_x = std_y = 0.0;
	for (int i=0; i<list.size(); i++)
	{
		std_x += pow(fabs(list.at(i).left_x - list.at(i).right_x) - avg_x, 2);
		std_y += pow(fabs(list.at(i).left_y - list.at(i).right_y) - avg_y, 2);
	}
	std_x = sqrt(std_x / double(list.size()));
	std_y = sqrt(std_y / double(list.size()));

	// Eliminate bad points (faster with 2 lists)
	std_x = std_x * sigma_x;
	std_y = std_y * sigma_y;
	double delta_x, delta_y;
	bad_points = 0;
	vector <MatchingPoints> new_list;
	for (int i=0; i < list.size(); i++)
	{
		delta_x = fabs(list.at(i).left_x - list.at(i).right_x);
		delta_y = fabs(list.at(i).left_y - list.at(i).right_y);

		if (delta_x < avg_x - std_x || delta_x > avg_x + std_x || delta_y < avg_y - std_y || delta_y > avg_y + std_y)
		{
			bad_points++;
			continue;
		}

		new_list.push_back(list.at(i));
	}

	// Copy new list
	list = new_list;
}

void MatchingPointsList::listMp()
{
	printf("Pair\tL_ID\tR_ID\tL_x\t\tL_y\t\tR_x\t\tR_y\t\tX\t\tY\t\tZ\t\tAcc\n");

	MatchingPoints *mp;

	for (int i=1; i<=list.size(); i++)
	{
		mp = &list.at(i-1);
		printf("%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",i, mp->left_image_id, mp->right_image_id, mp->left_x, mp->left_y, mp->right_x, mp->right_y, mp->X, mp->Y, mp->Z, mp->matching_accuracy);
	}
}

void MatchingPointsList::listMp(int pos)
{
	if (pos < 1 || pos > list.size())
		return;

	printf("Pair\tL_ID\tR_ID\tL_x\t\tL_y\t\tR_x\t\tR_y\t\tX\t\tY\t\tZ\t\tAcc\n");
	MatchingPoints *mp = &list.at(pos-1);
	printf("%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",pos, mp->left_image_id, mp->right_image_id, mp->left_x, mp->left_y, mp->right_x, mp->right_y, mp->X, mp->Y, mp->Z, mp->matching_accuracy);
}

int MatchingPointsList::save(char *filename, saveType type)
{
	FILE *fp;
	fp = fopen(filename,"wt");

	if (!fp)
		return 0;

	if (type == 0)
		fprintf(fp,"Pair\tM_ID\tL_ID\tR_ID\tL_x\t\tL_y\t\tR_x\t\tR_y\t\tX\t\tY\t\tZ\t\tAcc\n");

	if (type == 2)
		fprintf(fp,"10002\t0.0\t0.0\t0.0\t0.0\n");

	MatchingPoints *mp;

	for (int i=1; i<=list.size(); i++)
	{
		mp = &list.at(i-1);

		switch (type)
		{
			case 1 : fprintf(fp,"%f\t%f\t%f\t%f\n",mp->left_x, mp->left_y, mp->right_x, mp->right_y); break;
			case 2 : fprintf(fp,"%d\t%f\t%f\t%f\t%f\n",i, mp->left_x, mp->left_y, mp->right_x, mp->right_y); break;
			case 3 : fprintf(fp,"%f\t%f\t%f\n",mp->X, mp->Y, mp->Z); break;
			default: fprintf(fp,"%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",i,mp->matching_id, mp->left_image_id, mp->right_image_id, mp->left_x, mp->left_y, mp->right_x, mp->right_y, mp->X, mp->Y, mp->Z, mp->matching_accuracy);
		}
	}

	if (type == 2)
		fprintf(fp,"-99\t0.0\t0.0\t0.0\t0.0\n");

	fclose(fp);

	return 1;
}

int MatchingPointsList::load(char *filename, loadType type, bool append, int left_id, int right_id)
{
	FILE *fp;
	fp = fopen(filename,"rt");

	if (!fp)
		return 0;

	char line[255];
	if (type == 0 || type == 2)
		fgets(line,255,fp);

	// Clear list
	if (!append)
		list.clear();

	int i, m_id;
	float lx, ly, rx, ry, X, Y, Z, acc;
	lx = ly = rx = ry = X = Y = Z = acc = 0.0;
	MatchingPoints mp;

	while (!feof(fp))
	{
		switch (type)
		{
			case 1 : fscanf(fp,"%f\t%f\t%f\t%f\n",&lx, &ly, &rx, &ry); break;
			case 2 : fscanf(fp,"%d\t%f\t%f\t%f\t%f\n",&i, &lx, &ly, &rx, &ry); break;
			case 3 : fscanf(fp,"%f\t%f\t%f\n",&X, &Y, &Z); break;
			case 4 : fscanf(fp,"%d\t%f\t%f\t%f\n",&i,&X, &Y, &Z); break;
			default: fscanf(fp,"%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", &i, &m_id, &left_id, &right_id, &lx, &ly, &rx, &ry, &X, &Y, &Z, &acc);
		}

		mp.matching_id = m_id;
		mp.left_image_id = left_id;
		mp.right_image_id = right_id;
		mp.left_x = lx;
		mp.left_y = ly;
		mp.right_x = rx;
		mp.right_y = ry;
		mp.X = X;
		mp.Y = Y;
		mp.Z = Z;
		mp.matching_accuracy = acc;

		list.push_back(mp);
	}

	if (type == 2)
		list.pop_back();

	fclose(fp);

	return 1;
}

/*********************
 * List bounding box *
 *********************/

void MatchingPointsList::leftImageBoundingBox(int &xi, int &yi, int &xf, int &yf)
{
	double xxi, yyi, xxf, yyf, zi, zf;
	boundingBox(xxi, yyi, xxf, yyf, zi, zf, 0);
	xi = int(xxi);
	yi = int(yyi);
	xf = int(xxf);
	yf = int(yyf);
}

void MatchingPointsList::rightImageBoundingBox(int &xi, int &yi, int &xf, int &yf)
{
	double xxi, yyi, xxf, yyf, zi, zf;
	boundingBox(xxi, yyi, xxf, yyf, zi, zf, 1);
	xi = int(xxi);
	yi = int(yyi);
	xf = int(xxf);
	yf = int(yyf);
}

void MatchingPointsList::XYZboundingBox(double &xi, double &yi, double &xf, double &yf, double &zi, double &zf)
{
	boundingBox(xi, yi, xf, yf, zi, zf, 2);
}

void MatchingPointsList::boundingBox(double &xi, double &yi, double &xf, double &yf, double &zi, double &zf, int type)
{
	xi = yi = xf = yf = 0.0;

	if (list.size()<1)
		return;

	double x,y,z;

	switch(type)
	{
		case 0 : x = double(list.at(0).left_x); y = double(list.at(0).left_y); break;
		case 1 : x = double(list.at(0).right_x); y = double(list.at(0).right_y); break;
		default : x = list.at(0).X; y = list.at(0).Y; z = list.at(0).Z; break;
	}

	xi = xf = x;
	yi = yf = y;
	zi = zf = z;

	for (int i=1; i<list.size(); i++)
	{
		switch(type)
		{
			case 0 : x = double(list.at(i).left_x); y = double(list.at(i).left_y); break;
			case 1 : x = double(list.at(i).right_x); y = double(list.at(i).right_y); break;
			default : x = list.at(i).X; y = list.at(i).Y; z = list.at(i).Z; break;
		}

		if (x < xi) xi = x;
		if (y < yi) yi = y;
		if (z < zi) zi = z;
		if (x > xf) xf = x;
		if (y > yf) yf = y;
		if (z > zf) zf = z;
	}
}

/*************
 * List sort *
 *************/

bool MatchingPointsList::pop(int &x, int &y)
{
	if(stack != NULL)
	{
		x = stack->x;
		y = stack->y;
		aux = stack;
		stack = stack->prev;
		delete aux;
		return 1;
	}    
	else
	{
		return 0;
	}
}   
 
bool MatchingPointsList::push(int x, int y)
{
	aux = stack;
	stack = new stackCellQuick();
	stack->x = x;
	stack->y = y;
	stack->prev = aux;
	return 1;
}    

void MatchingPointsList::emptyStack()
{
	int x, y;
	while(pop(y,x));
}

void MatchingPointsList::switchElements(int i, int j)
{
	double aux[3];
	aux[0] = array[i][0];
	aux[1] = array[i][1];
	aux[2] = array[i][2];
	array[i][0] = array[j][0];
	array[i][1] = array[j][1];
	array[i][2] = array[j][2];
	array[j][0] = aux[0];
	array[j][1] = aux[1];
	array[j][2] = aux[2];
}

// Quick sort Algorithm based on:
// http://ds4beginners.wordpress.com/2006/09/22/quick-sort/
// http://pt.wikipedia.org/wiki/Quicksort
// http://en.wikipedia.org/wiki/Quicksort
// We improved the quick sort algorith changing the recursive call for stack, in order to deal with large point list
void MatchingPointsList::Quicksort(double **a, int l, int r)
{
	double t;
	double pivot[3];
	int n = list.size();
	int i, j;

	emptyStack();
	push(l,r);

	while(pop(l,r))
	{
		if(l>=r)
			continue;

		i=l;
		j=r+1;

		pivot[0] = a[l][0];
		pivot[1] = a[l][1];
		pivot[2] = a[l][2];

		while(true)
		{

			while(true)
			{
				i++;
				if (i>=n)
					break;

				if (a[i][1]==pivot[1] && a[i][2]<pivot[2])
					continue;

				if (a[i][1]>=pivot[1])
					break;
			}

			while(true)
			{
				j--;

				if (j<0)
					break;

				if (a[j][1]==pivot[1] && a[j][2]>pivot[2])
					continue;

				if (a[j][1]<=pivot[1])
					break;
			}

			if(i>=j)
				break;

			switchElements(i,j);
		}

		switchElements(l,j);

		if (!push(j+1,r))
			return;
		if (!push(l,j-1))
			return;
	}
}

void MatchingPointsList::createAuxiliaryList()
{
	int n = list.size();

	array = new double*[n];
	for (int i=0; i<n; i++)
		array[i] = new double[3];
}

void MatchingPointsList::deleteAuxiliaryList()
{
	int n = list.size();

	for (int i=0; i<n; i++)
		delete [] array[i];
	delete [] array;
}

void MatchingPointsList::sortList(sortCriteria criteria)
{
	// Max size of signed int 2147483647
	if (list.size() > 2147483647)
	{
		printf("Error: point list is too large for sorting.\n");
		return;
	}

	int n = list.size();
	stack = NULL;

	createAuxiliaryList();
	printf("Preparing list to be sorted ...\n");
	switch (criteria)
	{
		case 0 : copyListByLeft(); break;
		case 1 : copyListByRight(); break;
		default : copyListBy3D(); break;
	}

	printf("Sorting %d elements ...\n",n);
	Quicksort(array,0,n-1);
	printf("Updating data ...\n");
	updateList();
	deleteAuxiliaryList();
}

void MatchingPointsList::updateList()
{
	vector <MatchingPoints> listaux;
	MatchingPoints mp;
	int pos;

	for (int i=0; i<list.size(); i++)
	{
		pos = array[i][0];

		mp.left_image_id = list.at(pos).left_image_id;
		mp.right_image_id = list.at(pos).right_image_id;
		mp.left_x = list.at(pos).left_x;
		mp.left_y = list.at(pos).left_y;
		mp.right_x = list.at(pos).right_x;
		mp.right_y = list.at(pos).right_y;
		mp.X = list.at(pos).X;
		mp.Y = list.at(pos).Y;
		mp.Z = list.at(pos).Z;
		mp.matching_accuracy = list.at(pos).matching_accuracy;

		listaux.push_back(mp);
	}

	list = listaux;
}

void MatchingPointsList::copyListByLeft()
{
	for (int i=0; i<list.size(); i++)
	{
		array[i][0] = i;
		array[i][1] = double(list.at(i).left_x);
		array[i][2] = double(list.at(i).left_y);
	}
}

void MatchingPointsList::copyListByRight()
{
	for (int i=0; i<list.size(); i++)
	{
		array[i][0] = i;
		array[i][1] = double(list.at(i).right_x);
		array[i][2] = double(list.at(i).right_y);
	}
}

void MatchingPointsList::copyListBy3D()
{
	for (int i=0; i<list.size(); i++)
	{
		array[i][0] = i;
		array[i][1] = list.at(i).X;
		array[i][2] = list.at(i).Y;
	}
}
