/*******************************************************************************
                        RadiometricTransformation.cpp
*******************************************************************************/

#include "RadiometricTransformation.h"
#include "Matrix.h"

/************************
 * IMAGE NORMALIZATION  *
 ************************/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/*
  Performs radiometric transformation adjusting the histogram to in_min and in_max values
  Returns 1 if successful
*/
int RadiometricTransformation::imadjust(Matrix *m, double in_min, double in_max, double out_min, double out_max)
{
    // If both in_min and in_max are signed as 0, calculate the min and max values
    if ((fabs(in_min - 0.0) < 0.0000000000001) && (fabs(in_max - 0.0) < 0.0000000000001))
    {
        in_min = m->lowestValue();
        in_max = m->highestValue();
    }

    // Check for inconsistencies
    if ((in_min >= in_max) || (out_min >= out_max))
        return 0;

    // Apply transformation
    double pixel, delta_in = in_max - in_min, delta_out = out_max - out_min;
    for (unsigned int i=1; i<= m->getRows(); i++)
    {
        for (unsigned int j=1; j<= m->getCols(); j++)
        {
            // Get pixel
            pixel = m->get(i,j);

            // Fit pixel to given space
            if (pixel < in_min)
                pixel = in_min;
            if (pixel < in_min)
                pixel = in_min;

            // Calculate new color
            pixel = out_min + (delta_out*(pixel - in_min))/delta_in;

            // Change pixel
            m->set(i,j,pixel);
        }
    }

    return 1;
}

/*
 *  Apply image histogramm equalization
 *  Image must be between 0 and 1
 *  Defaul image level is 256
 */
int RadiometricTransformation::histeq(Matrix *m, int levels)
{
    if (levels < 1)
        levels = 1;

    // Calculate s = T(r);
    Matrix s = imcdf(m,levels);

    //	// Debug
    //	for (unsigned int k=1; k<=levels; k++)
    //		printf("%d = %f\n",k-1,s.get(k,1));

    // Apply transformation
    double pixel;
    int pos;
    for (unsigned int i=1; i<= m->getRows(); i++)
    {
        for (unsigned int j=1; j<= m->getCols(); j++)
        {
            // Get pixel
            pixel = m->get(i,j);

            // Check
            if (pixel < 0.0)
                pixel = 0.0;
            if (pixel > 1.0)
                pixel = 1.0;

            // Calculate position in the histogram
            pos = round(pixel*double(levels-1)) + 1;

            // Apply s = T(r)
            m->set(i, j, s.get(pos,1));
        }
    }

    return 1;
}


/***********************
 * HISTOGRAM MATCHING  *
 ***********************/

/*
  Performs radiometric transformation adjusting the histogram from image m2 to the specified m1 histogram
  Returns 1 if successful
*/
int RadiometricTransformation::histmatching(Matrix *m1, Matrix *m2, int levels)
{
    Matrix S,V;

    // Model
    S = imcdf(m1,levels);

    // Working image
    V = imcdf(m2,levels);

    // Create mapping function
    Matrix map(levels,1);
    double p,q;
    for (int i=1; i<=levels; i++)
    {
        q = fabs(V.get(i,1) - S.get(1,1));
        p = 1;
        for (int k=2; k<=levels; k++)
        {
            if (fabs(V.get(i,1) - S.get(k,1)) < q)
            {
                q = fabs(V.get(i,1) - S.get(k,1));
                p = k;
            }
        }
        map.set(i, 1, (p-1)/(levels-1));
    }

    // Apply changes to the image m2
    for (unsigned int i=1; i<=m2->getRows(); i++)
    {
        for (unsigned int j=1; j<=m2->getCols(); j++)
            m2->set(i,j,map.get(int(m2->get(i,j)*(levels -1))+1,1));
    }

    return 1;
}


/*********************
 * BASIC OPERATIONS  *
 *********************/

/*
 *  Calculate image histogram
 *  Image must be between 0 and 1
 *  Defaul image level is 256
 */
Matrix& RadiometricTransformation::imhist(Matrix *m, int levels)
{
    Matrix *h = new Matrix(levels,1);

    if (levels<1)
        levels = 1;

    // Calculate histogram
    double pixel;
    int pos;
    for (unsigned int i=1; i<= m->getRows(); i++)
    {
        for (unsigned int j=1; j<= m->getCols(); j++)
        {
            // Get pixel
            pixel = m->get(i,j);

            // Check
            if (pixel < 0.0)
                pixel = 0.0;
            if (pixel > 1.0)
                pixel = 1.0;

            // Calculate position in the histogram
            pos = round(pixel*double(levels-1)) + 1;

            // Inform histogram
            h->set(pos, 1, h->get(pos,1)+1);
        }
    }

    //	// Debug
    //	for (unsigned int i=1; i<=levels; i++)
    //		printf("%d = %d\n",i-1,int(h->get(i,1)));

    return *h;
}

/*
 *  Calculate s = T(r) using CDF (Cumulative Distribution Function)
 *  Image must be between 0 and 1
 *  Defaul image level is 256
 */
Matrix& RadiometricTransformation::imcdf(Matrix *m, int levels)
{
    Matrix *s = new Matrix(levels,1);
    Matrix hist = imhist(m,levels);

    // Calculate s = T(r)
    double p, n = double(m->getRows()*m->getCols());
    for (int k=1; k<=levels; k++)
    {
        // Calculate cumulated probability p
        p = 0.0;
        for (int j=1; j<=k; j++)
            p += hist.get(j,1)/n;

        s->set(k,1,p);
    }

    return *s;
}

/* Methods into disuse:
 *
 /Convert image to 0 and 1 space
int RadiometricTransformation::im2double(Matrix *m, int levels)
{
    if (levels<1)
        levels = 1;

    double pixel;
    for (unsigned int i=1; i<= m->getRows(); i++)
    {
        for (unsigned int j=1; j<= m->getCols(); j++)
        {
            // Get pixel
            pixel = m->get(i,j);

            // Check
            if (pixel < 0.0)
                pixel = 0.0;
            if (pixel > double(levels-1))
                pixel = double(levels-1);

            // Calculate new value
            pixel = pixel / double(levels-1);

            // Set value
            m->set(i,j,pixel);
        }
    }
}
*/

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
