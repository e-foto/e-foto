#include "SparseMatrix.h"

SparseMatrixElement::SparseMatrixElement()
{
	this->row = -1;
	this->col = -1;
	this->value = 0.0;
	this->next = NULL;
}

SparseMatrixElement::SparseMatrixElement(int myRow, int myCol, double myValue)
{
	this->row = myRow;
	this->col = myCol;
	this->value = myValue;
	this->next = NULL;
}

SparseMatrixElement::~SparseMatrixElement()
{

}

int SparseMatrixElement::getRow()
{
	return row;
}

int SparseMatrixElement::getCol()
{
	return col;
}

double SparseMatrixElement::getValue()
{
	return value;
}

SparseMatrixElement* SparseMatrixElement::getNext()
{
	return next;
}

void SparseMatrixElement::setRow(int newRow)
{
	row = newRow;
}

void SparseMatrixElement::setCol(int newCol)
{
	col = newCol;
}

void SparseMatrixElement::setValue(double newValue)
{
	value = newValue;
}

void SparseMatrixElement::setNext(SparseMatrixElement* newNext)
{
	next = newNext;
}

SparseMatrix::SparseMatrix()
{
	rows = 0;
	cols = 0;
}

SparseMatrix::SparseMatrix(Matrix source, double precision)
{
	bool firstOfRow;
	SparseMatrixElement* newElement;
	SparseMatrixElement* lastElement;

	rows = source.getRows();
	cols = source.getCols();

	for (unsigned int i = 1; i <= source.getRows(); i++)
	{
		firstOfRow = true;
		for (unsigned int j = 1; j <= source.getCols(); j++)
		{
			if (fabs(source.get(i,j)) > precision)
			{
				newElement = new SparseMatrixElement(i, j, source.get(i,j));
				if (firstOfRow == true)
				{
					elements.push_back(newElement);
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
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		checker = elements.at(i);
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
	Matrix result(rows, cols);
	SparseMatrixElement* currentElement;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		currentElement = elements.at(i);
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
	Matrix result(rows, target.getCols());
	SparseMatrixElement* currentElement;
	double value;
	int currentRow;

	if (cols == target.getRows())
	{
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			for (unsigned int j = 1; j <= target.getCols(); j++)
			{
				value = 0.0;
				currentElement = elements.at(i);
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
