#ifndef LEAST_SQUARES_MATCHING
#define LEAST_SQUARES_MATCHING
/*******************************************************************************
                                                 LeastSquaresMatching.h
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

#include "CommonDef.h"
#include "NormalizedCrossCorrelation.h"
#include "Matrix.h"

/**
* class LeastSquaresMatching
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

enum class FilterType { Gradient, Prewitt, Sobel };
enum class LsmFlag { Ok = 1, OutOfBonds = 0, LowStd = -1, Diverged = -2, MaxIterationsAchieved = -3};

class LeastSquaresMatching {
    int template_width{10};
    int template_height{10};
    int template_center_x;
    int template_center_y;
    int matching_window_center_x;
    int matching_window_center_y;
    double best_p;
    double limit_shift_values{0.05}; // Distance between matching window centers
    double limit_scale_values{0.01}; // a2 and b1
    double limit_shear_values{0.01}; // a1 and b2
    double acceptance_correlation{0.7};
    double acceptance_error_ellipse{5}; // Std for a0 and b0 arrays
    double best_x;
    double best_y;
    double over_it_distance{0.5};
    bool over_it{true};
    int max_iterations{10};
    int max_distance{20};
    FilterType gradient_filter{FilterType::Gradient};
    Matrix Gx;
    Matrix Gy;
    Matrix affine_coefficients;
    NormalizedCrossCorrelation ncc; // Used for evaluation
    int temp_growth_step{2};
    int temp_max_size{50};

public:
    LeastSquaresMatching() = default;

    LsmFlag searchHomologous(Matrix* img1,
                             Matrix* img2,
                             double Tx, double Ty,
                             double Mx, double My);
    void setTemplate(int _wh)
    {
        template_width = _wh;
        template_height = _wh;
    }
    void setTemplate(int _w, int _h)
    {
        template_width = _w;
        template_height = _h;
    }
    void setTemplateCenter(int _x, int _y)
    {
        template_center_x = _x;
        template_center_y = _y;
    };
    void setMatchingWindowCenter(int _x, int _y)
    {
        matching_window_center_x = _x;
        matching_window_center_y = _y;
    }

    double getBestP() const
    {
        return best_p;
    }
    double getBestX() const
    {
        return best_x;
    }
    double getBestY() const
    {
        return best_y;
    }
    Matrix& getAffineParameters()
    {
        return affine_coefficients;
    }
    void setMinStd(double _mstd)
    {
        ncc.setMinStd(_mstd);
    }
    void setMaxIterations(int _its)
    {
        max_iterations = _its;
    }
    void getNextPosition(double*, double*, int, int);
    void setConvergenceLimits(double _shift, double _scale, double _shear)
    {
        limit_shift_values = _shift;
        limit_scale_values = _scale;
        limit_shear_values = _shear;
    }
    void setMaxDistance(double _max)
    {
        max_distance = _max;
    }
    void setOverIt(bool ovit)
    {
        over_it = ovit;
    }
    void setOverItDist(double ovitd)
    {
        over_it_distance = ovitd;
    }
    void setTemplateGrothStep(int _gs)
    {
        temp_growth_step = _gs;
    }
    int getTemplateGrothStep() const
    {
        return temp_growth_step;
    }
    void setTemplateMaximumSize(int _ms)
    {
        temp_max_size = _ms;
    }
    int getTemplateMaximumSize() const
    {
        return temp_max_size;
    }

private:
    void imgGradient(Matrix*);
    double average(Matrix* m, int row_i = 0, int row_f = 0, int col_i = 0,
                   int col_f = 0);
    double stddev(Matrix* m, int row_i = 0, int row_f = 0, int col_i = 0,
                  int col_f = 0);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
