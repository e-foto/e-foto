#ifndef DEMGRID_H
#define DEMGRID_H
/******************************************************************************
                                DemGrid.h
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
* class DemGrid
*
* @author E-Foto group
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

enum class Filetype {BINARY, TEXT};
enum class InterpolationMethod {MOVING_AVERAGE,
                                MOVING_SURFACE,
                                TREND_SURFACE,
                                NEAREST_POINT
                               };
enum class MAWeight {INVERSE_DIST, LINEAR_DEC};
enum class TSSurface {PLANE,
                      LINEAR,
                      PARABOLIC,
                      SECONDDEGREE,
                      THIRDDEGREE
                     };

class DemGrid {

public:
    explicit DemGrid(double, double, double, double, double, double);
    void linkManager(DEMManager* man)
    {
        manager_ = man;
    };
    enum saveMode { efotoDEM, ascii };
    void saveDem(const char*, Filetype) const;
    void loadDem(char*, Filetype);

    void setPointList(MatchingPointsList*);
    void interpolateNearestPoint();
    void interpolateMovingAverage(double, double, MAWeight);
    void interpolateTrendSurface(TSSurface);
    void interpolateMovingSurface(double, double, MAWeight, TSSurface);
    double getElapsedTime()
    {
        return elap_time_;
    };


    void getMinMax(double&, double&);   // Return Min Z and Max Z
    double getMeanZ();

    double getHeightXY(double X, double Y);
    double getHeight(double row, double col);
    Matrix* getDemImage(double min = 0.0, double max = 0.0);
    int getWidth()
    {
        return dem_width_;
    };
    int getHeight()
    {
        return dem_height_;
    };
    void printData();
    void getDemParameters(double& Xi,
                          double& Yi,
                          double& Xf,
                          double& Yf,
                          double& res_x,
                          double& res_y)
    {
        Xi = Xi_;
        Yi = Yi_;
        Xf = Xf_;
        Yf = Yf_;
        res_x = res_x_;
        res_y = res_y_;
    };
    std::string calculateDemQuality(MatchingPointsList mpl);
    void overlayMap(const Matrix* const map);
    void setCancel()
    {
        cancel_flag_ = true;
    };

private:
    Matrix DEM_;
    bool cancel_flag_;
    double Xi_, Yi_, Xf_, Yf_, res_x_, res_y_;
    double elap_time_;
    unsigned int dem_width_, dem_height_;
    MatchingPointsList* point_list_;
    MatchingPointsGrid* mpg_;
    DEMManager* manager_;
    enum class InterpolationMode {NORMAL, FAST};

    void saveDemEfoto(const char*) const;
    void loadDemEfoto(const char*);
    void saveDemAscii(const char*) const;
    double getAsciiParameter(std::ifstream*, std::string);
    void loadDemAscii(char*);

    // Used with a lot of points
    void interpolateNearestPointFast();
    void interpolateMovingAverageFast(double, double, MAWeight);
    void interpolateTrendSurfaceFast(TSSurface);
    void interpolateMovingSurfaceFast(double, double, MAWeight, TSSurface);

    // Used with a few points
    void interpolateNearestPointNormal();
    void interpolateMovingAverageNormal(double, double, MAWeight);
    void interpolateMovingSurfaceNormal(double, double, MAWeight, TSSurface);

    //int chooseBestInterpolationMethod(double nf);
    InterpolationMode chooseBestInterpolationMethod(double nf);

    void createNewGrid(double, double, double Xf, double Yf, double res_x,
                       double res_y);
    void cutGrid(double min, double max, bool fromList);
    void eliminateBadPointsGrid(double sigma);
    double getStdZ();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif



