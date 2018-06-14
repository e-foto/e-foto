/*******************************************************************************
                       LeastSquaresMatching.cpp
*******************************************************************************/
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
#include "LeastSquaresMatching.h"
#include <math.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

LeastSquaresMatching::LeastSquaresMatching():
    template_width {10},
    template_height {10},
    limit_shift_values {0.05}, // Distance between matching window centers
    limit_scale_values {0.01}, // a2 and b1
    limit_shear_values {0.01}, // a1 and b2
    acceptance_correlation {0.7},
    acceptance_error_ellipse {5}, // Std for a0 and b0 arrays
    over_it_distance {0.5},
    over_it {true},
    max_iterations {10},
    max_distance {20},
    gradient_filter {Gradient},
    temp_growth_step {2},
    temp_max_size {50}
{
}

/*
 * Performs LSM, where img1 is the template and img2 the search image
 * Tx, Ty is the center of the template
 * Mx, My is the center of the matching window
 */
int LeastSquaresMatching::searchHomologous(Matrix* img1, Matrix* img2,
        double Tx, double Ty, double Mx, double My)
{
    // Save original template size
    auto ori_template_width = template_width;
    auto ori_template_height = template_height;
    // Clear best correlation
    best_p = 0.0;

    // Check template std - low std, give up
    // If low, try to increase window size by 2
    // Quit if window is too big
    while (true) {
        // Uses the template standard deviation test, from "NCC class"
        ncc.setTemplate(template_width, template_height);
        ncc.setTemplateCenter(Tx, Ty);

        if (!ncc.checkTemplateStd(img1)) {
            template_width += temp_growth_step;
            template_height += temp_growth_step;
        } else {
            break;
        }

        // Check if size is too big, with still low variance - really quit
        if (template_width > temp_max_size || template_height > temp_max_size) {
            template_width = ori_template_width;
            template_height = ori_template_height;
            return -1;
        }
    }

    const size_t ref_w {img1->getCols()};
    const size_t ref_h {img1->getRows()};
    const size_t s_w {img2->getCols()};
    const size_t s_h {img2->getRows()};
    const int delta_tx = round(template_width / 2.0);
    const int delta_ty = round(template_height / 2.0);
    const int delta_tx_end = template_width - delta_tx;
    const int delta_ty_end = template_height - delta_ty;

    // Error codes
    //  0 - Out of bonds
    // -1 - Low std
    // -2 - Diverged
    // -3 - Max iterations achieved

    // Check if template is out of bounds
    if ((Tx - delta_tx <= 0)
            || (Ty - delta_ty <= 0)
            || (Tx + delta_tx_end >= ref_w - 1)
            || (Ty + delta_ty_end >= ref_h - 1)) {
        template_width = ori_template_width;
        template_height = ori_template_height;
        return 0;
    }

    // Check if matching window is out of bounds
    if ((Mx - delta_tx <= 0)
            || (My - delta_ty <= 0)
            || (Mx + delta_tx_end >= s_w - 1)
            || (My + delta_ty_end >= s_h - 1)) {
        template_width = ori_template_width;
        template_height = ori_template_height;
        return 0;
    }

    // Calculate template bounding box
    const int template_xi = 1 + Tx - delta_tx;
    const int template_yi = 1 + Ty - delta_ty;

    // Calculate matching window bounding box
    const int matching_xi = 1 + Mx - delta_tx;
    const int matching_yi = 1 + My - delta_ty;
    // Create Template and Matching window
    Matrix T(template_height, template_width);
    Matrix M(template_height, template_width);
    Matrix M1(template_height, template_width);
    Matrix Maux(template_height + 2, template_width + 2);
    Gx.resize(template_height, template_width);
    Gy.resize(template_height, template_width);

    // Fill template
    for (auto l = 0; l < template_height; l++) {
        for (auto k = 0; k < template_width; k++) {
            T.set(l + 1, k + 1, img1->get(l + template_yi, k + template_xi));
        }
    }

    // Create affine matrices
    const double n = template_width * template_height;
    double i, j, x, y, NMx, NMy;
    double delta_x, delta_y;
    Matrix A(n, 6), c(n, 1);
    // Define initial affine coefficients
    Matrix X(6, 1), t(6, 1);
    X.set(1, 1, 0);
    X.set(2, 1, 1);
    X.set(3, 1, 0);
    X.set(4, 1, 0);
    X.set(5, 1, 0);
    X.set(6, 1, 1);
    affine_coefficients = X;
    // Convert absolute coordinate to relative coordinate
    double OMx, OMy, Abs_Mx, Abs_My;
    OMx = Abs_Mx = Mx;
    OMy = Abs_My = My;
    Mx = round(template_width / 2);
    My = round(template_height / 2);
    NMx = Mx;
    NMy = My;
    // LSM
    int pos, iterations = 0;

    for (auto it = 0; it < max_iterations; it++) {
        iterations++;

        // New tesselation for matching window
        for (auto l = -1; l <= template_height; l++) {
            for (auto k = -1; k <= template_width; k++) {
                // Calculate window position
                x = X.get(1, 1) + X.get(2, 1) * k + X.get(3, 1) * l;
                y = X.get(4, 1) + X.get(5, 1) * k + X.get(6, 1) * l;
                // Calculate absolute pixel position
                x = x + matching_xi;
                y = y + matching_yi;

                // Detect bad parameters (divergence)
                if (x < 1 || y < 1 || x > img2->getCols() || y > img2->getRows()) {
                    template_width = ori_template_width;
                    template_height = ori_template_height;
                    return -2;
                }

#define FLOOR(A) floor(A)
                // Calculate the gray level in search image using bilinear interpolation
                delta_x = x - floor(x);
                delta_y = y - floor(y);
                Maux.set(l + 2,
                         k + 2,
                         img2->get(FLOOR(y), FLOOR(x)) * (1 - delta_x) * (1 - delta_y)
                         + img2->get(FLOOR(y), FLOOR(x) + 1)*delta_x * (1 - delta_y)
                         + img2->get(FLOOR(y) + 1, FLOOR(x))*delta_y * (1 - delta_x)
                         + img2->get(FLOOR(y) + 1, FLOOR(x) + 1)*delta_x * delta_y);
            }
        }

        // Calculate gradients
        for (auto l = 1; l <= template_height; l++) {
            for (auto k = 1; k <= template_width; k++) {
                i = k + 1;
                j = l + 1;
                Gx.set(l, k, Maux.get(j, i + 1) - Maux.get(j, i - 1));
                Gy.set(l, k, Maux.get(j + 1, i) - Maux.get(j - 1, i));
                M1.set(l, k, Maux.get(j, i));
            }
        }

        // Create design matrix - i for x, j for y, as described in Schenk
        pos = 1;

        for (auto l = 1; l <= template_height; l++) {
            for (auto k = 1; k <= template_width; k++) {
                x = X.get(1, 1) + X.get(2, 1) * (k - 1) + X.get(3, 1) * (l - 1);
                y = X.get(4, 1) + X.get(5, 1) * (k - 1) + X.get(6, 1) * (l - 1);
                // Update A and c matrices
                A.set(pos, 1, Gx.get(l, k));
                A.set(pos, 2, Gx.get(l, k)*x);
                A.set(pos, 3, Gx.get(l, k)*y);
                A.set(pos, 4, Gy.get(l, k));
                A.set(pos, 5, Gy.get(l, k)*x);
                A.set(pos, 6, Gy.get(l, k)*y);
                c.set(pos, 1, T.get(l, k) - M1.get(l, k));
                pos++;
            }
        }

        // Calculate affine transformation
        t = (A.transpose() * A).inverse();
        t = t * (A.transpose() * c);
        // Copy matrix tesselation
        M = M1;

        // Check for limit shift values - convergence
        if ((fabs(t.get(1, 1)) < limit_shift_values)
                && (fabs(t.get(4, 1)) < limit_shift_values)) {
            break;
        }

        // Check for limit scale values - convergence
        if ((fabs(t.get(3, 1)) < limit_scale_values)
                && (fabs(t.get(5, 1)) < limit_scale_values)) {
            break;
        }

        // Check for limit shear values - convergence
        if ((fabs(t.get(2, 1)) < limit_scale_values)
                && (fabs(t.get(6, 1)) < limit_scale_values)) {
            break;
        }

        // Apply shifts to the original coefficients
        X = X + t;
        // Calculate new center
        NMx = X.get(1, 1) + X.get(2, 1) * Mx + X.get(3, 1) * My;
        NMy = X.get(4, 1) + X.get(5, 1) * Mx + X.get(6, 1) * My;
        // Calculate absolute new center
        Abs_Mx = OMx + (NMx - Mx);
        Abs_My = OMy + (NMy - My);

        // Check if new center is far from original center
        if (sqrt(pow(Mx - NMx, 2) + pow(My - NMy, 2)) > double(max_distance)) {
            template_width = ori_template_width;
            template_height = ori_template_height;
            return -2; // Diverged !
        }

        // If max iterations achieved, report bad result
        if (it >= max_iterations - 1 && (t.get(1, 1) > over_it_distance
                                         || t.get(4, 1) > over_it_distance || ~(over_it))) {
            template_width = ori_template_width;
            template_height = ori_template_height;
            return -3;
        }
    }

    // Evaluate data with NCC
    x = round(double(template_width) / 2.0);
    y = round(double(template_height) / 2.0);
    best_p = ncc.directCorrelation(&T, &M, x, y, x, y);
    // Update data
    affine_coefficients = X;
    best_x = Abs_Mx;
    best_y = Abs_My;
    // Return original template size
    template_width = ori_template_width;
    template_height = ori_template_height;
    return 1;
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
