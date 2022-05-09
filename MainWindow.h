#ifndef INCLUDED_MAINWINDOW_H
#define INCLUDED_MAINWINDOW_H

#include <QMainWindow>

class MainWidget;
class QMenu;
class QAction;
class QLabel;
class QProgressBar;
class HUD;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow() = default;

private slots:
    void about();
    // void updateStatusBar(QString, const int);
    void updateScreenCoords(const int, const int);
    void showHUD();
    // Shapes
    void showAddSphereDialog();
    void showAddCuboidDialog();
    // Shading Models
    void showLambertShadingDialog();
    void showPhongShadingDialog();
    void showBlinnPhongShadingDialog();
    // Rendering
    void tempRenderingActionHandler();

private:
    void    createActions();
    // Menu
    void    createHelpMenu();
    void    createFileMenu();
    void    createViewMenu();
    void    createAddMenu();
    void    createRenderingMenu();

    // ToolBar
    void    createToolBar();

    MainWidget  *d_mainWidget;
    HUD         *m_hud;

    // Menu Bar
    QMenu *d_fileMenu;
    QMenu *d_helpMenu;
    QMenu *d_viewMenu;
    QMenu *d_addMenu;
    QMenu *m_renderingMenu;

    // Actions
    QAction *d_closeAction;
    QAction *d_aboutAction;
    QAction *d_aboutQtAction;
    QAction *d_showHUDAction;

    // Shape Actions
    QAction *d_sphereAction;
    QAction *d_cuboidAction;

    // Shading Model Actions
    QAction *d_lambertShadingModelAction;
    QAction *d_phongShadingModelAction;
    QAction *d_blinnPhongShadingModelAction;

    // Rendering Actions
    QAction     *m_rasterizationAction;
    QAction     *m_pathTracingAction;
    QAction     *m_accelNoneAction;
    QAction     *m_accelBVHAction;
    
    // Labels
    QLabel          *d_statusLabel;
    // QProgressBar    *d_statusProgressBar;    
};

#endif // INCLUDED_MAINWINDOW_H
