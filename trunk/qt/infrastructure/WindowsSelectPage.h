#ifndef WINDOWSSELECTPAGE_H
#define WINDOWSSELECTPAGE_H

#include <QWidget>
#include <QString>
#include <QtGui>
#include <deque>

class WindowsSelectPage : public QWizardPage
{
	Q_OBJECT

public:
	WindowsSelectPage(QString leftBox, QString rightBox,QWidget *parent = 0);
	void setInitialList(QStringList list);
    std::deque<std::string> getSelectedItens();
	void initializePage();

public slots :
	void addAllItems();
	void addSelectedItem();
	void removeSelectedItem();
	void removeAllItems();
	void exchangeAllItems();
protected:

private:
	QStringListModel *uncheckedModel;
	QListView *checkedListView;
	QListView *uncheckedListView;
	QPushButton *itemAddButton;
	QPushButton *addAllButton;
	QPushButton *addButton;
	QPushButton *removeButton;
	QPushButton *removeAllButton;
	QPushButton *exchangeAllButton;
	QStringListModel *checkedModel;
};

#endif // WINDOWSSELECTPAGE_H
