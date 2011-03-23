#ifndef FLIGHTFORM_H
#define FLIGHTFORM_H

#include "ui_FlightLayout.h"
#include "AbstractForm.h"
//#include "EDom.h"

class FlightForm : public AbstractForm, public Ui::FlightForm
{
        Q_OBJECT

public:
        FlightForm(QWidget *parent = 0);

		//void fillvalues(string values);
		//string getvalues();
        void setReadOnly(bool state);
        void setFormLocale(QLocale locale);
        void cleanForm();
        bool isForm(string formName);
		~FlightForm();
};

#endif //FLIGHTFORM_H
