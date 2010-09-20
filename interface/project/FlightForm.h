#ifndef FLIGHTFORM_H
#define FLIGHTFORM_H

#include "AbstractForm.h"
#include "flightview.h"
#include <string>

class FlightForm : public AbstractForm
{
    Q_OBJECT

public:
   
    FlightForm(QWidget *parent = 0, string XmlString="");
    
    ~FlightForm();
    
    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
	bool isForm(string formName);
private:

    FlightView* pflightview;
};

#endif // FLIGHTFORM_H
