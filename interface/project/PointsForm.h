#ifndef POINTSFORM_H
#define POINTSFORM_H

#include "AbstractForm.h"
#include "pointsview.h"
#include <string>

class PointsForm : public AbstractForm //, public Ui::Form
{
    Q_OBJECT

public:
   // PointsForm(QWidget *parent = 0);
    PointsForm(QWidget *parent = 0, string XmlString="");

    // o destrutor precisa ser melhorado para destruir também os layouts
    ~PointsForm()
        {
     //   cout<< " ########## entrou no destrutor";
                delete psinglepointview2;
            delete MyProxyModel;
            delete ppointsview;
            delete ppointsmodel;}

    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
    bool isForm(string formName);
private:

    PointsModel* ppointsmodel;
    PointsView* ppointsview;
    SinglePointModel* MyProxyModel;
    SinglePointView* psinglepointview2;

public slots:
    void emitSignal(QModelIndex myIndex);

signals:

    void clicked(int pointKey);

};

#endif // POINTSFORM_H
