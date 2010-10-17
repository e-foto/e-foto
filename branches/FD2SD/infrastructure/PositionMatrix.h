#ifndef POSITIONMATRIX_H
#define POSITIONMATRIX_H

#include "RectSupport.h"
#include "Matrix.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

using namespace std;

class Matrix;

class PositionMatrix : public RectSupport
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

    friend class Matrix;

    //Constructors and destructors methods
    //
    PositionMatrix(unsigned int dimensions, string newUnit);
    PositionMatrix(const Matrix& anotherMatrix);
    PositionMatrix(const PositionMatrix& anotherMatrix);
    PositionMatrix();
    ~PositionMatrix();

    //Special matrix dispositions methods
    //
    PositionMatrix& resize(unsigned int rows, unsigned int cols = 1);
    PositionMatrix& zero();

    //File operations methods
    //
    int load(char* filename);
    int save(char* filename);

    //Recuperate elements methods
    //
    double get(const unsigned int i, const unsigned int j = 1) const;
    int getInt(const unsigned int i, const unsigned int j = 1) const;
    string getUnit() const;
    PositionMatrix sel(const unsigned int FirstRow, const unsigned int LastRow,
                       const unsigned int FirstCol = 1, const unsigned int LastCol = 1);
    void show();

    //Change elements methods
    //
    void set(unsigned int i, unsigned int j, double value) const;
    void set(unsigned int i, double value) const;
    void setInt(unsigned int i, unsigned int j, int value) const;
    void setInt(unsigned int i, int value) const;
    void setUnit(string newUnit);

    //Overloaded operators
    //
    PositionMatrix operator &(const PositionMatrix& Par_Matrix);
    PositionMatrix operator |(const PositionMatrix& Par_Matrix);
    PositionMatrix& operator =(const Matrix& Par_Matrix);
    PositionMatrix& operator =(const PositionMatrix& Par_Matrix);
    bool operator ==(const Matrix& Par_Matrix);
    bool operator ==(const PositionMatrix& Par_Matrix);
    bool operator !=(const Matrix& Par_Matrix);
    bool operator !=(const PositionMatrix& Par_Matrix);

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
    PositionMatrix toDiagonal();
    string toGmlPosFormat();

};

#endif //POSITIONMATRIX_H
