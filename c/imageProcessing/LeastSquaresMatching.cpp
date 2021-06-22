/*******************************************************************************
                       LeastSquaresMatching.cpp
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

#include "LeastSquaresMatching.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

LeastSquaresMatching::LeastSquaresMatching()
{
    template_width = 10;
    template_height = 10;
    max_iterations = 10;
    max_distance = 20;
    limit_shift_values = 0.05; // Distance between matching window centers
    limit_scale_values = 0.01; // a2 and b1
    limit_shear_values = 0.01; // a1 and b2
    acceptance_correlation = 0.7;
    acceptance_error_ellipse = 5; // Std for a0 and b0 arrays
    gradient_filter = Gradient;
    over_it = true;
    over_it_distance = 0.5;
        temp_growth_step = 2;
        temp_max_size = 50;
}

/*
 * Performs LSM, where img1 is the template and img2 the search image
 * Tx, Ty is the center of the template
 * Mx, My is the center of the matching window
 */
int LeastSquaresMatching::searchHomologous(Matrix *img1, Matrix *img2, double Tx, double Ty, double Mx, double My)
{
    // Save original template size
    int ori_template_width = template_width;
    int ori_template_height = template_height;

    // Clear best correlation
    best_p = 0.0;

    // Check template std - low std, give up
    // If low, try to increase window size by 2
    // Quit if window is too big
    while (true)
    {
                // Uses the template standard deviation test, from "NCC class"
        ncc.setTemplate(template_width,template_height);
        ncc.setTemplateCenter(Tx,Ty);

        if (!ncc.checkTemplateStd(img1))
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
            return -1;
        }
    }

    int ref_w = img1->getCols(), ref_h = img1->getRows();
    int s_w = img2->getCols(), s_h = img2->getRows();
    int delta_tx = round(double(template_width)/2.0), delta_ty = round(double(template_height)/2.0);
    int delta_tx_end = template_width - delta_tx, delta_ty_end = template_height - delta_ty;

    // Error codes
    //  0 - Out of bonds
    // -1 - Low std
    // -2 - Diverged
    // -3 - Max iterations achieved

    // Check if template is out of bounds
        if ((Tx - delta_tx <= 0) || (Ty - delta_ty <= 0) || (Tx + delta_tx_end >= ref_w-1) || (Ty + delta_ty_end >= ref_h-1))
    {
        template_width = ori_template_width;
        template_height = ori_template_height;
        return 0;
    }

    // Check if matching window is out of bounds
        if ((Mx - delta_tx <= 0) || (My - delta_ty <= 0) || (Mx + delta_tx_end >= s_w-1) || (My + delta_ty_end >= s_h-1))
    {
        template_width = ori_template_width;
        template_height = ori_template_height;
        return 0;
    }

    // Calculate template bounding box
    int template_xi = 1 + Tx - delta_tx, template_yi = 1 + Ty - delta_ty;
    //	int template_xf = template_xi + (template_width - 1), template_yf = template_yi + (template_height - 1);
    int matching_xi, matching_yi; //, matching_xf, matching_yf;


    // Calculate matching window bounding box
    matching_xi = 1 + Mx - delta_tx, matching_yi = 1 + My - delta_ty;
    //matching_xf = matching_xi + (template_width - 1), matching_yf = matching_yi + (template_height - 1);

    // Create Template and Matching window
    Matrix T(template_height, template_width);
    Matrix M(template_height, template_width);
    Matrix M1(template_height, template_width);
    Matrix Maux(template_height+2, template_width+2);
    Gx.resize(template_height, template_width);
    Gy.resize(template_height, template_width);

    // Fill template
    for (int l=0; l<template_height; l++)
    {
        for (int k=0; k<template_width; k++)
            T.set(l+1, k+1, img1->get(l + template_yi, k + template_xi));
    }

    // Create affine matrices
    double n = template_width * template_height;
    double i, j, x, y, NMx, NMy;
    double delta_x, delta_y;
    Matrix A(n,6), c(n,1);

    // Define initial affine coefficients
    Matrix X(6,1), t(6,1);
    X.set(1,1,0); X.set(2,1,1); X.set(3,1,0); X.set(4,1,0); X.set(5,1,0); X.set(6,1,1);
    affine_coefficients = X;

    // Convert absolute coordinate to relative coordinate
    double OMx, OMy, Abs_Mx, Abs_My;
    OMx = Abs_Mx = Mx;
    OMy = Abs_My = My;
    Mx = round(template_width/2);
    My = round(template_height/2);
    NMx = Mx;
    NMy = My;

    // LSM
    int pos, iterations=0;
    for (int it=0; it < max_iterations; it++)
    {
        iterations++;

        // New tesselation for matching window
        for (int l=-1; l <= template_height; l++)
        {
            for (int k=-1; k <= template_width; k++)
            {
                // Calculate window position
                x = X.get(1,1) + X.get(2,1)*k + X.get(3,1)*l;
                y = X.get(4,1) + X.get(5,1)*k + X.get(6,1)*l;

                // Calculate absolute pixel position
                x = x + matching_xi;
                y = y + matching_yi;

                // Detect bad parameters (divergence)
                if (x < 1 || y < 1 || x > img2->getCols() || y > img2->getRows())
                {
                    template_width = ori_template_width;
                    template_height = ori_template_height;
                    return -2;
                }

//#define FLOOR(A) ((floor(A)>0)?floor(A):1)
#define FLOOR(A) floor(A)

                // Calculate the gray level in search image using bilinear interpolation
                delta_x = x - floor(x);
                delta_y = y - floor(y);
                Maux.set(l+2, k+2, img2->get(FLOOR(y),FLOOR(x))*(1-delta_x)*(1-delta_y) + img2->get(FLOOR(y),FLOOR(x)+1)*delta_x*(1-delta_y) + img2->get(FLOOR(y)+1,FLOOR(x))*delta_y*(1-delta_x) + img2->get(FLOOR(y)+1,FLOOR(x)+1)*delta_x*delta_y);
            }
        }

        // Calculate gradients
        for (int l=1; l <= template_height; l++)
        {
            for (int k=1; k <= template_width; k++)
            {
                i = k+1;
                j = l+1;
                Gx.set(l, k, Maux.get(j, i+1) - Maux.get(j, i-1));
                Gy.set(l, k, Maux.get(j+1, i) - Maux.get(j-1, i));
                M1.set(l, k, Maux.get(j, i));
            }
        }

        // Create design matrix - i for x, j for y, as described in Schenk
        pos = 1;
        for (int l=1; l <= template_height; l++)
        {
            for (int k=1; k <= template_width; k++)
            {
                x = X.get(1,1) + X.get(2,1)*(k-1) + X.get(3,1)*(l-1);
                y = X.get(4,1) + X.get(5,1)*(k-1) + X.get(6,1)*(l-1);

                // Update A and c matrices
                A.set(pos,1,Gx.get(l, k)); A.set(pos,2,Gx.get(l, k)*x); A.set(pos,3,Gx.get(l, k)*y); A.set(pos,4,Gy.get(l, k)); A.set(pos,5,Gy.get(l, k)*x); A.set(pos,6,Gy.get(l, k)*y);
                c.set(pos,1, T.get(l, k) - M1.get(l, k));

                pos++;
            }
        }

        // Calculate affine transformation
        t = (A.transpose()*A).inverse();
        t = t*(A.transpose()*c);

        // Copy matrix tesselation
        M = M1;

        // Check for limit shift values - convergence
        if ((fabs(t.get(1,1)) < limit_shift_values) && (fabs(t.get(4,1)) < limit_shift_values))
            break;

        // Check for limit scale values - convergence
        if ((fabs(t.get(3,1)) < limit_scale_values) && (fabs(t.get(5,1)) < limit_scale_values))
            break;

        // Check for limit shear values - convergence
        if ((fabs(t.get(2,1)) < limit_scale_values) && (fabs(t.get(6,1)) < limit_scale_values))
            break;

        // Apply shifts to the original coefficients
        X = X + t;

        // Calculate new center
        NMx = X.get(1,1) + X.get(2,1)*Mx + X.get(3,1)*My;
        NMy = X.get(4,1) + X.get(5,1)*Mx + X.get(6,1)*My;

        // Calculate absolute new center
        Abs_Mx = OMx + (NMx - Mx);
        Abs_My = OMy + (NMy - My);

        // Check if new center is far from original center
        if (sqrt(pow(Mx-NMx,2) + pow(My-NMy,2)) > double(max_distance))
        {
            template_width = ori_template_width;
            template_height = ori_template_height;
            return -2; // Diverged !
        }

        // If max iterations achieved, report bad result
        if (it >= max_iterations - 1 && (t.get(1,1) > over_it_distance
                                         || t.get(4,1) > over_it_distance
                                         || !over_it))
        {
            template_width = ori_template_width;
            template_height = ori_template_height;
            return -3;
        }
    }

    //
    // Evaluate data with NCC
    //
    x = round(double(template_width)/2.0);
    y = round(double(template_height)/2.0);
    best_p = ncc.directCorrelation(&T,&M,x,y,x,y);

    //
    // Update data
    //
    affine_coefficients = X;
    best_x = Abs_Mx;
    best_y = Abs_My;

    // Return original template size
    template_width = ori_template_width;
    template_height = ori_template_height;

    return 1;
}

/*
 * Calcualte the next position of Region Growing seeds based on the affine parameters
 */
void LeastSquaresMatching::getNextPosition(double *x, double *y, int step_x, int step_y)
{
    double Mx, My, NMx, NMy;
    Mx = round(template_width/2);
    My = round(template_height/2);

    NMx = affine_coefficients.get(1,1) + affine_coefficients.get(2,1)*(Mx + step_x) + affine_coefficients.get(3,1)*(My + step_y);
    NMy = affine_coefficients.get(4,1) + affine_coefficients.get(5,1)*(Mx + step_x) + affine_coefficients.get(6,1)*(My + step_y);

    *x = *x + (NMx - Mx);
    *y = *y + (NMy - My);
}


/*********************
 *  Basic functions  *
 *********************/

/*
 * Calculate search image gradients
 */
void LeastSquaresMatching::imgGradient(Matrix *img2)
{
    // Select filter
    Matrix Hx, Hy;
    switch (gradient_filter)
    {
    case 1: prewitt(&Hx, &Hy); break;
    case 2: sobel(&Hx, &Hy); break;
    default: gradient(&Hx, &Hy); break;
    }

    // Generate gradients
    Gx = imfilter(img2, &Hx);
    Gy = imfilter(img2, &Hy);
}


/*************
 *  Filters  *
 *************/

void LeastSquaresMatching::gradient(Matrix *Hx, Matrix *Hy)
{
    // Hx filter
    Hx->resize(3,3);
    Hx->set(1,1, 0); Hx->set(1,2,0); Hx->set(1,3,0);
    Hx->set(2,1,-1); Hx->set(2,2,0); Hx->set(2,3,1);
    Hx->set(3,1, 0); Hx->set(3,2,0); Hx->set(3,3,0);

    // Hy filter
    Hy->resize(3,3);
    Hy->set(1,1,0); Hy->set(1,2,-1); Hy->set(1,3,0);
    Hy->set(2,1,0); Hy->set(2,2,0);  Hy->set(2,3,0);
    Hy->set(3,1,0); Hy->set(3,2,1);  Hy->set(3,3,0);
}

void LeastSquaresMatching::prewitt(Matrix *Hx, Matrix *Hy)
{
    // Hx filter
    Hx->resize(3,3);
    Hx->set(1,1,-1); Hx->set(1,2,0); Hx->set(1,3,1);
    Hx->set(2,1,-1); Hx->set(2,2,0); Hx->set(2,3,1);
    Hx->set(3,1,-1); Hx->set(3,2,0); Hx->set(3,3,1);

    // Hy filter
    Hy->resize(3,3);
    Hy->set(1,1,-1); Hy->set(1,2,-1); Hy->set(1,3,-1);
    Hy->set(2,1,0);  Hy->set(2,2,0);  Hy->set(2,3,0);
    Hy->set(3,1,1);  Hy->set(3,2,1);  Hy->set(3,3,1);
}

void LeastSquaresMatching::sobel(Matrix *Hx, Matrix *Hy)
{
    // Hx filter
    Hx->resize(3,3);
    Hx->set(1,1,-1); Hx->set(1,2,0); Hx->set(1,3,1);
    Hx->set(2,1,-2); Hx->set(2,2,0); Hx->set(2,3,2);
    Hx->set(3,1,-1); Hx->set(3,2,0); Hx->set(3,3,1);

    // Hy filter
    Hy->resize(3,3);
    Hy->set(1,1,-1); Hy->set(1,2,-2); Hy->set(1,3,-1);
    Hy->set(2,1,0);  Hy->set(2,2,0);  Hy->set(2,3,0);
    Hy->set(3,1,1);  Hy->set(3,2,2);  Hy->set(3,3,1);
}

Matrix& LeastSquaresMatching::imfilter(Matrix *img, Matrix *H)
{
    int img_width = img->getCols(), img_height = img->getRows();
    int filter_width = H->getCols(), filter_height = H->getRows();
    int filter_half_width = filter_width/2, filter_half_height = filter_height/2;
    Matrix *G = new Matrix(img_height, img_width);
    double value;

    // Outter is the image, innner is the filter
    for (int n = 1; n <= img_height; n++)
    {
        for (int m = 1; m <= img_width; m++)
        {
            value = 0.0;
            for (int l = -filter_half_height; l <= filter_half_height; l++)
            {
                for (int k = -filter_half_width; k <= filter_half_width; k++)
                {
                    // Check if current point is out of img
                    if ((m+k < 1) || (n+l < 1) || (m+k > img_width) || (n+l > img_height))
                        continue;
                    value += img->get(n+l, m+k) * H->get(l+filter_half_height+1, k+filter_half_width+1);
                }
            }
            G->set(n, m, value);
        }
    }

    return *G;
}


} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
