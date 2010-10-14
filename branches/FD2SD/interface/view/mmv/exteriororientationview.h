#ifndef EXTERIORORIENTAIONVIEW_H
#define EXTERIORORIENTAIONVIEW_H

#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <QHeaderView>
#include <QVariant>

using namespace std;

class ExteriorOrientationModel : public QStandardItemModel 
{
public:
	ExteriorOrientationModel(string numberkey, string nrgcp_id, string unit);
	int numberpoints;
	QString key;
	void fillvalues(string numberkey, string xmlefoto);
	void measure(string strID, string strcolumn, string strrow);
	QStandardItemModel *startDataOE;
	string getvalues();

	void showModel(void);
};

class ExteriorOrientationLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    ExteriorOrientationLayout(QWidget *);
    ~ExteriorOrientationLayout();
    
      QTableView *ExteriorOrientationTable;
      
      QVBoxLayout *tableLayout;
};

class ExteriorOrientationView : public QWidget
{
      Q_OBJECT
      
public:
    ExteriorOrientationView(QWidget *parent = 0,  ExteriorOrientationModel* exteriororientationmodel=0);
    ~ExteriorOrientationView();
    
private:
    ExteriorOrientationLayout * exteriororientationlayout;
    
};

#endif
