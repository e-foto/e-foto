#ifndef POINTVIEW_H
#define POINTVIEW_H

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QString>
#include <iostream>
#include <QGroupBox>
#include <string>

using namespace std;

class PointModel : public QStandardItemModel 
{
public:
	PointModel();
	void fillvalues(string strkeypoint, string xmlPointValues, string strunit);
	string getvalues();
	void showModel(void);
	string strunitvalue, strimgmeasurements, strkeyvalue;
};

class PointLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    PointLayout(QWidget *);
    ~PointLayout();
    
      QLabel *gcpIDLabel, *typeLabel, *descriptionLabel, *spatialCoord01Label,
             *spatialCoord02Label, *spatialCoord03Label,  *sigmaLabel;
      
      QLineEdit *gcpIDLineEdit, *descriptionLineEdit, *spatialCoord01LineEdit,
                *spatialCoord02LineEdit, *spatialCoord03LineEdit,
                *sigmaLineEdit;
                
	  QGroupBox *spatialCoordGroupBox;
      
      QComboBox *typeComboBox;
      
      QHBoxLayout *gcpIDLayout,*typeLayout, *descriptionLayout,
                  *spatialCoordHLayout, *sigmaLayout;
      
      QVBoxLayout *spatialCoordVLayout;
      
};

class PointMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(PointLayout* getlayoutMapper, PointModel *getMapperModel);
};       
    
class PointView : public QWidget
{
      Q_OBJECT
      
public:
    PointView(QWidget *parent = 0,  PointModel* pointmodel=0);
    ~PointView();
    
private:
    QVariant indexGridsType;
    PointLayout *pointlayout;
    PointMapper pointmapper;
    
private slots:
    void changeText();        
};

#endif
