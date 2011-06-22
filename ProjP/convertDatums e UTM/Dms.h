#ifndef DMS_H
#define DMS_H

#include <QSpinBox>
#include <QRegExpValidator>
#include <QString>

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
	QString toString(int decimals=20);
    Dms* stringToDms(QString dms);
    void setDegree(int newDegrees);
    void setMinute(int newMinutes);
    void setSeconds(double newSeconds);
    void setSignal(bool newSignal);
    int compareDegMinSecs(Dms *degMinSec);
	void setSecondsPrecision(int precision=2);
	int getSecondsPrecision();

    //Operations
	void addDegMinSecs(Dms* degMinSec1);
	void mulDegMinSecs(int factor);

    //Converters
    double dmsToDegreeDecimal();
    double dmsToRadiano();
    Dms* degreeDecimalToDms(double degreeDecimal);
    static double degreeDecimalToRadiano(double degree);
    Dms* radianoToDms(double radiano);
    static double radianoToDegreeDecimal(double radiano);
	static Dms * secondsToDms(double seconds);

private:
    int degree;
    int min;
    double sec;
    bool signal;
	int secondsPrecision;
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
