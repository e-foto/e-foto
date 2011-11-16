/*******************************************************************************
                         LeastSquaresMatching.h
*******************************************************************************/

#ifndef LEAST_SQUARES_MATCHING
#define LEAST_SQUARES_MATCHING

#include <stdio.h>
#include <math.h>
#include "Matrix.h"
#include "NormalizedCrossCorrelation.h"

/**
* class LeastSquaresMatching
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 03/08/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

class LeastSquaresMatching
{

public:
	LeastSquaresMatching();
	enum filterType { Gradient, Prewitt, Sobel };
	int searchHomologous(Matrix *img1, Matrix *img2, double Tx, double Ty, double Mx, double My);
	void setTemplate(int _wh) { template_width = _wh; template_height = _wh; };
	void setTemplate(int _w, int _h) { template_width = _w; template_height = _h; };
	void setTemplateCenter(int _x, int _y) { template_center_x = _x; template_center_y = _y; };
	void setMatchingWindowCenter(int _x, int _y) { matching_window_center_x = _x; matching_window_center_y = _y; };
	void setGradientFilter(filterType _ft) { gradient_filter = _ft; };
	int searchHomologous(Matrix *, Matrix *);
	double getBestP() { return best_p; };
	double getBestX() { return best_x; };
	double getBestY() { return best_y; };
	Matrix& getAffineParameters() { return affine_coefficients; };
	void setMinStd(double _mstd) { ncc.setMinStd(_mstd); };
	void setMaxIterations(int _its) { max_iterations = _its; };
	void getNextPosition(double *, double *, int, int);
	void setConvergenceLimits(double _shift, double _scale, double _shear) { limit_shift_values = _shift; limit_scale_values = _scale; limit_shear_values = _shear; };
        void setMaxDistance(double _max) { max_distance = _max; };
        void setOverIt(bool ovit) { over_it = ovit; };
        void setOverItDist(double ovitd) { over_it_distance = ovitd; };

private:
	void imgGradient(Matrix *);
	double average(Matrix *m, int row_i=0, int row_f=0, int col_i=0, int col_f=0);
	double stddev(Matrix *m, int row_i=0, int row_f=0, int col_i=0, int col_f=0);
	int template_width, template_height;
	int template_center_x, template_center_y, matching_window_center_x, matching_window_center_y;
	double best_p, limit_shift_values, limit_scale_values, limit_shear_values, acceptance_correlation, acceptance_error_ellipse;
	double best_x, best_y;
        double over_it_distance;
        bool over_it;
	int max_iterations, max_distance;
	void gradient(Matrix *, Matrix *);
	void prewitt(Matrix *, Matrix *);
	void sobel(Matrix *, Matrix *);
	Matrix& imfilter(Matrix *, Matrix *);
	filterType gradient_filter;
	Matrix Gx, Gy;
	Matrix affine_coefficients;
	NormalizedCrossCorrelation ncc; // Used for evaluation
};

#endif
