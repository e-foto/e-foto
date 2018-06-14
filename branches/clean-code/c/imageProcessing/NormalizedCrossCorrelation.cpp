/*******************************************************************************
                       NormalizedCrossCorrelation.cpp
*******************************************************************************/

#include "NormalizedCrossCorrelation.h"
#include "Matrix.h"

#include <cmath>
#include <stdio.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

NormalizedCrossCorrelation::NormalizedCrossCorrelation():
    template_width_{7},
    template_height_{7},
    search_window_width_{50},
    search_window_height_{50},
    min_std_acceptance_{0.15},
    temp_growth_step_{2},
    temp_max_size_{50}
{
}

int NormalizedCrossCorrelation::searchHomologous(Matrix* refmat,
        Matrix* searchmat)
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
    if ((search_window_width_ < template_width_)
            || (search_window_height_ < template_height_)) {
        return -1;
    }

    // Check the size of the template
    if ((template_width_ < 3) || (template_height_ < 3)) {
        return -2;
    }

    // Save original template size (template size may increase with the next test)
    const int ori_template_width {template_width_};
    const int ori_template_height{template_height_};
    const bool flag{true};

    // Read image matrices data
    const int ref_w = refmat->getCols();
    const int ref_h = refmat->getRows();
    const int s_w = searchmat->getCols();
    const int s_h = searchmat->getRows();

    //
    // Calculate ideal search window size based on standard deviation (templation size may increase)
    //
    while (flag) {
        // Calculate template limits
        calculateTemplateBoundingBox();

        // Check if template center is out of bounds
        if ((template_xi <= 0)
                || (template_yi <= 0)
                || (template_xf >= ref_w - 1)
                || (template_yf >= ref_h - 1)) {
            template_width_ = ori_template_width;
            template_height_ = ori_template_height;
            return -3;
        }

        // Check if low variance
        if (!checkTemplateStd(refmat)) {
            template_width_ += temp_growth_step_;
            template_height_ += temp_growth_step_;
        } else {
            break;
        }

        // Check if size is too big, with still low variance - really quit
        if (template_width_ > temp_max_size_
                || template_height_ > temp_max_size_) {
            template_width_ = ori_template_width;
            template_height_ = ori_template_height;
            return -4;
        }
    }

    // Start searching
    // Calculate seach window limits
    const int search_window_xi = search_window_center_x
            - floor(search_window_width_ / 2.0);
    const int search_window_yi = search_window_center_y
            - floor(search_window_height_ / 2.0);
    const int delta_tx = floor(template_width_ / 2.0);
    const int delta_ty = floor(template_height_ / 2.0);

    for (auto sw_i = search_window_yi;
         sw_i <= search_window_yi + (search_window_height_ - 1);
         sw_i++) {
        for (auto sw_j = search_window_xi ;
             sw_j <= search_window_xi + (search_window_width_ - 1);
             sw_j++) {
            // Check if matching window is out of bounds
            if ((sw_i <= 0) || (sw_j <= 0) || (sw_i >= s_h - 1) || (sw_j >= s_w - 1)) {
                continue;
            }

            // Calculate correlation
            p = correlation(refmat, searchmat, template_yi, template_yf, template_xi,
                            template_xf, sw_i, sw_i + (template_height_ - 1), sw_j,
                            sw_j + (template_width_ - 1));

            if (p > best_p) {
                best_p = p;
                best_x = (double) sw_j + (delta_tx);
                best_y = (double) sw_i + (delta_ty);
            }
        }
    }

    // Return original template size
    template_width_ = ori_template_width;
    template_height_ = ori_template_height;
    // Add fraction part to result
    best_x += frac_tc_x;
    best_y += frac_tc_y;
    // Matching successful
    return 1;
}

double NormalizedCrossCorrelation::average(Matrix* m,
                                           int row_i,
                                           int row_f,
                                           int col_i,
                                           int col_f)
{
    // If assigned with 0, calculate the average for the whole matrix
    if ((row_i < 1) || (row_f < 1) || (col_i < 1) || (col_f < 1)) {
        row_i = 1;
        col_i = 1;
        row_f = m->getRows();
        col_f = m->getCols();
    }

    double sum = 0.0;
    const int n = (1 + (row_f - row_i)) * (1 + (col_f - col_i));

    for (auto i = row_i; i <= row_f; i++) {
        for (auto j = col_i; j <= col_f; j++) {
            sum += m->get(i, j);
        }
    }

    return sum / n;
}


double NormalizedCrossCorrelation::stddev(Matrix* m,
                                          int row_i,
                                          int row_f,
                                          int col_i,
                                          int col_f)
{
    // If assigned with 0, calculate the standard deviation for the whole matrix
    if ((row_i < 1) || (row_f < 1) || (col_i < 1) || (col_f < 1)) {
        row_i = 1;
        col_i = 1;
        row_f = m->getRows();
        col_f = m->getCols();
    }

    const double avg = average(m, row_i, row_f, col_i, col_f);
    double sum = 0.0;
    const int n = (1 + (row_f - row_i)) * (1 + (col_f - col_i));

    for (auto i = row_i; i <= row_f; i++) {
        for (auto j = col_i; j <= col_f; j++) {
            sum += pow(m->get(i, j) - avg, 2);
        }
    }

    return sqrt(sum / double(n - 1));
}

double NormalizedCrossCorrelation::covXY(Matrix* X, Matrix* Y, int row_x_i,
        int row_x_f, int col_x_i, int col_x_f, int row_y_i, int row_y_f, int col_y_i,
        int col_y_f)
{
    // If assigned with 0, calculate the standard deviation for the whole matrix
    if ((row_x_i < 1) || (row_x_f < 1) || (col_x_i < 1) || (col_x_f < 1)) {
        row_x_i = 1;
        col_x_i = 1;
        row_x_f = X->getRows();
        col_x_f = X->getCols();
    }

    if ((row_y_i < 1) || (row_y_f < 1) || (col_y_i < 1) || (col_y_f < 1)) {
        row_y_i = 1;
        col_y_i = 1;
        row_y_f = Y->getRows();
        col_y_f = Y->getCols();
    }

    const int delta_i = row_y_i - row_x_i;
    const int delta_j = col_y_i - col_x_i;
    const double avgX = average(X, row_x_i, row_x_f, col_x_i, col_x_f);
    const double avgY = average(Y, row_y_i, row_y_f, col_y_i, col_y_f);
    double sum = 0.0;
    int n = (1 + (row_x_f - row_x_i)) * (1 + (col_x_f - col_x_i));

    for (auto i = row_x_i; i <= row_x_f; i++) {
        for (auto j = col_x_i; j <= col_x_f; j++) {
            sum += (X->get(i, j) - avgX) * (Y->get(i + delta_i, j + delta_j) - avgY);
        }
    }

    return sum / double(n - 1);
}

/*
 *  This function calculates the correlation between the template and the matching window (not the whole search window)
 */
double NormalizedCrossCorrelation::directCorrelation(Matrix* X, Matrix* Y,
        int template_cx, int template_cy, int matching_cx, int matching_cy)
{
    const int ref_w = X->getCols();
    const int ref_h = X->getRows();
    const int s_w = Y->getCols();
    const int s_h = Y->getRows();
    const int delta_tx = round(double(template_width_) / 2.0);
    const int delta_ty = round(double(template_height_) / 2.0);
    const int delta_tx_end = template_width_ - delta_tx;
    const int delta_ty_end = template_height_ - delta_ty;

    // Check if template is out of bounds
    if ((template_cx - delta_tx < 0)
            || (template_cy - delta_ty < 0)
            || (template_cx + delta_tx_end > ref_w)
            || (template_cy + delta_ty_end > ref_h)) {
        return 0.0;
    }

    // Check if matching window is out of bounds
    if ((matching_cx - delta_tx < 0)
            || (matching_cy - delta_ty < 0)
            || (matching_cx + delta_tx_end > s_w)
            || (matching_cy + delta_ty_end > s_h)) {
        return 0.0;
    }

    const int template_xi = 1 + template_cx - delta_tx;
    const int template_yi = 1 + template_cy - delta_ty;
    const int template_xf = template_xi + (template_width_ - 1);
    const int template_yf = template_yi + (template_height_ - 1);
    const int matching_window_xi = 1 + matching_cx - delta_tx;
    const int matching_window_yi = 1 + matching_cy - delta_ty;
    const int matching_window_xf = matching_window_xi + (template_width_ - 1);
    const int matching_window_yf = matching_window_yi + (template_height_ - 1);
    const double p = correlation(X,
                                 Y,
                                 template_yi,
                                 template_yf,
                                 template_xi,
                                 template_xf,
                                 matching_window_yi,
                                 matching_window_yf,
                                 matching_window_xi,
                                 matching_window_xf);
    return p;
}

double NormalizedCrossCorrelation::correlation(Matrix* X, Matrix* Y,
        int row_x_i, int row_x_f, int col_x_i, int col_x_f, int row_y_i, int row_y_f,
        int col_y_i, int col_y_f)
{
    // If assigned with 0, calculate the standard deviation for the whole matrix
    if ((row_x_i < 1) || (row_x_f < 1) || (col_x_i < 1) || (col_x_f < 1)) {
        row_x_i = 1;
        col_x_i = 1;
        row_x_f = X->getRows();
        col_x_f = X->getCols();
    }

    if ((row_y_i < 1) || (row_y_f < 1) || (col_y_i < 1) || (col_y_f < 1)) {
        row_y_i = 1;
        col_y_i = 1;
        row_y_f = Y->getRows();
        col_y_f = Y->getCols();
    }

    // Checks respect for the image boundaries
    if ((unsigned int)row_x_f > X->getRows()) {
        return 0.0;
    }

    if ((unsigned int)col_x_f > X->getCols()) {
        return 0.0;
    }

    if ((unsigned int)row_y_f > Y->getRows()) {
        return 0.0;
    }

    if ((unsigned int)col_y_f > Y->getCols()) {
        return 0.0;
    }

    // Define number of elements in submatrix
    const int n = (1 + (row_x_f - row_x_i)) * (1 + (col_x_f - col_x_i));
    // Define delta (the translation)
    const int delta_i = row_y_i - row_x_i;
    const int delta_j = col_y_i - col_x_i;
    // Calculate the Average X & Average Y
    double avgX{0.0};
    double avgY{0.0};

    for (auto i = row_x_i; i <= row_x_f; i++) {
        for (auto j = col_x_i; j <= col_x_f; j++) {
            avgX += X->get(i, j);
            avgY += Y->get(i + delta_i, j + delta_j);
        }
    }

    avgX = avgX / double(n);
    avgY = avgY / double(n);
    // Calculate the Standard Deviation X, Standard Deviation Y and Covariance XY
    double covXY = 0.0;
    double stdDevX = 0.0;
    double stdDevY = 0.0;
    {
        for (int i = row_x_i; i <= row_x_f; i++) {
            for (int j = col_x_i; j <= col_x_f; j++) {
                covXY += (X->get(i, j) - avgX) * (Y->get(i + delta_i, j + delta_j) - avgY);
                stdDevX += pow(X->get(i, j) - avgX, 2);
                stdDevY += pow(Y->get(i + delta_i, j + delta_j) - avgY, 2);
            }
        }
    }
    covXY = covXY / (n - 1);
    stdDevX = sqrt(stdDevX / (n - 1));
    stdDevY = sqrt(stdDevY / (n - 1));
    // Calculate the Correlation
    return covXY / (stdDevX * stdDevY);
}

bool NormalizedCrossCorrelation::checkTemplateStd(Matrix* refmat)
{
    return (calculateTemplateStd(refmat) >= min_std_acceptance_);
}

double NormalizedCrossCorrelation::calculateTemplateStd(Matrix* refmat)
{
    calculateTemplateBoundingBox();
    return stddev(refmat, template_yi, template_yf, template_xi, template_xf);
}

void NormalizedCrossCorrelation::calculateTemplateBoundingBox()
{
    template_xi = template_center_x - template_width_ / 2;
    template_yi = template_center_y - template_height_ / 2;
    template_xf = template_xi + template_width_ - 1;
    template_yf = template_yi + template_height_ - 1;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
