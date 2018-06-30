#ifndef SIGMAFORM_H
#define SIGMAFORM_H

#include <QWidget>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>

#include "Matrix.h"
#include "EDom.h"
#include "ScienceSpinBox.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class SigmaFormController: public QObject
{
	Q_OBJECT

public:
	SigmaFormController();
    explicit SigmaFormController(std::string myMode, unsigned int myDimension = 1);
    explicit SigmaFormController(Matrix myValues);

public:
	QWidget* getContent();
    void setDirection(std::string newDirection);
    void setMode(std::string newMode);
    std::string getMode();
    void fillValues(std::string xml);
    std::string getValues();
    bool getValidate();

private:
    void init();
    QWidget* contentWidget{nullptr};
    QList<QLabel*> labels{};
    QList<ScienceSpinBox*> edits{};
    QStringList titles{};
    unsigned int dimension{0};
    std::string direction{""};
    std::string mode{""};
    Matrix values{};

public slots:
	virtual void toMode(QString newMode);
	virtual void toNotAvailable();
	virtual void toStDev();
    virtual void toMatrix();
    virtual void setReadOnly(bool);
    virtual void changeValidate(QString);

signals:
	void changeMatrixButtonVisibility(bool);
	void changeToReadOnly(bool);
	void changeToMode(QString);
	void validateChanged();
};

class SigmaFormTypeSelector: public QComboBox
{
	Q_OBJECT
private:
    SigmaFormController* controller{nullptr};
public:
    explicit SigmaFormTypeSelector(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
	void blockCovarianceMatrixOption();
public slots:
	virtual void toMode(QString);
};

class SigmaFormContent: public QFrame
{
	Q_OBJECT
private:
    SigmaFormController* controller{nullptr};
public:
    explicit SigmaFormContent(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
};

class SigmaFormDialogButton: public QPushButton
{
	Q_OBJECT
private:
    SigmaFormController* controller{nullptr};
public:
    explicit SigmaFormDialogButton(QWidget * parent = 0);
	void setSigmaFormController(SigmaFormController* newController);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // SIGMAFORM_H
