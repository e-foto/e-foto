/*******************************************************************************
                              ImageMatching.h
*******************************************************************************/

#ifndef IMAGE_MATCHING
#define IMAGE_MATCHING

#include <stdio.h>
#include <math.h>
#include "Matrix.h"
#include "NormalizedCrossCorrelation.h"
#include "LeastSquaresMatching.h"
#include "RadiometricTransformation.h"
#include "MatchingPoints.h"
#include "DEMManager.h"

/**
* class ImageMatching
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 18/08/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

class stackCell
{
public:
	stackCell() { prev = NULL; };
	int coord, scoord;
	stackCell *prev;
};

class ImageMatching
{

public:
        ImageMatching();
        ImageMatching(DEMManager *_dm) { manager = _dm; };
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

private:
	int image_depth;
	double coverage, max_size, num_visited;
	double corr_th;
	bool perform_readiometric, radiometric_mode;
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
	bool pop(int&, int&, int&, int&);
	bool push(int,int,int,int);
	void emptyStack();
	void region_growing(Matrix *, Matrix *, MatchingPointsList *, int x, int y, int sx, int sy);
        DEMManager *manager;
};

#endif
