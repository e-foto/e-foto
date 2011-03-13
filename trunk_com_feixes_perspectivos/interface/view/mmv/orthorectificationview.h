#ifndef ORTHORECTIFICATION_H
#define ORTHORECTIFICATION_H

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <iostream>
#include <QGroupBox>
#include <string>

using namespace std;

class OrthorectificationModel : public QStandardItemModel 
{
public:
	OrthorectificationModel();
	void fillvalues(string xmlOrthorectificationValues);
	string getvalues();
	void showModel(void);
	string strorthoimagekey;
};

class OrthorectificationLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    OrthorectificationLayout(QWidget *, string strunitlabel);
    ~OrthorectificationLayout();
    
      QLabel *filePathLabel, *fileNameLabel, *upperleftCoordXLabel, *upperleftCoordYLabel,
      		 *upperleftCoordZLabel, *bottomrightCoordXLabel, *bottomrightCoordYLabel,
      		 *bottomrightCoordZLabel, *GSDLabel, *greylevelLabel;
      
      QLineEdit *filePathLineEdit, *fileNameLineEdit, *upperleftCoordXLineEdit, *upperleftCoordYLineEdit,
      		    *upperleftCoordZLineEdit, *bottomrightCoordXLineEdit, *bottomrightCoordYLineEdit,
      		    *bottomrightCoordZLineEdit, *GSDLineEdit;
                
      QPushButton *importPushButton, *createPushButton;               
               
      QGroupBox *upperLeftGroupBox, *bottomRightGroupBox;
      
      QComboBox *greylevelComboBox;
      
      QHBoxLayout *filePathLayout,*fileNameLayout, *GSDLayout,
                  *upperLeftCoordLayout, *bottomRightCoordLayout, *greylevelLayout;
      
      QString strcoord01, strcoord02, strcoord03;
 
};

class OrthorectificationMapper : public QDataWidgetMapper
{
	  Q_OBJECT
public:
      void makeMapper(OrthorectificationLayout* getlayoutMapper, OrthorectificationModel *getMapperModel);
};       
    
class OrthorectificationView : public QWidget
{
      Q_OBJECT
      
public:
    OrthorectificationView(QWidget *parent = 0,  OrthorectificationModel* orthorectificationmodel=0, string strunit=0);
    ~OrthorectificationView();
    
private:
    QVariant indexGridsType;
    OrthorectificationLayout *orthorectificationlayout;
    OrthorectificationMapper orthorectificationmapper;
    
private slots:
    void changeText(); 
    void import();       
    void create();
};

#endif
