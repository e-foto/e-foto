#ifndef SCIENCESPINBOX
#define SCIENCESPINBOX

#include <QtWidgets/QDoubleSpinBox>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <QtGui/QMouseEvent>
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QTimer>

// This class contains code of Matthias Pospiech to reimplement some of the validation methods.

namespace br {
namespace uerj {
namespace eng {
namespace efoto {



class ScienceSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
public:
    explicit ScienceSpinBox(QWidget * parent = 0);

	int decimals() const;
	void setDecimals(int value);

	QString textFromValue ( double value ) const;
	double valueFromText ( const QString & text ) const;

	void setFullRange();

	QString mantissa(bool forceDot = false) const;
	QString exponent() const;
	QString textValue() const;
	void setTextValue(QString value);
	void selectMantissa();
	void selectExponent();

private:
	int dispDecimals;
	QTimer adjustDelayer; QTimer aux;
	QChar delimiter; QChar thousand;
	QDoubleValidator * v;


private:
	void initLocalValues(QWidget *parent);
	int delimiterPosition() const;
	int exponentialPosition() const;
	int decimalOffset() const;
	bool eventFilter(QObject *obj, QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	//void keyReleaseEvent(QKeyEvent *event);
	void focusInEvent(QFocusEvent *event);
	bool isIntermediateValue(const QString &str) const;
	QVariant validateAndInterpret(QString &input, int &pos, QValidator::State &state) const;
	QValidator::State validate(QString &text, int &pos) const;
	void fixup(QString &input) const;
	QString stripped(const QString &t, int *pos) const;
	double round(double value) const;
	void stepBy(int steps);

public slots:
	void stepDown();
	void stepUp();
	void stepDownMantissa();
	void stepUpMantissa();

private slots:
	void adjustDisplay();

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //SCIENCESPINBOX
