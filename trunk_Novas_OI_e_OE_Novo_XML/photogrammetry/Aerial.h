/**************************************************************************
                        Aerial.h
**************************************************************************/


#ifndef AERIAL_H
#define AERIAL_H

#include "Sensor.h"
#include "Matrix.h"
#include "AnalogFiductialMark.h"

/**
  * class Aerial
  *
  * @author E-Foto group
  *
  * * * * * * * * * * * *
  * @date 06/05/2009
  * @version 1.2 - Rafael Alves de Aguiar & Irving da Silva Badolato
  */

class Aerial : public Sensor
{

    //EOBJECT

private:

    //Private attributes
    //
    Matrix Lb;
    Matrix SigmaLb;

    //Composed objects
    //
    deque<AnalogFiductialMark> anaFidMarks;

public:

    //Constructors
    //
    Aerial();
    Aerial(const Sensor& sensor);
    Aerial(int myId);
    Aerial(int myId, const Matrix& myLb);
    Aerial(int myId, const Matrix& myLb, const Matrix& mySigmaLb);

    //Private attribute accessor methods
    //
    void setLb(const Matrix& newLb);
    void setLb(const Matrix& myLb, const Matrix& mySigmaLb);
    Matrix getLb();
    Matrix getSigmaLb();

    //Composed object accessor methods
    //
    void setAnaFidMarks(deque<AnalogFiductialMark> newAnaFidMarks);
    deque<AnalogFiductialMark> getAnaFidMarks();

    void putAnaFidMark(AnalogFiductialMark newAnaFidMark);
    AnalogFiductialMark getAnaFidMark(int id);
    AnalogFiductialMark getAnaFidMarkAt(unsigned int index);
    int countAnaFidMarks();

    void deleteAnaFidMark(int id);
    void deleteAnaFidMarkAt(unsigned int index);
    void clearAnaFidMarks();

    // EObject methods
    //
    string objectType(void);
    string objectAssociations(void);
    bool is(string s);

    // XML methods
    //
    void xmlSetData(string xml);
    string xmlGetData();

    //Other methods
    //
    void marksToLb();

};

#endif // AERIAL_H
