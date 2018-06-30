#ifndef NORMALIZED_CROSS_CORRELATION
#define NORMALIZED_CROSS_CORRELATION

/*******************************************************************************
                         NormalizedCrossCorrelation.h
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
* class NormalizedCrossCorrelation
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 19/07/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

class NormalizedCrossCorrelation {

public:
    NormalizedCrossCorrelation();
    void setTemplate(int _wh)
    {
        template_width_ = _wh;
        template_height_ = _wh;
    };
    void setTemplate(int _w, int _h)
    {
        template_width_ = _w;
        template_height_ = _h;
    };
    void setSearchWindow(int _wh)
    {
        search_window_width_ = _wh;
        search_window_height_ = _wh;
    };
    void setSearchWindow(int _w, int _h)
    {
        search_window_width_ = _w;
        search_window_height_ = _h;
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
    int searchHomologous(Matrix*, Matrix*);
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
        min_std_acceptance_ = _mstd;
    };
    bool checkTemplateStd(Matrix* refmat);
    void setTemplateGrothStep(int _gs)
    {
        temp_growth_step_ = _gs;
    };
    void setTemplateMaximumSize(int _ms)
    {
        temp_max_size_ = _ms;
    };

private:
    int template_width_, template_height_, search_window_width_,
        search_window_height_;
    int template_center_x, template_center_y, search_window_center_x,
        search_window_center_y;
    // Template bounding box
    int template_xi, template_xf, template_yi, template_yf;
    double frac_tc_x, frac_tc_y; // Store subpixels from template
    double best_p;
    double best_x, best_y;
    double min_std_acceptance_;
    int temp_growth_step_, temp_max_size_;

    static double average(Matrix* m, int row_i = 0, int row_f = 0, int col_i = 0,
                   int col_f = 0);
    static double stddev(Matrix* m, int row_i = 0, int row_f = 0, int col_i = 0,
                  int col_f = 0);
    static double correlation(Matrix* X, Matrix* Y, int row_x_i = 0, int row_x_f = 0,
                       int col_x_i = 0, int col_x_f = 0, int row_y_i = 0, int row_y_f = 0,
                       int col_y_i = 0, int col_y_f = 0);
    double calculateTemplateStd(Matrix* refmat);
    void calculateTemplateBoundingBox();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
