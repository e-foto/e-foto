/*******************************************************************************
					   NormalizedCrossCorrelation.cpp
*******************************************************************************/

#include "NormalizedCrossCorrelation.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

NormalizedCrossCorrelation::NormalizedCrossCorrelation()
{
	template_width = 7;
	template_height = 7;
	search_window_width = 50;
	search_window_height = 50;
	//	min_std_acceptance = 0.019607843; // 5.0 in 0-255 range
	min_std_acceptance = 0.15;
        temp_growth_step = 2;
        temp_max_size = 50;
}

int NormalizedCrossCorrelation::searchHomologous(Matrix *refmat, Matrix *searchmat)
{
	double p;
        best_p = 0.0;

	//
	// Check if data was correctly assigned - Notice that Matrix notation ranges from 1,1 to N,M
	//

	// Error codes:
	//  1 - No errors
	// -1 - Search window is smaller than template
	// -2 - Template is too small, less than 3 pixels
	// -3 - Template is out of the image
	// -4 - Template has low std

	// Check search window size
	if ((search_window_width < template_width) || (search_window_height < template_height))
		return -1;

	// Check the size of the template
	if ((template_width < 3) || (template_height < 3))
		return -2;

	// Save original template size
	int ori_template_width = template_width;
	int ori_template_height = template_height;

	bool flag = true;
        int ref_w, ref_h, s_w, s_h, template_xi, template_yi, template_xf, template_yf;

        // Read image matrices data
        ref_w = refmat->getCols();
        ref_h = refmat->getRows();
        s_w = searchmat->getCols();
        s_h = searchmat->getRows();

        // Calculate ideal search window size based on standard deviation
	while (flag)
	{
                // Calculate template limits
                template_xi = template_center_x - floor(double(template_width)/2.0);
                template_yi = template_center_y - floor(double(template_height)/2.0);
                template_xf = template_xi + template_width - 1;
                template_yf = template_yi + template_height - 1;

		// Check if template center is out of bounds
                if ((template_xi <= 0) || (template_yi <= 0) || (template_xf >= ref_w - 1) || (template_yf >= ref_h - 1))
			return -3;

		// Check if low variance
		if (!checkTemplateStd(refmat))
		{
                        template_width += temp_growth_step;
                        template_height += temp_growth_step;
		}
		else
			break;

		// Check if size is too big, with still low variance - really quit
                if (template_width > temp_max_size || template_height > temp_max_size)
		{
			template_width = ori_template_width;
			template_height = ori_template_height;
			return -4;
		}
	}

	//
	// Start searching
	//

        // Calculate seach window limits
        int search_window_xi = search_window_center_x - floor(double(search_window_width)/2.0), search_window_yi = search_window_center_y - floor(double(search_window_height)/2.0);
        int delta_tx = floor(double(template_width)/2.0);
        int delta_ty = floor(double(template_height)/2.0);

	for (int sw_i = search_window_yi ; sw_i <= search_window_yi + (search_window_height - 1); sw_i++)
	{
		for (int sw_j = search_window_xi ; sw_j <= search_window_xi + (search_window_width - 1); sw_j++)
		{
			// Check if matching window is out of bounds
                        if ((sw_i <= 0) || (sw_j <= 0) || (sw_i >= s_h-1) || (sw_j >= s_w-1))
				continue;

			// Calculate correlation
			p = correlation(refmat, searchmat, template_yi, template_yf, template_xi, template_xf, sw_i, sw_i + (template_height-1), sw_j, sw_j + (template_width-1));

			if (p > best_p)
			{
				best_p = p;
                                best_x = sw_j + (delta_tx);
                                best_y = sw_i + (delta_ty);
			}
		}
	}

	// Return original template size
	template_width = ori_template_width;
	template_height = ori_template_height;

        // Matching successful
        return 1;
}

double NormalizedCrossCorrelation::average(Matrix *m, int row_i, int row_f, int col_i, int col_f)
{
	// If assigned with 0, calculate the average for the whole matrix
	if ((row_i < 1) || (row_f < 1) || (col_i < 1) ||(col_f < 1))
	{
		row_i = 1;
		col_i = 1;
		row_f = m->getRows();
		col_f = m->getCols();
	}

	double sum=0.0;
	int n = (1+(row_f-row_i))*(1+(col_f-col_i));
	for (int i=row_i; i<=row_f; i++)
	{
		for (int j=col_i; j<=col_f; j++)
			sum += m->get(i,j);
	}

	return sum/double(n);
}


double NormalizedCrossCorrelation::stddev(Matrix *m, int row_i, int row_f, int col_i, int col_f)
{
	// If assigned with 0, calculate the standard deviation for the whole matrix
	if ((row_i < 1) || (row_f < 1) || (col_i < 1) ||(col_f < 1))
	{
		row_i = 1;
		col_i = 1;
		row_f = m->getRows();
		col_f = m->getCols();
	}

	double avg = average(m, row_i, row_f, col_i, col_f);
	double sum=0.0;
	int n = (1+(row_f-row_i))*(1+(col_f-col_i));
	for (int i=row_i; i<=row_f; i++)
	{
		for (int j=col_i; j<=col_f; j++)
			sum += pow(m->get(i,j) - avg, 2);
	}

	return sqrt(sum/double(n-1));
}

double NormalizedCrossCorrelation::covXY(Matrix *X, Matrix *Y, int row_x_i, int row_x_f, int col_x_i, int col_x_f, int row_y_i, int row_y_f, int col_y_i, int col_y_f)
{
	// If assigned with 0, calculate the standard deviation for the whole matrix
	if ((row_x_i < 1) || (row_x_f < 1) || (col_x_i < 1) ||(col_x_f < 1))
	{
		row_x_i = 1;
		col_x_i = 1;
		row_x_f = X->getRows();
		col_x_f = X->getCols();
	}

	if ((row_y_i < 1) || (row_y_f < 1) || (col_y_i < 1) ||(col_y_f < 1))
	{
		row_y_i = 1;
		col_y_i = 1;
		row_y_f = Y->getRows();
		col_y_f = Y->getCols();
	}

	int delta_i = row_y_i - row_x_i;
	int delta_j = col_y_i - col_x_i;
	double avgX = average(X, row_x_i, row_x_f, col_x_i, col_x_f);
	double avgY = average(Y, row_y_i, row_y_f, col_y_i, col_y_f);
	double sum=0.0;
	int n = (1+(row_x_f-row_x_i))*(1+(col_x_f-col_x_i));
	for (int i=row_x_i; i<=row_x_f; i++)
	{
		for (int j=col_x_i; j<=col_x_f; j++)
		{
			sum += (X->get(i,j) - avgX)*(Y->get(i+delta_i, j+delta_j) - avgY);
		}
	}

	return sum/double(n-1);
}

/*
 *  This function calculates the correlation between the template and the matching window (not the whole search window)
 */
double NormalizedCrossCorrelation::directCorrelation(Matrix *X, Matrix *Y, int template_cx, int template_cy, int matching_cx, int matching_cy)
{
	int ref_w = X->getCols(), ref_h = X->getRows();
	int s_w = Y->getCols(), s_h = Y->getRows();
	int delta_tx = round(double(template_width)/2.0), delta_ty = round(double(template_height)/2.0);
	int delta_tx_end = template_width - delta_tx, delta_ty_end = template_height - delta_ty;

	// Check if template is out of bounds
	if ((template_cx - delta_tx < 0) || (template_cy - delta_ty < 0) || (template_cx + delta_tx_end > ref_w) || (template_cy + delta_ty_end > ref_h))
		return 0.0;

	// Check if matching window is out of bounds
	if ((matching_cx - delta_tx < 0) || (matching_cy - delta_ty < 0) || (matching_cx + delta_tx_end > s_w) || (matching_cy + delta_ty_end > s_h))
		return 0.0;

	int template_xi = 1 + template_cx - delta_tx, template_yi = 1 + template_cy - delta_ty;
	int template_xf = template_xi + (template_width - 1), template_yf = template_yi + (template_height - 1);
	int matching_window_xi = 1 + matching_cx - delta_tx, matching_window_yi = 1 + matching_cy - delta_ty;
	int matching_window_xf = matching_window_xi + (template_width - 1), matching_window_yf = matching_window_yi + (template_height - 1);

	return correlation(X, Y, template_yi, template_yf, template_xi, template_xf, matching_window_yi, matching_window_yf, matching_window_xi, matching_window_xf);
}

double NormalizedCrossCorrelation::correlation(Matrix *X, Matrix *Y, int row_x_i, int row_x_f, int col_x_i, int col_x_f, int row_y_i, int row_y_f, int col_y_i, int col_y_f)
{
	// If assigned with 0, calculate the standard deviation for the whole matrix
	if ((row_x_i < 1) || (row_x_f < 1) || (col_x_i < 1) ||(col_x_f < 1))
	{
		row_x_i = 1;
		col_x_i = 1;
		row_x_f = X->getRows();
		col_x_f = X->getCols();
	}

	if ((row_y_i < 1) || (row_y_f < 1) || (col_y_i < 1) ||(col_y_f < 1))
	{
		row_y_i = 1;
		col_y_i = 1;
		row_y_f = Y->getRows();
		col_y_f = Y->getCols();
	}

	return covXY(X, Y, row_x_i, row_x_f, col_x_i, col_x_f, row_y_i, row_y_f, col_y_i, col_y_f) / (stddev(X, row_x_i, row_x_f, col_x_i, col_x_f) * stddev(Y, row_y_i, row_y_f, col_y_i, col_y_f));
}

bool NormalizedCrossCorrelation::checkTemplateStd(Matrix *refmat)
{
	return (calculateTemplateStd(refmat) >= min_std_acceptance);
}

double NormalizedCrossCorrelation::calculateTemplateStd(Matrix *refmat)
{
	int delta_tx = round(double(template_width)/2.0), delta_ty = round(double(template_height)/2.0);
	int template_xi = 1 + template_center_x - delta_tx, template_yi = 1 + template_center_y - delta_ty;
	int template_xf = template_xi + (template_width - 1), template_yf = template_yi + (template_height - 1);

	return stddev(refmat, template_yi, template_yf, template_xi, template_xf);
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
