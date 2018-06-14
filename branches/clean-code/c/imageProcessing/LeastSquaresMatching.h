#ifndef LEAST_SQUARES_MATCHING
#define LEAST_SQUARES_MATCHING
/*******************************************************************************
                         LeastSquaresMatching.h
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

#include "NormalizedCrossCorrelation.h"
#include "Matrix.h"

/**
* class LeastSquaresMatching
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 03/08/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class LeastSquaresMatching {

public:
    LeastSquaresMatching();
    enum filterType { Gradient, Prewitt, Sobel };
    int searchHomologous(Matrix* img1, Matrix* img2, double Tx, double Ty,
                         double Mx, double My);
    void setTemplate(int _wh)
    {
        template_width = _wh;
        template_height = _wh;
    };

    int searchHomologous(Matrix*, Matrix*);
    double getBestP()
    {
        return best_p;
    };
    double getBestX()
    {
        return best_x;
    };
    double getBestY()
    {
        return best_y;
    };

    void setMinStd(double _mstd)
    {
        ncc.setMinStd(_mstd);
    };
    void setMaxIterations(int _its)
    {
        max_iterations = _its;
    };
    void setConvergenceLimits(double _shift, double _scale, double _shear)
    {
        limit_shift_values = _shift;
        limit_scale_values = _scale;
        limit_shear_values = _shear;
    };
    void setMaxDistance(double _max)
    {
        max_distance = _max;
    };
    void setOverIt(bool ovit)
    {
        over_it = ovit;
    };
    void setOverItDist(double ovitd)
    {
        over_it_distance = ovitd;
    };
    void setTemplateGrothStep(int _gs)
    {
        temp_growth_step = _gs;
    };
    void setTemplateMaximumSize(int _ms)
    {
        temp_max_size = _ms;
    };


private:
    int template_width, template_height;
    int template_center_x, template_center_y, matching_window_center_x,
        matching_window_center_y;
    double best_p, limit_shift_values, limit_scale_values, limit_shear_values,
           acceptance_correlation, acceptance_error_ellipse;
    double best_x, best_y;
    double over_it_distance;
    bool over_it;
    int max_iterations, max_distance;
    filterType gradient_filter;
    Matrix Gx, Gy;
    Matrix affine_coefficients;
    NormalizedCrossCorrelation ncc; // Used for evaluation
    int temp_growth_step, temp_max_size;
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
