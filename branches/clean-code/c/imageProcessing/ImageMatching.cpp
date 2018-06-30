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
#include "ImageMatching.h"
#include "DEMManager.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

ImageMatching::ImageMatching():
    ImageMatching(nullptr)
{
}

ImageMatching::ImageMatching(DEMManager* man):
    manager_{man},
    image_depth_{256},
    coverage{0.0},
    max_size{0.0},
    num_visited{0.0},
    corr_th_{0.7},
    perform_readiometric_{true},
    radiometric_mode_{HistMatching},
    cancel_flag_{false},
    matching_xi_{0},
    matching_xf_{0},
    matching_yi_{0},
    matching_yf_{0},
    smatching_xi_{0},
    smatching_xf_{0},
    smatching_yi_{0},
    smatching_yf_{0},
    left_image_id{0},
    right_image_id{0},
    step_x_{3},
    step_y_{3},
    img_width{0},
    img_height{0},
    simg_width{0},
    simg_height{0},
    matching_method_{LSM},
    stack_{nullptr},
    elim_bad_pts_{false},
    perform_RG{false},
    elap_time_{0.0}
{
}

void ImageMatching::setMinStd(double std)
{
    ncc.setMinStd(std);
    lsm.setMinStd(std);
}

void ImageMatching::performImageMatching(Matrix* img1, Matrix* img2,
        MatchingPointsList* repository, MatchingPointsList* mpoints)
{
    cancel_flag_ = false;

    // Step 1 - Radiometric Tranformation
    if (perform_readiometric_) {
        // Histogram equalization
        if (radiometric_mode_ == Equalization) {
            rt.histeq(img1, image_depth_);
            rt.histeq(img2, image_depth_);
        }

        // Histogram matching
        if (radiometric_mode_ == HistMatching) {
            rt.histmatching(img1, img2, image_depth_);
        }
    }

    // Step 2 - Evaluate and fix matching limits
    const int border = 30;
    img_width = img1->getCols();
    img_height = img1->getRows();
    simg_width = img2->getCols();
    simg_height = img2->getRows();

    if (matching_xi_ < 1 + border || matching_xi_ > img_width - border) {
        matching_xi_ = 1 + border;
    }

    if (matching_xf_ < 1 + border || matching_xf_ > img_width - border) {
        matching_xf_ = img_width - border;
    }

    if (matching_yi_ < 1 + border || matching_yi_ > img_height - border) {
        matching_yi_ = 1 + border;
    }

    if (matching_yf_ < 1 + border || matching_yf_ > img_height - border) {
        matching_yf_ = img_height - border;
    }

    smatching_xi_ = 1 + border;
    smatching_xf_ = simg_width - border;
    smatching_yi_ = 1 + border;
    smatching_yf_ =  simg_height - border;
    // Step 3 - Create Region Growing map
    auto map_width = 1 + (img_width / step_x_),
         map_height = 1 + (img_height / step_y_);
    map.resize(map_height, map_width);
    fillMap(mpoints);
    // Step 4 - Read seed repository
    // NOTE: NCC and LSM parameters must be set by the parent class or program
    const int num_seeds = repository->size();

    // Calculate coverage
    coverage = 0.0;
    max_size = static_cast<double>(map.getCols() * map.getRows());
    num_visited = 0.0;

    for (int i = 1; i <= num_seeds; i++) {
        // Read current seed, converting to matrix coordinate system
        MatchingPoints mp = *repository->get(i);
        double lx = mp.left_x + 1.0;
        double ly = mp.left_y + 1.0;
        double rx = mp.right_x + 1.0;
        double ry = mp.right_y + 1.0;
        int curr_left_id = mp.left_image_id;
        int curr_right_id = mp.right_image_id;

        // Step 5 - Perform Region Growing
        if (curr_left_id == left_image_id && curr_right_id == right_image_id) {
            region_growing(img1, img2, mpoints, lx, ly, rx, ry);

            if (cancel_flag_) {
                return;
            }
        }
    }

    // Step 6 - Eliminate bad points using statistics
    // Default values: sigma_x=3, sigma_y=1.5
    if (elim_bad_pts_) {
        mpoints->filterBadPoints2D();
    }

    elap_time_ = 0.0;
}

/*
 * Fill map using matching list, if this is the 2nd or greater running time
 **/
void ImageMatching::fillMap(MatchingPointsList* mpoints) const
{
    for (size_t f = 1; f <= mpoints->size(); f++) {
        MatchingPoints* mp = mpoints->get(f);

        if (left_image_id != mp->left_image_id
                || right_image_id != mp->right_image_id) {
            continue;
        }

        /* Se definirmos que o sensor vai ser alterado para especificar
         * onde esta o ponto zero do pixel este ponto vai ser alterado
         * com a adicao de uma estrutura condicional.
         */
        int lx = static_cast<int>(mp->left_x);
        int ly = static_cast<int>(mp->left_y);
        int i = (ly / step_y_) + 1;
        int j = (lx / step_x_) + 1;
        map.set(i, j, 1.0);
    }
}

// DOUBLE STACK - NEW
// Double stack - ref_x, ref_y, cor_x, cor_y, pointer_prev
// Reference and corresponding image coordinates
bool ImageMatching::pop(double& x, double& y, double& sx, double& sy)
{
    if (stack_ != nullptr) {
        x = stack_->ref_x;
        y = stack_->ref_y;
        sx = stack_->cor_x;
        sy = stack_->cor_y;
        stackCell* aux = stack_;
        stack_ = stack_->prev;
        delete aux;
        return 1;
    }

    return 0;
}

bool ImageMatching::push(double x, double y, double sx, double sy)
{
    stackCell* aux = stack_;
    stack_ = new stackCell();
    stack_->ref_x = x;
    stack_->ref_y = y;
    stack_->cor_x = sx;
    stack_->cor_y = sy;
    stack_->prev = aux;
    return 1;
}

void ImageMatching::emptyStack()
{
    double x, y, sx, sy;

    while (pop(x, y, sx, sy)) {
    }
}

void ImageMatching::region_growing(Matrix* img1,
                                   Matrix* img2,
                                   MatchingPointsList* mpoints,
                                   double x,
                                   double y,
                                   double sx,
                                   double sy)
{
    int ncc_flag, lsm_flag;
    double lx, ly, rx, ry;
    double new_x = 0.0, new_y = 0.0, p = 0.0;
    emptyStack();
    push(x, y, sx, sy);

    while (pop(lx, ly, rx, ry)) {
        // Map coordinates
        int i = (static_cast<int>(ly) / step_y_) + 1;
        int j = (static_cast<int>(lx) / step_x_) + 1;

        // Check if current point is inside determined area
        if (lx < matching_xi_ || lx > matching_xf_ || ly < matching_yi_
                || ly > matching_yf_) {
            continue;
        }

        if (rx < smatching_xi_ || rx > smatching_xf_ || ry < smatching_yi_
                || ry > smatching_yf_) {
            continue;
        }

        // Check if visited
        if (static_cast<int>((map.get(i, j))) != 0) {
            continue;
        }

        // Choose correlation method
        // Normalized Cross Correlation  - Integer precision
        if (matching_method_ == NCC) {
            // Truncate reference coordinates
            ncc.setTemplateCenter(lx, ly);
            ncc.setSearchWindowCenter(rx, ry);
            ncc_flag = ncc.searchHomologous(img1, img2);
            p = ncc.getBestP();

            if (p < corr_th_ || ncc_flag != 1) {
                continue;
            }

            new_x = ncc.getBestX();
            new_y = ncc.getBestY();
            // Add 4-neighbor pixels - NCC - Faster if inside this 'if'
            // If new seed are based on new_x and new_y, may diverge !!
            push(lx + static_cast<double>(step_x_),
                 ly,
                 rx + static_cast<double>(step_x_),
                 ry);
            push(lx - static_cast<double>(step_x_),
                 ly,
                 rx - static_cast<double>(step_x_),
                 ry);
            push(lx,
                 ly + static_cast<double>(step_y_),
                 rx,
                 ry + static_cast<double>(step_y_));
            push(lx,
                 ly - static_cast<double>(step_y_),
                 rx,
                 ry - static_cast<double>(step_y_));
        }

        // Least Squares Matching - Double precision
        if (matching_method_ == LSM) {
            lsm_flag = lsm.searchHomologous(img1, img2, lx, ly, rx, ry);
            p = lsm.getBestP();

            if (p < corr_th_ || lsm_flag < 1) {
                continue;
            }

            new_x = lsm.getBestX();
            new_y = lsm.getBestY();
            // Add 4-neighbor pixels - LSM - Faster if inside this 'if'
            push(lx + static_cast<double>(step_x_), ly,
                 new_x + static_cast<double>(step_x_), new_y);
            push(lx - static_cast<double>(step_x_), ly,
                 new_x - static_cast<double>(step_x_), new_y);
            push(lx, ly + static_cast<double>(step_y_), new_x,
                 new_y + static_cast<double>(step_y_));
            push(lx, ly - static_cast<double>(step_y_), new_x,
                 new_y - static_cast<double>(step_y_));
        }

        // Add pair to list, converting to c++ coordinates system
        mpoints->add(left_image_id,
                     right_image_id,
                     static_cast<double>(lx - 1),
                     static_cast<double>(ly - 1),
                     new_x - 1.0, new_y - 1.0,
                     p);
        // Set visited
        map.set(i, j, 1.0);
        // Calculate coverage
        num_visited += 1.0;
        coverage = num_visited / max_size;

        if (manager_ == nullptr) {
            throw "%.2f %%\r";    // coverage*100;
        } else {
            manager_->setProgress(static_cast<int>(coverage * 100));
        }

        // If Region Growing is not selected, then perform
        // matching only once for each seed
        if ((cancel_flag_) || (!perform_RG)) {
            emptyStack();
            return;
        }
    }
}


}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
