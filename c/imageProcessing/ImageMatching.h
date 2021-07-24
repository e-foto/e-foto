#ifndef IMAGE_MATCHING
#define IMAGE_MATCHING
/*******************************************************************************
                              ImageMatching.h
*******************************************************************************/

/*Copyright 2002-2021 e-foto team (UERJ)
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


namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class MatchingPointsList;

class DEMManager;

class stackCell {
public:
    stackCell() = default;
    float ref_x{0.0}, ref_y{0.0}, cor_x{0.0}, cor_y{0.0};
    stackCell* prev{nullptr};
};

enum class ImgRadioCorrection { NoCorrection, Equalization, HistMatching };
enum class MatchingMet { NCrossCorr, LeastSqMatch };

class ImageMatching {

public:
    ImageMatching();
    explicit ImageMatching(DEMManager*);


    void setMatchingMethod();
    LeastSquaresMatching* getLSM()
    {
        return &lsm;
    }
    NormalizedCrossCorrelation* getNCC()
    {
        return &ncc;
    }
    void setMatchingMethod(MatchingMet _mode)
    {
        matching_method = _mode;
    }
    void setRadiometricMode(ImgRadioCorrection _mode)
    {
        radiometric_mode = _mode;
    }
    void setImageDepth(int _depth)
    {
        image_depth = _depth;
    }
    void performImageMatching(Matrix*, Matrix*, MatchingPointsList*,
                              MatchingPointsList*);
    void setImagesIds(int _lid, int _rid)
    {
        left_image_id = _lid;
        right_image_id = _rid;
    }
    void setCorrelationThreshold(double _th)
    {
        corr_th = _th;
    }
    double getCoverage() const
    {
        return 100.0 * coverage;
    }
    void setMinStd(double);
    void setElimanteBadPoints(bool _el)
    {
        elim_bad_pts = _el;
    }
    void setCancel()
    {
        cancel_flag = true;
    }
    Matrix& getMap()
    {
        return map;
    }
    double getElapsedTime() const
    {
        return elap_time;
    }
    void setPerformRegionGrowing(bool _p_rg)
    {
        perform_RG = _p_rg;
    }

private:
    int image_depth{256};
    double coverage{0.0}, max_size, num_visited;
    double corr_th{0.7};
    ImgRadioCorrection radiometric_mode{ImgRadioCorrection::HistMatching};
    bool cancel_flag{false};
    int matching_xi, matching_yi, matching_xf, matching_yf;
    int smatching_xi{0};
    int smatching_yi{0};
    int smatching_xf{0};
    int smatching_yf{0};
    int left_image_id, right_image_id;
    int step_x{3};
    int step_y{3};
    int img_width, img_height, simg_width, simg_height;
    MatchingMet matching_method{MatchingMet::LeastSqMatch};
    stackCell* stack{nullptr};
    stackCell* aux{nullptr};
    RadiometricTransformation rt;
    LeastSquaresMatching lsm;
    NormalizedCrossCorrelation ncc;
    Matrix map;
    DEMManager* manager;
    void fillMap(MatchingPointsList*) const;
    bool elim_bad_pts{false};
    bool perform_RG;
    double elap_time{0.0};
    bool pop(double&, double&, double&, double&);
    bool push(double, double, double, double);
    void region_growing(Matrix*, Matrix*, MatchingPointsList*, double x, double y,
                        double sx, double sy);
    void emptyStack();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
