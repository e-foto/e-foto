#ifndef NORMALIZED_CROSS_CORRELATION
#define NORMALIZED_CROSS_CORRELATION

/*******************************************************************************
                         NormalizedCrossCorrelation.h
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

/**
* class NormalizedCrossCorrelation
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

enum class NccFlag { NoErrors=1,
                     SearchWindowIsSmallerThanTemplate = -1,
                     TemplateIsTooSmallLessThan3Pixels = -2,
                     TemplateIsOutOfImage = -3,
                     TemplateHasLowStd = -4};

class NormalizedCrossCorrelation {

public:
    NormalizedCrossCorrelation();
    void setTemplate(int _wh)
    {
        template_width = _wh;
        template_height = _wh;
    };
    void setTemplate(int _w, int _h)
    {
        template_width = _w;
        template_height = _h;
    };
    void setSearchWindow(int _wh)
    {
        search_window_width = _wh;
        search_window_height = _wh;
    };
    void setSearchWindow(int _w, int _h)
    {
        search_window_width = _w;
        search_window_height = _h;
    };
    void setTemplateCenter(double _x, double _y)
    {
        template_center_x = int(_x);
        template_center_y = int(_y);
        frac_tc_x = _x - int(_x);
        frac_tc_y = _y - int(_y);
    };
    void setSearchWindowCenter(double _x, double _y)
    {
        search_window_center_x = int(_x);
        search_window_center_y = int(_y);
    };
    NccFlag searchHomologous(Matrix*, Matrix*);
    double directCorrelation(Matrix* X, Matrix* Y, int template_cx, int template_cy,
                             int matching_cx, int matching_cy);
    double getBestP() const
    {
        return best_p;
    };
    double getBestX() const
    {
        return best_x;
    };
    double getBestY() const
    {
        return best_y;
    };
    void setMinStd(double _mstd)
    {
        min_std_acceptance = _mstd;
    };
    bool checkTemplateStd(Matrix* refmat);
    void setTemplateGrothStep(int _gs)
    {
        temp_growth_step = _gs;
    };
    int getTemplateGrothStep() const
    {
        return temp_growth_step;
    }  ;
    void setTemplateMaximumSize(int _ms)
    {
        temp_max_size = _ms;
    };
    int getTemplateMaximumSize() const
    {
        return temp_max_size;
    }

private:
    double calculateTemplateStd(Matrix* refmat);
    void calculateTemplateBoundingBox();
    int template_width, template_height, search_window_width, search_window_height;
    int template_center_x, template_center_y, search_window_center_x,
        search_window_center_y;
    int template_xi, template_xf, template_yi, template_yf; // Template bounding box
    double frac_tc_x, frac_tc_y; // Store subpixels from template
    double best_p;
    double best_x, best_y;
    double min_std_acceptance;
    int temp_growth_step, temp_max_size;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
