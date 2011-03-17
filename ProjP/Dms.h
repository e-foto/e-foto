#ifndef DMS_H
#define DMS_H

#include <QSpinBox>
#include <QRegExpValidator>
#include <QString>
#include <QLocale>

class QRegExpValidator;

class Dms
{

public:
    Dms();
    Dms(int degree, int minute, double second,bool signal=false);
	Dms(double seconds);
    Dms(QString degree);
    int getDegree();
    int getMinute();
    double getSeconds();
    bool isValid();
    bool hasSignal();

    Dms* getDms();
    void setDms(Dms newDms);
    void setDms(int degree, int minute , double seconds, bool signal=false);
    QString toString(int decimals=3);
	static Dms* stringToDms(QString dms);
    void setDegree(int newDegrees);
    void setMinute(int newMinutes);
    void setSeconds(double newSeconds);
    void setSignal(bool newSignal);
	void setLocale(QLocale locale);
	QLocale getLocale();
    int compareDegMinSecs(Dms *degMinSec);
	void setSecondsPrecision(int precision=2);
	int getSecondsPrecision();

    //Operations
	//void addDegMinSecs(Dms* degMinSec1);
	//void mulDegMinSecs(int factor);

    //Converters
    double dmsToDegreeDecimal();
    double dmsToRadiano();
	void degreeDecimalToDms(double degreeDecimal);
    static double degreeDecimalToRadiano(double degree);
	void radianoToDms(double radiano);
    static double radianoToDegreeDecimal(double radiano);
	static Dms * secondsToDms(double seconds);

private:
	QLocale localeDms;
    int degree;
    int min;
    double sec;
    bool signal;
	int secondsPrecision;
};

#endif // DMS_H
