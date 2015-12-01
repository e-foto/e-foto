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
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 18/08/2011
* @version 1.0 - Marcelo Teixeira Silveira
* @version 2.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class MatchingPointsList;

class DEMManager;
/*
// INTEGER STACK
// For integer stack usage, please check float stack and uncheck this class
// It takes 8 x sizeof(stackCell*) bytes for each object
class stackCell
{
public:
	stackCell() { prev = NULL; };
	int coord, scoord;
	stackCell *prev;
};
*/

// FLOAT STACK
// For float stack usage, please check integer stack and uncheck this class
// It takes 16 x sizeof(stackCell*) bytes for each object
class stackCell
{
public:
        stackCell() { prev = NULL; };
        float ref_x, ref_y, cor_x, cor_y;
        stackCell *prev;
};

class ImageMatching
{

public:
	ImageMatching();
    explicit ImageMatching(DEMManager *);
	enum matmet { NCC, LSM };
	enum eadmod { Equalization, HistMatching };
	void setMatchingMethod();
	LeastSquaresMatching* getLSM() { return &lsm; };
	NormalizedCrossCorrelation* getNCC() { return &ncc; };
	void setPerformRadiometric(bool _pr) { perform_readiometric = _pr; };
	void setMatchingMethod(int _mode) { matching_method = _mode % 2; };
	void setRadiometricMode(int _mode) { radiometric_mode = _mode % 2; };
	void setImageDepth(int _depth) { image_depth = _depth; };
	void performImageMatching(Matrix *, Matrix *, MatchingPointsList *, MatchingPointsList *);
	void setMatchingLimits(int, int, int, int);
	void setImagesIds(int _lid, int _rid) { left_image_id = _lid; right_image_id = _rid; };
	void setStep(int, int);
	void setCorrelationThreshold(double _th) { corr_th = _th; };
	double getCoverage() { return 100.0*coverage; };
	void setMinStd(double);
	void setElimanteBadPoints(bool _el) { elim_bad_pts = _el; };
	void setCancel() { cancel_flag = true; };
	Matrix & getMap() { return map; };
        double getElapsedTime() { return elap_time; };
        void setPerformRegionGrowing(bool _p_rg) { perform_RG = _p_rg; };

private:
        int image_depth;
	double coverage, max_size, num_visited;
	double corr_th;
	bool perform_readiometric, radiometric_mode;
	bool cancel_flag;
	int matching_xi, matching_yi, matching_xf, matching_yf;
	int smatching_xi, smatching_yi, smatching_xf, smatching_yf;
	int left_image_id, right_image_id;
	int step_x, step_y;
	int img_width, img_height, simg_width, simg_height;
	int matching_method;
	stackCell *stack, *aux;
	RadiometricTransformation rt;
	LeastSquaresMatching lsm;
	NormalizedCrossCorrelation ncc;
	Matrix map;
	DEMManager *manager;
	void init();
	void fillMap(MatchingPointsList *);
	bool elim_bad_pts;
        bool perform_RG;
        double elap_time;
        // INTEGER STACK
/*	bool pop(int&, int&, int&, int&);
        bool push(int,int,int,int);
        void region_growing(Matrix *, Matrix *, MatchingPointsList *, int x, int y, int sx, int sy); */
        // DOUBLE STACK
        bool pop(double&, double&, double&, double&);
        bool push(double,double,double,double);
        void region_growing(Matrix *, Matrix *, MatchingPointsList *, double x, double y, double sx, double sy);
        void emptyStack();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
