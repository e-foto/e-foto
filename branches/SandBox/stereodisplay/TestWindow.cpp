#include "TestWindow.h"


InitWindow::InitWindow():
	QMainWindow()
{
	QWidget* widget = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout(widget);
	QPushButton* stereo = new QPushButton("Stereo");
	QPushButton* photoTri = new QPushButton("PhotoTri");
	layout->addWidget(stereo);
	layout->addWidget(photoTri);
	widget->setLayout(layout);
	setCentralWidget(widget);
	connect(stereo,SIGNAL(clicked()),this,SLOT(startTestStereo()));
	connect(photoTri,SIGNAL(clicked()),this,SLOT(startTestPT()));
}
void InitWindow::startTestStereo()
{
	TestStereoWindow* ts = new TestStereoWindow();
	ts->showMaximized();
	close();
}
void InitWindow::startTestPT()
{
	TestPTWindow* tpt = new TestPTWindow();
	tpt->showMaximized();
	close();
}



// TestStereoWindow

TestStereoWindow::TestStereoWindow():
	QMainWindow()
{
	StereoService ss;
	StereoView* sv = ss.getStereoView(this, "", "");
	sd = ss.getStereoDisplay(this, sv);

	ld = sd->getLeftDisplay();
	rd = sd->getRightDisplay();

	DetailDisplay* ldd = ld->getDetailDisplay();
	DetailDisplay* rdd = rd->getDetailDisplay();

	OverviewDisplay* lod = ld->getOverviewDisplay();
	OverviewDisplay* rod = rd->getOverviewDisplay();

	QDockWidget* stereoDock = new QDockWidget("Stereo Display");
	stereoDock->setWidget(sd);
	stereoDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	setCentralWidget(stereoDock);

	//setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
	//setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

	QDockWidget* overviewLDock = new QDockWidget("Overview Display");
	QDockWidget* overviewRDock = new QDockWidget("Overview Display");
	overviewLDock->setWidget(lod);
	overviewRDock->setWidget(rod);
	overviewLDock->setMaximumSize(200,200);
	overviewRDock->setMaximumSize(200,200);
	overviewLDock->setMinimumSize(200,200);
	overviewRDock->setMinimumSize(200,200);

	QDockWidget* leftDock = new QDockWidget("Left Image Display");
	leftDock->setWidget(ld);

	QDockWidget* rightDock = new QDockWidget("Right Image Display");
	rightDock->setWidget(rd);

	QDockWidget* detailLDock = new QDockWidget("Detail Display");
	QDockWidget* detailRDock = new QDockWidget("Detail Display");
	detailLDock->setWidget(ldd);
	detailRDock->setWidget(rdd);

	addDockWidget(Qt::BottomDockWidgetArea,overviewLDock);
	addDockWidget(Qt::BottomDockWidgetArea,leftDock);
	addDockWidget(Qt::BottomDockWidgetArea,detailLDock);
	addDockWidget(Qt::BottomDockWidgetArea,detailRDock);
	addDockWidget(Qt::BottomDockWidgetArea,rightDock);
	addDockWidget(Qt::BottomDockWidgetArea,overviewRDock);

	setStatusBar(new QStatusBar(this));
	connect(sd,SIGNAL(mousePositionsChanged(QPointF*,QPointF*)),this,SLOT(changeStatusBar(QPointF*,QPointF*)));
	connect(ld,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(changeStatusBar(QPointF*)));
	connect(rd,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(changeStatusBar(QPointF*)));
	connect(sd,SIGNAL(mouseClicked(QPointF*,QPointF*)),this,SLOT(changeStatusBarOnClick(QPointF*,QPointF*)));
	connect(ld,SIGNAL(mouseClicked(QPointF*)),this,SLOT(changeStatusBarOnClickLeft(QPointF*)));
	connect(rd,SIGNAL(mouseClicked(QPointF*)),this,SLOT(changeStatusBarOnClickRight(QPointF*)));

	QAction* leftLoadAction = new QAction(QString::fromUtf8("Open left"),this);
	QAction* rightLoadAction = new QAction(QString::fromUtf8("Open right"),this);
	QAction* stereoAction = new QAction(QString::fromUtf8("View stereo"),this);
	stereoAction->setCheckable(true);
	stereoAction->setChecked(true);
	QAction* cloneScaleAction = new QAction(QString::fromUtf8("Clone scale"),this);
	cloneScaleAction->setCheckable(true);
	cloneScaleAction->setChecked(true);
	connect(leftLoadAction,SIGNAL(triggered()),this,SLOT(leftLoad()));
	connect(rightLoadAction,SIGNAL(triggered()),this,SLOT(rightLoad()));
	connect(stereoAction,SIGNAL(toggled(bool)),centralWidget(),SLOT(setVisible(bool)));
	connect(cloneScaleAction,SIGNAL(toggled(bool)),ld,SLOT(setCloneScale(bool)));
	connect(cloneScaleAction,SIGNAL(toggled(bool)),rd,SLOT(setCloneScale(bool)));

	QLabel* detailZoomLabel = new QLabel("Detail zoom");
	QComboBox* detailComboBox = new QComboBox();
	detailComboBox->addItems(QString("1x 2x 4x 8x 16x").split(" "));
	detailComboBox->setCurrentIndex(1);
	detailZoom = 2.0;
	connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDetailZoom(int)));

	QToolBar* tool = addToolBar(tr("Tool"));
	tool->addAction(leftLoadAction);
	tool->addAction(rightLoadAction);
	tool->addSeparator();
	tool->addAction(stereoAction);
	tool->addAction(cloneScaleAction);
	tool->addSeparator();
	tool->addWidget(detailZoomLabel);
	tool->addWidget(detailComboBox);
	addToolBar(Qt::TopToolBarArea,tool);
}

void TestStereoWindow::changeStatusBar(QPointF* p)
{
	if (p)
		statusBar()->showMessage(QString::fromUtf8("ModoDisplay diz que está medindo a coordenada ").append(QString::number(p->x())).append("x").append(QString::number(p->y())));
}

void TestStereoWindow::changeStatusBar(QPointF* pl, QPointF* pr)
{
	if (pl && pr)
		statusBar()->showMessage(QString::fromUtf8("StereoDisplay diz que está medindo as coordenadas ").append(QString::number(pl->x())).append("x").append(QString::number(pl->y())).append(" e ").append(QString::number(pr->x())).append("x").append(QString::number(pr->y())));
	else if (pl)
		statusBar()->showMessage(QString::fromUtf8("StereoDisplay diz que está medindo as coordenadas ").append(QString::number(pl->x())).append("x").append(QString::number(pl->y())).append(" e ").append("?x?"));
	else if (pr)
		statusBar()->showMessage(QString::fromUtf8("StereoDisplay diz que está medindo as coordenadas ").append("?x?").append(" e ").append(QString::number(pr->x())).append("x").append(QString::number(pr->y())));
}

void TestStereoWindow::changeStatusBarOnClickLeft(QPointF* p)
{
	QSize ls = sd->getCurrentView()->getLeftView()->imageSize();

	if (p)
	{
		double x = (p->x() >= 0 && p->x() <= ls.width() - 1) ? p->x() : -1;
		double y = (p->y() >= 0 && p->y() <= ls.height() - 1) ? p->y() : -1;
		statusBar()->showMessage(QString::fromUtf8("Clique na imagem da esquerda em ").append(QString::number(x)).append("x").append(QString::number(y)));
	}
}

void TestStereoWindow::changeStatusBarOnClickRight(QPointF* p)
{
	QSize rs = sd->getCurrentView()->getRightView()->imageSize();

	if (p)
	{
		double x = (p->x() >= 0 && p->x() <= rs.width() - 1) ? p->x() : -1;
		double y = (p->y() >= 0 && p->y() <= rs.height() - 1) ? p->y() : -1;
		statusBar()->showMessage(QString::fromUtf8("Clique na imagem da direita em ").append(QString::number(x)).append("x").append(QString::number(y)));
	}
}

void TestStereoWindow::changeStatusBarOnClick(QPointF* pl, QPointF* pr)
{
	QSize ls = sd->getCurrentView()->getLeftView()->imageSize();
	QSize rs = sd->getCurrentView()->getRightView()->imageSize();

	double lx, ly, rx, ry;
	if (pl)
	{
		lx = (pl->x() >= 0 && pl->x() <= ls.width() - 1) ? pl->x() : -1;
		ly = (pl->y() >= 0 && pl->y() <= ls.height() - 1) ? pl->y() : -1;
	}
	if (pr)
	{
		rx = (pr->x() >= 0 && pr->x() <= rs.width() - 1) ? pr->x() : -1;
		ry = (pr->y() >= 0 && pr->y() <= rs.height() - 1) ? pr->y() : -1;
	}

	if (pl && pr)
		statusBar()->showMessage(QString::fromUtf8("Clique em ").append(QString::number(lx)).append("x").append(QString::number(ly)).append(" e ").append(QString::number(rx)).append("x").append(QString::number(ry)));
	else if (pl)
		statusBar()->showMessage(QString::fromUtf8("Clique em ").append(QString::number(lx)).append("x").append(QString::number(ly)).append(" e ").append("?x?"));
	else if (pr)
		statusBar()->showMessage(QString::fromUtf8("Clique em ").append("?x?").append(" e ").append(QString::number(rx)).append("x").append(QString::number(ry)));
}

void TestStereoWindow::changeDetailZoom(int nz)
{
	switch (nz){
	case 0:
		ld->setDetailZoom(1.0);
		rd->setDetailZoom(1.0);
		break;
	case 1:
		ld->setDetailZoom(2.0);
		rd->setDetailZoom(2.0);
		break;
	case 2:
		ld->setDetailZoom(4.0);
		rd->setDetailZoom(4.0);
		break;
	case 3:
		ld->setDetailZoom(8.0);
		rd->setDetailZoom(8.0);
		break;
	case 4:
		ld->setDetailZoom(16.0);
		rd->setDetailZoom(16.0);
		break;
	default :
		ld->setDetailZoom(2.0);
		rd->setDetailZoom(2.0);
		break;
	}
}

void TestStereoWindow::leftLoad()
{
	// Realiza o dialogo para decidir a imagem que será aberta
	QString filename = QFileDialog::getOpenFileName(this, "Open left image", "", "*.tif *.png *.bmp *.jpg *.ppm");
	if (filename.isEmpty())
		return;

	sd->getCurrentView()->getLeftView()->loadImage(filename);
}

void TestStereoWindow::rightLoad()
{
	// Realiza o dialogo para decidir a imagem que será aberta
	QString filename = QFileDialog::getOpenFileName(this, "Open right image", "", "*.tif *.png *.bmp *.jpg *.ppm");
	if (filename.isEmpty())
		return;

	sd->getCurrentView()->getRightView()->loadImage(filename);
	sd->updateAll();
}



// TestPTWindow

TestPTWindow::TestPTWindow():
	QMainWindow()
{
	StereoService ss;
	MonoView* lmv = ss.getMonoView(this,"");
	MonoView* rmv = ss.getMonoView(this,"");
	ld = ss.getMonoDisplay(this,lmv);
	rd = ss.getMonoDisplay(this,rmv);

	DetailDisplay* ldd = ld->getDetailDisplay();
	DetailDisplay* rdd = rd->getDetailDisplay();

	OverviewDisplay* lod = ld->getOverviewDisplay();
	OverviewDisplay* rod = rd->getOverviewDisplay();

	QDockWidget* overviewLDock = new QDockWidget("Overview Display");
	QDockWidget* overviewRDock = new QDockWidget("Overview Display");
	overviewLDock->setWidget(lod);
	overviewRDock->setWidget(rod);
	overviewLDock->setMaximumSize(200,200);
	overviewRDock->setMaximumSize(200,200);
	overviewLDock->setMinimumSize(200,200);
	overviewRDock->setMinimumSize(200,200);

	QDockWidget* uniDock = new QDockWidget("Images Display");
	QWidget* uni = new QWidget(this);
	QHBoxLayout* layout = new QHBoxLayout(uni);
	uni->setLayout(layout);
	layout->addWidget(ld);
	layout->addWidget(rd);
	uniDock->setWidget(uni);

	setCentralWidget(uniDock);

	QDockWidget* detailLDock = new QDockWidget("Detail Display");
	QDockWidget* detailRDock = new QDockWidget("Detail Display");
	detailLDock->setWidget(ldd);
	detailRDock->setWidget(rdd);

	addDockWidget(Qt::TopDockWidgetArea,overviewLDock);
	addDockWidget(Qt::TopDockWidgetArea,detailLDock);
	addDockWidget(Qt::TopDockWidgetArea,detailRDock);
	addDockWidget(Qt::TopDockWidgetArea,overviewRDock);

	setStatusBar(new QStatusBar(this));
	connect(ld,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(changeStatusBar(QPointF*)));
	connect(rd,SIGNAL(mousePositionChanged(QPointF*)),this,SLOT(changeStatusBar(QPointF*)));
	connect(ld,SIGNAL(mouseClicked(QPointF*)),this,SLOT(changeStatusBarOnClickLeft(QPointF*)));
	connect(rd,SIGNAL(mouseClicked(QPointF*)),this,SLOT(changeStatusBarOnClickRight(QPointF*)));

	QAction* leftLoadAction = new QAction(QString::fromUtf8("Open left"),this);
	QAction* rightLoadAction = new QAction(QString::fromUtf8("Open right"),this);
	connect(leftLoadAction,SIGNAL(triggered()),this,SLOT(leftLoad()));
	connect(rightLoadAction,SIGNAL(triggered()),this,SLOT(rightLoad()));

	QLabel* detailZoomLabel = new QLabel("Detail zoom");
	QComboBox* detailComboBox = new QComboBox();
	detailComboBox->addItems(QString("1x 2x 4x 8x 16x").split(" "));
	detailComboBox->setCurrentIndex(1);
	detailZoom = 2.0;
	connect(detailComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDetailZoom(int)));

	QToolBar* tool = addToolBar(tr("Tool"));
	tool->addAction(leftLoadAction);
	tool->addAction(rightLoadAction);
	tool->addSeparator();
	tool->addWidget(detailZoomLabel);
	tool->addWidget(detailComboBox);
	addToolBar(Qt::TopToolBarArea,tool);
}

void TestPTWindow::changeStatusBar(QPointF* p)
{
	if (p)
		statusBar()->showMessage(QString::fromUtf8("ModoDisplay diz que está medindo a coordenada ").append(QString::number(p->x())).append("x").append(QString::number(p->y())));
}

void TestPTWindow::changeStatusBarOnClickLeft(QPointF* p)
{
	QSize ls = ld->getCurrentView()->imageSize();

	if (p)
	{
		double x = (p->x() >= 0 && p->x() <= ls.width() - 1) ? p->x() : -1;
		double y = (p->y() >= 0 && p->y() <= ls.height() - 1) ? p->y() : -1;
		statusBar()->showMessage(QString::fromUtf8("Clique na imagem da esquerda em ").append(QString::number(x)).append("x").append(QString::number(y)));
	}
}

void TestPTWindow::changeStatusBarOnClickRight(QPointF* p)
{
	QSize rs = rd->getCurrentView()->imageSize();

	if (p)
	{
		double x = (p->x() >= 0 && p->x() <= rs.width() - 1) ? p->x() : -1;
		double y = (p->y() >= 0 && p->y() <= rs.height() - 1) ? p->y() : -1;
		statusBar()->showMessage(QString::fromUtf8("Clique na imagem da direita em ").append(QString::number(x)).append("x").append(QString::number(y)));
	}
}

void TestPTWindow::changeDetailZoom(int nz)
{
	switch (nz){
	case 0:
		ld->setDetailZoom(1.0);
		rd->setDetailZoom(1.0);
		break;
	case 1:
		ld->setDetailZoom(2.0);
		rd->setDetailZoom(2.0);
		break;
	case 2:
		ld->setDetailZoom(4.0);
		rd->setDetailZoom(4.0);
		break;
	case 3:
		ld->setDetailZoom(8.0);
		rd->setDetailZoom(8.0);
		break;
	case 4:
		ld->setDetailZoom(16.0);
		rd->setDetailZoom(16.0);
		break;
	default :
		ld->setDetailZoom(2.0);
		rd->setDetailZoom(2.0);
		break;
	}
}

void TestPTWindow::leftLoad()
{
	// Realiza o dialogo para decidir a imagem que será aberta
	QString filename = QFileDialog::getOpenFileName(this, "Open left image", "", "*.tif *.png *.bmp *.jpg *.ppm");
	if (filename.isEmpty())
		return;

	ld->getCurrentView()->loadImage(filename);
	ld->updateAll();
}

void TestPTWindow::rightLoad()
{
	// Realiza o dialogo para decidir a imagem que será aberta
	QString filename = QFileDialog::getOpenFileName(this, "Open right image", "", "*.tif *.png *.bmp *.jpg *.ppm");
	if (filename.isEmpty())
		return;

	rd->getCurrentView()->loadImage(filename);
	rd->updateAll();
}
