/*******************************************************************************
                         RadiometricTransformation.h
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
#ifndef RADIOMETRIC_TRANSFORMATION
#define RADIOMETRIC_TRANSFORMATION

/**
* class RadiometricTransformation
*
* @author E-Foto group
*
* * * * * * * * * * * *
* @date 19/07/2011
* @version 1.0 - Marcelo Teixeira Silveira
*/

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Matrix;

class RadiometricTransformation
{

public:
    int imadjust(Matrix *m, double in_min=0.0, double in_max=0.0, double out_min=0.0, double out_max=1.0);
    int histmatching(Matrix *m1, Matrix *m2, int levels=256);
    Matrix& imhist(Matrix *m, int levels=256);
    Matrix& imcdf(Matrix *m, int levels=256);
    int histeq(Matrix *m, int levels=256);

    /* Method into disuse:
    int im2double(Matrix *m, int levels=256);
    */
private:


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif
