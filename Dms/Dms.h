#ifndef DMS_H
#define DMS_H

#include <QSpinBox>
#include <QRegExpValidator>
#include <QString>
#include "Matrix.h"

enum PositionValue{ NONE = -1 ,DEGREES, MINUTES, SECONDS};

class QRegExpValidator;

enum GeoSystem { CORREGOALEGRE, SAD69, SIRGAS2000, WGS84};


class TransformationsCoordinates
{
private:
	double lambda;
	double phi;
	double haltura;
	double E, N, H;

public :
        static Matrix GeoelipToGeocentricCartesian(double phi, double lambda, double haltura, GeoSystem system=SIRGAS2000);
        static Matrix GeocentricCartesianToGeoelip(double Xi, double Yi, double Zi, GeoSystem system=SIRGAS2000);
        static Matrix GeoelipToUtm(double phi, double lambda, double lambda0, double haltura, GeoSystem system=SIRGAS2000);
        Matrix getMatrixR(double phi, double lambda);

};


class Dms
{
        //Q_OBJECT
public:
	Dms();
	Dms(int degree, int minute, double second,bool signal=false);
	Dms(QString degree);
	int getDegree();
    int getMinute();
    double getSeconds();
	bool isValid();
	bool hasSignal();

	Dms* getDms();
	void setDms(Dms newDms);
	void setDms(int degree, int minute , double seconds, bool signal=false);
    QString toString();
	static Dms* stringToDms(QString dms);
    void setDegree(int newDegrees);
    void setMinute(int newMinutes);
    void setSeconds(double newSeconds);
	void setSignal(bool newSignal);
	int compareDegMinSecs(Dms *degMinSec);

    //Operations
	static Dms* addDegMinSecs(Dms* degMinSec1,Dms* degMinSec2);
	static Dms* mulDegMinSecs(int factor, Dms* degMinSec1);

    //Convertations
	double dmsToDegreeDecimal();
	double dmsToRadiano();
	Dms* degreeDecimalToDms(double degreeDecimal);
    static double degreeDecimalToRadiano(double degree);
	Dms* radianoToDms(double radiano);
    static double radianoToDegreeDecimal(double radiano);

private:
	int degree;
	int min;
    double sec;
	bool signal;
};

/*
class DegreeRadSpin:public QSpinBox
{
    Q_OBJECT
public:
    DegreeRadSpin(QWidget *parent=0);
    void stepUp();
    void stepDown();
    void stepBy(int steps);
    void setMinimum(int min);
    void setMaximum(int max);
    int minimum();
    int maximum();


    QString textFromValue(int val) const;
    int valueFromText(const QString &text) const;
    QValidator::State validate(QString &input, int &pos) const;
    PositionValue positionValue();
    void validateAndInterprete();
    void correctCursorPosition(int oldPosition, int oldLength);

private:
	Dms *DmsThis;
    QRegExpValidator *validator;
    QLineEdit *texto;

public slots:
    void showTexto();
};
*/
#endif // DMS_H
