#ifndef MATCHINGPOINTS_H
#define MATCHINGPOINTS_H
/******************************************************************************
                           MatchingPoints.h
*******************************************************************************/
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
/**
* class MatchingPoints
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 15/08/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

#include <vector>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

class MPImagePairs
{
public:
    int img1, img2;
    double Xi, Xf, Yi, Yf;
};

class stackCellQuick
{
public:
    int x, y;
    stackCellQuick *prev;
};

class MatchingPoints
{
public:
    MatchingPoints();
    int matching_id;
    int left_image_id, right_image_id;
    double left_x, left_y, right_x, right_y;
    double X, Y, Z;
    double matching_accuracy;

private:

};

class MatchingPointsList
{
public:
    explicit MatchingPointsList() { bad_points = 0; }
    enum saveType { saveFull, savePairs, savePairsHann, saveXYZ };
    enum loadType { loadFull, loadPairs, loadPairsHann, loadXYZ, loadidXYZ };
    enum sortCriteria { byLeft, byRight, by3D, byGridpos };
    void clear() { list.clear(); }
    void add(int, int, double, double, double, double, double, double, double, double);
    void add(int, int, double, double, double, double, double);
    void add(double, double, double);
    void modify(int, int, int, double, double, double, double, double);
    void modify(int, double, double, double);
    void remove(int);
    void filterBadPoints2D(double sigma_x=3, double sigma_y=1.5);
    void listMp();
    void listMp(int);
    MatchingPoints* get(int);
    int save(char *, int);
    int load(char *, int, bool append=false, int left_id=-1, int right_id=-1);
    unsigned int size() { return list.size(); }
    unsigned int getNumBadPoints() { return bad_points; }
    void sortList(sortCriteria criteria);
    void leftImageBoundingBox(int &, int &, int &, int &);
    void rightImageBoundingBox(int &, int &, int &, int &);
    void XYZboundingBox(double &, double &, double &, double &, double &, double &);
    Matrix *getDemImage(double res_x, double res_y);
    bool hasEmptyPairs();

private:
    std::vector <MatchingPoints> list;
    unsigned int bad_points;
    void Quicksort(double **, int, int);
    void switchElements(int, int);
    bool pop(int &, int &);
    bool push(int, int);
    void emptyStack();
    stackCellQuick *stack, *aux;
    double **array;
    void createAuxiliaryList();
    void deleteAuxiliaryList();
    void updateList();
    void copyListByLeft();
    void copyListByRight();
    void copyListBy3D();
    void boundingBox(double &, double &, double &, double &, double &, double &, int);
    bool checkFile(char *, int);
    int strTokensCount(char *);
    int checkImagePairs(int left, int right, bool add=false);
    void identImagePairs();
    std::vector <MPImagePairs> MPImagePairsList;
    void drawCross(Matrix *, unsigned int, unsigned int, double);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif

