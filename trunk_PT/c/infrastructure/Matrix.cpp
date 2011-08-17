#include "Matrix.h"

//Protected methods
//

/**
 * This method deletes the matrix in memory.
 */
void Matrix::del()
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
void Matrix::nw(const unsigned int rows, const unsigned int cols)
{
    Matrix::_Mat = new double[rows * cols];       // Allocate matrix
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
Matrix::Matrix(unsigned int rows, unsigned int cols):RectSupport(rows, cols)
{
    nw(rows, cols); // Allocate matrix and fill its attributes.
}

/**
 * This is one version of the overloaded constructor of the class matrix. It invokes the constructor of RectSupport and creates matrices of Rows x Cols.
 * @param rows
 * @param cols
 * @return a new matrix object
 */
Matrix::Matrix(const PositionMatrix& anotherMatrix):RectSupport(anotherMatrix.getRows(), anotherMatrix.getCols())
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
Matrix::Matrix(const Matrix& anotherMatrix):RectSupport(anotherMatrix.getRows(), anotherMatrix.getCols())
{
    nw(anotherMatrix.getRows(), anotherMatrix.getCols());
    *this = anotherMatrix; // Allocate matrix and fill its attributes.
}

/**
 * This is one version of the overloaded constructor of the class matrix. It invokes the constructor of RectSupport and creates default matrices of 0 x 0.
 * @return a new empty matrix object
 */
Matrix::Matrix():RectSupport(0, 0)
{
    _Mat = NULL;
    unit = "";
}

/**
 * Destructor method of the class matrix.
 * @return
 */
Matrix::~Matrix()
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
Matrix& Matrix::resize(unsigned int rows, unsigned int cols)
{
    Matrix result;
    result.nw(rows, cols);
    for (unsigned int i = 1; (i <= result.nrows)&&(i <= nrows); i++)
        for (unsigned int j = 1; (j <= result.ncols)&&(j <= ncols); j++)
            result.set(i,j,get(i,j));
    *this = result;
    return *this;
}

/**
 * Create an identity matrix of any given size.
 * @param size number of columns and rows of the identity matrix
 */
Matrix& Matrix::identity(unsigned int size)
{
    if ((size != ncols)||(size != nrows))
    {
        del();
        nw(size, size);
    }
    for(unsigned int i = 0; i < (size * size); i++) // creates an zeros matrix
        _Mat[i] = 0;
    for(unsigned int i = 1; i <= (size); i++)       // assigns ones to the main diagolnal elements.
        set((unsigned int) i, (unsigned int) i, 1);
    return *this;
}

/**
 * This member function turns the current matrix object into a Rows x Cols filled with ones.
 * @param rows number of rows of the ones matrix.
 * @param cols number of columns of the ones matrix.
 */
Matrix& Matrix::ones()
{
    for(unsigned int i = 0; i < (nrows * ncols); i++)
        _Mat[i] = 1;
    return *this;
}

/**
 * This member function turns the current matrix object into a Rows x Cols filled with zeros.
 * @param rows number of rows of the zeros matrix.
 * @param cols number of columns of the zeros matrix.
 */
Matrix& Matrix::zero()
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
int Matrix::load(char* filename)
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
int Matrix::save(char* filename)
{
    ofstream emfile(filename); // open the emf file
    if (!emfile.fail())
    {
        emfile << "E-foto project double Matrix Format" << endl; // write file header
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
double Matrix::get(const unsigned int i, const unsigned int j) const
{
    if ((i >= 1)&&(i <= nrows)&&(j >= 1)&&(j <= ncols))
        return _Mat[(i-1) * ncols + j - 1];
    else
        cerr << "Get ["<< intToString(i) << ","<< intToString(j) << "] values out of the range of the matrix." << endl;
    return 0;
}

/**
 * This method returns the value of the element i,j of the matrix cast in int.
 * @param i row index
 * @param j column index
 * @return (int) Matrix [i,j]
 */
int Matrix::getInt(const unsigned int i, const unsigned int j) const
{
    return (int) get(i,j);
}

/**
 * This method
 */
string Matrix::getUnit()
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
Matrix Matrix::sel(const unsigned int FirstRow, const unsigned int LastRow,
                   const unsigned int FirstCol, const unsigned int LastCol)
{
    Matrix DraftResult(1, 1);
    Matrix Result;
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
/*
void Matrix::show()
{
    cout << "Matrix " << nrows << 'x' << ncols << endl;
    cout.setf(ios::fixed | ios::scientific);
    for (unsigned int i = 1; i <= nrows; i++)
    {
        for (unsigned int j = 1; j <= ncols; j++)
            //printf("%10.1f ", get((unsigned int) i, (unsigned int) j));
            cout << setw(15) << setprecision(8) <<  get((unsigned int) i, (unsigned int) j) << " ";
        cout << endl;
    }
    cout.unsetf(ios::fixed | ios::scientific);
    cout << endl;
}*/

void Matrix::show(char mode, int precision, char *name)
{
	cout << "Matrix " << name << " ["<<nrows << 'x' << ncols << "] "<<endl;
	if (mode=='f')
		cout.setf(ios::fixed);
	if (mode=='e')
		cout.setf(ios::scientific);
	for (unsigned int i = 1; i <= nrows; i++)
	{
		for (unsigned int j = 1; j <= ncols; j++)
			//printf("%10.1f ", get((unsigned int) i, (unsigned int) j));
			cout << setw(15) << setprecision(precision) <<  get((unsigned int) i, (unsigned int) j) << " ";
		cout << endl;
	}
	if (mode=='f')
		cout.unsetf(ios::fixed);
	if (mode=='e')
		cout.unsetf(ios::scientific);

	cout << endl;
}

/**
 * This method returns the highest value of the elements in matrix.
 * @return highest value in matrix
 */
double Matrix::highestValue() const
{
    double Result = NAN;
    if (_Mat != NULL)
    {
        Result = _Mat[0];
        for (unsigned int i = 1; i < (ncols*nrows); i++)
            if (_Mat[i] > Result)
                Result = _Mat[i];
    }
    return Result;
}

/**
 * This method returns the lowest value of the elements in matrix.
 * @return lowest value in matrix
 */
double Matrix::lowestValue() const
{
    double Result = NAN;
    if (_Mat != NULL)
    {
        Result = _Mat[0];
        for (unsigned int i = 1; i < (ncols*nrows); i++)
            if (_Mat[i] < Result)
                Result = _Mat[i];
    }
    return Result;
}



//Change elements methods
//

/**
 * This method changes the value of the element (i,j) of the matrix.
 * @param i row index
 * @param j column index
 * @param value new value of the matrix (i,j) element
 */
void Matrix::set(unsigned int i, unsigned int j, double value) const
{

    if ((i >= 1)&&(i <= nrows)&&(j >= 1)&&(j <= ncols))
        _Mat[(i-1) * ncols + j - 1]=value;
    else
        cerr << "Set ["<< intToString(i) << ","<< intToString(j) << "] values out of the range of the matrix." << endl;
}

/**
 * This method
 */
void Matrix::setInt(unsigned int i, unsigned int j, int value) const
{
    set(i,j, (double) value);
}

/**
 * This method
 */
void Matrix::setUnit(string newUnit)
{
    unit = newUnit;
}



//Verifications methods
//

/**
 * This member function checks if the matrix is identity.
 * @return boolean flag: 1 for identity and 0 for non identity
 */
bool Matrix::isIdentity()
{
    Matrix I;
    if (getCols() != getRows())
        return false;
    I.identity(getCols());
    if (*this != I)
        return false;
    return true;
}

/**
 * This member function checks if the matrix is unitriangular.
 * @return boolean flag: 1 for unitriangular and 0 for non unitriangular
 */
bool Matrix::isTriang()
{
	for (unsigned int j = 1; j <= ncols; j++)
	{
		if ((1-get((unsigned int)j,(unsigned int)j) > 0.000001 || 1-get((unsigned int)j,(unsigned int)j) < -0.000001))
			return false;
		for (unsigned int i = (j+1); i <= nrows; i++)
			if (get((unsigned int)i,(unsigned int)j) > 0.000001 || get((unsigned int)i,(unsigned int)j) < -0.000001)
				return false;
	}
	return true;
}

/**
 * This member function checks if the matrix is zeroes composed.
 * @return boolean flag: 1 for zeroes composed and 0 for non zeroes composed
 */
bool Matrix::isZeroes()
{
	for (unsigned int j = 1; j <= ncols; j++)
	{
		for (unsigned int i = 1; i <= nrows; i++)
			if (get((unsigned int)i,(unsigned int)j) > 0.000001 || get((unsigned int)i,(unsigned int)j) < -0.000001)
				return false;
	}
	return true;
}



//Overloaded operators
//

/**
 * The operator & horizontally concatenates two matrices. The matrices must have the same number of rows which is the same number of rows of the resulting matrix.
 * The number of columns of the resulting matrix is equal to the sum of the columns of both input matrices.
 * @param Par_Matrix matrix to be concatenated to the current scope matrix.
 * @return the resulting matrix
 */
Matrix Matrix::operator &(const Matrix& Par_Matrix)
{//@CODE_141
    Matrix DraftResult;
    Matrix Result;
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
Matrix Matrix::operator |(const Matrix& Par_Matrix)
{//@CODE_141
    Matrix Result;

    if (Par_Matrix.ncols!=ncols)
    {
        cerr << "Error detected by the | operator:" << endl<< "Both matrixes must have the same number of cols." << endl;
    }
    else
    {
        Matrix DraftResult(nrows+Par_Matrix.nrows, ncols);

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
 * This method implements the mathemetical addition of matrices, the matrix on the left of the + is the object in the current scope.
 * @param Par_Matrix is the matrix on the right of + operator
 * @return the resulting of the addition operation
 */
Matrix Matrix::operator +(const Matrix& Par_Matrix)
{
    Matrix DraftResult(Par_Matrix.nrows, Par_Matrix.ncols);
    Matrix Result;
    if ((Par_Matrix.nrows!=nrows)||(Par_Matrix.ncols!=ncols) )
    {
        cerr << "Error detected by the + operator: Both matrixes must have the same dimensions." << endl;
    }
    else
    {
        if ((DraftResult.nrows*DraftResult.ncols)==(nrows*ncols))
        {
            DraftResult.nrows=nrows;
            DraftResult.ncols=ncols;
        }
        else
        {
            DraftResult.nw(nrows, ncols);
        }
        for (unsigned int i=0; i<(nrows*ncols); i++)
        {
            DraftResult._Mat[i]=_Mat[i] + Par_Matrix._Mat[i];
        }
        Result = DraftResult;
        Result.unit = unit;
        return Result;
    }
    return Result;
}

/**
 * This method implements the mathemetical subtraction of matrices, the matrix on the left of the - is the object in the current scope.
 * @param Par_Matrix is the matrix on the right of operator -
 * @return the resulting of the matrices subtraction.
 */
Matrix Matrix::operator -(const Matrix& Par_Matrix)
{
    Matrix DraftResult(Par_Matrix.nrows, Par_Matrix.ncols);
    Matrix Result;
    if ((Par_Matrix.nrows!=nrows)||(Par_Matrix.ncols!=ncols))
    {
        cerr << "Error detected by the - operator: Both matrixes must have the same dimensions." << endl;
    }
    else
    {
        if ((DraftResult.nrows*DraftResult.ncols)==(nrows*ncols) )
        {
            DraftResult.nrows=nrows;
            DraftResult.ncols=ncols;
        }
        else
        {
            DraftResult.nw(nrows, ncols);
        }
        for (unsigned int i=0; i<(nrows*ncols); i++)
        {
            DraftResult._Mat[i]=_Mat[i] - Par_Matrix._Mat[i];
        }
        Result = DraftResult;
        Result.unit = unit;
        return Result;
    }
    return Result;
}

/**
 * This method implements the mathemetical multiplication of matrices, the matrix on the left of the * is the object in the current scope.
 * @param Par_Matrix is the matrix on the right of the operator *
 * @return the result of the matrices multiplication
 */
Matrix Matrix::operator *(const Matrix& Par_Matrix)
{
    Matrix Result;
    Matrix *DraftResult = new Matrix;
    DraftResult->resize(nrows, Par_Matrix.ncols).zero();

    if (ncols!=Par_Matrix.nrows)
    {
        cerr << "Matrices dimensions are imcompatible." << endl;
		//system("PAUSE");
        exit (-1);
    }
    else
    {
        //Allocates matrix
        for (unsigned int i = 1; i <=DraftResult->nrows; i++)
        {
            for (unsigned int j= 1;j<=DraftResult->ncols; j++)
            {
                double sum=0;
                for (unsigned int k=1; k <= ncols; k++)
                {
                    sum+=(get(i,k)*Par_Matrix.get(k,j));
                } // calculates one element of the result
                DraftResult->set(i,j,sum);
            }
        }

        Result = *DraftResult;
        Result.unit = unit;
        delete DraftResult;
        return Result;
    }
}

/**
 * This method implements the addition of a matrix and a scalar.
 * @param Par_Scalar is the scalar value to be added to the matrix.
 * @return The result of the addition of the Matrix in the current scope and Par_Scalar.
 */
Matrix Matrix::operator +(double Par_Scalar)
{
    Matrix DraftResult(nrows, ncols);
    Matrix Result(nrows, ncols);
    for (unsigned int i = 1; i <= Result.nrows; i++)
    {
        for (unsigned int j= 1;j<=Result.ncols; j++)
        {
            DraftResult.set(i,j,this->get(i,j)+Par_Scalar);
        }
    }
    Result = DraftResult;
    Result.unit = unit;
    return Result;
}

/**
 * This method implements the subtraction of a matrix by a scalar.
 * @param Par_Scalar is the scalar value to be subtracted to the matrix.
 * @return The result of the subtraction of the Matrix in the current scope by Par_Scalar.
 */
Matrix Matrix::operator -(double Par_Scalar)
{
    Matrix DraftResult(nrows, ncols);
    Matrix Result(nrows, ncols);
    for (unsigned int i = 1; i <= Result.nrows; i++)
    {
        for (unsigned int j= 1;j<= Result.ncols; j++)
        {
            DraftResult.set(i,j,(this->get(i,j)-Par_Scalar));
        }
    }
    Result = DraftResult;
    Result.unit = unit;
    return Result;
}

/**
 * This method implements the multiplication of a matrix by a scalar.
 * @param Par_Scalar is the scalar value by which the matrix will be multiplied.
 * @return The result of the multiplication of the Matrix in the current scope by Par_Scalar.
 */
Matrix Matrix::operator *(double Par_Scalar)
{
    Matrix DraftMatrix (nrows, ncols);
    Matrix Result(nrows, ncols);
    for (unsigned int i = 1; i <= DraftMatrix.nrows; i++)
    {
        for (unsigned int j= 1;j<= DraftMatrix.ncols; j++)
        {
            DraftMatrix.set(i,j,this->get(i,j)*Par_Scalar);
        }
    }
    Result = DraftMatrix;
    Result.unit = unit;
    return Result;
}

/**
 * This method implements the division of a matrix by a scalar.
 * @param Par_Scalar is the scalar value by which the matrix will be divided.
 * @return The result of the division of the Matrix in the current scope by Par_Scalar.
 */
Matrix Matrix::operator /(double Par_Scalar)
{
    Matrix DraftResult(nrows, ncols);
    Matrix Result(nrows, ncols);
    for (unsigned int i = 1; i <= Result.nrows; i++)
    {
        for (unsigned int j= 1;j<= Result.ncols; j++)
        {
            DraftResult.set(i,j,this->get(i,j)/Par_Scalar);
        }
    }
    Result = DraftResult;
    Result.unit = unit;
    return Result;
}

/**
 * This method implements the assignment operator, making the matrix in the current scope equal to the matrix on the right of the operator =
 * @param Par_Matrix
 */
Matrix& Matrix::operator =(const PositionMatrix& Par_Matrix)
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
Matrix& Matrix::operator =(const Matrix& Par_Matrix)
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
bool Matrix::operator ==(const PositionMatrix& Par_Matrix)
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
bool Matrix::operator ==(const Matrix& Par_Matrix)
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
bool Matrix::operator !=(const PositionMatrix& Par_Matrix)
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
bool Matrix::operator !=(const Matrix& Par_Matrix)
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
string Matrix::objectType(void)
{
    return "Matrix";
}

/**
 *
 */
string Matrix::objectAssociations(void)
{
    return "";
}

/**
 *
 */
bool Matrix::is(string s)
{
    return (s == "Matrix" ? true : false);
}

// XML methods
//

/**
 * 
 */
string Matrix::xmlGetData()
{
    stringstream result;
    result << "<mml:matrix>\n";
    for (unsigned int i = 1; i <= getRows(); i++)
    {
        result << "<mml:matrixrow>\n";
        for (unsigned int j = 1; j <= getCols(); j++)
            result << "<mml:cn>" << doubleToString(get(i, j)) << "</mml:cn>\n";
        result << "</mml:matrixrow>\n";
    }
    result << "</mml:matrix>\n";
    return result.str();
}

/**
 * 
 */
void Matrix::xmlSetData(string xml)
{
    EDomElement root(xml);

    deque<EDomElement> matRows, matCols;

    matRows = root.children();
    if (matRows.size() == 0)
        return;
    matCols = matRows.at(0).children();

    resize(matRows.size(), matCols.size());

    for (unsigned int i = 0; i < matRows.size(); i++)
    {
        matCols = matRows.at(i).children();
        for (unsigned int j = 0; j < matCols.size(); j++)
            set(i + 1, j + 1, matCols.at(j).toDouble());
    }
}

//Other methods
//

/**
 * This method calculates the inverse matrix using the Gauss-Jordan Elimination.
 * @return The inverse of the matrix in the current scope
 */
Matrix Matrix::inverse()
{
    Matrix Inv;
    if (nrows != ncols)
        cerr << "Error detected by the inversion algorithm." << endl << "Matrix must  be square."<< endl;
    else
    {
        //Inversion method: standard Gauss-Jordan Elimination algorithm
        //When debuging this member function, you can uncomment the following lines.
        Matrix A_I;
        Matrix I;
        I.identity(ncols);

        A_I = *this;
        A_I = A_I & I;
		A_I = A_I.triang();
		//A_I.show('f',3,"Inverse triang");
        A_I = A_I.reduction();
		//A_I.show('f',3,"Inverse reduction");
        A_I = A_I.sel(1,nrows,(ncols+1),(2*ncols));
		//A_I.show('f',3,"Inverse");
        Inv = A_I;
    }
    return Inv;
}

/**
 * This method calculates the transpose matrix
 * @return The tranpose of the matrix in the current scope
 */
Matrix Matrix::transpose()
{
    Matrix Result;
    Result.resize(ncols, nrows).zero();
    for (unsigned int i = 1; i <= nrows; i++)
        for (unsigned int j = 1; j <= ncols; j++)
            Result.set(j, i, this->get(i,j));
    Result.unit = unit;
    return Result;
}

/**
 * This method returns the reduction to the canonic form of the matrix in the current scope.
 * @return A matrix equivalent to the reduction to the canonic form of the matrix in the current scope.
 */
Matrix Matrix::reduction()
{

    Matrix Result;
    Matrix LastResult;
    Result = *this;
    // this if the first value is too small it will performed a pivoting
    // @bug Bug may exist here!
	if (sqrt(pow(Result.get(1,1),2))<0.0000000000000000001)
    {
        double MaximumValueinFirstColumn=0; // maximum absolute value in first columns
        unsigned int Maximum_Row=0;
        for(unsigned int i=1;i<=nrows;i++)
        {
            if (sqrt(pow(Result.get(i,1),2))>MaximumValueinFirstColumn)
            {
                Maximum_Row=i;
                MaximumValueinFirstColumn=Result.get(i,1);
            }
        }
        for (unsigned int j=1;j<=ncols;j++) //permutes the rows
        {
            Result.set(1,j,this->get(Maximum_Row,j));
            Result.set(Maximum_Row,j,this->get(1,j));
        }
    }
    // end of pivoting
    for (unsigned int i=nrows;i>=1;i--) // for each row
    {
        LastResult = Result;
        for(unsigned int j=ncols; j>=1;j--)
        {
            Result.set(i,j,(Result.get(i,j)/LastResult.get(i,i))); // divides all elements of row i per the element i, i of LastInteraction
            for(unsigned int i2=i-1; i2>=1; i2--)
                Result.set(i2,j,( Result.get(i2,j) - Result.get(i,j)*LastResult.get(i2,i)) ); // makes the elements in the column j of the next rows of the matrix equal to zero using linear combination of line
        }
    }
    return Result;
}

/**
 * This method performs the triangularization of the matrix
 * @return the result of the triangularization
 */
Matrix Matrix::triang()
{
    // @bug A bug may exist here
    Matrix Result;
    Matrix LastResult;
    Result = *this;
    for (unsigned int i=1;i<=nrows;i++) // for each row
    {
        LastResult= Result;
        for(unsigned int j=i; j<=ncols;j++)
        {
            Result.set(i,j,(Result.get(i,j)/LastResult.get(i,i))); // divides all elements of row i per the element i, i of LastInteraction
            for(unsigned int i2=i+1; i2<=nrows; i2++)
                Result.set(i2,j, (Result.get(i2,j)-Result.get(i,j)*LastResult.get(i2,i)) ); // makes the elements in the column j of the next rows of the matrix equal to zero using linear combination of line
        }
    }
    return Result;
}

/**
 * This method
 */
Matrix Matrix::toDiagonal()
{
    Matrix Result;
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
Matrix Matrix::selDiagonal()
{
    Matrix Result(*this);
    for (unsigned int i = 1; (i <= nrows) && (i <= ncols); i++)
    {
        Result.set(i,1,get(i,i));
    }
    Result.resize(nrows,1);
    return Result;
}

/* Metodos adicionados por Paulo Andr� Batista Pupim 15/07/2011*/
Matrix& Matrix::putMatrix(Matrix put, int lin, int col)
{
    if (lin<=this->getRows() || col<=this->getCols())
    {
       //printf("sobrescrevendo matrix original\n");
    }

    int newRows=max(lin-1+put.getRows(),this->getRows());
    int newCols=max(col-1+put.getCols(),this->getCols());

    Matrix temp(newRows,newCols);
    for(int i=1;i<=newRows;i++)
    {
        for(int j=1;j<=newCols;j++)
        {
            if ( (lin<=i && i<lin+put.getRows()) && (col<=j && j<col+put.getCols()) )
            {
                temp.set(i,j,put.get(i-lin+1,j-col+1));
            }
            else if ( i<= this->getRows() && j<=this->getCols())
            {
                temp.set(i,j,this->get(i,j));
            }
        }
    }
   // imprime(temp,"temp");
    *this = temp;
    return *this;
}

Matrix* Matrix::convertToMatrix(int *array, int linhas, int colunas)
{
    Matrix mat(linhas,colunas);
    for (int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            mat.setInt(i+1,j+1,array[i*colunas+j]);
    return &mat;
}

Matrix* Matrix::convertToMatrix(double *array, int linhas, int colunas)
{
    Matrix mat(linhas,colunas);
    for (int i=0;i<linhas;i++)
        for(int j=0;j<colunas;j++)
            mat.set(i+1,j+1,array[i*colunas+j]);
    return &mat;
}
