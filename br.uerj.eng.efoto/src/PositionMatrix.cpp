#include "PositionMatrix.h"



//Protected methods
//

/**
 * This method deletes the matrix in memory.
 */
void PositionMatrix::del()
{
    delete[] _Mat;
    _Mat = NULL;
    ncols = 0;
    nrows = 0;
    unit = "";
}

/**
 * This method allocates a matrix in memory, fills the values of its attributes and set its values to zero.
 * @param rows number of rows of the new matrix
 * @param cols number of columns of the new matrix
 */
void PositionMatrix::nw(const unsigned int rows, const unsigned int cols)
{
    _Mat = new double[rows * cols];               // Allocate matrix
    nrows = rows; ncols = cols;                   // Fill matrix' attributes.
    for(unsigned int i = 0; i < (rows * cols); i++) //set its elements to zero
        _Mat[i] = 0;
    unit = "";
}



//Constructors and destructors methods
//

/**
 * This is one version of the overloaded constructor of the class matrix. It invokes the constructor of RectSupport and creates matrices of Rows x Cols.
 * @param rows
 * @param cols
 * @return a new matrix object
 */
PositionMatrix::PositionMatrix(unsigned int dimensions, string newUnit):RectSupport(dimensions, 1)
{
    nw(dimensions, 1); // Allocate matrix and fill its attributes.
    unit = newUnit;
}

/**
 * This is one version of the overloaded constructor of the class matrix. It invokes the constructor of RectSupport and creates matrices of Rows x Cols.
 * @param rows
 * @param cols
 * @return a new matrix object
 */
PositionMatrix::PositionMatrix(const Matrix& anotherMatrix):RectSupport(anotherMatrix.getRows(), anotherMatrix.getCols())
{
    nw(anotherMatrix.getRows(), anotherMatrix.getCols());
    *this = anotherMatrix; // Allocate matrix and fill its attributes.
}

/**
 * This is one version of the overloaded constructor of the class matrix. It invokes the constructor of RectSupport and creates matrices of Rows x Cols.
 * @param rows
 * @param cols
 * @return a new matrix object
 */
PositionMatrix::PositionMatrix(const PositionMatrix& anotherMatrix):RectSupport(anotherMatrix.getRows(), anotherMatrix.getCols())
{
    nw(anotherMatrix.getRows(), anotherMatrix.getCols());
    *this = anotherMatrix; // Allocate matrix and fill its attributes.
}

/**
 * This is one version of the overloaded constructor of the class matrix. It invokes the constructor of RectSupport and creates default matrices of 0 x 0.
 * @return a new empty matrix object
 */
PositionMatrix::PositionMatrix():RectSupport(0, 0)
{
    _Mat = NULL;
}

/**
 * Destructor method of the class matrix.
 * @return
 */
PositionMatrix::~PositionMatrix()
{
    del();
}



//Special matrix dispositions methods
//

/**
 * This member function turns the current matrix object into a Rows x Cols filled with zeros.
 * @param rows number of rows of the zeros matrix.
 * @param cols number of columns of the zeros matrix.
 */
PositionMatrix& PositionMatrix::resize(unsigned int rows, unsigned int cols)
{
    PositionMatrix result;
    result.nw(rows, cols);
    for (unsigned int i = 1; (i <= result.nrows)&&(i <= nrows); i++)
        for (unsigned int j = 1; (j <= result.ncols)&&(j <= ncols); j++)
            result.set(i,j,get(i,j));
    *this = result;
    return *this;
}

/**
 * This member function turns the current matrix object into a Rows x Cols filled with zeros.
 * @param rows number of rows of the zeros matrix.
 * @param cols number of columns of the zeros matrix.
 */
PositionMatrix& PositionMatrix::zero()
{
    for(unsigned int i = 0; i < (nrows * ncols); i++)
        _Mat[i] = 0;
    return *this;
}



//File operations methods
//

/**
 * This member function Loads an emf, E-foto Matrix File, which is an ascii format, similarly pbm/pgm/ppm. The first line of the file contains the header, second line
 * the filename, third line the number of columns and rows of the matrix and fourth line its elements (ascii double) separated by spaces.
 *
 * Example of emf:
 *
 * E-foto project double Matrix Format
 * filename.emf
 * 24 7
 * 0.3  7e3  87.3  8.3   ...
 *
 * @param filename path + name of the file to be opened
 * @return integer flag:1 for successfull load and 0 for unsuccessfull
 */
int PositionMatrix::load(char* filename)
{//@CODE_63
    char str[256];
    unsigned int cols;
    unsigned int rows;

    ifstream arq(filename); // open the emf file
    if (!arq.fail())
    {
        arq.getline(str,255); // read file header
        if (!arq.fail())
        {
            arq.getline(str,255); // read filename
            if (!arq.fail())
            {
                arq >> cols;
                if (!arq.fail())
                {
                    arq >> rows;
                    if ((nrows != rows)||(ncols != cols))
                    {
                        del();
                        nw(rows, cols);
                    }
                    for (unsigned int i=0; (!arq.eof())||(!arq.fail())||(i < (cols * rows)); i++)
                    {
                        arq >> _Mat[i]; // read one matrix element
                    }
                    arq.close();
                    return 1;
                }
            }

        }
    }
    return 0;
}

/**
 * This method saves a emf E-foto Matrix File which is an ascii format, similarly to pbm/pgm/ppm. The first line of the file contains the  header, second line
 * the filename, third line the number of columns and rows of the matrix and fourth line its elements (ascii double) separated by spaces.
 *
 * Example of emf:
 *
 * E-foto project double Matrix Format
 * filename.emf
 * 24 7
 * 0.3  7e3  87.3  8.3   ...
 *
 * @param filename path + name of the file to be saved
 * @return integer flag: 1 for successfull save and 0 for unsuccessfull
 */
int PositionMatrix::save(char* filename)
{
    ofstream emfile(filename); // open the emf file
    if (!emfile.fail())
    {
        emfile << "E-foto project double PositionMatrix Format" << endl; // write file header
        if (!emfile.fail())
        {
            emfile << filename << endl; // write filename
            if (!emfile.fail())
            {
                emfile << ncols << " " << nrows << endl;
                if (!emfile.fail())
                {
                    for (unsigned int i = 0; (i < (ncols * nrows)); i++)
                    {
                        emfile << _Mat[i] << " "; // write one matrix element
                    }
                    emfile.close();
                    return 1;
                }

            }
        }
    }
    return 0;
}



//Recuperate elements methods
//

/**
 * This method returns the value of the element i,j of the matrix.
 * @param i row index
 * @param j column index
 * @return Matrix [i,j]
 */
double PositionMatrix::get(const unsigned int i, const unsigned int j) const
{
    if ((i >= 1)&&(i <= nrows)&&(j >= 1)&&(j <= ncols))
        return _Mat[(i-1) * ncols + j - 1];
    else
        cerr << "i, j values out of the range of the matrix." << endl;
    return 0;
}

/**
 * This method returns the value of the element i,j of the matrix cast in int.
 * @param i row index
 * @param j column index
 * @return (int) Matrix [i,j]
 */
int PositionMatrix::getInt(const unsigned int i, const unsigned int j) const
{
    return (int) get(i,j);
}

/**
 * This method
 */
string PositionMatrix::getUnit() const
{
    return unit;
}

/**
 * This method return one selected submatrix of the matrix represented by the current object.
 * @param FirstRow first row of the selection
 * @param LastRow  last row of the selection
 * @param FirstCol first column of the selection
 * @param LastCol  last column of the selection
 * @return the submatrix Matrix mxn, m = (LastRow - FirstRow) and n = (LastCol - FirstCol)
 */
PositionMatrix PositionMatrix::sel(const unsigned int FirstRow, const unsigned int LastRow,
                                   const unsigned int FirstCol, const unsigned int LastCol)
{
    PositionMatrix DraftResult;
    PositionMatrix Result;
    if((FirstRow > LastRow)||(FirstCol > LastCol)||(FirstRow < 1)||(FirstCol < 1)||(LastRow > nrows)||(LastCol > ncols))
    {
        cerr << "Error detected by the Matrix.sel() method:" << endl << "Input parameters out of range or incorrect."<<endl;
        return Result;
    }
    if ( (DraftResult.nrows != (LastRow-FirstRow+1)) || (DraftResult.ncols != (LastCol-FirstCol+1)) )
    {
        DraftResult.del();
        DraftResult.nw( (unsigned int)(LastRow-FirstRow+1), (unsigned int) (LastCol-FirstCol+1) );
    }
    for (unsigned int i = 1; i <= (LastRow-FirstRow+1); i++)
        for (unsigned int j = 1; j <= (LastCol-FirstCol+1); j++)
            DraftResult.set(i, j, get((FirstRow + i - 1),(FirstCol + j -1)));

    Result = DraftResult;
    Result.unit = unit;
    return Result;
}

/**
 * This method displays on the default output stream the contents of the current matrix.
 * @bug Won't work under Windows XP OS.
 */
void PositionMatrix::show()
{
    cout << "PositionMatrix " << nrows << 'x' << ncols << " (" << unit << ")" << endl;
    cout.setf(ios::fixed | ios::scientific);
    for (unsigned int i = 1; i <= nrows; i++)
    {
        for (unsigned int j = 1; j <= ncols; j++)
            //printf("%10.1f ", get((unsigned int) i, (unsigned int) j));
            cout << setw(9) << setprecision(3) <<  get((unsigned int) i, (unsigned int) j) << " ";
        cout << endl;
    }
    cout.unsetf(ios::fixed | ios::scientific);
    cout << endl;
}

//Change elements methods
//

/**
 * This method changes the value of the element (i,j) of the matrix.
 * @param i row index
 * @param j column index
 * @param value new value of the matrix (i,j) element
 */
void PositionMatrix::set(unsigned int i, unsigned int j, double value) const
{
    if ((i >= 1)&&(i <= nrows)&&(j >= 1)&&(j <= ncols))
        _Mat[(i-1) * ncols + j - 1]=value;
    else
        cerr << "i, j values out of the range of the matrix." << endl;
}

/**
 * This method changes the value of the element (i,j) of the matrix.
 * @param i row index
 * @param j column index
 * @param value new value of the matrix (i,j) element
 */
void PositionMatrix::set(unsigned int i, double value) const
{
    set(i,1, value);
}

/**
 * This method
 */
void PositionMatrix::setInt(unsigned int i, unsigned int j, int value) const
{
    set(i,j, (double) value);
}

/**
 * This method
 */
void PositionMatrix::setInt(unsigned int i, int value) const
{
    set(i,1, (double) value);
}

/**
 * This method
 */
void PositionMatrix::setUnit(string newUnit)
{
    unit = newUnit;
}



//Overloaded operators
//

/**
 * The operator & horizontally concatenates two matrices. The matrices must have the same number of rows which is the same number of rows of the resulting matrix.
 * The number of columns of the resulting matrix is equal to the sum of the columns of both input matrices.
 * @param Par_Matrix matrix to be concatenated to the current scope matrix.
 * @return the resulting matrix
 */
PositionMatrix PositionMatrix::operator &(const PositionMatrix& Par_Matrix)
{//@CODE_141
    PositionMatrix DraftResult;
    PositionMatrix Result;
    if (Par_Matrix.nrows!=nrows)
    {
        cerr << "Error detected by the & operator:" << endl<< "Both matrixes must have the same number of rows." << endl;
    }
    else
    {
        if ((DraftResult.nrows*DraftResult.ncols)==(nrows*(ncols+Par_Matrix.ncols))) // If total size of the resulting matrix is correct
        {
            DraftResult.nrows=nrows;                                // Just update the dimensions
            DraftResult.ncols=ncols+Par_Matrix.ncols;
        }
        else
        {
            DraftResult.del();
            DraftResult.nw(nrows, ncols+Par_Matrix.ncols);
        }
        for (unsigned int i=1; i<=(nrows); i++)
        {
            for (unsigned int j=1; j<=(ncols); j++)
                DraftResult.set((unsigned int)i,(unsigned int)j,get((unsigned int)i,(unsigned int)j));
            for (unsigned int j=1; j<=(Par_Matrix.ncols); j++)
                DraftResult.set((unsigned int)i,(unsigned int)(j+ncols),Par_Matrix.get((unsigned int)i,(unsigned int)j));
        }
        Result = DraftResult;
        Result.unit = unit;
        return Result;
    }
    return Result;
}

/**
 * The operator | vertically concatenates two matrices. The matrices must have the same number of cols which is the same number of cols of the resulting matrix.
 * The number of rows of the resulting matrix is equal to the sum of the rows of both input matrices.
 * @param Par_Matrix matrix to be concatenated to the current scope matrix.
 * @return the resulting matrix
 */
PositionMatrix PositionMatrix::operator |(const PositionMatrix& Par_Matrix)
{//@CODE_141
    PositionMatrix Result;

    if (Par_Matrix.ncols!=ncols)
    {
        cerr << "Error detected by the | operator:" << endl<< "Both matrixes must have the same number of cols." << endl;
    }
    else
    {
        PositionMatrix DraftResult;
        DraftResult.resize(nrows+Par_Matrix.nrows, ncols);

        for (unsigned int j=1; j<=(ncols); j++)
        {
            for (unsigned int i=1; i<=(nrows); i++)
                DraftResult.set((unsigned int)i,(unsigned int)j,get((unsigned int)i,(unsigned int)j));
            for (unsigned int i=1; i<=(Par_Matrix.nrows); i++)
                DraftResult.set((unsigned int)(i+nrows),(unsigned int)j,Par_Matrix.get((unsigned int)i,(unsigned int)j));
        }
        Result = DraftResult;
        Result.unit = unit;
    }

    return Result;
}

/**
 * This method implements the assignment operator, making the matrix in the current scope equal to the matrix on the right of the operator =
 * @param Par_Matrix
 */
PositionMatrix& PositionMatrix::operator =(const Matrix& Par_Matrix)
                                          {
    if ((nrows!=Par_Matrix.nrows)||(ncols!=Par_Matrix.ncols))
    {
        del();
        nw(Par_Matrix.nrows,Par_Matrix.ncols);
    }
    for (unsigned int i = 0; i < (nrows*ncols); i++)
    {
        _Mat[i]=Par_Matrix._Mat[i];
    }
    unit = Par_Matrix.unit;
    return *this;
}

/**
 * This method implements the assignment operator, making the matrix in the current scope equal to the matrix on the right of the operator =
 * @param Par_Matrix
 */
PositionMatrix& PositionMatrix::operator =(const PositionMatrix& Par_Matrix)
                                          {
    if ((nrows!=Par_Matrix.nrows)||(ncols!=Par_Matrix.ncols))
    {
        del();
        nw(Par_Matrix.nrows,Par_Matrix.ncols);
    }
    for (unsigned int i = 0; i < (nrows*ncols); i++)
    {
        _Mat[i]=Par_Matrix._Mat[i];
    }
    unit = Par_Matrix.unit;
    return *this;
}

/**
 * This method implements the equality operator, return 1 for equal matrix and 0 for inequal matrix.
 * @param Par_Matrix
 */
bool PositionMatrix::operator ==(const Matrix& Par_Matrix)
{
    if ((nrows!=Par_Matrix.nrows)||(ncols!=Par_Matrix.ncols))
        return 0;
    else
        for (unsigned int i = 0; i < (ncols*nrows); i++)
            if (_Mat[i] - Par_Matrix._Mat[i] > 0.00000001 || _Mat[i] - Par_Matrix._Mat[i] < -0.00000001)
                return 0;
    return 1;
}

/**
 * This method implements the equality operator, return 1 for equal matrix and 0 for inequal matrix.
 * @param Par_Matrix
 */
bool PositionMatrix::operator ==(const PositionMatrix& Par_Matrix)
{
    if ((nrows!=Par_Matrix.nrows)||(ncols!=Par_Matrix.ncols))
        return 0;
    else
        for (unsigned int i = 0; i < (ncols*nrows); i++)
            if (_Mat[i] - Par_Matrix._Mat[i] > 0.00000001 || _Mat[i] - Par_Matrix._Mat[i] < -0.00000001)
                return 0;
    return 1;
}

/**
 * This method implements the inequality operator, return 1 for inequal matrix and 0 for equal matrix.
 * @param Par_Matrix
 */
bool PositionMatrix::operator !=(const Matrix& Par_Matrix)
{
    if ((nrows!=Par_Matrix.nrows)||(ncols!=Par_Matrix.ncols))
        return 1;
    else
        for (unsigned int i = 0; i < (ncols*nrows); i++)
            if (_Mat[i] - Par_Matrix._Mat[i] > 0.00000001 || _Mat[i] - Par_Matrix._Mat[i] < -0.00000001)
                return 1;
    return 0;
}

/**
 * This method implements the inequality operator, return 1 for inequal matrix and 0 for equal matrix.
 * @param Par_Matrix
 */
bool PositionMatrix::operator !=(const PositionMatrix& Par_Matrix)
{
    if ((nrows!=Par_Matrix.nrows)||(ncols!=Par_Matrix.ncols))
        return 1;
    else
        for (unsigned int i = 0; i < (ncols*nrows); i++)
            if (_Mat[i] - Par_Matrix._Mat[i] > 0.00000001 || _Mat[i] - Par_Matrix._Mat[i] < -0.00000001)
                return 1;
    return 0;
}

// EObject methods
//

/**
 *
 */
string PositionMatrix::objectType(void)
{
    return "PositionMatrix";
}

/**
 *
 */
string PositionMatrix::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool PositionMatrix::is(string s)
{
    return (s == "PositionMatrix" ? true : false);
}

// XML methods
//

/**
 *
 */
string PositionMatrix::xmlGetData()
{
    stringstream result;
    result << "<gml:pos>" << this->toGmlPosFormat() << "</gml:pos>\n";
    return result.str();
}

/**
 *
 */
void PositionMatrix::xmlSetData(string xml)
{
    EDomElement root(xml);
    deque<double> values = root.toGmlPos();
    resize(values.size());
    for (unsigned int i = 0; i < values.size(); i++)
        set(i, values.at(i));
}


//Other methods
//

/**
 * This method
 */
PositionMatrix PositionMatrix::toDiagonal()
{
    PositionMatrix Result;
    if ((ncols == 1)||(nrows == 1))
    {
        Result.resize(ncols*nrows, ncols*nrows);
        for (unsigned int i = 1; i <= nrows; i++)
            for(unsigned int j = 1; j <= ncols; j++)
                Result.set(i*j, i*j, get(i,j));
    }
    //Aqui pode ser incluido um else que revele um erro nessa operação que pede um vetor como entrada.
    Result.unit = unit;
    return Result;
}

/**
 * This method
 */
string PositionMatrix::toGmlPosFormat()
{
    stringstream oss;
    for (unsigned int i = 1; i <= nrows; i++)
        for (unsigned int j = 1; j <= ncols; j++)
        {
        oss << doubleToString(get((unsigned int) i, (unsigned int) j));
        if ( !( ( i == nrows) && (j == ncols) ) )
            oss << " ";
    };
    return oss.str();
}
