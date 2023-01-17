#ifndef INCLUDED_MAINWINDOW_H
#define INCLUDED_MAINWINDOW_H

#include <QMainWindow>

class MainWidget;
class QMenu;
class QAction;
class QLabel;
class QProgressBar;
class HUD;

namespace sandbox
{
    enum class RendererType 
    {
        RASTERIZATION,
        PATHTRACING
    };
}

using sandbox::RendererType;

/**
 * @class MainWindow
 * @brief Main application window
 * @see <a href="https://doc.qt.io/qt-5/qmainwindow.html#details">QMainWindow</a>
 * 
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructs the main application user interface.
     * @param parent this windows parent
    */
    MainWindow(QWidget *parent = 0);

    /**
     * Default destructor.
    */
    ~MainWindow() = default;

private slots:
    /**
     * Shows the about dialog. Called when the "About " menu item is selected
     * from the Help menu.
     * @see <a href="https://doc.qt.io/qt-5/qmessagebox.html#about">QMessageBox::about</a>
    */
    void about();
    
    /**
     * Update the status bar with the x- and y-coordinates of the mouse.
     * @param x the current mouse x-coordinate
     * @param y the current mouse y-coordinate
    */
    void updateScreenCoords(const int x, const int y);

    /**
     * Show the heads-up display.
    */
    void showHUD();

    /**
     * Shows the dialog for adding a sphere to the scene.
    */
    void showAddSphereDialog();

    /**
     * Shows the dialog for adding a cuboid to the scene.
    */
    void showAddCuboidDialog();

    //@{
    void showLambertShadingDialog();
    void showPhongShadingDialog();
    void showBlinnPhongShadingDialog();
    //@}

    // TODO: remove once all action handlers implemented
    void tempActionHandler();

    //@{
    /**
     * Updates the UI and switches the canvas between rasterization and path tracing.
    */
    void rasterizationActionHandler();
    void pathtracingActionHandler();
    //@}

private:
    void    createActions();
    void    createHelpMenu();
    void    createFileMenu();
    void    createViewMenu();
    void    createAddMenu();
    void    createRenderingMenu();
    void    createToolBar();

    RendererType getRendererType() const;
    void connectRendererSignals(const RendererType);

    MainWidget  *m_mainWidget;
    QWidget     *m_pathTracerWidget;
    HUD         *m_hud;

    // Menu Bar
    QMenu *m_fileMenu;
    QMenu *m_helpMenu;
    QMenu *m_viewMenu;
    QMenu *m_addMenu;
    QMenu *m_renderingMenu;

    // Actions
    QAction *m_closeAction;
    QAction *m_aboutAction;
    QAction *m_aboutQtAction;
    QAction *m_showHUDAction;

    // Shape Actions
    QAction *m_sphereAction;
    QAction *m_cuboidAction;

    // Shading Model Actions
    QAction *m_lambertShadingModelAction;
    QAction *m_phongShadingModelAction;
    QAction *m_blinnPhongShadingModelAction;

    // Rendering Actions
    QAction     *m_rasterizationAction;
    QAction     *m_pathTracingAction;
    QAction     *m_accelNoneAction;
    QAction     *m_accelBVHAction;
    
    // Labels
    QLabel      *m_statusLabel; 
};

#endif // INCLUDED_MAINWINDOW_H
