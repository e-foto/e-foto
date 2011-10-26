#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <deque>
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SparseMatrixElement
{
	int row;
	int col;
	double value;
	SparseMatrixElement* next;

public:
	SparseMatrixElement();
	SparseMatrixElement(int myRow, int myCol, double myValue);
	~SparseMatrixElement();
	int getRow();
	int getCol();
	double getValue();
	SparseMatrixElement* getNext();
	void setRow(int newRow);
	void setCol(int newCol);
	void setValue(double newValue);
	void setNext(SparseMatrixElement* newNext);
};

class SparseMatrix
{
	int rows;
	int cols;
	deque<SparseMatrixElement*> elements;

public:
	SparseMatrix();
	SparseMatrix(Matrix source, double precision = 1e-20);
	~SparseMatrix();
	Matrix toMatrix();
	Matrix operator*(Matrix target);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SPARSEMATRIX_H
