#include "RectSupport.h"

// Constructors and destructors
//

/**
 * This is the empty constructor, it does not allocate memory.
 * @param parNcols number of columns.
 * @param parNrows number of rows.
 * @return an RectSupport object.
 */
RectSupport::RectSupport(unsigned int parNcols, unsigned int parNrows)
{
    ncols=parNcols;
    nrows=parNrows;
}

/**
 * This is the destructor of the class RectSupport.
 * @return no returning parameter.
 */
RectSupport::~RectSupport()
{

}

// EObject methods
//

/**
 *
 */
string RectSupport::objectType(void)
{
    return "RectSupport";
}

// Other methods
//

/**
 * This member function returns the numeber of columns of the matrix.
 * @return Number of columns of the matrix.
 */
unsigned int RectSupport::getCols() const
{
    return ncols;
}

/**
 * This member function returns the number of lines of the matrix.
 * @return Number of lines of the matrix.
 */
unsigned int RectSupport::getRows() const
{
    return nrows;
}
