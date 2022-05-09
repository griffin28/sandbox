#include "MainWindow.h"
#include "MainWidget.h"
#include "Canvas.h"
#include "SimpleScene.h"
#include "HUD.h"

#include <QtGui>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QLabel>
#include <QProgressBar>
#include <QStatusBar>
#include <QApplication>
#include <QIcon>
#include <QToolBar>

using namespace std;

// ****************************************************************************
//  Method: MainWindow constructor
//
//  Programmer: Kevin Griffin 
//  Creation:   Fri Aug 14 13:50:31 PST 2020
//
// ****************************************************************************

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	d_mainWidget = new MainWidget(this);
	setCentralWidget(d_mainWidget);
	setWindowTitle("Rendering Sandbox");

	createActions();

    // Create Menus
    createFileMenu();    
    createRenderingMenu();
    createAddMenu();
    createViewMenu();
	createHelpMenu();

    // Create Toolbar
    createToolBar();

    // Initialize Status Bar
    d_statusLabel = new QLabel(this);
    d_statusLabel->setText(tr("x: 0 y: 0"));

    // d_statusProgressBar = new QProgressBar(this);
    // d_statusProgressBar->setRange(0, 100);
    // d_statusProgressBar->setTextVisible(false);
    // d_statusProgressBar->setValue(0);

    this->statusBar()->addPermanentWidget(d_statusLabel);
    // this->statusBar()->addPermanentWidget(d_statusProgressBar);

  	// Connect signals
    connect(d_mainWidget->d_canvas, &Canvas::screenCoordsChanged,
            this, &MainWindow::updateScreenCoords);

    // Initialize HUD
    m_hud = new HUD(d_mainWidget->d_canvas);    
    connect(d_mainWidget->d_canvas, &Canvas::frameRenderTimeChanged,
            m_hud, &HUD::updateFrameRenderTime);    
    m_hud->show();
}

// ****************************************************************************
// Method: MainWindow::createActions 
//
// Purpose:
//   Create actions to be used in menus and toolbars
//
// Programmer: Kevin Griffin 
// Creation:   January 9, 2008
//
// Modifications:
//
// ****************************************************************************

void 
MainWindow::createActions() 
{
    // Exit program
    d_closeAction = new QAction(QIcon(":/images/power.png"), tr("Exit"), this);
    connect(d_closeAction, &QAction::triggered, qApp, &QApplication::quit);

    // Show about dialog
	d_aboutAction = new QAction(tr("&About Training"), this);
//	connect(d_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(d_aboutAction, &QAction::triggered, this, &MainWindow::about);

	// About QT
	d_aboutQtAction = new QAction(tr("About &Qt"), this);
	connect(d_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Show HUD 
    d_showHUDAction = new QAction(QIcon(":/images/dot.png"), tr("Show HUD"), this);
    connect(d_showHUDAction, &QAction::triggered, this, &MainWindow::showHUD);

    // Shape Actions
    d_sphereAction = new QAction(QIcon(":images/sphere.png"), tr("Sphere"), this);
    d_sphereAction->setToolTip(tr("Add Sphere"));
    connect(d_sphereAction, &QAction::triggered, this, &MainWindow::showAddSphereDialog);

    d_cuboidAction = new QAction(QIcon(":images/cuboid.png"), tr("Cuboid"), this);
    d_cuboidAction->setToolTip(tr("Add Cuboid"));
    connect(d_cuboidAction, &QAction::triggered, this, &MainWindow::showAddCuboidDialog);

    // Shading Model Actions
    d_lambertShadingModelAction = new QAction(QIcon(":images/dot.png"), tr("Lambert"), this);
    connect(d_lambertShadingModelAction, &QAction::triggered, this, &MainWindow::showLambertShadingDialog);

    d_phongShadingModelAction = new QAction(QIcon(":images/dot.png"), tr("Phong"), this);
    d_phongShadingModelAction->setIconVisibleInMenu(false);
    connect(d_phongShadingModelAction, &QAction::triggered, this, &MainWindow::showPhongShadingDialog);

    d_blinnPhongShadingModelAction = new QAction(QIcon(":images/dot.png"), tr("Blinn-Phong"), this);
    d_blinnPhongShadingModelAction->setIconVisibleInMenu(false);
    connect(d_blinnPhongShadingModelAction, &QAction::triggered, this, &MainWindow::showBlinnPhongShadingDialog);

    // Rendering Actions
    m_rasterizationAction = new QAction(QIcon(":images/dot.png"), tr("Rasterization"), this);
    connect(m_rasterizationAction, &QAction::triggered, this, &MainWindow::tempRenderingActionHandler);

    m_pathTracingAction = new QAction(QIcon(":images/dot.png"), tr("Path Tracing"), this);
    m_pathTracingAction->setIconVisibleInMenu(false);
    connect(m_pathTracingAction, &QAction::triggered, this, &MainWindow::tempRenderingActionHandler);

    m_accelNoneAction = new QAction(QIcon(":images/dot.png"), tr("None"), this);
    connect(m_accelNoneAction, &QAction::triggered, this, &MainWindow::tempRenderingActionHandler);

    m_accelBVHAction = new QAction(QIcon(":images/dot.png"), tr("BVH"), this);
    m_accelBVHAction->setIconVisibleInMenu(false);
    connect(m_accelBVHAction, &QAction::triggered, this, &MainWindow::tempRenderingActionHandler);
}

void
MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar("Main");

    // Shape section
    toolBar->addAction(d_sphereAction);
    toolBar->addAction(d_cuboidAction);
    toolBar->addSeparator();
}

// ****************************************************************************
// Method: MainWindow::createFileMenu 
//
// Purpose:
//   Create the File Menu and sub menus
//
// Programmer: Kevin Griffin 
// Creation:   August 24, 2021
//
// Modifications:
//
// ****************************************************************************

void 
MainWindow::createFileMenu() 
{
    // File Menu
    d_fileMenu = menuBar()->addMenu(tr("File"));
    d_fileMenu->addSeparator();
    d_fileMenu->addAction(d_closeAction);
}

void
MainWindow::createAddMenu()
{
    // Root
    d_addMenu = menuBar()->addMenu(tr("Add"));

    // Shape submenu
    QMenu *shapeMenu = d_addMenu->addMenu(tr("Shape"));
    shapeMenu->addAction(d_sphereAction);
    shapeMenu->addAction(d_cuboidAction);

    QMenu *shadingMenu = d_addMenu->addMenu(tr("Shading Model"));
    shadingMenu->addAction(d_lambertShadingModelAction);
    shadingMenu->addAction(d_phongShadingModelAction);
    shadingMenu->addAction(d_blinnPhongShadingModelAction);
}

void
MainWindow::createRenderingMenu()
{
    // Root
    m_renderingMenu = menuBar()->addMenu(tr("Rendering"));

    // Mode
    QMenu *modeMenu = m_renderingMenu->addMenu(tr("Mode"));
    modeMenu->addAction(m_rasterizationAction);
    modeMenu->addAction(m_pathTracingAction);

    // Acceleration
    QMenu *accelMenu = m_renderingMenu->addMenu(tr("Acceleration"));
    accelMenu->addAction(m_accelNoneAction);
    accelMenu->addAction(m_accelBVHAction);
}

// ****************************************************************************
// Method: MainWindow::createViewMenu 
//
// Purpose:
//   Create the View Menu and sub menus
//
// Programmer: Kevin Griffin 
// Creation:   August 24, 2021
//
// Modifications:
//
// ****************************************************************************

void 
MainWindow::createViewMenu() 
{
    // File Menu
    d_viewMenu = menuBar()->addMenu(tr("View"));
    d_viewMenu->addAction(d_showHUDAction);
}

// ****************************************************************************
// Method: MainWindow::createHelpMenu 
//
// Purpose:
//   Create the Help Menu and sub menus
//
// Programmer: Kevin Griffin 
// Creation:   August 24, 2021
//
// Modifications:
//
// ****************************************************************************

void 
MainWindow::createHelpMenu() 
{
    // Help Menu
	d_helpMenu = menuBar()->addMenu(tr("Help"));
	d_helpMenu->addAction(d_aboutAction);
	d_helpMenu->addAction(d_aboutQtAction);
}

//
// Slots
//

// ****************************************************************************
// Method: MainWindow::about 
//
// Purpose:
//   Slot: show the about dialog. Called when the "About training" menu item
//   is selected from the Help menu.
//
// Programmer: Kevin Griffin 
// Creation:   August 24, 2021
//
// Modifications:
//
// ****************************************************************************

void 
MainWindow::about() 
{
	QMessageBox::about(this, tr("About Rendering Sandbox"),
			tr("<h2>Rendering Sandbox 1.0</h2>"
			"<p>Copyright &copy; 2022 Kevin Griffin kevin.s.griffin@gmail.com"));
}

// ****************************************************************************
// Method: MainWindow::updateStatusBar 
//
// Purpose:
//   Slot: Update the status bar. Called when there's an update for the status
//   bar.
//
// Programmer: Kevin Griffin 
// Creation:   August 24, 2021
//
// Modifications:
//
// ****************************************************************************

// void 
// MainWindow::updateStatusBar(QString msg, const int value)
// {
//     // int fps = 0;

//     // if(d_fpsTimer == nullptr) {
//     //     d_fpsTimer = new QElapsedTimer();
//     //     d_fpsTimer->start();
//     // } else {
//     //     auto elapsedTime = d_fpsTimer->elapsed();
//     //     fps = (1.0f/static_cast<float>(elapsedTime)) * 1000;
//     //     d_fpsTimer->restart();
//     // }

//     // char buffer[20];
//     // snprintf(buffer, 20, "%d fps", fps);
// //    statusBar()->showMessage(tr("50 fps"));

//     d_statusLabel->setText(msg);
//     d_statusProgressBar->setValue(value);
// }

void
MainWindow::updateScreenCoords(const int x, const int y)
{
    char buffer[50];
    snprintf(buffer, 50, "x: %d y: %d", x, y);
    d_statusLabel->setText(tr(buffer));
}

// ****************************************************************************
// Method: MainWindow::showHUD
//
// Purpose:
//   Slot: show the heads-up display. Called when the "Show HUD" menu item
//   is selected from the Voew menu.
//
// Programmer: Kevin Griffin 
// Creation:   August 24, 2021
//
// Modifications:
//
// ****************************************************************************

void
MainWindow::showHUD() 
{
    if(m_hud->isVisible()) 
    {
        m_hud->setVisible(false);
        d_showHUDAction->setIconVisibleInMenu(false);
    } 
    else
    {
        m_hud->setVisible(true);
        d_showHUDAction->setIconVisibleInMenu(true);
    }
}

void
MainWindow::showAddSphereDialog()
{
    // TODO:
}

void
MainWindow::showAddCuboidDialog()
{
    // TODO:
}

void
MainWindow::showLambertShadingDialog()
{
    if(!d_lambertShadingModelAction->isIconVisibleInMenu())
    {
        d_lambertShadingModelAction->setIconVisibleInMenu(true);
        d_phongShadingModelAction->setIconVisibleInMenu(false);
        d_blinnPhongShadingModelAction->setIconVisibleInMenu(false);
    }
}

void
MainWindow::showPhongShadingDialog()
{
    if(!d_phongShadingModelAction->isIconVisibleInMenu())
    {
        d_phongShadingModelAction->setIconVisibleInMenu(true);
        d_lambertShadingModelAction->setIconVisibleInMenu(false);
        d_blinnPhongShadingModelAction->setIconVisibleInMenu(false);
    }
}

void
MainWindow::showBlinnPhongShadingDialog()
{
    if(!d_blinnPhongShadingModelAction->isIconVisibleInMenu())
    {
        d_blinnPhongShadingModelAction->setIconVisibleInMenu(true);
        d_lambertShadingModelAction->setIconVisibleInMenu(false);
        d_phongShadingModelAction->setIconVisibleInMenu(false);
    }
}

void
MainWindow::tempRenderingActionHandler()
{
    // TODO: remove when all the rendering handlers are implemented
    m_hud->setRenderingMode("Path Tracing");
}

