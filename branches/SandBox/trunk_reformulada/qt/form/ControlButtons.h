#ifndef CONTROLBUTTONS_H
#define CONTROLBUTTONS_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>

class ControlButtons : public QWidget
{
    Q_OBJECT

public:

    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* newButton;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    QHBoxLayout* mainLayout;

    ControlButtons();

public slots:
    virtual void toSimpleMode();
    virtual void toSingleMode();
    virtual void toMultiMode();
    virtual void toEditMode();
    virtual void toInvisibleMode();
    virtual void disconnectAll();
    virtual void simpleConnect();
    virtual void singleConnect();
    virtual void multiConnect();
};

#endif // CONTROLBUTTONS_H
