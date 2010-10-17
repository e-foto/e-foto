#ifndef MATRIX_H
#define MATRIX_H

#include "RectSupport.h"
#include "PositionMatrix.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <stdlib.h>

using namespace std;

class PositionMatrix;

class Matrix : public RectSupport
{
    //EOBJECT

    //Private atributes
    //
    double* _Mat;
    string unit;

protected:

    //Protected methods
    //
    void del();
    void nw(const unsigned int rows, const unsigned int cols);

public:

    friend class PositionMatrix;

    //Constructors and destructors methods
    //
    Matrix(unsigned int rows, unsigned int cols);
    Matrix(const PositionMatrix& anotherMatrix);
    Matrix(const Matrix& anotherMatrix);
    Matrix();
    ~Matrix();

    //Special matrix dispositions methods
    //
    Matrix& resize(unsigned int rows, unsigned int cols); // Esses metodos alteram a própria matrix,
    Matrix& identity(unsigned int size);                  // mas permitem o uso em sequencia
    Matrix& ones();                                       // já que retornam uma referencia ao resultado.
    Matrix& zero();

    //File operations methods
    //
    int load(char* filename);
    int save(char* filename);

    //Recuperate elements methods
    //
    double get(const unsigned int i, const unsigned int j) const;
    int getInt(const unsigned int i, const unsigned int j) const;
    string getUnit();
    Matrix sel(const unsigned int FirstRow, const unsigned int LastRow,
               const unsigned int FirstCol, const unsigned int LastCol);
    void show();
    double highestValue() const;
    double lowestValue() const;

    //Change elements methods
    //
    void set(unsigned int i, unsigned int j, double value) const;
    void setInt(unsigned int i, unsigned int j, int value) const;
    void setUnit(string newUnit);

    //Verifications methods
    //
    bool isIdentity();
    bool isTriang();
	bool isZeroes();

    //Overloaded operators
    //
    Matrix operator &(const Matrix& Par_Matrix);
    Matrix operator |(const Matrix& Par_Matrix);
    Matrix operator +(const Matrix& Par_Matrix);
    Matrix operator -(const Matrix& Par_Matrix);
    Matrix operator *(const Matrix& Par_Matrix);
    Matrix operator +(double Par_double);
    Matrix operator -(double Par_double);
    Matrix operator *(double Par_double);
    Matrix operator /(double Par_double);
    Matrix& operator =(const PositionMatrix& Par_Matrix);
    Matrix& operator =(const Matrix& Par_Matrix);
    bool operator ==(const PositionMatrix& Par_Matrix);
    bool operator ==(const Matrix& Par_Matrix);
    bool operator !=(const PositionMatrix& Par_Matrix);
    bool operator !=(const Matrix& Par_Matrix);

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    //
    string xmlGetData();
    void xmlSetData(string xml);

    //Other methods
    //
    Matrix inverse();
    Matrix transpose();
    Matrix reduction();
    Matrix triang();
    Matrix toDiagonal();
    Matrix selDiagonal();

};

#endif //MATRIX_H
