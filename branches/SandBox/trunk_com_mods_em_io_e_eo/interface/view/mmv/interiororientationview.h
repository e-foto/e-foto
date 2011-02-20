#ifndef INTERIORORIENTAIONVIEW_H
#define INTERIORORIENTAIONVIEW_H

#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <QHeaderView>
#include <QVariant>

using namespace std;

class InteriorOrientationModel : public QStandardItemModel 
{
public:
	InteriorOrientationModel(string StrSensor);
	int NumberRows;
	void fillvalues(string StrFromSensor, string StrFromIO);
	string getvalues();
	void measure(string strkey, string strcolumn, string strrow);
	void showModel(void);
};

class InteriorOrientationLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    InteriorOrientationLayout(QWidget *);
    ~InteriorOrientationLayout();
    
      QTableView *InteriorOrientationTable;
      
      QVBoxLayout *tableLayout;
};

class InteriorOrientationView : public QWidget
{
      Q_OBJECT
      
public:
    InteriorOrientationView(QWidget *parent = 0,  InteriorOrientationModel* interiororientationmodel=0);
    ~InteriorOrientationView();
    
private:
    InteriorOrientationLayout * interiororientationlayout;
};

#endif
