/*Copyright 2002-2018 e-foto team (UERJ)
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

#include "DemGrid.h"
#include "DEMManager.h"
#include "MatchingPointsGrid.h"

#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DemGrid::DemGrid(double Xi,
                 double Yi,
                 double Xf,
                 double Yf,
                 double res_x,
                 double res_y):
    manager_{nullptr},
    point_list_{nullptr},
    mpg_{nullptr},
    cancel_flag_{false},
    Xi_ {Xi},
    Yi_ {Yi},
    Xf_ {Xf},
    Yf_ {Yf},
    res_x_ {res_x},
    res_y_ {res_y},
    elap_time_{0.0}
{
    // Calculate DEM size
    dem_width_ = static_cast<int>(1.0 + floor((Xf_ - Xi_) / res_x_));
    dem_height_ = static_cast<int>(1.0 + floor((Yf_ - Yi_) / res_y_));
    // Create DEM
    DEM_.resize(dem_height_, dem_width_);
}

void DemGrid::setPointList(MatchingPointsList* mpl)
{
    // Link
    point_list_ = mpl;
}

void DemGrid::getMinMax(double& min, double& max) const
{
    max = min = DEM_.get(1, 1);
    double Z;

    for (size_t i = 1; i <= dem_height_; i++) {
        for (size_t j = 1; j <= dem_width_; j++) {
            Z = DEM_.get(i, j);

            if (fabs(Z) < 0.00000000001) {
                continue;
            }

            if (Z < min) {
                min = Z;
            } else if (Z > max) {
                max = Z;
            }
        }
    }
}

double DemGrid::getMeanZ() const
{
    double meanZ {0.0}, Z{0.0};
    unsigned int count {0};

    for (size_t i = 1; i <= dem_height_; i++) {
        for (size_t j = 1; j <= dem_width_; j++) {
            Z = DEM_.get(i, j);

            // If hole, continue
            if (fabs(Z) < 0.000000000000000001) {
                continue;
            }

            meanZ += Z;
            count++;
        }
    }

    return meanZ / count;
}

double DemGrid::getStdZ() const
{
    const auto meanZ = getMeanZ();
    double stdZ {0.0}, Z{0.0};
    unsigned int count {0};

    for (size_t i = 1; i <= dem_height_; i++) {
        for (size_t j = 1; j <= dem_width_; j++) {
            Z = DEM_.get(i, j);

            // If hole, continue
            if (fabs(Z) < 0.000000000000000001) {
                continue;
            }

            stdZ += pow(Z - meanZ, 2);
            count++;
        }
    }

    return sqrt(stdZ / count);
}

double DemGrid::getHeightXY(double X, double Y) const
{
    // Calculate col,row - double
    const auto col = 1.0 + (X - Xi_) / res_x_;
    const auto row = 1.0 + (Y - Yi_) / res_y_;
    return getHeight(row, col);
}

double DemGrid::getHeight(double row, double col) const
{
    // Check limits
    if (col < 1.0) {
        col = 1.0;
    }

    if (row < 1.0) {
        row = 1.0;
    }

    if (col >= static_cast<double>(dem_width_)) {
        col = static_cast<double>(dem_width_) - 1;
    }

    if (row >= static_cast<double>(dem_height_)) {
        row = static_cast<double>(dem_height_) - 1;
    }

    // Calculate Z using bilinear interpolation
    const auto col_i = floor(col), row_i = floor(row);
    const auto delta_x = col - col_i, delta_y = row - row_i;
    const auto Z = DEM_.get(row_i, col_i) * (1 - delta_x) * (1 - delta_y)
                   + DEM_.get(row_i, col_i + 1) * delta_x * (1 - delta_y)
                   + DEM_.get(row_i + 1, col_i) * (1 - delta_x) * delta_y
                   + DEM_.get(row_i + 1, col_i + 1) * delta_x * delta_y;
    return Z;
}

Matrix* DemGrid::getDemImage(double min, double max)
{
    if (static_cast<int>(min) == 0 && static_cast<int>(max) == 0) {
        getMinMax(min, max);
    }

    if (min >= max) {
        printf("Error: min value is greater or equal max.\n");
        return nullptr;
    }

    Matrix* img = new Matrix(dem_height_, dem_width_);
    // Convert DEM to image - (0.0 to 1.0)
    const auto deltaZ {
        max - min
    };
    constexpr auto p1 {1.0 / 255.0};
    constexpr auto p2 {1 - p1};
    double gray, Z;

    for (size_t i = 1; i <= dem_height_; i++) {
        for (size_t j = 1; j <= dem_width_; j++) {
            Z = DEM_.get(i, j);

            // If values inside Zmin and Zmax
            if (fabs(Z) < 0.000000000000001) {
                gray = 0.0;
            } else {
                gray = p2 * ((Z - min) / deltaZ) + p1;
            }

            // Else
            if (Z < min) {
                gray = p1;
            }

            if (Z > max) {
                gray = 1.0;
            }

            // Change 180 degrees from spatial coordinate system to image
            img->set(dem_height_ - i + 1, j, gray);
        }
    }

    return img;
}

/*
 * INTERPOLATION DECISION METHOD
*/

void DemGrid::interpolateNearestPoint()
{
    // nf set to 1.0 is the size of the matrix grid cell (not terrain)
    (chooseBestInterpolationMethod(1.0) == InterpolationMode::NORMAL) ?
    interpolateNearestPointNormal() :
    interpolateNearestPointFast();
}

void DemGrid::interpolateTrendSurface(TSSurface mode)
{
    // No difference between fast and normal methods
    interpolateTrendSurfaceFast(mode);
}

void DemGrid::interpolateMovingAverage(double n, double D0, MAWeight mode)
{
    const double nf = (n / res_x_) * (n / res_y_);
    (chooseBestInterpolationMethod(nf) == InterpolationMode::NORMAL) ?
    interpolateMovingAverageNormal(n, D0, mode) :
    interpolateMovingAverageFast(n, D0, mode);
}

void DemGrid::interpolateMovingSurface(double n,
                                       double D0,
                                       MAWeight ma_weight,
                                       TSSurface ts_surface)
{
    const double nf = (n / res_x_) * (n / res_y_);
    (chooseBestInterpolationMethod(nf) == InterpolationMode::NORMAL) ?
    interpolateMovingSurfaceNormal(n, D0, ma_weight, ts_surface) :
    interpolateMovingSurfaceFast(n, D0, ma_weight, ts_surface);
}

/*
 * Input:
 * nf- Structure matrix area in cells (new subdivision of the grid)
 *
 * Output:
 *  Use normal or fast interpolation
 */
DemGrid::InterpolationMode DemGrid::chooseBestInterpolationMethod(
    double nf) const
{
    const auto no_points = point_list_->size();

    if (no_points < 1000) {
        return InterpolationMode::NORMAL;
    }

    // May have very large numbers - double prefered !!
    // Calculate DEM matrix area - not terrain area
    const double area = static_cast<double>(dem_width_)
                        * static_cast<double> (dem_height_);
    // Points per area
    double grid_density = static_cast<double>(no_points)
                          / static_cast<double>(area);

    // If density is less than a cell size
    if (grid_density < 1.0) {
        grid_density = 1.0;
    }

    // Let's calculate the computational effort for each grid cell
    // For not overfloating values, we consider the sqtr for both equations
    const auto no_its_normal = no_points;           // no_points^2
    const auto no_its_fast = sqrt(area * grid_density * nf);

    if (no_its_normal > no_its_fast) {
        return InterpolationMode::NORMAL;
    }

    return InterpolationMode::FAST;
}


/***********************************************
 *                                             *
 * 3D Point INTERPOLATION using fast structure *
 *                                             *
 ***********************************************/

/*
 * These 4 3D points algorithms were based on
 * ILWIS open source: http://www.ilwis.org:
 * Nearest Point
 * Moving Average
 * Moving Surface
 * Trend Surface
 */
void DemGrid::interpolateNearestPointFast()
{
    cancel_flag_ = false;

    if (point_list_ == nullptr) {
        printf("Error! No point list linked.\n");
        return;
    }

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

    // Create faster structure to deal with the cloud of points
    // As NP must find a point, even in a hole.
    // Holes decreases performance. So, we increase the sctructure reolution
    // to solve this problem.
    mpg_ = new MatchingPointsGrid(point_list_, res_x_ * 10, res_y_ * 10);
    double Px{0.0}, Py{0.0}, Z{0.0};
    int best_point{0};
    MatchingPoints* mp{nullptr};
    printf("Interpolating using Nearest Point Fast ...\n");

    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            Px = Xi_ + static_cast<double>(x - 1) * res_x_;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_;
            best_point = mpg_->getNearestPoint(Px, Py);
            Z = 0.0;

            if (best_point > 0) {
                mp = point_list_->get(best_point);
                Z = mp->Z;
            }

            DEM_.set(y, x, Z);

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }

    elap_time_ = 0.0;
    delete mpg_;
}

const std::map<TSSurface, unsigned> matriceSize{{TSSurface::PLANE, 3},
    {TSSurface::LINEAR, 4},
    {TSSurface::PARABOLIC, 5},
    {TSSurface::SECONDDEGREE, 6},
    {TSSurface::THIRDDEGREE, 10}
};

// This interpolation method does not use the fast structure
void DemGrid::interpolateTrendSurfaceFast(TSSurface ts_surface)
{
    cancel_flag_ = false;

    if (point_list_ == nullptr) {
        printf("Error! No point list linked.\n");
        return;
    }

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

    // Mode:
    // 0 - Plane
    // 1 - 2nd Degree Linear
    // 2 - 2nd Degree Parabolic
    // 3 - 2nd Degree
    // 4 - 3rd Degree
    Matrix X, A, L;
    printf("Interpolating using Trend Surface Fast ...\n");
    const auto no_points = point_list_->size();
    A.resize(no_points, matriceSize.at(ts_surface));
    L.resize(no_points, 1);
    // Calculate the cendroid of the mass of data
    MatchingPoints* mp;
    double Cx{0.0}, Cy{0.0};

    for (size_t i = 1; i <= no_points; i++) {
        mp = point_list_->get(i);
        Cx += mp->X;
        Cy += mp->Y;
    }

    Cx = Cx / static_cast<double>(no_points);
    Cy = Cy / static_cast<double>(no_points);

    // Fill matrices A and L
    for (size_t i = 1; i <= no_points; i++) {
        mp = point_list_->get(i);
        A.set(i, 1, 1.0);
        A.set(i, 2, mp->X - Cx);
        A.set(i, 3, mp->Y - Cy);

        if (ts_surface == TSSurface::LINEAR ||
                ts_surface == TSSurface::SECONDDEGREE ||
                ts_surface == TSSurface::THIRDDEGREE) {
            A.set(i, 4, (mp->X - Cx) * (mp->Y - Cy));
        }

        if (ts_surface == TSSurface::PARABOLIC) {
            A.set(i, 4, pow(mp->X - Cx, 2));
            A.set(i, 5, pow(mp->Y - Cy, 2));
        }

        if (ts_surface == TSSurface::SECONDDEGREE ||
                ts_surface == TSSurface::THIRDDEGREE) {
            A.set(i, 1, pow(mp->X - Cx, 2));
            A.set(i, 6, pow(mp->Y - Cy, 2));
        }

        if (ts_surface == TSSurface::THIRDDEGREE) {
            A.set(i, 7, pow(mp->X - Cx, 3));
            A.set(i, 8, pow(mp->X - Cx, 2) * (mp->Y - Cy));
            A.set(i, 9, pow(mp->Y - Cy, 2) * (mp->X - Cx));
            A.set(i, 10, pow(mp->Y - Cy, 3));
        }

        L.set(i, 1, mp->Z);

        if (cancel_flag_) {
            return;
        }
    }

    // Least-squares
    X = (A.transpose() * A).inverse() * A.transpose() * L;
    double Px{0.0}, Py{0.0};

    // Interpolate
    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            Px = Xi_ + static_cast<double>(x - 1) * res_x_ - Cx;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_ - Cy;

            switch (ts_surface) {
            case TSSurface::PLANE :
                DEM_.set(y, x, X.get(1, 1) + X.get(2, 1)*Px + X.get(3, 1)*Py);
                break;

            case TSSurface::LINEAR :
                DEM_.set(y, x, X.get(1, 1) + X.get(2, 1)*Px + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py);
                break;

            case TSSurface::PARABOLIC :
                DEM_.set(y, x, X.get(1, 1) + X.get(2, 1)*Px + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Px + X.get(5, 1)*Py * Py);
                break;

            case TSSurface::SECONDDEGREE :
                DEM_.set(y, x, X.get(1, 1) + X.get(2, 1)*Px + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py + X.get(5, 1)*Px * Px
                         + X.get(6, 1)*Py * Py);
                break;

            case TSSurface::THIRDDEGREE :
                DEM_.set(y, x, X.get(1, 1) + X.get(2, 1)*Px + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py + X.get(5, 1)*Px * Px
                         + X.get(6, 1)*Py * Py + X.get(7, 1)*Px * Px * Px
                         + X.get(8, 1)*Px * Px * Py + X.get(9, 1)*Px * Py * Py
                         + X.get(10, 1)*Py * Py * Py);
                break;
            }

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }
}

// Using fast structure
void DemGrid::interpolateMovingAverageFast(double n, double D0, MAWeight mode)
{
    cancel_flag_ = false;

    if (point_list_ == nullptr) {
        printf("Error! No point list linked.\n");
        return;
    }

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

    // Mode 0 = inverse distance
    // Mode 1 = linear decrease
    double Px{0.0}, Py{0.0}, d{0.0}, D{0.0}, sum1{0.0}, sum2{0.0}, weight{0.0};
    int point_id{0};
    // Create fast structure
    mpg_ = new MatchingPointsGrid(point_list_, res_x_, res_y_);
    MatchingPoints* mp{nullptr};
    printf("Interpolating using Moving Average Fast ...\n");

    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            Px = Xi_ + static_cast<double>(x - 1) * res_x_;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_;
            // Calculate weights
            sum1 = 0.0;
            sum2 = 0.0;
            // Returns a list with points inside the
            // circle with center PX,PY and radius D0
            mpg_->getPointsClose(Px, Py, D0);

            for (size_t i = 0; i < mpg_->selected_points.size(); i++) {
                point_id = mpg_->selected_points.at(i);
                mp = point_list_->get(point_id);
                D = sqrt(pow(Px - mp->X, 2) + pow(Py - mp->Y, 2));

                if (D < D0) {
                    d = D / D0;

                    if (mode == MAWeight::INVERSE_DIST) {
                        weight = (1 / pow(d, n)) - 1;
                    } else {
                        weight = 1 - pow(d, n);
                    }

                    sum1 += weight * mp->Z;
                    sum2 += weight;
                }
            }

            if (sum2 > 0.0) {
                DEM_.set(y, x, sum1 / sum2);
            } else {
                DEM_.set(y, x, 0.0);
            }

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }

    elap_time_ = 0.0;
    delete mpg_;
}

// Using fast structure
void DemGrid::interpolateMovingSurfaceFast(double n,
        double D0,
        MAWeight ma_weight,
        TSSurface ts_surface)
{
    cancel_flag_ = false;

    if (point_list_ == nullptr) {
        printf("Error! No point list linked.\n");
        return;
    }

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

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
    double Px{0.0}, Py{0.0}, d{0.0}, D{0.0};
    double weight{0.0}, Cx{0.0}, Cy{0.0}, Pxi{0.0}, Pyi{0.0};
    int point_id{0};
    size_t no_valid_points{0};
    std::map<TSSurface, size_t> min_points {
        {TSSurface::PLANE, 5},
        {TSSurface::LINEAR, 6},
        {TSSurface::PARABOLIC, 7},
        {TSSurface::SECONDDEGREE, 8},
        {TSSurface::THIRDDEGREE, 12}
    };
    Matrix X, A, L;
    // Create fast structure
    mpg_ = new MatchingPointsGrid(point_list_, res_x_, res_y_);
    MatchingPoints* mp{nullptr};
    printf("Interpolating using Moving Surface Fast ...\n");

    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            // Calculate Px and Py
            Px = Xi_ + static_cast<double>(x - 1) * res_x_;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_;
            // Moving Average Part
            // Returns a list with points inside the circle with
            // center PX,PY and radius D0
            mpg_->getPointsClose(Px, Py, D0);
            no_valid_points = mpg_->selected_points.size();

            if (no_valid_points < min_points.at(ts_surface)) {
                continue;
            }

            // Trend Surface Part
            A.resize(no_valid_points, matriceSize.at(ts_surface));
            L.resize(no_valid_points, 1);
            // Calculate the cendroid of the mass of data
            Cx = 0.0;
            Cy = 0.0;

            for (size_t i = 0; i < no_valid_points; i++) {
                point_id = mpg_->selected_points.at(i);
                mp = point_list_->get(point_id);
                Cx += mp->X;
                Cy += mp->Y;
            }

            Cx = Cx / static_cast<double>(no_valid_points);
            Cy = Cy / static_cast<double>(no_valid_points);
            // Calculate Px and Py with centroids
            Pxi = Px;
            Pyi = Py;
            Px = Px - Cx;
            Py = Py - Cy;

            // Fill matrices A and L
            for (size_t i = 1; i <= no_valid_points; i++) {
                point_id = mpg_->selected_points.at(i - 1);
                mp = point_list_->get(point_id);
                // Calculate weight
                D = sqrt(pow(Pxi - mp->X, 2) + pow(Pyi - mp->Y, 2));
                d = D / D0;

                if (ma_weight == MAWeight::INVERSE_DIST) {
                    weight = (1 / pow(d, n)) - 1;
                } else {
                    weight = 1 - pow(d, n);
                }

                A.set(i, 1, 1.0 * weight);
                A.set(i, 2, (mp->X - Cx) * weight);
                A.set(i, 3, (mp->Y - Cy) * weight);

                if (ts_surface == TSSurface::LINEAR ||
                        ts_surface == TSSurface::SECONDDEGREE ||
                        ts_surface == TSSurface::THIRDDEGREE) {
                    A.set(i, 4, (mp->X - Cx) * (mp->Y - Cy) * weight);
                }

                if (ts_surface == TSSurface::PARABOLIC) {
                    A.set(i, 4, pow(mp->X - Cx, 2) * weight);
                    A.set(i, 5, pow(mp->Y - Cy, 2) * weight);
                }

                if (ts_surface == TSSurface::SECONDDEGREE ||
                        ts_surface == TSSurface::THIRDDEGREE) {
                    A.set(i, 5, pow(mp->X - Cx, 2) * weight);
                    A.set(i, 6, pow(mp->Y - Cy, 2) * weight);
                }

                if (ts_surface == TSSurface::THIRDDEGREE) {
                    A.set(i, 7, pow(mp->X - Cx, 3) * weight);
                    A.set(i, 8, pow(mp->X - Cx, 2) * (mp->Y - Cy) * weight);
                    A.set(i, 9, pow(mp->Y - Cy, 2) * (mp->X - Cx) * weight);
                    A.set(i, 10, pow(mp->Y - Cy, 3) * weight);
                }

                L.set(i, 1, mp->Z * weight);
            }

            // Least-squares
            X = (A.transpose() * A).inverse() * A.transpose() * L;

            switch (ts_surface) {
            case TSSurface::PLANE :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py);
                break;

            case TSSurface::LINEAR :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py);
                break;

            case TSSurface::PARABOLIC :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Px
                         + X.get(5, 1)*Py * Py);
                break;

            case TSSurface::SECONDDEGREE :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py
                         + X.get(5, 1)*Px * Px
                         + X.get(6, 1)*Py * Py);
                break;

            case TSSurface::THIRDDEGREE :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py
                         + X.get(5, 1)*Px * Px
                         + X.get(6, 1)*Py * Py
                         + X.get(7, 1)*Px * Px * Px
                         + X.get(8, 1)*Px * Px * Py
                         + X.get(9, 1)*Px * Py * Py
                         + X.get(10, 1)*Py * Py * Py);
                break;
            }

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }

    eliminateBadPointsGrid(3.0);
    elap_time_ = 0.0;
    delete mpg_;
}

void DemGrid::eliminateBadPointsGrid(double sigma)
{
    const auto meanZ = getMeanZ();
    const auto stdZ = getStdZ();
    cutGrid(meanZ - stdZ * sigma, meanZ + stdZ * sigma, true);
}

void DemGrid::cutGrid(double min, double max, bool fromList = true)
{
    if (fromList) {
        double Xi, Xf, Yi, Yf, Zi, Zf;
        point_list_->XYZboundingBox(Xi, Yi, Xf, Yf, Zi, Zf);
        min = Zi;
        max = Zf;
    }

    if (min >= max) {
        printf("Error: min is greater or equal than max\n");
        return;
    }

    printf("Cutting DEM: accept from %f to %f\n", min, max);

    for (size_t i = 1; i <= DEM_.getRows(); i++) {
        for (size_t j = 1; j <= DEM_.getCols(); j++) {
            if (DEM_.get(i, j) < min || DEM_.get(i, j) > max) {
                DEM_.set(i, j, 0.0);
            }
        }
    }
}

/************************
 *                      *
 * Read/Write functions *
 *                      *
 ************************/

void DemGrid::saveDem(const char* filename, Filetype mode) const
{
    if (mode == Filetype::BINARY) {
        saveDemEfoto(filename);
        return;
    }

    saveDemAscii(filename);
    return;
}

void DemGrid::loadDem(char* filename, Filetype mode)
{
    if (mode == Filetype::BINARY) {
        loadDemEfoto(filename);
        return;
    }

    loadDemAscii(filename);
    return;
}

void DemGrid::saveDemEfoto(const char* filename) const
{
    static_assert( sizeof(double) == 8, "Double must be 8 bytes in size" );
    FILE* fp;
    fp = fopen(filename, "wb");

    if (fp) {
        const int DOUBLE_SIZE = 8;
        // Write header
        double header[8];
        header[0] = Xi_;
        header[1] = Yi_;
        header[2] = Xf_;
        header[3] = Yf_;
        header[4] = res_x_;
        header[5] = res_y_;
        header[6] = static_cast<double>(dem_width_);
        header[7] = static_cast<double>(dem_height_);
        fwrite(&header, 1, sizeof(header), fp);
        // Write DEM
        int p{0};
        size_t file_size = dem_width_ * dem_height_;
        double* data = new double[file_size];

        for (size_t i = 1; i <= dem_height_; i++) {
            for (size_t j = 1; j <= dem_width_; j++) {
                data[p] = DEM_.get(i, j);
                p++;
            }
        }

        fwrite(data, 1, file_size * DOUBLE_SIZE, fp);
        fclose(fp);
        delete[] data;
    }
}

void DemGrid::loadDemEfoto(const char* filename)
{
    static_assert( sizeof(double) == 8, "Double must be 8 bytes in size" );
    std::ifstream fp;
    fp.open(filename, std::ios::binary);

    if (fp) {
        const int DOUBLE_SIZE = 8;
        // Read header
        double header[8];
        fp.read(reinterpret_cast<char*>(header), sizeof(header));
        Xi_ = header[0];
        Yi_ = header[1];
        Xf_ = header[2];
        Yf_ = header[3];
        res_x_ = header[4];
        res_y_ = header[5];
        dem_width_ = static_cast<int>(header[6]);
        dem_height_ = static_cast<int>(header[7]);
        // Read DEM
        DEM_.resize(dem_height_, dem_width_);
        size_t file_size{dem_width_ * dem_height_};
        double* data = new double[file_size];
        fp.read(reinterpret_cast<char*>(data), dem_width_ * dem_height_ * DOUBLE_SIZE);
        int p {0};

        for (size_t i = 1; i <= dem_height_; i++) {
            for (size_t j = 1; j <= dem_width_; j++) {
                DEM_.set(i, j, data[p]);
                p++;
            }
        }

        fp.close();
        delete []data;
    }
}

void DemGrid::saveDemAscii(const char* filename) const
{
    std::ofstream outfile(filename);
    // Write header
    outfile << "-= EFOTO DSM GRID DATA =-\n\n";
    outfile << "Header info:\n";
    outfile << std::fixed << std::setprecision(5);
    outfile << "Xi=" << Xi_ << "\nYi=" << Yi_ << "\nXf=" << Xf_
            << "\nYf=" << Yf_ << "\n";
    outfile << "Res_X=" << res_x_ << "\nRes_Y=" << res_y_ << "\n";
    outfile << "Width=" << dem_width_ << "\nHeight=" << dem_height_ << "\n";
    outfile << "\nGRID data:\n";
    double X{0.0}, Y{0.0};

    for (size_t i = 1; i <= dem_height_; i++) {
        for (size_t j = 1; j <= dem_width_; j++) {
            X = Xi_ + (j - 1) * res_x_;
            Y = Yi_ + (i - 1) * res_y_;
            outfile << X << "\t" << Y << "\t" << DEM_.get(i, j) << "\n";
        }
    }

    outfile.close();
}

// For later conversion to stream
double DemGrid::getAsciiParameter(std::ifstream* file, std::string tag)
{
    char line[256];
    std::string sline;
    int line_count {0};
    // find -1 = not found
    file->seekg(0, std::ios::beg);

    while (!file->eof() && line_count < 50) {
        file->getline(line, 256);
        sline = (std::string) line;
        int p = sline.find(tag);

        if (p >= 0) {
            sline = sline.substr(p + tag.length());
            return atof(sline.c_str());
        }

        line_count++;
    }

    return -1.0;
}

void DemGrid::loadDemAscii(char* filename)
{
    std::ifstream arq(filename);

    if (arq.fail()) {
        throw "Error while opening file!\n";
    }

    // Read header
    Xi_ = getAsciiParameter(&arq, "Xi=");
    Yi_ = getAsciiParameter(&arq, "Yi=");
    Xf_ = getAsciiParameter(&arq, "Xf=");
    Yf_ = getAsciiParameter(&arq, "Yf=");
    res_x_ = getAsciiParameter(&arq, "Res_X=");
    res_y_ = getAsciiParameter(&arq, "Res_Y=");
    dem_width_ = getAsciiParameter(&arq, "Width=");
    dem_height_ = getAsciiParameter(&arq, "Height=");

    // Check header
    if (Xi_ < 0 || Yi_ < 0 || Xf_ < 0 || Yf_ < 0 || res_x_ < 0 || res_y_ < 0) {
        arq.close();
        printf("Error while reading file header!\n");
        return;
    }

    // Read data
    DEM_.resize(dem_height_, dem_width_);
    // Set pointer to first line of the GRID
    getAsciiParameter(&arq, "GRID data:");
    double X, Y, Z;

    for (size_t i = 1; i <= dem_height_; i++) {
        for (size_t j = 1; j <= dem_width_; j++) {
            arq >> X >> Y >> Z;
            DEM_.set(i, j, Z);

            if (arq.eof()) {
                printf("Error: missing data!\n");
            }
        }
    }

    arq.close();
}


/********************************************************
 * Interpolation functions without using fast structure *
 *                                                      *
 ********************************************************/

void DemGrid::interpolateNearestPointNormal()
{
    cancel_flag_ = false;

    if (point_list_ == nullptr) {
        printf("Error! No point list linked.\n");
        return;
    }

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

    double Px{0.0}, Py{0.0}, dist{0.0}, best_dist{0.0}, Z{0.0};
    MatchingPoints* mp{nullptr};
    printf("Interpolating using Nearest Point ...\n");

    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            Px = Xi_ + static_cast<double>(x - 1) * res_x_;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_;
            mp = point_list_->get(1);
            best_dist = sqrt(pow(Px - mp->X, 2) + pow(Py - mp->Y, 2));
            Z = 0.0;

            for (size_t i = 2; i <= point_list_->size(); i++) {
                mp = point_list_->get(i);
                dist = sqrt(pow(Px - mp->X, 2) + pow(Py - mp->Y, 2));

                if (dist < best_dist) {
                    best_dist = dist;
                    Z = mp->Z;
                }
            }

            DEM_.set(y, x, Z);

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }

    elap_time_ = 0.0;
}

void DemGrid::interpolateMovingAverageNormal(double n, double D0, MAWeight mode)
{
    cancel_flag_ = false;

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

    // Mode 0 = inverse distance
    // Mode 1 = linear decrease
    double Px{0.0}, Py{0.0}, d{0.0}, D{0.0}, sum1{0.0}, sum2{0.0}, weight{0.0};
    MatchingPoints* mp{nullptr};
    printf("Interpolating using Moving Average ...\n");

    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            Px = Xi_ + static_cast<double>(x - 1) * res_x_;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_;
            // Calculate weights
            sum1 = 0.0;
            sum2 = 0.0;

            for (size_t i = 1; i <= point_list_->size(); i++) {
                mp = point_list_->get(i);
                D = sqrt(pow(Px - mp->X, 2) + pow(Py - mp->Y, 2));

                if (D < D0) {
                    d = D / D0;

                    if (mode == MAWeight::INVERSE_DIST) {
                        weight = (1 / pow(d, n)) - 1;
                    } else {
                        weight = 1 - pow(d, n);
                    }

                    sum1 += weight * mp->Z;
                    sum2 += weight;
                }
            }

            if (sum2 > 0.0) {
                DEM_.set(y, x, sum1 / sum2);
            } else {
                DEM_.set(y, x, 0.0);
            }

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }

    elap_time_ = 0.0;
}

void DemGrid::interpolateMovingSurfaceNormal(double n,
        double D0,
        MAWeight ma_weight,
        TSSurface ts_surface)
{
    cancel_flag_ = false;

    // If empty list of points
    if (point_list_->size() < 1) {
        return;
    }

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
    std::vector <int> validPointList;
    std::vector <double> validPointListWeight;
    double Px{0.0}, Py{0.0}, d{0.0}, D{0.0}, weight{0.0}, Cx{0.0}, Cy{0.0};
    size_t no_valid_points{0};
    Matrix X, A, L;
    MatchingPoints* mp{nullptr};
    printf("Interpolating using Moving Surface ...\n");

    for (size_t y = 1; y <= DEM_.getRows(); y++) {
        for (size_t x = 1; x <= DEM_.getCols(); x++) {
            // Calculate Px and Py
            Px = Xi_ + static_cast<double>(x - 1) * res_x_;
            Py = Yi_ + static_cast<double>(y - 1) * res_y_;
            //
            // Moving Average Part
            //
            // Check for valid points
            validPointList.clear();

            for (size_t i = 1; i <= point_list_->size(); i++) {
                mp = point_list_->get(i);
                D = sqrt(pow(Px - mp->X, 2) + pow(Py - mp->Y, 2));

                if (D < D0) {
                    d = D / D0;

                    if (ma_weight == MAWeight::INVERSE_DIST) {
                        weight = (1 / pow(d, n)) - 1;
                    } else {
                        weight = 1 - pow(d, n);
                    }

                    validPointList.push_back(i);
                    validPointListWeight.push_back(weight);
                }
            }

            no_valid_points = validPointList.size();

            if (no_valid_points == 0) {
                continue;
            }

            // Trend Surface Part
            A.resize(no_valid_points, matriceSize.at(ts_surface));
            L.resize(no_valid_points, 1);
            // Calculate the cendroid of the mass of data
            Cx = 0.0;
            Cy = 0.0;

            for (size_t i = 0; i < no_valid_points; i++) {
                mp = point_list_->get(validPointList.at(i));
                Cx += mp->X;
                Cy += mp->Y;
            }

            Cx = Cx / static_cast<double>(no_valid_points);
            Cy = Cy / static_cast<double>(no_valid_points);
            // Calculate Px and Py with centroids
            Px = Px - Cx;
            Py = Py - Cy;

            // Fill matrices A and L
            for (size_t i = 1; i <= no_valid_points; i++) {
                mp = point_list_->get(validPointList.at(i - 1));
                weight = validPointListWeight.at(i - 1);
                A.set(i, 1, 1.0 * weight);
                A.set(i, 2, (mp->X - Cx) * weight);
                A.set(i, 3, (mp->Y - Cy) * weight);

                if (ts_surface == TSSurface::LINEAR ||
                        ts_surface == TSSurface::SECONDDEGREE ||
                        ts_surface == TSSurface::THIRDDEGREE) {
                    A.set(i, 4, (mp->X - Cx) * (mp->Y - Cy) * weight);
                }

                if (ts_surface == TSSurface::PARABOLIC) {
                    A.set(i, 4, pow(mp->X - Cx, 2) * weight);
                    A.set(i, 5, pow(mp->Y - Cy, 2) * weight);
                }

                if (ts_surface == TSSurface::SECONDDEGREE ||
                        ts_surface == TSSurface::THIRDDEGREE) {
                    A.set(i, 5, pow(mp->X - Cx, 2) * weight);
                    A.set(i, 6, pow(mp->Y - Cy, 2) * weight);
                }

                if (ts_surface == TSSurface::THIRDDEGREE) {
                    A.set(i, 7, pow(mp->X - Cx, 3) * weight);
                    A.set(i, 8, pow(mp->X - Cx, 2) * (mp->Y - Cy) * weight);
                    A.set(i, 9, pow(mp->Y - Cy, 2) * (mp->X - Cx) * weight);
                    A.set(i, 10, pow(mp->Y - Cy, 3) * weight);
                }

                L.set(i, 1, mp->Z * weight);
            }

            // Least-squares
            X = (A.transpose() * A).inverse() * A.transpose() * L;

            switch (ts_surface) {
            case TSSurface::PLANE :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py);
                break;

            case TSSurface::LINEAR :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py);
                break;

            case TSSurface::PARABOLIC :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Px
                         + X.get(5, 1)*Py * Py);
                break;

            case TSSurface::SECONDDEGREE :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py
                         + X.get(5, 1)*Px * Px
                         + X.get(6, 1)*Py * Py);
                break;

            case TSSurface::THIRDDEGREE :
                DEM_.set(y, x, X.get(1, 1)
                         + X.get(2, 1)*Px
                         + X.get(3, 1)*Py
                         + X.get(4, 1)*Px * Py
                         + X.get(5, 1)*Px * Px
                         + X.get(6, 1)*Py * Py
                         + X.get(7, 1)*Px * Px * Px
                         + X.get(8, 1)*Px * Px * Py
                         + X.get(9, 1)*Px * Py * Py
                         + X.get(10, 1)*Py * Py * Py);
                break;
            }

            if (cancel_flag_) {
                return;
            }
        }

        if (manager_ != nullptr) {
            manager_->setProgress((100 * y) / DEM_.getRows());
        }
    }

    elap_time_ = 0.0;
}


/***************
 *             *
 * DEM QUALITY *
 *             *
 ***************/

std::string DemGrid::calculateDemQuality(MatchingPointsList mpl) const
{
    std::stringstream txt;
    MatchingPoints* mp{nullptr};
    auto list_size = mpl.size();
    Matrix Zerr(list_size, 1);
    double Zerror{0.0};
    txt << "E-FOTO DSM Quality\n\n";
    txt << "Number of testing points: " << list_size << "\n\n";
    // Calculate whole errors
    txt << "Whole set Z error:\n";

    for (size_t i = 0; i < list_size; i++) {
        mp = mpl.get(i + 1);
        double Z = mp->Z;
        double Zgrid = getHeightXY(mp->X, mp->Y);

        // -1.0 is a DEM hole flag
        if (Zgrid - 0.0 < 0.0000000000000001) {
            Zerror = -1.0;
            txt << "Point " << i + 1 << ": Not used\n";
        } else {
            Zerror = fabs(Z - Zgrid);
            txt << "Point " << i + 1 << ": " << Zerror << "\n";
        }

        Zerr.set(i + 1, 1, Zerror);
    }

    bool flag {true};
    auto pts_used = mpl.size();
    double Zmean{0.0}, Zstd;

    while (flag && pts_used > 2) {
        // Calculate useful points mean
        Zmean = 0.0;
        pts_used = 0;

        for (size_t i = 1; i <= list_size; i++) {
            if (static_cast<int>(Zerr.get(i, 1)) == -1) {
                continue;
            }

            pts_used++;
            Zmean += Zerr.get(i, 1);
        }

        Zmean /= static_cast<double>(pts_used);
        // Calculate STD
        Zstd = 0.0;

        for (size_t i = 1; i <= list_size; i++) {
            if (static_cast<int>(Zerr.get(i, 1)) == -1) {
                continue;
            }

            Zstd += pow(Zerr.get(i, 1) - Zmean, 2);
        }

        Zstd = sqrt(Zstd / (pts_used));
        // Eliminate outliers
        flag = false;

        for (size_t i = 1; i <= list_size; i++) {
            if (static_cast<int>(Zerr.get(i, 1)) == -1) {
                continue;
            }

            if (Zerr.get(i, 1) > Zmean + 3.0 * Zstd
                    || Zerr.get(i, 1) < Zmean - 3.0 * Zstd) {
                flag = true;
                Zerr.set(i, 1, -1.0);
            }
        }
    }

    // Show final results
    txt << "\nFinal set Z error:\n";

    for (size_t i = 1; i <= list_size; i++) {
        if (static_cast<int>(Zerr.get(i, 1)) == -1) {
            continue;
        }

        txt << "Point " << i << ": " << Zerr.get(i, 1) << "\n";
    }

    txt << "\nPoints used: " << pts_used << "\n";
    txt << "Z error average: " << Zmean << "\n";
    txt << "Z error standard deviation: " << Zstd << "\n\n";
    return txt.str();
}

void DemGrid::overlayMap(const Matrix* const map) const
{
    if (map->getCols() != DEM_.getCols() || map->getRows() != DEM_.getRows()) {
        return;
    }

    for (size_t i = 1; i <= DEM_.getRows(); i++) {
        for (size_t j = 1; j <= DEM_.getCols(); j++) {
            if (map->get(i, j) - 0.0 < 0.000000000001) {
                continue;
            }

            DEM_.set(i, j, map->get(i, j));
        }
    }
}

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
