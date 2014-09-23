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

#include "Matrix.h"

#include "PositionMatrix.h"

#include <iostream>
#include <fstream>

#include <math.h>
#include <sstream>
#include <iomanip>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

void Matrix::del()
{
	delete[] _Mat;
	_Mat = NULL;
	ncols = 0;
	nrows = 0;
    unit_ = "";
}

void Matrix::nw(const unsigned int rows, const unsigned int cols)
{
	Matrix::_Mat = new double[rows * cols];       // Allocate matrix
	nrows = rows; ncols = cols;                   // Fill matrix' attributes.
	for(unsigned int i = 0; i < (rows * cols); i++) //set its elements to zero
		_Mat[i] = 0;
    unit_ = "";
}

Matrix::Matrix(unsigned int rows, unsigned int cols):RectSupport(rows, cols)
{
	nw(rows, cols); // Allocate matrix and fill its attributes.
}

Matrix::Matrix(const PositionMatrix& anotherMatrix):RectSupport(anotherMatrix.getRows(), anotherMatrix.getCols())
{
	nw(anotherMatrix.getRows(), anotherMatrix.getCols());
	*this = anotherMatrix; // Allocate matrix and fill its attributes.
}

Matrix::Matrix(const Matrix& anotherMatrix):RectSupport(anotherMatrix.getRows(), anotherMatrix.getCols())
{
	nw(anotherMatrix.getRows(), anotherMatrix.getCols());
	*this = anotherMatrix; // Allocate matrix and fill its attributes.
}

Matrix::Matrix():RectSupport(0, 0)
{
	_Mat = NULL;
    unit_ = "";
}

Matrix::~Matrix()
{
	del();
}

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

Matrix& Matrix::ones()
{
	for(unsigned int i = 0; i < (nrows * ncols); i++)
		_Mat[i] = 1;
	return *this;
}

Matrix& Matrix::zero()
{
	for(unsigned int i = 0; i < (nrows * ncols); i++)
		_Mat[i] = 0;
	return *this;
}

int Matrix::load(char* filename)
{
	char str[256];
	unsigned int cols;
	unsigned int rows;

    std::ifstream arq(filename); // open the emf file
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

int Matrix::save(char* filename)
{
    std::ofstream emfile(filename); // open the emf file
	if (!emfile.fail())
	{
        emfile << "E-foto project double Matrix Format" << std::endl; // write file header
		if (!emfile.fail())
		{
            emfile << filename << std::endl; // write filename
			if (!emfile.fail())
			{
                emfile << ncols << " " << nrows << std::endl;
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

double Matrix::get(const unsigned int i, const unsigned int j) const
{
	if ((i >= 1)&&(i <= nrows)&&(j >= 1)&&(j <= ncols))
		return _Mat[(i-1) * ncols + j - 1];
	else
        std::cerr << "Get ["<< Conversion::intToString(i) << ","<< Conversion::intToString(j) << "] values out of the range of the matrix." << std::endl;
	return 0;
}

int Matrix::getInt(const unsigned int i, const unsigned int j) const
{
	return (int) get(i,j);
}

std::string Matrix::getUnit()
{
    return unit_;
}

Matrix Matrix::sel(const unsigned int FirstRow, const unsigned int LastRow,
				   const unsigned int FirstCol, const unsigned int LastCol)
{
	Matrix DraftResult(1, 1);
	Matrix Result;
	if((FirstRow > LastRow)||(FirstCol > LastCol)||(FirstRow < 1)||(FirstCol < 1)||(LastRow > nrows)||(LastCol > ncols))
	{
        std::cerr << "Error detected by the Matrix.sel() method:" << std::endl << "Input parameters out of range or incorrect."<< std::endl;
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
    Result.unit_ = unit_;
	return Result;
}

void Matrix::show(char mode, int precision, std::string name)
{
    std::cout << "Matrix " << name << " ["<<nrows << 'x' << ncols << "] "<< std::endl;
	if (mode=='f')
        std::cout.setf(std::ios::fixed);
	if (mode=='e')
        std::cout.setf(std::ios::scientific);
	for (unsigned int i = 1; i <= nrows; i++)
	{
		for (unsigned int j = 1; j <= ncols; j++)
            std::cout << std::setw(15) << std::setprecision(precision) <<  get((unsigned int) i, (unsigned int) j) << " ";
        std::cout << std::endl;
	}
	if (mode=='f')
        std::cout.unsetf(std::ios::fixed);
	if (mode=='e')
        std::cout.unsetf(std::ios::scientific);

    std::cout << std::endl;
}

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

void Matrix::set(unsigned int i, unsigned int j, double value) const
{
	if ((i >= 1)&&(i <= nrows)&&(j >= 1)&&(j <= ncols))
		_Mat[(i-1) * ncols + j - 1]=value;
	else
        std::cerr << "Set ["<< Conversion::intToString(i) << ","<< Conversion::intToString(j) << "] values out of the range of the matrix." << std::endl;
}

void Matrix::setInt(unsigned int i, unsigned int j, int value) const
{
	set(i,j, (double) value);
}

void Matrix::setUnit(std::string newUnit)
{
    unit_ = newUnit;
}

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

bool Matrix::isZeroes()
{
	for (unsigned int j = 1; j <= ncols; j++)
	{
		for (unsigned int i = 1; i <= nrows; i++)
			if (get((unsigned int)i,(unsigned int)j) > 0.00000000001 || get((unsigned int)i,(unsigned int)j) < -0.00000000001)
				return false;
	}
	return true;
}

Matrix Matrix::operator &(const Matrix& Par_Matrix)
{
	Matrix DraftResult;
	Matrix Result;
	if (Par_Matrix.nrows!=nrows)
	{
        std::cerr << "Error detected by the & operator:" << std::endl<< "Both matrixes must have the same number of rows." << std::endl;
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
        Result.unit_ = unit_;
		return Result;
	}
	return Result;
}

Matrix Matrix::operator |(const Matrix& Par_Matrix)
{
	Matrix Result;

	if (Par_Matrix.ncols!=ncols)
	{
        std::cerr << "Error detected by the | operator:" << std::endl<< "Both matrixes must have the same number of cols." << std::endl;
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
        Result.unit_ = unit_;
	}

	return Result;
}

Matrix Matrix::operator +(const Matrix& Par_Matrix)
{
	Matrix DraftResult(Par_Matrix.nrows, Par_Matrix.ncols);
	Matrix Result;
	if ((Par_Matrix.nrows!=nrows)||(Par_Matrix.ncols!=ncols) )
	{
        std::cerr << "Error detected by the + operator: Both matrixes must have the same dimensions." << std::endl;
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
        Result.unit_ = unit_;
		return Result;
	}
	return Result;
}

Matrix Matrix::operator -(const Matrix& Par_Matrix)
{
	Matrix DraftResult(Par_Matrix.nrows, Par_Matrix.ncols);
	Matrix Result;
	if ((Par_Matrix.nrows!=nrows)||(Par_Matrix.ncols!=ncols))
	{
        std::cerr << "Error detected by the - operator: Both matrixes must have the same dimensions." << std::endl;
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
        Result.unit_ = unit_;
		return Result;
	}
	return Result;
}

Matrix Matrix::operator *(const Matrix& Par_Matrix)
{
	Matrix Result;
	Matrix *DraftResult = new Matrix;
	DraftResult->resize(nrows, Par_Matrix.ncols).zero();

	if (ncols!=Par_Matrix.nrows)
	{
        throw "Matrices dimensions are incompatible.";
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
        Result.unit_ = unit_;
		delete DraftResult;
		return Result;
	}
}

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
    Result.unit_ = unit_;
	return Result;
}

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
    Result.unit_ = unit_;
	return Result;
}

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
    Result.unit_ = unit_;
	return Result;
}

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
    Result.unit_ = unit_;
	return Result;
}

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
    unit_ = Par_Matrix.unit;
	return *this;
}

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
    unit_ = Par_Matrix.unit_;
	return *this;
}

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

std::string Matrix::objectType(void)
{
	return "Matrix";
}

std::string Matrix::objectAssociations(void)
{
	return "";
}

bool Matrix::is(std::string s)
{
	return (s == "Matrix" ? true : false);
}

std::string Matrix::xmlGetData()
{
    std::stringstream result;
    result << "<mml:matrix>\n";
    for (unsigned int i = 1; i <= getRows(); i++)
    {
        result << "<mml:matrixrow>\n";
        for (unsigned int j = 1; j <= getCols(); j++)
            result << "<mml:cn>" << Conversion::doubleToString(get(i, j)) << "</mml:cn>\n";
        result << "</mml:matrixrow>\n";
    }
    result << "</mml:matrix>\n";
    return result.str();
}

std::string Matrix::xmlGetData(int prec)
{
    std::stringstream result;
    result << "<mml:matrix>\n";
    for (unsigned int i = 1; i <= getRows(); i++)
    {
        result << "<mml:matrixrow>\n";
        for (unsigned int j = 1; j <= getCols(); j++)
            result << "<mml:cn>" << Conversion::doubleToString(get(i, j),prec) << "</mml:cn>\n";
        result << "</mml:matrixrow>\n";
    }
    result << "</mml:matrix>\n";
    return result.str();
}

void Matrix::xmlSetData(std::string xml)
{
	EDomElement root(xml);

    std::deque<EDomElement> matRows, matCols;

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
/*
void ajusta(float dado[numPontos][3], float matrix[2+1][2*(2+1)], float vector[2+1], float c[2+1])
{
 int i,j,k;
 float element;

 //preenchendo a matriz
 for(k=0;k<numPontos;k++)
 {
  for(i=0;i<(2+1);i++)
  {
   vector[i]+=dado[k][1]*pow(dado[k][0],i)/pow(dado[k][2],2);
   for(j=0;j<(2+1);j++)
	matrix[i][j]+=pow(dado[k][0],(i+j))/pow(dado[k][2],2);
  }
 }
 //inversão da matriz

 for(i=0;i<2+1;i++) //definindo a linha a trabalhar
 {
  element=(matrix[i][i]);
  for(j=0;j<(2*(2+1));j++)
  {
   matrix[i][j]=(matrix[i][j])/element; //normalizando a linha
  }
  for(k=0;k<(2+1);k++)
  {
   if(k!=i)
   {
	element=matrix[k][i];
	for(j=0;j<(2*(2+1));j++)
	{
	 matrix[k][j]=(matrix[k][j])-(element*(matrix[i][j]));
	}
   }
  }
 }
 for(i=0;i<(2+1);i++)
  for(j=0;j<(2+1);j++)
   c[i]+=matrix[i][j+2+1]*vector[j];
}
*/
Matrix Matrix::inverse2()
{
	Matrix Inv = *this;
	double element;
	if (nrows != ncols)
        std::cerr << "Error detected by the inversion algorithm." << std::endl << "Matrix must  be square."<< std::endl;
	else
	{
        for(int i = 1; i<=nrows; i++) //definindo a linha a trabalhar
		{
			element = Inv.get(i,i);
			for(int j = 1; j<=ncols; j++)
			{
				Inv.set(i,j,Inv.get(i,j)/element); //normalizando a linha
			}
			for(int k = 1; k<=nrows; k++)
			{
				if(k != i)
				{
					element = Inv.get(k,i);
					for(int j = 1; j<=ncols; j++)
					{
						Inv.set(k,j,Inv.get(k,j)-(element*Inv.get(i,j)));
					}
				}
			}
		}
	}
	return Inv;
}

Matrix Matrix::inverse()
{
	Matrix Inv;
	if (nrows != ncols)
        std::cerr << "Error detected by the inversion algorithm." << std::endl << "Matrix must  be square."<< std::endl;
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
		A_I = A_I.reduction();
		A_I = A_I.sel(1,nrows,(ncols+1),(2*ncols));
		Inv = A_I;
	}
	return Inv;
}

/**
 * This method calculates the inverse matrix using the OSU method.
 * @return The inverse of the matrix in the current scope
 */
Matrix Matrix::osuInverse()
{
	Matrix Inv;
	if (nrows != ncols)
        std::cerr << "Error detected by the inversion algorithm." << std::endl << "Matrix must be square."<< std::endl;
	else
	{
		Inv = *this;
		Matrix B(nrows,1);

		// The entire sequence must be done x times, where x is the matrix's size.
		for (int steps = 0; steps < nrows; steps++)
		{
			// Setting the B vector. Last row is different from the others.
			for (int i = 1; i < nrows; i++)
			{
				B.set(i,1,Inv.get(1,i+1)/Inv.get(1,1));
			}
			B.set(nrows,1,1/Inv.get(1,1));

			// Going for the rows. Last row is different from the others.
			for (int i = 1; i < nrows; i++)
			{
				// Going for the columns. Last column is different from the others.
				for (int j = 1; j < ncols; j++)
				{
					Inv.set(i,j,Inv.get(i+1,j+1)-Inv.get(i+1,1)*B.get(j,1));
				}
				Inv.set(i,ncols,-1*Inv.get(i+1,1)*B.get(ncols,1));
			}
			for (int j = 1; j <= ncols; j++)
			{
				Inv.set(nrows,j,B.get(j,1));
			}
		}

		/*Matrix A_I;
				Matrix I;
				I.identity(ncols);

				A_I = *this;
				A_I = A_I & I;
				A_I = A_I.triang();
				A_I = A_I.reduction();
				A_I = A_I.sel(1,nrows,(ncols+1),(2*ncols));
				Inv = A_I;*/
	}
	return Inv;
}

Matrix Matrix::transpose()
{
	Matrix Result;
	Result.resize(ncols, nrows).zero();
	for (unsigned int i = 1; i <= nrows; i++)
		for (unsigned int j = 1; j <= ncols; j++)
			Result.set(j, i, this->get(i,j));
    Result.unit_ = unit_;
	return Result;
}

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

Matrix Matrix::triang()
{
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
    Result.unit_ = unit_;
	return Result;
}

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

Matrix& Matrix::putMatrix(Matrix put, int lin, int col)
{
	if (lin<=this->getRows() || col<=this->getCols())
	{
		//printf("sobrescrevendo matrix original\n");
	}

    int newRows=std::max(lin-1+put.getRows(),this->getRows());
    int newCols=std::max(col-1+put.getCols(),this->getCols());

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

Matrix* Matrix::convertToMatrix(int *array, int lin, int col)
{
	Matrix mat(lin, col);
	for (int i=0;i<lin;i++)
		for(int j=0;j<col;j++)
			mat.setInt(i+1,j+1,array[i*col+j]);
	return &mat;
}

Matrix* Matrix::convertToMatrix(double *array, int lin, int col)
{
	Matrix mat(lin, col);
	for (int i=0;i<lin;i++)
		for(int j=0;j<col;j++)
			mat.set(i+1,j+1,array[i*col+j]);
	return &mat;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
