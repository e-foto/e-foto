#include "ScienceSpinBox.h"

#include <limits>
#include <math.h>

ScienceSpinBox::ScienceSpinBox(QWidget * parent): QDoubleSpinBox(parent)
{
	setButtonSymbols(QAbstractSpinBox::NoButtons);
	initLocalValues(parent);
	setDecimals(8);
	QDoubleSpinBox::setDecimals(1000);
	setFullRange();
	v = new QDoubleValidator(this);
	v->setDecimals(1000);
	v->setNotation(QDoubleValidator::ScientificNotation);
	lineEdit()->setValidator(v);
	lineEdit()->installEventFilter(this);
	installEventFilter(this);
	//connect(&adjustDelayer,SIGNAL(timeout()),this,SLOT(adjustDisplay()));
	setValue(0);
}

// total of decimals to displayed
int ScienceSpinBox::decimals() const
{
	return dispDecimals;
}

// set total of decimals to displayed
void ScienceSpinBox::setDecimals(int value)
{
	dispDecimals = value;
}

// text to be displayed in spinbox
QString ScienceSpinBox::textFromValue(double value) const
{
	//qDebug("textFromValue");
	// convert to string -> Using exponetial display with internal decimals
	QString str = locale().toString(value, 'e', dispDecimals);
	// remove thousand sign
	if (qAbs(value) >= 1000.0)
	{
		str.remove(thousand);
	}
	return str;
}

// validate text input and value computing
double ScienceSpinBox::valueFromText(const QString &text) const
{
	//qDebug("valueFromText");
	QString copy = text;
	int pos = this->lineEdit()->cursorPosition();
	QValidator::State state = QValidator::Acceptable;
	bool ok;
	double directValue = stripped(text,0).toDouble(&ok);
	if (ok && directValue<=minimum())
		return minimum();
	else if (ok && directValue>=maximum())
		return maximum();
	else
		return validateAndInterpret(copy, pos, state).toDouble();
}

// set Range to maximum possible values
void ScienceSpinBox::setFullRange()
{
	double doubleMax = std::numeric_limits<double>::max();
	setRange(-doubleMax, doubleMax);
}

// text equal to mantissa displayed
// this method no detect mantissa to "special value text" (see QDoubleSpinBox documentation)
QString ScienceSpinBox::mantissa(bool forceDot) const
{
	QString text = lineEdit()->text();
	QString prefixtext = prefix();
	if (specialValueText().size() != 0 && text == specialValueText())
		return QString("");
	int from = 0;
	if (prefixtext.size() && text.startsWith(prefixtext)) {
		from += prefixtext.size();
	}
	int size = exponentialPosition()-from;
	text = text.mid(from, size);
	if (forceDot)
		text.replace(text.indexOf(delimiter,0),1,'.');
	return text;
}

// text equal to exponent displayed
QString ScienceSpinBox::exponent() const
{
	QString text = lineEdit()->text();
	QString suffixtext = suffix();
	if (specialValueText().size() != 0 || text == specialValueText())
		return QString("");
	int from = exponentialPosition()+1;
	int size = text.size() - from;
	if (suffixtext.size() && text.endsWith(suffixtext)) {
		size -= suffixtext.size();
	}
	text = text.mid(from, size);
	return text;
}

// text equal to scientific notation displayed adjusted to dot delimiter
QString ScienceSpinBox::textValue() const
{
	QString result = stripped(text(),0);
	if (delimiter == '.')
		return result;
	result.replace(result.indexOf(delimiter,0),1,'.');
	return result;
}

//
void ScienceSpinBox::setTextValue(QString value)
{
	//setValue(0);
	if (value.isEmpty())
		return;
	value.replace(".",delimiter);
	lineEdit()->setText(value);
	setValue(valueFromText(lineEdit()->text()));
}

// select mantissa in display
// this method no detect mantissa to "special value text" (see QDoubleSpinBox documentation) and does no selecting anything in this case
void ScienceSpinBox::selectMantissa()
{
	QString text = lineEdit()->text();
	QString prefixtext = prefix();
	if (specialValueText().size() != 0 && text == specialValueText())
		return;
	if (prefixtext.size() && text.startsWith(prefixtext))
	{
		lineEdit()->setSelection(prefixtext.size(),exponentialPosition()-prefixtext.size());
		return;
	}
	lineEdit()->setSelection(0,exponentialPosition());
}

// select exponent in display
// this method no detect mantissa to "special value text" (see QDoubleSpinBox documentation) and does no selecting anything in this case
void ScienceSpinBox::selectExponent()
{
	QString text = lineEdit()->text();
	QString suffixtext = suffix();
	if (specialValueText().size() != 0 && text == specialValueText())
		return;
	if (suffixtext.size() && text.endsWith(suffixtext))
	{
		lineEdit()->setSelection(text.size()-suffixtext.size(),-(text.size()-exponentialPosition()-1-suffixtext.size()));
		return;
	}
	lineEdit()->setSelection(text.size(),-(text.size()-exponentialPosition()-1));
}

// record the notation for the location configured in the system
void ScienceSpinBox::initLocalValues(QWidget *parent)
{
	const QString str = (parent ? parent->locale() : QLocale()).toString(4567.1);
	if (str.size() == 6) {
		delimiter = str.at(4);
		thousand = QChar((ushort)0);
	} else if (str.size() == 7) {
		thousand = str.at(1);
		delimiter = str.at(5);
	}
}

// returns the index of delimiter symbol on display
// this method no detect delimiter to "special value text" (see QDoubleSpinBox documentation) and returns -1 with this case
int ScienceSpinBox::delimiterPosition() const
{
	QString text = lineEdit()->text();
	QString prefixtext = prefix();
	if (specialValueText().size() != 0 && text == specialValueText())
		return -1;
	if (prefixtext.size() && text.startsWith(prefixtext))
		return text.indexOf(delimiter,prefix().size());
	return text.indexOf(delimiter,0);
}

// returns the index of exponential symbol on display
// this method no detect exponential to "special value text" (see QDoubleSpinBox documentation) and returns -1 with this case
int ScienceSpinBox::exponentialPosition() const
{
	QString text = lineEdit()->text();
	QString prefixtext = prefix();
	if (specialValueText().size() != 0 && text == specialValueText())
		return -1;
	if (prefixtext.size() && text.startsWith(prefixtext))
		return text.indexOf("e",prefix().size());
	return text.indexOf("e",0);
}

int ScienceSpinBox::decimalOffset() const
{
	QString text = lineEdit()->text();
	QString prefixtext = prefix();
	if (specialValueText().size() != 0 && text == specialValueText())
		return -1;
	int delimiterPos;
	int cursorPos;
	/* Creio que isso gerava muitos erros
	if (prefixtext.size() && text.startsWith(prefixtext))
	{
		delimiterPos = text.indexOf(delimiter,prefix().size());
		cursorPos = lineEdit()->cursorPosition() - prefixtext.size();
	}
	delimiterPos = text.indexOf(delimiter,prefix().size());
	cursorPos = lineEdit()->cursorPosition() - prefixtext.size();
	*/
	if (prefixtext.size() && text.startsWith(prefixtext))
	{
		delimiterPos = text.indexOf(delimiter,prefix().size())- prefixtext.size();
		cursorPos = lineEdit()->cursorPosition() - prefixtext.size();
	}
	else
	{
		delimiterPos = text.indexOf(delimiter,prefix().size());
		cursorPos = lineEdit()->cursorPosition();
	}
	if (cursorPos > delimiterPos)
		return delimiterPos-cursorPos;
	return abs(cursorPos-delimiterPos+1);
}

// redirects some of the events to achieve the desired behavior with the widget
bool ScienceSpinBox::eventFilter(QObject *obj, QEvent *event)
{
	// this serves to double clicks not select the text partially
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		selectAll();
		return true;
	}

	// this serves to cancel whellEvents
	if (event->type() == QEvent::Wheel)
	{
		return true;
	}

	//if (obj == lineEdit())
	//{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			mousePressEvent(mouseEvent);
			return true;
		}
	//}

	if (event->type() == QEvent::FocusIn)
	{
		QFocusEvent *focusEvent = static_cast<QFocusEvent*>(event);
		focusInEvent(focusEvent);
		return true;
	}

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		keyPressEvent(keyEvent);
		return true;
	}

	//if (event->type() == QEvent::KeyRelease)
	//{
		//QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		//keyReleaseEvent(keyEvent);
		//return true;
	//}

	return QDoubleSpinBox::eventFilter(obj,event);
}

// identifies and selects the clicked part of scientific notation
void ScienceSpinBox::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (lineEdit()->cursorPositionAt(event->pos()) < exponentialPosition())
		{
			if (lineEdit()->selectedText() != mantissa())
			{
				selectMantissa();
				return;
			}
		}
		else
		{
			if (lineEdit()->selectedText() != exponent())
			{
				selectExponent();
				return;
			}
		}
		lineEdit()->setCursorPosition(lineEdit()->cursorPositionAt(event->pos()));
		return;
	}
	else if (event->button() == Qt::RightButton)
	{
		selectAll();
	}
	else if (event->button() == Qt::MidButton)
	{
		//selectAll();
		lineEdit()->setCursorPosition(lineEdit()->cursorPositionAt(event->pos()));
	}
	else
	{
		QDoubleSpinBox::mousePressEvent(event);
	}
}

// it recognizes the direction of focus and selects the appropriate part of scientific notation
void ScienceSpinBox::focusInEvent(QFocusEvent *event)
{
	if (event->reason() == Qt::TabFocusReason)
	{
		selectMantissa();
	}
	else if (event->reason() == Qt::BacktabFocusReason)
	{
		selectExponent();
	}
	else
	{
		QDoubleSpinBox::focusInEvent(event);
	}
}

/*
void ScienceSpinBox::timerEvent(QTimerEvent *event)
{
	qDebug("EventTimer");
	setValue(valueFromText(lineEdit()->text()));
	selectMantissa();
	killTimer(event->timerId());
}
*/

// this ensures that there are events "next child" and "previous child" between the mantissa and exponent.
void ScienceSpinBox::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Tab)
	{
		if (lineEdit()->cursorPosition()<=exponentialPosition())
		{
			selectExponent();
			return;
		}
	}
	else if (event->key() == Qt::Key_Tab +1)//(event->key() == 16777218) // this is Shift+Tab Qt value
	{
		if (lineEdit()->cursorPosition()>exponentialPosition())
		{
			selectMantissa();
			return;
		}
	}/*
	else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up || event->key() == Qt::Key_Right || event->key() == Qt::Key_Left)
	{
		if (adjustDelayer.isActive())
		{
			adjustDelayer.stop();
			//qDebug("adjustActive capted");
		}
		adjustDelayer.start(5000);
	}*/
	// pass the event on to the parent class
	QDoubleSpinBox::keyPressEvent(event);
}

// this ensures that there are events "next child" and "previous child" between the mantissa and exponent.
/*
void ScienceSpinBox::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up)
	{
		qDebug("key_Released");
		//adjustDelayer.start(1000);
	}
	// pass the event on to the parent class
	QDoubleSpinBox::keyPressEvent(event);
}
*/

// reimplemented function, copied from QDoubleSpinBoxPrivate::isIntermediateValue
bool ScienceSpinBox::isIntermediateValue(const QString &str) const
{
	//qDebug("isIntermediateValue");
	//qDebug() << "input is" << str << minimum() << maximum();
	qint64 dec = 1;

	for (int i=0; i < decimals(); ++i)
		dec *= 10;

	const QLatin1Char dot('.');


	// determine minimum possible values on left and right of Decimal-char

	// I know QString::number() uses CLocale so I use dot
	const QString minstr = QString::number(minimum(), 'f', QDoubleSpinBox::decimals());
	qint64 min_left = minstr.left(minstr.indexOf(dot)).toLongLong();
	qint64 min_right = minstr.mid(minstr.indexOf(dot) + 1).toLongLong();

	const QString maxstr = QString::number(maximum(), 'f', QDoubleSpinBox::decimals());
	qint64 max_left = maxstr.left(maxstr.indexOf(dot)).toLongLong();
	qint64 max_right = maxstr.mid(maxstr.indexOf(dot) + 1).toLongLong();

	// determine left and right long values (left and right of delimiter)
	const int dotindex = str.indexOf(delimiter);
	const bool negative = maximum() < 0;
	qint64 left = 0, right = 0;
	bool doleft = true;
	bool doright = true;
	// no separator -> everthing in left
	if (dotindex == -1) {
		left = str.toLongLong();
		doright = false;
	}
	// separator on left or contains '+'
	else if (dotindex == 0 || (dotindex == 1 && str.at(0) == QLatin1Char('+'))) {
		// '+' at negative max
		if (negative) {
			//qDebug() << __FILE__ << __LINE__ << "returns false";
			return false;
		}
		doleft = false;
		right = str.mid(dotindex + 1).toLongLong();
	}
	// contains '-'
	else if (dotindex == 1 && str.at(0) == QLatin1Char('-')) {
		// '-' at positiv max
		if (!negative) {
			//qDebug() << __FILE__ << __LINE__ << "returns false";
			return false;
		}
		doleft = false;
		right = str.mid(dotindex + 1).toLongLong();
	} else {
		left = str.left(dotindex).toLongLong();
		if (dotindex == str.size() - 1) { // nothing right of Separator
			doright = false;
		} else {
			right = str.mid(dotindex + 1).toLongLong();
		}
	}
	// left > 0, with max < 0 and no '-'
	if ((left >= 0 && max_left < 0 && !str.startsWith(QLatin1Char('-')))
	// left > 0, with min > 0
		|| (left < 0 && min_left >= 0))
	{
		//qDebug("returns false");
		return false;
	}

	qint64 match = min_left;
	if (doleft && !isIntermediateValueHelper(left, min_left, max_left, &match)) {
		//qDebug() << __FILE__ << __LINE__ << "returns false";
		return false;
	}
	if (doright) {
		//qDebug("match %lld min_left %lld max_left %lld", match, min_left, max_left);
		if (!doleft) {
			if (min_left == max_left) {
				const bool ret = isIntermediateValueHelper(qAbs(left),
														   negative ? max_right : min_right,
														   negative ? min_right : max_right);
				//qDebug() << __FILE__ << __LINE__ << "returns" << ret;
				return ret;
			} else if (qAbs(max_left - min_left) == 1) {
				const bool ret = isIntermediateValueHelper(qAbs(left), min_right, negative ? 0 : dec)
								 || isIntermediateValueHelper(qAbs(left), negative ? dec : 0, max_right);
				//qDebug() << __FILE__ << __LINE__ << "returns" << ret;
				return ret;
			} else {
				const bool ret = isIntermediateValueHelper(qAbs(left), 0, dec);
				//qDebug() << __FILE__ << __LINE__ << "returns" << ret;
				return ret;
			}
		}
		if (match != min_left) {
			min_right = negative ? dec : 0;
		}
		if (match != max_left) {
			max_right = negative ? 0 : dec;
		}
		qint64 tmpl = negative ? max_right : min_right;
		qint64 tmpr = negative ? min_right : max_right;
		const bool ret = isIntermediateValueHelper(right, tmpl, tmpr);
		//qDebug() << __FILE__ << __LINE__ << "returns" << ret;
		return ret;
	}
	//qDebug() << __FILE__ << __LINE__ << "returns true";
	return true;
}

//	\internal Multi purpose function that parses input, sets state to
//	the appropriate state and returns the value it will be interpreted
//	as.
// reimplemented function, copied from QDoubleSpinBoxPrivate::validateAndInterpret
QVariant ScienceSpinBox::validateAndInterpret(QString &input, int &pos, QValidator::State &state) const
{
	//qDebug("validateAndInterpret");
	// return 'cachedText' if
	// input = cachedText, or input Empty

	static QString cachedText;
	static QValidator::State cachedState;
	static QVariant cachedValue;

	if (cachedText == input && !input.isEmpty()) {
		state = cachedState;
		//qDebug() << "cachedText was" << "'" << cachedText << "'" << "state was "
		//		   << state << " and value was " << cachedValue;
		return cachedValue;
	}
	const double max = maximum();
	const double min = minimum();

	// removes prefix & suffix
	QString copy = stripped(input, &pos);
	//qDebug() << "input" << input << "copy" << copy;

	int len = copy.size();
	double num = min;
	const bool plus = max >= 0;
	const bool minus = min <= 0;

	// Test possible 'Intermediate' reasons
	switch (len)
	{
		case 0:
			// Length 0 is always 'Intermediate', except for min=max
			if (max != min)	{
				state = QValidator::Intermediate;
			} else {
				state = QValidator::Invalid;
			}
			goto end;
		case 1:
			// if only char is '+' or '-'
			if (copy.at(0) == delimiter
				|| (plus && copy.at(0) == QLatin1Char('+'))
				|| (minus && copy.at(0) == QLatin1Char('-'))) {
				state = QValidator::Intermediate;
				goto end;
			}
			break;
		case 2:
			// if only chars are '+' or '-' followed by Comma seperator (delimiter)
			if (copy.at(1) == delimiter
				&& ((plus && copy.at(0) == QLatin1Char('+')) || (minus && copy.at(0) == QLatin1Char('-')))) {
				state = QValidator::Intermediate;
				goto end;
			}
			break;
		default: break;
	} // end switch


	// First char must not be thousand-char
	if (copy.at(0) == thousand)
	{
		//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
		state = QValidator::Invalid;
		goto end;
	}
	// Test possible 'Invalid' reasons
	else if (len > 1)
	{
		const int dec = copy.indexOf(delimiter); // position of delimiter
		// if decimal separator (delimiter) exists
		if (dec != -1) {
			// not two delimiters after one other (meaning something like ',,')
			if (dec + 1 < copy.size() && copy.at(dec + 1) == delimiter && pos == dec + 1) {
				copy.remove(dec + 1, 1); // typing a delimiter when you are on the delimiter
			}							 // should be treated as typing right arrow
			// too many decimal points
			if (copy.size() - dec > QDoubleSpinBox::decimals() + 1) {
				//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
				state = QValidator::Invalid;
				goto end;
			}
			// after decimal separator no thousand char
			for (int i=dec + 1; i<copy.size(); ++i) {
				if (copy.at(i).isSpace() || copy.at(i) == thousand) {
					//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
					state = QValidator::Invalid;
					goto end;
				}
			}
		// if no decimal separator exists
		} else {
			const QChar &last = copy.at(len - 1);
			const QChar &secondLast = copy.at(len - 2);
			// group of two thousand or space chars is invalid
			if ((last == thousand || last.isSpace())
				&& (secondLast == thousand || secondLast.isSpace())) {
				state = QValidator::Invalid;
				//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
				goto end;
			}
			// two space chars is invalid
			else if (last.isSpace() && (!thousand.isSpace() || secondLast.isSpace())) {
				state = QValidator::Invalid;
				//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
				goto end;
			}
		}
	} // end if (len > 1)

	// block of remaining test before 'end' mark
	{
		bool ok = false;
		bool notAcceptable = false;

		// convert 'copy' to double, and check if that was 'ok'
		QLocale loc(locale());
		num = loc.toDouble(copy, &ok);
		//qDebug() << __FILE__ << __LINE__ << loc << copy << num << ok;


		// conversion to double did fail
		if (!ok) {
			// maybe thousand char was responsable
			if (thousand.isPrint())
			{
				// if no thousand sign is possible, then
				// something else is responable -> Invalid
				if (max < 1000 && min > -1000 && copy.contains(thousand)) {
					state = QValidator::Invalid;
					//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
					goto end;
				}

				// two thousand-chars after one other are not valid
				const int len = copy.size();
				for (int i=0; i<len- 1; ++i) {
					if (copy.at(i) == thousand && copy.at(i + 1) == thousand) {
						//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
						state = QValidator::Invalid;
						goto end;
					}
				}

				// remove thousand-chars
				const int s = copy.size();
				copy.remove(thousand);
				pos = qMax(0, pos - (s - copy.size()));

				num = loc.toDouble(copy, &ok);
				//qDebug() << thousand << num << copy << ok;

				// if conversion still not valid, then reason unknown -> Invalid
				if (!ok) {
					state = QValidator::Invalid;
					//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
					goto end;
				}
				notAcceptable = true; // -> state = Intermediate
			} // endif: (thousand.isPrint())
		}

		// no thousand sign, but still invalid for unknown reason
		if (!ok) {
			state = QValidator::Invalid;
			//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
		}
		// number valid and within valid range
		else if (num >= min && num <= max) {
			if (notAcceptable) {
				state = QValidator::Intermediate; // conversion to num initially failed
			} else {
				state = QValidator::Acceptable;
			}
			//qDebug() << __FILE__ << __LINE__<< "state is set to "
			//		   << (state == QValidator::Intermediate ? "Intermediate" : "Acceptable");
		}
		// when max and min is the same the only non-Invalid input is max (or min)
		else if (max == min) {
			state = QValidator::Invalid;
			//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
		} else {
			// value out of valid range (coves only special cases)
			if ((num >= 0 && num > max) || (num < 0 && num < min)) {
				state = QValidator::Invalid;
				//qDebug() << __FILE__ << __LINE__<< "state is set to Invalid";
			} else {
				// invalid range, further test with 'isIntermediateValue'
				if (isIntermediateValue(copy)) {
					state =  QValidator::Intermediate;
				} else {
					state =  QValidator::Invalid;
				}
				//qDebug() << __FILE__ << __LINE__<< "state is set to "
				//		   << (state == QValidator::Intermediate ? "Intermediate" : "Acceptable");
			}
		}
	}

end:
	// if something went wrong, set num to something valid
	if (state != QValidator::Acceptable) {
		num = max > 0 ? min : max;
	}

	// save (private) cache values
	cachedText = prefix() + copy + suffix();
	cachedState = state;
	cachedValue = QVariant(num);
	// return resulting valid num
	return QVariant(num);
}

// overwritten virtual function of QAbstractSpinBox
QValidator::State ScienceSpinBox::validate(QString &text, int &pos) const
{
	//qDebug("validate");
	QValidator::State state;
	validateAndInterpret(text, pos, state);
	return state;
}

// overwritten virtual function of QAbstractSpinBox
void ScienceSpinBox::fixup(QString &input) const
{
	//qDebug("fixup");
	input.remove(thousand);
}

// strips any prefix/suffix from \a text.
// reimplemented function, copied from QAbstractSpinBoxPrivate::stripped
QString ScienceSpinBox::stripped(const QString &t, int *pos) const
{
	//qDebug("stripped");
	QString text = t;
	QString prefixtext = prefix();
	QString suffixtext = suffix();

	if (specialValueText().size() == 0 || text != specialValueText()) {
		int from = 0;
		int size = text.size();
		bool changed = false;
		if (prefixtext.size() && text.startsWith(prefixtext)) {
			from += prefixtext.size();
			size -= from;
			changed = true;
		}
		if (suffixtext.size() && text.endsWith(suffixtext)) {
			size -= suffixtext.size();
			changed = true;
		}
		if (changed)
			text = text.mid(from, size);
	}

	const int s = text.size();
	text = text.trimmed();
	if (pos)
		(*pos) -= (s - text.size());
	return text;
}

// this function is never used...?
double ScienceSpinBox::round(double value) const
{
	qDebug("round");
	const QString strDbl = locale().toString(value, 'g', dispDecimals);
	return locale().toDouble(strDbl);
}

// overwritten virtual function of QAbstractSpinBox
void ScienceSpinBox::stepBy(int steps)
{
	//qDebug("stepBy");
	if (steps < 0)
		stepDown();
	else
		stepUp();
}

// overwritten virtual function of QAbstractSpinBox
void ScienceSpinBox::stepDown()
{
	//qDebug("stepDown");
	if (lineEdit()->cursorPosition()>exponentialPosition())
	{
		setValue(value()/10.0);
		selectExponent();
	}
	else
	{
		stepDownMantissa();
	}
}

// overwritten virtual function of QAbstractSpinBox
void ScienceSpinBox::stepUp()
{
	//qDebug("stepUp");
	if (lineEdit()->cursorPosition()>exponentialPosition())
	{
		setValue(value()*10.0);
		selectExponent();
	}
	else
	{
		stepUpMantissa();
	}
}

void ScienceSpinBox::stepDownMantissa()
{
	//qDebug("stepDownMantissa");
	QString text = lineEdit()->text();
	QString dest = mantissa();
	QString src = locale().toString(dest.toDouble()-pow(10,decimalOffset()/*-dispDecimals*/), 'f', dispDecimals);
	int cursorPos = lineEdit()->cursorPosition();
	if (cursorPos == delimiterPosition() || cursorPos == exponentialPosition()
		|| (cursorPos==0 && valueFromText(text)<0))
		return;
	// remove thousand sign
	if (qAbs(value()) >= 1000.0)
	{
		src.remove(thousand);
	}
	text.replace(dest,src);
	if (valueFromText(text) > minimum())
	{
		lineEdit()->setText(text);
		/*
		if (dest.size()<src.size())
			cursorPos++;
		else if (dest.size()>src.size())
			cursorPos--;
		lineEdit()->setSelection(cursorPos+1,-1);
		*/
	}
	else
	{
		setValue(minimum());
	}
	int oldExp = exponent().toInt();
//	int cursorPos = lineEdit()->cursorPosition();
	int aux = (delimiterPosition() > cursorPos);
	setValue(valueFromText(lineEdit()->text()));
	int offset = abs(cursorPos+(exponent().toInt()-oldExp))-aux;
	offset += (offset == delimiterPosition());
	offset -= (offset == exponentialPosition());
	lineEdit()->setCursorPosition(offset);
}

void ScienceSpinBox::stepUpMantissa()
{
	//qDebug("stepUpMantissa");
	QString text = lineEdit()->text();
	QString dest = mantissa();
	QString src = locale().toString(dest.toDouble()+pow(10,decimalOffset()/*-dispDecimals*/), 'f', dispDecimals);
	int cursorPos = lineEdit()->cursorPosition();
	if (cursorPos == delimiterPosition() || cursorPos == exponentialPosition()
		|| (cursorPos==0 && valueFromText(text)<0))
		return;
	// remove thousand sign
	if (qAbs(value()) >= 1000.0)
	{
		src.remove(thousand);
	}
	text.replace(dest,src);
	if (valueFromText(text) < maximum())
	{
		lineEdit()->setText(text);
		/*
		if (dest.size()<src.size())
			cursorPos++;
		else if (dest.size()>src.size())
			cursorPos--;
		lineEdit()->setSelection(cursorPos+1,-1);
		*/
	}
	else
	{
		setValue(maximum());
	}
	int oldExp = exponent().toInt();
//	int cursorPos = lineEdit()->cursorPosition();
	int aux = (delimiterPosition() > cursorPos);
	setValue(valueFromText(lineEdit()->text()));
	int offset = abs(cursorPos+(exponent().toInt()-oldExp))-aux;
	offset -= (offset == delimiterPosition());
	offset -= (offset == exponentialPosition());
	lineEdit()->setCursorPosition(offset);
}

void ScienceSpinBox::adjustDisplay()
{
	qDebug("AdjustDysplay");
	adjustDelayer.stop();
	//QString actualMantissa = mantissa(true);

	// Isso funciona com algumas limitações
	int oldExp = exponent().toInt();
	int cursorPos = lineEdit()->cursorPosition();
	int aux = (delimiterPosition() > cursorPos);
	setValue(valueFromText(lineEdit()->text()));
	int offset = abs(cursorPos+(exponent().toInt()-oldExp))-aux;
	lineEdit()->setCursorPosition(offset);

	/* Isso não funciona
	int oldExp = exponent().toInt();
	setValue(valueFromText(lineEdit()->text()));
	int newExp = exponent().toInt();
	if (newExp>oldExp)
		lineEdit()->cursorForward(false,newExp-oldExp+2);
	else if (newExp<oldExp)
		lineEdit()->cursorBackward(false,oldExp-newExp+2);
		*/
	//lineEdit()->setCursorPosition(delimiterPosition()-1);
}

// reimplemented function, copied from qspinbox.cpp
static bool isIntermediateValueHelper(qint64 num, qint64 min, qint64 max, qint64 *match)
{
	//qDebug("isIntermediateValueHelper");
	//qDebug("%lld %lld %lld", num, min, max);

	if (num >= min && num <= max) {
		if (match)
			*match = num;
		//qDebug("returns true 0");
		return true;
	}
	qint64 tmp = num;

	int numDigits = 0;
	int digits[10];
	if (tmp == 0) {
		numDigits = 1;
		digits[0] = 0;
	} else {
		tmp = qAbs(num);
		for (int i=0; tmp > 0; ++i) {
			digits[numDigits++] = tmp % 10;
			tmp /= 10;
		}
	}

	int failures = 0;
	qint64 number;
	for (number=max; number>=min; --number) {
		tmp = qAbs(number);
		for (int i=0; tmp > 0;) {
			if (digits[i] == (tmp % 10)) {
				if (++i == numDigits) {
					if (match)
						*match = number;
					//qDebug("returns true 1");
					return true;
				}
			}
			tmp /= 10;
		}
		if (failures++ == 500000) { //upper bound
			if (match)
				*match = num;
			//qDebug("returns true 2");
			return true;
		}
	}
	//qDebug("returns false");
	return false;
}
