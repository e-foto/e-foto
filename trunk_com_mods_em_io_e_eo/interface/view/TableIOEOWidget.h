#ifndef TABLEIOEOWIDGET_H
#define TABLEIOEOWIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QResizeEvent>

#include "../../../infrastructure/Matrix.h"
class TableIOEOWidget: public QTableWidget
{
    Q_OBJECT
private:
    Matrix *io;

public:
    TableIOEOWidget(QWidget *parent=0);
    TableIOEOWidget(Matrix values,QWidget *parent=0);
    void setTableData(Matrix values,char mode='e',int precision=6);
    void setTableData(EDomElement xml,char mode='e',int precision=6);
    void setTableData(string xmlString,char mode='e',int precision=6);

    void resizeTable();

public slots:
    void autoCopy();

signals:
    void focusReceived();

protected:
   // void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *, QEvent *);
    void focusInEvent(QFocusEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
};

#endif // TABLEIOEOWIDGET_H
