/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MatchingPoints.h"

#include "Matrix.h"

#include <iomanip>
//#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

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

void MatchingPointsList::add(int left_id, int right_id, double lx, double ly, double rx, double ry, double X, double Y, double Z, double acc)
{
    MatchingPoints mp;
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

void MatchingPointsList::add(double X, double Y, double Z)
{
    MatchingPoints mp;
    mp.X = X;
    mp.Y = Y;
    mp.Z = Z;

    list.push_back(mp);
}

// List ranges from 1 to size
void MatchingPointsList::modify(int pos, int left_id, int right_id, double lx, double ly, double rx, double ry, double acc)
{
    if (pos < 1 || unsigned(pos) > list.size())
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
    if (pos < 1 || unsigned(pos) > list.size())
        return;

    MatchingPoints *mp = &list.at(pos-1);
    mp->X = X;
    mp->Y = Y;
    mp->Z = Z;
}

void MatchingPointsList::remove(int pos)
{
    if (pos < 1 || unsigned(pos) > list.size())
        return;

    list.erase(list.begin()+(pos-1));
}

MatchingPoints* MatchingPointsList::get(int pos)
{
    if (pos < 1 || unsigned(pos) > list.size())
        return NULL;

    return &list.at(pos-1);
}

void MatchingPointsList::filterBadPoints2D(double sigma_x, double sigma_y)
{
    double avg_x, avg_y, std_x, std_y;

    // Calculate average
    avg_x = avg_y = 0.0;
    for (unsigned int i=0; i<list.size(); i++)
    {
        avg_x += fabs(list.at(i).left_x - list.at(i).right_x);
        avg_y += fabs(list.at(i).left_y - list.at(i).right_y);
    }
    avg_x = avg_x / double(list.size());
    avg_y = avg_y / double(list.size());

    // Calculate standard deviation
    std_x = std_y = 0.0;
    for (unsigned int i=0; i<list.size(); i++)
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
    std::vector <MatchingPoints> new_list;
    for (unsigned int i=0; i < list.size(); i++)
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
    std::cout << "Pair\tL_ID\tR_ID\tL_x\t\tL_y\t\tR_x\t\tR_y\t\tX\t\tY\t\tZ\t\tAcc\n";

    MatchingPoints *mp;

    for (unsigned int i=1; i<=list.size(); i++)
    {
        mp = &list.at(i-1);
        std::cout << i << "\t" << mp->left_image_id << "\t" << mp->right_image_id << "\t" << mp->left_x << "\t" << mp->left_y << "\t" << mp->right_x << "\t"
                << mp->right_y << "\t" << mp->X << "\t" << mp->Y << "\t" << mp->Z << "\t" << mp->matching_accuracy << std::endl;
    }
}

void MatchingPointsList::listMp(int pos)
{
    if (pos < 1 || unsigned(pos) > list.size())
        return;

    std::cout << "Pair\tL_ID\tR_ID\tL_x\t\tL_y\t\tR_x\t\tR_y\t\tX\t\tY\t\tZ\t\tAcc\n";
    MatchingPoints *mp = &list.at(pos-1);
    std::cout << pos << "\t" << mp->left_image_id << "\t" << mp->right_image_id << "\t" << mp->left_x << "\t" << mp->left_y << "\t" << mp->right_x
            << "\t" << mp->right_y << "\t" << mp->X << "\t" << mp->Y << "\t" << mp->Z << "\t" << mp->matching_accuracy << std::endl;
}

int MatchingPointsList::save(char *filename, int type)
{
    std::ofstream outfile(filename);

    if (outfile.fail())
        return 0;

    if (type == 0)
        outfile << "Pair\tM_ID\tL_ID\tR_ID\tL_x\tL_y\tR_x\tR_y\tX\tY\tZ\tAcc\n";

    if (type == 2)
        outfile << "10002\t0.0\t0.0\t0.0\t0.0\n";

    MatchingPoints *mp;

    outfile << std::fixed << std::setprecision(5);

    for (unsigned int i=1; i<=list.size(); i++)
    {
        mp = &list.at(i-1);

        switch (type)
        {
        case 1 : outfile << mp->left_x << "\t" << mp->left_y << "\t" << mp->right_x << "\t" << mp->right_y << "\n"; break;
        case 2 : outfile << i << "\t" << mp->left_x << "\t" <<  mp->left_y << "\t" << mp->right_x << "\t" << mp->right_y << "\n"; break;
        case 3 : outfile << mp->X << "\t" << mp->Y << "\t" << mp->Z << "\n"; break;
                case 4 : outfile << i << "\t" << mp->X << "\t" << mp->Y << "\t" << mp->Z << "\n"; break;
        default: outfile << i << "\t" << mp->matching_id << "\t" << mp->left_image_id << "\t" << mp->right_image_id << "\t" << mp->left_x << "\t" << mp->left_y << "\t" << mp->right_x << "\t" << mp->right_y << "\t" << mp->X << "\t" << mp->Y << "\t" << mp->Z << "\t" << mp->matching_accuracy << "\n";
        }
    }

    if (type == 2)
        outfile << "-99\t0.0\t0.0\t0.0\t0.0\n";

    outfile.close();

    return 1;
}

int MatchingPointsList::strTokensCount(char * array)
{
    char * token;
    int tokens=0;

    token = strtok(array," \t");

    while (token != NULL || tokens > 20)
    {
        tokens++;
        token = strtok(NULL," \t");
    }

    return tokens;
}

bool MatchingPointsList::checkFile(char *filename, int type)
{
    char line[256];
    int tokens, exp_tokens;

    // Get number of expected tokens
    switch (type)
    {
    case 1: exp_tokens = 4; break;
    case 2: exp_tokens = 5; break;
    case 3: exp_tokens = 3; break;
    case 4: exp_tokens = 4; break;
    default: exp_tokens = 12; break;
    }

    // Check file
    std::ifstream infile(filename);

    infile.getline(line,256);
    infile.getline(line,256); // Take the second line

    // Check the number of tokens
    tokens = strTokensCount(line);

    infile.close();

    return (tokens == exp_tokens);
}

int MatchingPointsList::load(char *filename, int type, bool append, int left_id, int right_id)
{
    // Check file before loading
    if (!checkFile(filename,type))
        return -1;

    std::ifstream infile(filename);

    if (infile.fail())
        return 0;

    char line[256];
    if (type == 0 || type == 2)
        infile.getline(line,256);

    // Clear list
    if (!append)
        list.clear();

    int i, m_id=-1;
    double lx, ly, rx, ry, X, Y, Z, acc;
    lx = ly = rx = ry = X = Y = Z = acc = 0.0;
    MatchingPoints mp;

    while (!infile.eof())
    {
        lx = X = -1.0;

        switch (type)
        {
        case 1 : infile >> lx >> ly >> rx >> ry; break;
        case 2 : infile >> i >> lx >> ly >> rx >> ry; break;
        case 3 : infile >> X >> Y >> Z; break;
        case 4 : infile >> i >> X >> Y >> Z; break;
        default: infile >> i >> m_id >> left_id >> right_id >> lx >> ly >> rx >> ry >> X >> Y >> Z >> acc; break;
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

        // Check reading error
        if (int(X) == -1 && int(lx) == -1)
            break;

        list.push_back(mp);
    }

    if (type == 2)
        list.pop_back();

    infile.close();

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
    xi = yi = xf = yf = zi = zf = 0.0;

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

    for (unsigned int i=1; i<list.size(); i++)
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
    //double t;
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
    std::vector <MatchingPoints> listaux;
    MatchingPoints mp;
    int pos;

    for (unsigned int i=0; i<list.size(); i++)
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
    for (unsigned int i=0; i<list.size(); i++)
    {
        array[i][0] = i;
        array[i][1] = double(list.at(i).left_x);
        array[i][2] = double(list.at(i).left_y);
    }
}

void MatchingPointsList::copyListByRight()
{
    for (unsigned int i=0; i<list.size(); i++)
    {
        array[i][0] = i;
        array[i][1] = double(list.at(i).right_x);
        array[i][2] = double(list.at(i).right_y);
    }
}

void MatchingPointsList::copyListBy3D()
{
    for (unsigned int i=0; i<list.size(); i++)
    {
        array[i][0] = i;
        array[i][1] = list.at(i).X;
        array[i][2] = list.at(i).Y;
    }
}

/*
 * This function check if there are unmeasured pairs
 */
bool MatchingPointsList::hasEmptyPairs()
{
    for (unsigned int i=0; i<list.size(); i++)
    {
        if (list.at(i).left_x < 0.1 || list.at(i).left_y < 0.1 || list.at(i).right_x < 0.1 || list.at(i).right_y < 0.1)
            return true;
    }

    return false;
}

/******************
 * Generate image *
 ******************/

int MatchingPointsList::checkImagePairs(int left, int right, bool add)
{
    MPImagePairs MPI;
    for (unsigned int i=0; i< MPImagePairsList.size(); i++)
    {
        MPI = MPImagePairsList.at(i);
        if (MPI.img1 == left && MPI.img2 == right)
            return i;
    }

    if (!add)
        return -1;

    // If new, add it
    MPI.img1 = left;
    MPI.img2 = right;
    MPI.Xf = 0.0; MPI.Xf = 0.0; MPI.Yi = 0.0; MPI.Yf = 0.0;
    MPImagePairsList.push_back(MPI);

    return MPImagePairsList.size()-1;
}

void MatchingPointsList::identImagePairs()
{
    MatchingPoints mp;
    int left, right;
    double X,Y;//,Z;

    MPImagePairsList.clear();

    // Check pairs
    for (unsigned int k=0; k<list.size(); k++)
    {
        mp = list.at(k);

        left = mp.left_image_id;
        right = mp.right_image_id;

        checkImagePairs(left, right, true);
    }

    // Calculate bounding box for each pair
    int p;
    MPImagePairs * MPI;
    for (unsigned int k=0; k<list.size(); k++)
    {
        mp = list.at(k);
        X = mp.X;
        Y = mp.Y;

        left = mp.left_image_id;
        right = mp.right_image_id;

        p = checkImagePairs(left, right);

        MPI = &MPImagePairsList.at(p);

        if (X < MPI->Xi || int(MPI->Xi) == 0) MPI->Xi=X;
        if (X > MPI->Xf || int(MPI->Xf) == 0) MPI->Xf=X;
        if (Y < MPI->Yi || int(MPI->Yi) == 0) MPI->Yi=Y;
        if (Y > MPI->Yf || int(MPI->Yf) == 0) MPI->Yf=Y;
    }
}

Matrix * MatchingPointsList::getDemImage(double res_x, double res_y)
{
    double Xi, Yi, Xf, Yf, Zi, Zf;
    XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);

    int dem_width, dem_height;

    // Color code:
    // uint 00 RR GG BB
    // Converted to double 0.0 - 1.0

    // Black bg
    double pixel_point = double(0xFFFF00)/double(0xFFFFFF); // Yellow
    double pixel_border = double(0xFFFFFF)/double(0xFFFFFF); // White

    // Calculate image size
    dem_width = (res_x + (Xf-Xi)) / res_x;
    dem_height = (res_y + (Yf-Yi)) / res_y;

    Matrix *img = new Matrix(dem_height,dem_width);

    // Identify the pairs and their bounding boxes
    identImagePairs();

    // Convert DEM to image - (Color 0.0 to 1.0)
    MatchingPoints mp;
    double X,Y,Z;
    int i,j, left, right;//, p;
    for (unsigned int k=0; k<list.size(); k++)
    {
        mp = list.at(k);
        X = mp.X;
        Y = mp.Y;
        Z = mp.Z;

        if (Z - 0.0 < 0.000000000000001)
            continue;

        left = mp.left_image_id;
        right = mp.right_image_id;
        /*p =*/ checkImagePairs(left, right);

        i = 1 + int((Y-Yi)*res_y);
        j = 1 + int((X-Xi)*res_x);

        if (i<1 || j<1 || i>dem_height || j>dem_width)
            continue;

        //img->set(dem_height-i+1,j,pixel_point);
        drawCross(img, dem_height-i+1, j, pixel_point);
    }

    // Add bounding box
    int xi_bb, xf_bb, yi_bb, yf_bb;
    for (unsigned int k=0; k<MPImagePairsList.size(); k++)
    {
        xi_bb = 1 + int((MPImagePairsList.at(k).Xi-Xi)*res_x);
        xf_bb = 1 + int((MPImagePairsList.at(k).Xf-Xi)*res_x);
        yi_bb = 1 + int((MPImagePairsList.at(k).Yi-Yi)*res_y);
        yf_bb = 1 + int((MPImagePairsList.at(k).Yf-Yi)*res_y);

        for (j=xi_bb; j<=xf_bb; j++)
        {
            img->set(dem_height-yi_bb+1,j,pixel_border);
            img->set(dem_height-yf_bb+1,j,pixel_border);
        }

        for (i=yi_bb; i<=yf_bb; i++)
        {
            img->set(dem_height-i+1,xi_bb,pixel_border);
            img->set(dem_height-i+1,xf_bb,pixel_border);
        }
    }

    return img;
}

void MatchingPointsList::drawCross(Matrix * img, unsigned int i, unsigned int j, double color)
{
    // Cross radius
    unsigned int r=2;

    if (i < r || j < r)
        return;

    for (unsigned int n=i-r; n<=i+r; n++)
    {
        if (n>0 && n<=img->getRows())
            img->set(n,j,color);
    }

    for (unsigned int m=j-r; m<=j+r; m++)
    {
        if (m>0 && m<=img->getCols())
            img->set(i,m,color);
    }
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
