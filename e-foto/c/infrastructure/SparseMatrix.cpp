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
#include "SparseMatrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

SparseMatrixElement::SparseMatrixElement(size_t myRow, size_t myCol, double myValue)
    :row_(myRow),
     col_(myCol),
     value_(myValue),
     next_(NULL)
{

}

SparseMatrixElement::~SparseMatrixElement()
{

}

size_t SparseMatrixElement::getRow()
{
    return row_;
}

size_t SparseMatrixElement::getCol()
{
    return col_;
}

double SparseMatrixElement::getValue()
{
    return value_;
}

SparseMatrixElement* SparseMatrixElement::getNext()
{
    return next_;
}

void SparseMatrixElement::setRow(size_t newRow)
{
    row_ = newRow;
}

void SparseMatrixElement::setCol(size_t newCol)
{
    col_ = newCol;
}

void SparseMatrixElement::setValue(double newValue)
{
    value_ = newValue;
}

void SparseMatrixElement::setNext(SparseMatrixElement* newNext)
{
    next_ = newNext;
}


SparseMatrix::SparseMatrix(Matrix source, double precision)
{
	bool firstOfRow;
    SparseMatrixElement* newElement = NULL;
    SparseMatrixElement* lastElement = NULL;

    rows_ = source.getRows();
    cols_ = source.getCols();

    for (size_t i = 1; i <= rows_; i++)
	{
		firstOfRow = true;
        for (size_t j = 1; j <= cols_; j++)
		{
			if (fabs(source.get(i,j)) > precision)
			{
				newElement = new SparseMatrixElement(i, j, source.get(i,j));
				if (firstOfRow == true)
				{
                    elements_.push_back(newElement);
					lastElement = newElement;
					firstOfRow = false;
				}
				else
				{
					lastElement->setNext(newElement);
					lastElement = newElement;
				}
			}
		}
	}
}

SparseMatrix::~SparseMatrix()
{
	SparseMatrixElement* checker;
	SparseMatrixElement* deleter;
    for (size_t i = 0; i < elements_.size(); i++)
	{
        checker = elements_.at(i);
		while (checker != NULL)
		{
			deleter = checker;
			checker = checker->getNext();
			delete(deleter);
		}
	}
}

Matrix SparseMatrix::toMatrix()
{
    Matrix result(rows_, cols_);
	SparseMatrixElement* currentElement;

    for (size_t i = 0; i < elements_.size(); i++)
	{
        currentElement = elements_.at(i);
		while (currentElement != NULL)
		{
			result.set(currentElement->getRow(), currentElement->getCol(), currentElement->getValue());
			currentElement = currentElement->getNext();
		}
	}

	return result;
}

Matrix SparseMatrix::operator*(Matrix target)
{
    Matrix result(rows_, target.getCols());
	SparseMatrixElement* currentElement;
	double value;
	int currentRow;

    if (cols_ == target.getRows())
	{
        for (size_t i = 0; i < elements_.size(); i++)
		{
            for (size_t j = 1; j <= target.getCols(); j++)
			{
				value = 0.0;
                currentElement = elements_.at(i);
				currentRow = currentElement->getRow();
				while (currentElement != NULL)
				{
					value += currentElement->getValue() * target.get(currentElement->getCol(), j);
					currentElement = currentElement->getNext();
				}
				result.set(currentRow, j, value);
			}
		}
	}

	return result;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
