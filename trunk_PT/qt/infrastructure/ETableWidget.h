#ifndef ETABLEWIDGET_H
#define ETABLEWIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "EDom.h"
#include "Matrix.h"


class ETableWidget: public QTableWidget
{
    Q_OBJECT
private:
    Matrix *io;
	char mode;
	int decimals;
	QString type;
	QSpinBox *itemSpinBox;
	int spinBoxRow;
	int spinBoxColumn;
	QDoubleSpinBox *itemDoubleSpinBox;
	int doubleSpinBoxRow;
	int doubleSpinBoxColumn;
	QList<int> typedColumns;

public:
	ETableWidget(QWidget *parent=0);
	ETableWidget(Matrix values,char mode='e',int precision=6,QWidget *parent=0);
    void setTableData(Matrix values,char mode='e',int precision=6);
    void setTableData(EDomElement xml,char mode='e',int precision=6);
    void setTableData(string xmlString,char mode='e',int precision=6);
	void setVerticalHeadersLabelsHtml(QStringList list);
	void resizeTable();
    void enableAutoCopy(bool enable=true);
	void putIn(Matrix input, int row, int column,char mode='e',int precision=6);
	void putInColumn(QStringList list, int column);
	void putInRow(QStringList list,int row);

	void setColumnType(int colIndex,QString type="QSpinBox", bool enable=true, double minValue=0, double maxValue=100);
	void setType(QString type="NONE");
	QString getType();

	void clearSelection();
	void setMode(char newMode);
	void setDecimals(int newDecimals);
	char getMode();
	int getDecimals();

public slots:
    void autoCopy();
	void enableAutoCopy(int enable);
	void updateTableValues(char mode, int precision);
	void updateModoValues(int modo);
	void updateDecimalsValues(int decimal);
	void selectionBackground();
	void avaliateType(QTableWidgetItem *);
	void validateItem();

signals:
    void focusReceived();
	void validatedItem(int, int, int);
	void validatedItem(int, int, double);

protected:
   // void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *, QEvent *);
	void keyPressEvent(QKeyEvent *event);
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
};

#endif // ETABLEWIDGET_H
