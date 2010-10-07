#ifndef SENSORFORM_H
#define SENSORFORM_H

#include "ui_SensorLayout.h"
#include "AbstractForm.h"
#include "SigmaForm.h"
#include "EDom.h"

class SensorForm : public AbstractForm, public Ui::SensorView
{
    Q_OBJECT

	SigmaFormController* calibratedSigmaController;
	SigmaFormController* fid0SigmaController;
	SigmaFormController* fid1SigmaController;
	SigmaFormController* fid2SigmaController;
	SigmaFormController* fid3SigmaController;
	SigmaFormController* fid4SigmaController;
	SigmaFormController* fid5SigmaController;
	SigmaFormController* fid6SigmaController;
	SigmaFormController* fid7SigmaController;
	SigmaFormController* radialSigmaController;
	SigmaFormController* decenteredSigmaController;
	SigmaFormController* principalSigmaController;

public:
    EDomElement ede;
    SensorForm(QWidget *parent =0);

    void fillvalues(string values);
    string getvalues();
    void setReadOnly(bool state);
    bool isForm(string formName);
    void showLast4Fiductial(bool state);

public slots:
	void detectorGroup(int);
	void fiductialGroup(int);
	//void setSigmasVisible(bool);

};

#endif // SENSORFORM_H
