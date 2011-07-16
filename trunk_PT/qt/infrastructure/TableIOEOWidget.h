#ifndef TABLEIOEOWIDGET_H
#define TABLEIOEOWIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QLabel>
#include "EDom.h"
#include "Matrix.h"


class TableIOEOWidget: public QTableWidget
{
    Q_OBJECT
private:
    Matrix *io;
	char mode;
	int decimals;

public:
    TableIOEOWidget(QWidget *parent=0);
    TableIOEOWidget(Matrix values,QWidget *parent=0);
    void setTableData(Matrix values,char mode='e',int precision=6);
    void setTableData(EDomElement xml,char mode='e',int precision=6);
    void setTableData(string xmlString,char mode='e',int precision=6);
	void setVerticalHeadersLabelsHtml(QStringList list);
	void resizeTable();
    void enableAutoCopy(bool enable=true);

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

signals:
    void focusReceived();

protected:
   // void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *, QEvent *);
    void focusInEvent(QFocusEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
};

#endif // TABLEIOEOWIDGET_H
