#include "TestWindow.h"

TestWindow::TestWindow():
	QMainWindow()
{
	StereoService ss;
	sv = ss.getStereoView(this, "", "");
	StereoDisplay* sd = ss.getStereoDisplay(this, sv);

	MonoDisplay* ld = sd->getLeftDisplay();
	MonoDisplay* rd = sd->getRightDisplay();

	DetailDisplay* ldd = ld->getDetailDisplay();
	DetailDisplay* rdd = rd->getDetailDisplay();

	OverviewDisplay* lod = ld->getOverviewDisplay();
	OverviewDisplay* rod = rd->getOverviewDisplay();

	QDockWidget* stereoDock = new QDockWidget("Stereo Display");
	stereoDock->setWidget(sd);
	stereoDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	setCentralWidget(stereoDock);

	setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
	setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

	QDockWidget* overviewLDock = new QDockWidget("Overview Display");
	QDockWidget* overviewRDock = new QDockWidget("Overview Display");
	overviewLDock->setWidget(lod);
	overviewRDock->setWidget(rod);
	overviewLDock->setMaximumSize(200,200);
	overviewRDock->setMaximumSize(200,200);
	overviewLDock->setMinimumSize(200,200);
	overviewRDock->setMinimumSize(200,200);
	addDockWidget(Qt::LeftDockWidgetArea,overviewLDock);
	addDockWidget(Qt::RightDockWidgetArea,overviewRDock);

	QDockWidget* leftDock = new QDockWidget("Left Image Display");
	leftDock->setWidget(ld);
	addDockWidget(Qt::BottomDockWidgetArea,leftDock);

	QDockWidget* rightDock = new QDockWidget("Right Image Display");
	rightDock->setWidget(rd);
	addDockWidget(Qt::BottomDockWidgetArea,rightDock);

	QDockWidget* detailLDock = new QDockWidget("Detail Display");
	QDockWidget* detailRDock = new QDockWidget("Detail Display");
	detailLDock->setWidget(ldd);
	detailRDock->setWidget(rdd);
	addDockWidget(Qt::LeftDockWidgetArea,detailLDock);
	addDockWidget(Qt::RightDockWidgetArea,detailRDock);

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
	connect(leftLoadAction,SIGNAL(triggered()),this,SLOT(leftLoad()));
	connect(rightLoadAction,SIGNAL(triggered()),this,SLOT(rightLoad()));
	connect(stereoAction,SIGNAL(toggled(bool)),centralWidget(),SLOT(setVisible(bool)));

	QToolBar* tool = addToolBar(tr("Tool"));
	tool->addAction(leftLoadAction);
	tool->addAction(rightLoadAction);
	tool->addAction(stereoAction);
	addToolBar(Qt::TopToolBarArea,tool);
}

void TestWindow::changeStatusBar(QPointF* p)
{
	if (p)
		statusBar()->showMessage(QString::fromUtf8("ModoDisplay diz que está medindo a coordenada ").append(QString::number(p->x())).append("x").append(QString::number(p->y())));
}

void TestWindow::changeStatusBar(QPointF* pl, QPointF* pr)
{
	if (pl && pr)
		statusBar()->showMessage(QString::fromUtf8("StereoDisplay diz que está medindo as coordenadas ").append(QString::number(pl->x())).append("x").append(QString::number(pl->y())).append(" e ").append(QString::number(pr->x())).append("x").append(QString::number(pr->y())));
	else if (pl)
		statusBar()->showMessage(QString::fromUtf8("StereoDisplay diz que está medindo as coordenadas ").append(QString::number(pl->x())).append("x").append(QString::number(pl->y())).append(" e ").append("?x?"));
	else if (pr)
		statusBar()->showMessage(QString::fromUtf8("StereoDisplay diz que está medindo as coordenadas ").append("?x?").append(" e ").append(QString::number(pr->x())).append("x").append(QString::number(pr->y())));
}

void TestWindow::changeStatusBarOnClickLeft(QPointF* p)
{
	QSize ls = sv->getLeftView()->imageSize();

	if (p)
	{
		double x = (p->x() >= 0 && p->x() <= ls.width() - 1) ? p->x() : -1;
		double y = (p->y() >= 0 && p->y() <= ls.height() - 1) ? p->y() : -1;
		statusBar()->showMessage(QString::fromUtf8("Clique na imagem da esquerda em ").append(QString::number(x)).append("x").append(QString::number(y)));
	}
}

void TestWindow::changeStatusBarOnClickRight(QPointF* p)
{
	QSize rs = sv->getRightView()->imageSize();

	if (p)
	{
		double x = (p->x() >= 0 && p->x() <= rs.width() - 1) ? p->x() : -1;
		double y = (p->y() >= 0 && p->y() <= rs.height() - 1) ? p->y() : -1;
		statusBar()->showMessage(QString::fromUtf8("Clique na imagem da direita em ").append(QString::number(x)).append("x").append(QString::number(y)));
	}
}

void TestWindow::changeStatusBarOnClick(QPointF* pl, QPointF* pr)
{
	QSize ls = sv->getLeftView()->imageSize();
	QSize rs = sv->getRightView()->imageSize();

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

void TestWindow::leftLoad()
{
	// Realiza o dialogo para decidir a imagem que será aberta
	QString filename = QFileDialog::getOpenFileName(this, "Open left image", "", "*.tif *.png *.bmp *.jpg *.ppm");
	if (filename.isEmpty())
		return;

	sv->getLeftView()->loadImage(filename);
}

void TestWindow::rightLoad()
{
	// Realiza o dialogo para decidir a imagem que será aberta
	QString filename = QFileDialog::getOpenFileName(this, "Open right image", "", "*.tif *.png *.bmp *.jpg *.ppm");
	if (filename.isEmpty())
		return;

	sv->getRightView()->loadImage(filename);
}
