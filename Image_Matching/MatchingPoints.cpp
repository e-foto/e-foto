#include "MatchingPoints.h"

/*******************
 * Matching Points *
 *******************/

MatchingPoints::MatchingPoints()
{
	// Clear all variables
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
		fprintf(fp,"Pair\tL_ID\tR_ID\tL_x\t\tL_y\t\tR_x\t\tR_y\t\tX\t\tY\t\tZ\t\tAcc\n");

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
			default: fprintf(fp,"%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",i, mp->left_image_id, mp->right_image_id, mp->left_x, mp->left_y, mp->right_x, mp->right_y, mp->X, mp->Y, mp->Z, mp->matching_accuracy);
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

	int i;
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
			default: fscanf(fp,"%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", &i, &left_id, &right_id, &lx, &ly, &rx, &ry, &X, &Y, &Z, &acc);
		}

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

