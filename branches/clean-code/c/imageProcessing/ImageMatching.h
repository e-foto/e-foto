#ifndef IMAGE_MATCHING
#define IMAGE_MATCHING
/*******************************************************************************
                              ImageMatching.h
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
#include "RadiometricTransformation.h"


/**
* class ImageMatching
*
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class MatchingPointsList;

class DEMManager;
// FLOAT STACK
// For float stack usage, please check integer stack and uncheck this class
// It takes 16 x sizeof(stackCell*) bytes for each object
class stackCell {
 public:
    stackCell()
    {
        prev = NULL;
    };
    float ref_x, ref_y, cor_x, cor_y;
    stackCell* prev;
};

class ImageMatching {
 public:
    ImageMatching();
    explicit ImageMatching(DEMManager*);
    enum matmet { NCC, LSM };
    enum eadmod { Equalization, HistMatching };
    void setMatchingMethod();
    LeastSquaresMatching* getLSM()
    {
        return &lsm;
    };
    NormalizedCrossCorrelation* getNCC()
    {
        return &ncc;
    };
    void setPerformRadiometric(bool pr)
    {
        perform_readiometric_ = pr;
    };
    void setMatchingMethod(int mode)
    {
        matching_method_ = mode % 2;
    };
    void setRadiometricMode(int mode)
    {
        radiometric_mode_ = mode % 2;
    };
    void setImageDepth(int depth)
    {
        image_depth_ = depth;
    };
    void performImageMatching(Matrix*, Matrix*, MatchingPointsList*,
                              MatchingPointsList*);
    void setMatchingLimits(int, int, int, int);
    void setImagesIds(int lid, int rid)
    {
        left_image_id = lid;
        right_image_id = rid;
    };
    void setStep(int, int);
    void setCorrelationThreshold(double th)
    {
        corr_th_ = th;
    };
    double getCoverage()
    {
        return 100.0 * coverage;
    };
    void setMinStd(double);
    void setElimanteBadPoints(bool el)
    {
        elim_bad_pts_ = el;
    };
    void setCancel()
    {
        cancel_flag_ = true;
    };
    Matrix& getMap()
    {
        return map;
    };
    double getElapsedTime()
    {
        return elap_time_;
    };
    void setPerformRegionGrowing(bool p_rg)
    {
        perform_RG = p_rg;
    };

 private:
    DEMManager* manager_;
    Matrix map;
    int image_depth_;
    double coverage, max_size, num_visited;
    double corr_th_;
    bool perform_readiometric_, radiometric_mode_;
    bool cancel_flag_;
    int matching_xi_, matching_xf_, matching_yi_, matching_yf_;
    int smatching_xi, smatching_yi, smatching_xf, smatching_yf;
    int left_image_id, right_image_id;
    int step_x_, step_y_;
    int img_width, img_height, simg_width, simg_height;
    int matching_method_;
    stackCell* stack_;
    RadiometricTransformation rt;
    LeastSquaresMatching lsm;
    NormalizedCrossCorrelation ncc;
    bool elim_bad_pts_;
    bool perform_RG;
    double elap_time_;

    void fillMap(MatchingPointsList*);
    // DOUBLE STACK
    bool pop(double&, double&, double&, double&);
    bool push(double, double, double, double);
    void region_growing(Matrix*,
                        Matrix*,
                        MatchingPointsList*,
                        double x,
                        double y,
                        double sx,
                        double sy);
    void emptyStack();
};

}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br

#endif  // IMAGE_MATCHING
