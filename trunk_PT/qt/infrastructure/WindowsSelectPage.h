#ifndef WINDOWSSELECTPAGE_H
#define WINDOWSSELECTPAGE_H

#include <QWidget>
#include <QString>
#include <QtGui>
#include <deque>

using namespace std;

class WindowsSelectPage : public QWizardPage
{
	Q_OBJECT

	public:
		WindowsSelectPage(QString leftBox, QString rightBox,QWidget *parent = 0);
		void setInitialList(QStringList list);
		deque<string> getSelectedItens();
		void initializePage();
	protected:


	private slots:
		void addAllItems();
		void addSelectedItem();
		void removeSelectedItem();
		void removeAllItems();
		void exchangeAllItems();

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
