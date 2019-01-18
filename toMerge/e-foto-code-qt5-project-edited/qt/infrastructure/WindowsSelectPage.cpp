#include "WindowsSelectPage.h"

WindowsSelectPage::WindowsSelectPage(QString leftBox, QString rightBox, QWidget *parent) : QWizardPage(parent)
{
	//	setTitle(tr("Selection Images and Points"));
	//	setSubTitle(tr("Inform points and images which enter in calculation"));

	uncheckedListView = new QListView;
	checkedListView = new QListView;
	addAllButton = new QPushButton(tr(">>"));
	addButton = new QPushButton(tr(">"));
	removeButton = new QPushButton(tr("<"));
	removeAllButton = new QPushButton(tr("<<"));
	exchangeAllButton = new QPushButton(tr("<>"));

	//completeList = new QLineEdit;
	//registerField("WindowsList", completeList);

	connect(addAllButton, SIGNAL(clicked()),this, SLOT(addAllItems()));
	connect(addButton, SIGNAL(clicked()),this, SLOT(addSelectedItem()));
	connect(removeButton, SIGNAL(clicked()),this, SLOT(removeSelectedItem()));
	connect(removeAllButton, SIGNAL(clicked()),this, SLOT(removeAllItems()));
	connect(exchangeAllButton, SIGNAL(clicked()),this, SLOT(exchangeAllItems()));

	checkedModel = new QStringListModel();
	checkedListView->setModel(checkedModel);

	QFrame *group = new QFrame;

	QVBoxLayout *box = new QVBoxLayout;
	box->addStretch();
	box->addWidget(addAllButton);
	box->addWidget(addButton);
	box->addWidget(removeButton);
	box->addWidget(removeAllButton);
	box->addWidget(exchangeAllButton);
	box->addStretch();

	group->setLayout(box);

	box = new QVBoxLayout;
	box->addWidget(uncheckedListView);

	QGroupBox *group2 = new QGroupBox(leftBox);
	group2->setLayout(box);

	box = new QVBoxLayout;
	box->addWidget(checkedListView);

	QGroupBox *group3 = new QGroupBox(rightBox);
	group3->setLayout(box);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(group2,0,0);
	layout->addWidget(group,0,1);
	layout->addWidget(group3,0,2);
	setLayout(layout);
}

void WindowsSelectPage::addAllItems()
{
	QStringList empty;
	QStringList list = checkedModel->stringList();
	list = list + (uncheckedModel->stringList());
	checkedModel->setStringList(list);
	uncheckedModel->setStringList(empty);
	//	completeList->setText(checkedModel->stringList().join("#"));
}

void WindowsSelectPage::addSelectedItem()
{
	QModelIndex currentPosition = uncheckedListView->currentIndex();
	if (uncheckedListView->currentIndex().isValid())
	{
		QModelIndex selectedItem = uncheckedListView->currentIndex();
		QStringList checkedList = checkedModel->stringList();
		QStringList uncheckedList = uncheckedModel->stringList();
		checkedList.append(uncheckedModel->data(selectedItem, Qt::DisplayRole).toString());
		uncheckedList.removeAt(uncheckedList.indexOf(uncheckedModel->data(selectedItem, Qt::DisplayRole).toString()));
		checkedModel->setStringList(checkedList);
		uncheckedModel->setStringList(uncheckedList);
	}
	//	completeList->setText(checkedModel->stringList().join("#"));
}

void WindowsSelectPage::removeSelectedItem()
{
	QModelIndex currentPosition = checkedListView->currentIndex();
	if (checkedListView->currentIndex().isValid())
	{
		QModelIndex selectedItem = checkedListView->currentIndex();
		QStringList checkedList = checkedModel->stringList();
		QStringList uncheckedList = uncheckedModel->stringList();
		uncheckedList.append(checkedModel->data(selectedItem, Qt::DisplayRole).toString());
		checkedList.removeAt(checkedList.indexOf(checkedModel->data(selectedItem, Qt::DisplayRole).toString()));
		uncheckedModel->setStringList(uncheckedList);
		checkedModel->setStringList(checkedList);

	}
	//	completeList->setText(checkedModel->stringList().join("#"));
}

void WindowsSelectPage::removeAllItems()
{
	QStringList empty;
	QStringList list = checkedModel->stringList();
	list = list + (uncheckedModel->stringList());
	uncheckedModel->setStringList(list);
	checkedModel->setStringList(empty);
	//	completeList->setText(checkedModel->stringList().join("#"));
}

void WindowsSelectPage::exchangeAllItems()
{
	QStringList checkedList = checkedModel->stringList();
	QStringList uncheckedList = uncheckedModel->stringList();
	checkedModel->setStringList(uncheckedList);
	uncheckedModel->setStringList(checkedList);
	//	completeList->setText(checkedModel->stringList().join("#"));
}

void WindowsSelectPage::initializePage()
{

}

void WindowsSelectPage::setInitialList(QStringList list)
{
	uncheckedModel=new QStringListModel(list);
	uncheckedListView->setModel(uncheckedModel);
}

std::deque<std::string> WindowsSelectPage::getSelectedItens()
{
    std::deque<std::string> list;

	//QString str;
	//str.toStdString().c_str();
	//qDebug()<<checkedModel->stringList();
	int size=checkedModel->stringList().size();
	for (int i=0;i<size;i++)
        list.push_back(checkedModel->stringList().at(i).toLocal8Bit().constData());
	return list;
}
