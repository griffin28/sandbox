#include "MainWindow.h"
#include "MainWidget.h"
#include "Canvas.h"
#include "SimpleScene.h"
#include "HUD.h"
#include "SphereDialog.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <QApplication>
#include <QToolBar>

//----------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	m_mainWidget = new MainWidget(this);
	setCentralWidget(m_mainWidget);
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
    m_statusLabel = new QLabel(this);
    m_statusLabel->setText(tr("x: 0 y: 0"));

    this->statusBar()->addPermanentWidget(m_statusLabel);

  	// Connect signals
    connect(m_mainWidget->m_canvas, &Canvas::screenCoordsChanged,
            this, &MainWindow::updateScreenCoords);

    // Initialize HUD
    m_hud = new HUD(m_mainWidget->m_canvas);    
    connect(m_mainWidget->m_canvas, &Canvas::frameRenderTimeChanged,
            m_hud, &HUD::updateFrameRenderTime);    
    m_hud->show();
}

//----------------------------------------------------------------------------------
void 
MainWindow::createActions() 
{
    // Exit program
    m_closeAction = new QAction(QIcon(":/images/power.png"), tr("Exit"), this);
    connect(m_closeAction, &QAction::triggered, qApp, &QApplication::quit);

    // Show about dialog
	m_aboutAction = new QAction(tr("&About Training"), this);
//	connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);

	// About QT
	m_aboutQtAction = new QAction(tr("About &Qt"), this);
	connect(m_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Show HUD 
    m_showHUDAction = new QAction(QIcon(":/images/dot.png"), tr("Show HUD"), this);
    connect(m_showHUDAction, &QAction::triggered, this, &MainWindow::showHUD);

    // Shape Actions
    m_sphereAction = new QAction(QIcon(":images/sphere.png"), tr("Sphere"), this);
    m_sphereAction->setToolTip(tr("Add Sphere"));
    connect(m_sphereAction, &QAction::triggered, this, &MainWindow::showAddSphereDialog);

    m_cuboidAction = new QAction(QIcon(":images/cuboid.png"), tr("Cuboid"), this);
    m_cuboidAction->setToolTip(tr("Add Cuboid"));
    connect(m_cuboidAction, &QAction::triggered, this, &MainWindow::showAddCuboidDialog);

    // Shading Model Actions
    m_lambertShadingModelAction = new QAction(QIcon(":images/dot.png"), tr("Lambert"), this);
    connect(m_lambertShadingModelAction, &QAction::triggered, this, &MainWindow::showLambertShadingDialog);

    m_phongShadingModelAction = new QAction(QIcon(":images/dot.png"), tr("Phong"), this);
    m_phongShadingModelAction->setIconVisibleInMenu(false);
    connect(m_phongShadingModelAction, &QAction::triggered, this, &MainWindow::showPhongShadingDialog);

    m_blinnPhongShadingModelAction = new QAction(QIcon(":images/dot.png"), tr("Blinn-Phong"), this);
    m_blinnPhongShadingModelAction->setIconVisibleInMenu(false);
    connect(m_blinnPhongShadingModelAction, &QAction::triggered, this, &MainWindow::showBlinnPhongShadingDialog);

    // Rendering Actions
    m_rasterizationAction = new QAction(QIcon(":images/dot.png"), tr("Rasterization"), this);
    connect(m_rasterizationAction, &QAction::triggered, this, &MainWindow::rasterizationActionHandler);

    m_pathTracingAction = new QAction(QIcon(":images/dot.png"), tr("Path Tracing"), this);
    m_pathTracingAction->setIconVisibleInMenu(false);
    connect(m_pathTracingAction, &QAction::triggered, this, &MainWindow::pathtracingActionHandler);

    m_accelNoneAction = new QAction(QIcon(":images/dot.png"), tr("None"), this);
    connect(m_accelNoneAction, &QAction::triggered, this, &MainWindow::tempActionHandler);

    m_accelBVHAction = new QAction(QIcon(":images/dot.png"), tr("BVH"), this);
    m_accelBVHAction->setIconVisibleInMenu(false);
    connect(m_accelBVHAction, &QAction::triggered, this, &MainWindow::tempActionHandler);
}

//----------------------------------------------------------------------------------
void
MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar("Main");

    // Shape section
    toolBar->addAction(m_sphereAction);
    toolBar->addAction(m_cuboidAction);
    toolBar->addSeparator();
}

//----------------------------------------------------------------------------------
void 
MainWindow::createFileMenu() 
{
    // File Menu
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_closeAction);
}

//----------------------------------------------------------------------------------
void
MainWindow::createAddMenu()
{
    // Root
    m_addMenu = menuBar()->addMenu(tr("Add"));

    // Shape submenu
    QMenu *shapeMenu = m_addMenu->addMenu(tr("Shape"));
    shapeMenu->addAction(m_sphereAction);
    shapeMenu->addAction(m_cuboidAction);

    QMenu *shadingMenu = m_addMenu->addMenu(tr("Shading Model"));
    shadingMenu->addAction(m_lambertShadingModelAction);
    shadingMenu->addAction(m_phongShadingModelAction);
    shadingMenu->addAction(m_blinnPhongShadingModelAction);
}

//----------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------
void 
MainWindow::createViewMenu() 
{
    // File Menu
    m_viewMenu = menuBar()->addMenu(tr("View"));
    m_viewMenu->addAction(m_showHUDAction);
}

//----------------------------------------------------------------------------------
void 
MainWindow::createHelpMenu() 
{
    // Help Menu
	m_helpMenu = menuBar()->addMenu(tr("Help"));
	m_helpMenu->addAction(m_aboutAction);
	m_helpMenu->addAction(m_aboutQtAction);
}

//
// Slots
//
//----------------------------------------------------------------------------------
void 
MainWindow::about() 
{
	QMessageBox::about(this, tr("About Rendering Sandbox"),
			tr("<h2>Rendering Sandbox 1.0</h2>"
			"<p>Copyright &copy; 2022 Kevin Griffin kevin.s.griffin@gmail.com"));
}

//----------------------------------------------------------------------------------
void
MainWindow::updateScreenCoords(const int x, const int y)
{
    char buffer[50];
    snprintf(buffer, 50, "x: %d y: %d", x, y);
    m_statusLabel->setText(tr(buffer));
}

//----------------------------------------------------------------------------------
void
MainWindow::showHUD() 
{
    if(m_hud->isVisible()) 
    {
        m_hud->setVisible(false);
        m_showHUDAction->setIconVisibleInMenu(false);
    } 
    else
    {
        m_hud->setVisible(true);
        m_showHUDAction->setIconVisibleInMenu(true);
    }
}

//----------------------------------------------------------------------------------
void
MainWindow::showAddSphereDialog()
{
    SphereDialog sphereDialog(this);
    
    if(sphereDialog.exec() == QDialog::Rejected)
    {
        return; 
    }

    float radius = sphereDialog.getRadius();
    float *center = sphereDialog.getCenter();

    float color[4];
    sphereDialog.getColor(color);

    SimpleScene *scene = m_mainWidget->m_canvas->getScene();
    scene->addSphere(radius, center, color);
}

//----------------------------------------------------------------------------------
void
MainWindow::showAddCuboidDialog()
{
    // TODO:
}

//----------------------------------------------------------------------------------
void
MainWindow::showLambertShadingDialog()
{
    if(!m_lambertShadingModelAction->isIconVisibleInMenu())
    {
        m_lambertShadingModelAction->setIconVisibleInMenu(true);
        m_phongShadingModelAction->setIconVisibleInMenu(false);
        m_blinnPhongShadingModelAction->setIconVisibleInMenu(false);
    }
}

//----------------------------------------------------------------------------------
void
MainWindow::showPhongShadingDialog()
{
    if(!m_phongShadingModelAction->isIconVisibleInMenu())
    {
        m_phongShadingModelAction->setIconVisibleInMenu(true);
        m_lambertShadingModelAction->setIconVisibleInMenu(false);
        m_blinnPhongShadingModelAction->setIconVisibleInMenu(false);
    }
}

//----------------------------------------------------------------------------------
void
MainWindow::showBlinnPhongShadingDialog()
{
    if(!m_blinnPhongShadingModelAction->isIconVisibleInMenu())
    {
        m_blinnPhongShadingModelAction->setIconVisibleInMenu(true);
        m_lambertShadingModelAction->setIconVisibleInMenu(false);
        m_phongShadingModelAction->setIconVisibleInMenu(false);
    }
}

//----------------------------------------------------------------------------------
void
MainWindow::rasterizationActionHandler()
{
    if(!m_rasterizationAction->isIconVisibleInMenu())
    {
        m_rasterizationAction->setIconVisibleInMenu(true);
        m_pathTracingAction->setIconVisibleInMenu(false);
        m_hud->setRenderingMode("Rasterization");
    }

    // TODO: switch out path tracing canvas to rasterization
}

//----------------------------------------------------------------------------------
void
MainWindow::pathtracingActionHandler()
{
    if(!m_pathTracingAction->isIconVisibleInMenu())
    {
        m_pathTracingAction->setIconVisibleInMenu(true);
        m_rasterizationAction->setIconVisibleInMenu(false);
        m_hud->setRenderingMode("Path Tracing");
    }

    // TODO: switch out path tracing canvas to rasterization
}

//----------------------------------------------------------------------------------
void
MainWindow::tempActionHandler()
{
    // TODO: remove when all the rendering handlers are implemented
}