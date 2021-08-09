#ifndef DEMGRID_H
#define DEMGRID_H
/******************************************************************************
                                DemGrid.h
*******************************************************************************/
/*Copyright 2002-2021 e-foto team (UERJ)
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
* class DemGrid
*/

#include <string>
#include "MatchingPoints.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class DEMManager;
class MatchingPointsGrid;
enum class InterpolationMethod { NORMAL, FAST};

class DemGrid {

public:
    explicit DemGrid(double, double, double, double, double, double);
    void linkManager(DEMManager* _man)
    {
        manager = _man;
    };
    enum saveMode { efotoDEM, ascii };
    void saveDem(char*, int);
    void loadDem(char*, int);

    void setPointList(MatchingPointsList*);
    void interpolateNearestPoint();
    void interpolateMovingAverage(double, double, int);
    void interpolateTrendSurface(int);
    void interpolateMovingSurface(double, double, int, int);
    double getElapsedTime() const
    {
        return elap_time;
    };


    void getMinMax(double&, double&) const;   // Return Min Z and Max Z
    double getMeanZ() const;

    double getHeightXY(double X, double Y);
    double getHeight(double row, double col) const;
    Matrix* getDemImage(double min = 0.0, double max = 0.0);

    int getWidth() const
    {
        return dem_width;
    };
    int getHeight() const
    {
        return dem_height;
    };

    void getDemParameters(double& _Xi,
                          double& _Yi,
                          double& _Xf,
                          double& _Yf,
                          double& _res_x,
                          double& _res_y) const
    {
        _Xi = Xi;
        _Yi = Yi;
        _Xf = Xf;
        _Yf = Yf;
        _res_x = res_x;
        _res_y = res_y;
    };
    std::string calculateDemQuality(MatchingPointsList mpl);
    void overlayMap(Matrix* map) const;
    void setCancel()
    {
        cancel_flag = true;
    };

private:
    Matrix DEM;
    bool cancel_flag{false};
    double Xi, Yi, Xf, Yf, res_x, res_y;
    double elap_time{0.0};
    unsigned int dem_width, dem_height;
    MatchingPointsList* point_list;
    MatchingPointsGrid* mpg;
    void saveDemEfoto(char*) const;
    void loadDemEfoto(char*);
    void saveDemAscii(char*) const;
    double getAsciiParameter(std::ifstream*, std::string);
    void loadDemAscii(char*);
    DEMManager* manager;

    // Used with a lot of points
    void interpolateNearestPointFast();
    void interpolateMovingAverageFast(double, double, int);
    void interpolateTrendSurfaceFast(int);
    void interpolateMovingSurfaceFast(double, double, int, int);

    // Used with a few points
    void interpolateNearestPointNormal();
    void interpolateMovingAverageNormal(double, double, int);
    void interpolateMovingSurfaceNormal(double, double, int, int);

    InterpolationMethod chooseBestInterpolationMethod(double nf);

    void createNewGrid(double, double, double, double, double, double);
    void cutGrid(double min, double max, bool fromList);
    void eliminateBadPointsGrid(double sigma);
    double getStdZ();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif



