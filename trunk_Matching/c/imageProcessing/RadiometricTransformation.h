/*******************************************************************************
                         RadiometricTransformation.h
*******************************************************************************/

#ifndef RADIOMETRIC_TRANSFORMATION
#define RADIOMETRIC_TRANSFORMATION

#include <stdio.h>
#include <math.h>
#include "Matrix.h"

/**
* class RadiometricTransformation
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 19/07/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

class RadiometricTransformation
{

public:
	int imadjust(Matrix *m, double in_min=0.0, double in_max=0.0, double out_min=0.0, double out_max=1.0);
	int histmatching(Matrix *m1, Matrix *m2, int levels=256);
	Matrix& imhist(Matrix *m, int levels=256);
	Matrix& imcdf(Matrix *m, int levels=256);
	int histeq(Matrix *m, int levels=256);
	int im2double(Matrix *m, int levels=256);

private:


};

#endif
