#ifndef NORMALIZATIONVIEW_H
#define NORMALIZATIONVIEW_H

#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>
#include <string>
#include <QHeaderView>
#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDataWidgetMapper>

using namespace std;

class NormalizationModel : public QStandardItemModel 
{
public:
	NormalizationModel(string xmlnormalizationmodel);
	int numberpoints;
	void fillvalues(string xmlNormalization);
	QStandardItemModel *normalizedpairmodel;
	string getvalues();

	void showModel(void);
};

class NormalizationLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    NormalizationLayout(QWidget *);
    ~NormalizationLayout();
    
      QTableView *NormalizationTable;
      
      QVBoxLayout *columnLayout, *rowLayout;
      
      QHBoxLayout  *differenceHLayout, *meancolumnHLayout, *standardcolumnHLayout,
      			   *meanrowHLayout, *standardrowHLayout;
      
      QGroupBox *columnGroupBox, *rowGroupBox;
      
      QLabel *meancolumnLabel, *standardcolumnLabel,
      		 *meanrowLabel, *standardrowLabel;
      		 
      QLineEdit *meancolumnLineEdit, *standardcolumnLineEdit,
      		    *meanrowLineEdit, *standardrowLineEdit;		 
      
};

class NormalizationMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(NormalizationLayout* LayoutMapper, NormalizationModel *getMapperModel);
      
//public slots:
//      void changeText();
};       

class NormalizationView : public QWidget
{
      Q_OBJECT
      
public:
    NormalizationView(QWidget *parent = 0,  NormalizationModel* normalizationmodel=0);
    ~NormalizationView();
    
private:
    NormalizationLayout *normalizationlayout;
    NormalizationMapper normalizationmapper;
     
};

#endif
