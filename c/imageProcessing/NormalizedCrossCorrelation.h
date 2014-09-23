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

class NormalizedCrossCorrelation
{

public:
	NormalizedCrossCorrelation();
	void setTemplate(int _wh) { template_width = _wh; template_height = _wh; };
	void setTemplate(int _w, int _h) { template_width = _w; template_height = _h; };
	void setSearchWindow(int _wh) { search_window_width = _wh; search_window_height = _wh; };
	void setSearchWindow(int _w, int _h) { search_window_width = _w; search_window_height = _h; };
        void setTemplateCenter(double _x, double _y) { template_center_x = int(_x); template_center_y = int(_y); frac_tc_x = _x - int(_x); frac_tc_y = _y - int(_y); };
        void setSearchWindowCenter(double _x, double _y) { search_window_center_x = int(_x); search_window_center_y = int(_y); };
	int searchHomologous(Matrix *, Matrix *);
	double directCorrelation(Matrix *X, Matrix *Y, int template_cx, int template_cy, int matching_cx, int matching_cy);
	double getBestP() { return best_p; };
        double getBestX() { return best_x; };
        double getBestY() { return best_y; };
	void setMinStd(double _mstd) { min_std_acceptance = _mstd; };
	bool checkTemplateStd(Matrix *refmat);
        void setTemplateGrothStep(int _gs) { temp_growth_step = _gs; };
        int getTemplateGrothStep() { return temp_growth_step; };
        void setTemplateMaximumSize(int _ms) { temp_max_size = _ms; };
        int getTemplateMaximumSize() { return temp_max_size; };

private:
	double average(Matrix *m, int row_i=0, int row_f=0, int col_i=0, int col_f=0);
	double stddev(Matrix *m, int row_i=0, int row_f=0, int col_i=0, int col_f=0);
	double covXY(Matrix *X, Matrix *Y, int row_x_i=0, int row_x_f=0, int col_x_i=0, int col_x_f=0, int row_y_i=0, int row_y_f=0, int col_y_i=0, int col_y_f=0);
	double correlation(Matrix *X, Matrix *Y, int row_x_i=0, int row_x_f=0, int col_x_i=0, int col_x_f=0, int row_y_i=0, int row_y_f=0, int col_y_i=0, int col_y_f=0);
	double calculateTemplateStd(Matrix *refmat);
        void calculateTemplateBoundingBox();
	int template_width, template_height, search_window_width, search_window_height;
	int template_center_x, template_center_y, search_window_center_x, search_window_center_y;
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
