#ifndef MATCHINGPOINTSGRID_H
#define MATCHINGPOINTSGRID_H
/******************************************************************************
                           MatchingPointsGrid.h
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
/*
 * This class was designed to increase the performance to find point close to a given point.
 * The method used in E-FOTO DEM module, version 2.6, performe an exhaustive search for points,
 * in On^2 order.
 */

/**
* class MatchingPointsGrid
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 10/10/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

#include <vector>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class MatchingPointsList;

class MPGCell
{
public:
    std::vector <int> points;

};

class MatchingPointsGrid
{
public:
    explicit MatchingPointsGrid(MatchingPointsList *, double, double);
    void createNewGrid(double, double);
    int getNearestPoint(double, double);
    void getPointsClose(double, double, double);
    std::vector <int> selected_points;

private:
    unsigned int width, height;
    double Xi, Xf, Yi, Yf;
    double res_x, res_y;
    std::vector <MPGCell> gridMap;
    int no_inside_points, no_outside_points;
    MatchingPointsList *point_list;
    void createStructure();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
