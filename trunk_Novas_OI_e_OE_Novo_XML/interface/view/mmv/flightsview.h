#ifndef FLIGHTSVIEW_H
#define FLIGHTSVIEW_H

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QString>
#include <iostream>
#include <string>
#include <QDialog>

using namespace std;

class FlightsModel : public QStandardItemModel 
{
public:
	FlightsModel(void);
	void fillvalues(string xmlFlightsValues);
	string getvalues();
	void showModel(void);
};

class FlightsLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    FlightsLayout(QWidget *);
    ~FlightsLayout();
     
    QLineEdit 	*FlightsNameLineEdit, *ProducerNameLineEdit, *FlightHeightLineEdit, 
    			*NominalScaleLineEdit;

	QDateEdit *ExecutionDateTimeEdit;

	QLabel 		*ProducerNameLabel, *FlightHeightLabel, *NominalScaleLabel,
				*ExecutionDateLabel, *LongLabel, *TransvLabel, *FlightsNameLabel;
	
	QSpinBox *LongSpinBox, *TransvSpinBox;
				
	QGroupBox   *OverlapGroupBox;
	
	QHBoxLayout *LongHorizLayout, *TransvHorizLayout, *ProducerHorizLayout,
                *ExecutiondateHorizLayout, *NominalScaleHorizLayout,
                *FlightHeightHorizLayout,*LocalFlightLayout,*OverlapLayout;
                
    QVBoxLayout  *FlightsVLayout;           
};

class FlightsMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(FlightsLayout* LayoutMapper, FlightsModel *getMapperModel);
public slots:
      void changeText();
};       
    
class FlightsView : public QWidget
{
      Q_OBJECT
      
public:
    FlightsView(QWidget *parent = 0,  FlightsModel* flightsmodel=0);
    ~FlightsView();
    
private:
    FlightsLayout * flightslayout;
    FlightsMapper flightsmapper;
};

#endif
