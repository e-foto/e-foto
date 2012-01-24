/*******************************************************************************
						 NormalizedCrossCorrelation.h
*******************************************************************************/

#ifndef NORMALIZED_CROSS_CORRELATION
#define NORMALIZED_CROSS_CORRELATION

#include <stdio.h>
#include <math.h>
#include "Matrix.h"

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

class NormalizedCrossCorrelation
{

public:
	NormalizedCrossCorrelation();
	void setTemplate(int _wh) { template_width = _wh; template_height = _wh; };
	void setTemplate(int _w, int _h) { template_width = _w; template_height = _h; };
	void setSearchWindow(int _wh) { search_window_width = _wh; search_window_height = _wh; };
	void setSearchWindow(int _w, int _h) { search_window_width = _w; search_window_height = _h; };
	void setTemplateCenter(int _x, int _y) { template_center_x = _x; template_center_y = _y; };
	void setSearchWindowCenter(int _x, int _y) { search_window_center_x = _x; search_window_center_y = _y; };
	int searchHomologous(Matrix *, Matrix *);
	double directCorrelation(Matrix *X, Matrix *Y, int template_cx, int template_cy, int matching_cx, int matching_cy);
	double getBestP() { return best_p; };
	int getBestX() { return best_x; };
	int getBestY() { return best_y; };
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
	int template_width, template_height, search_window_width, search_window_height;
	int template_center_x, template_center_y, search_window_center_x, search_window_center_y;
	double best_p;
	int best_x, best_y;
	double min_std_acceptance;
        int temp_growth_step, temp_max_size;

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
