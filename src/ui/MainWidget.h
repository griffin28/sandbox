#ifndef INCLUDED_MAINWIDGET_H
#define INCLUDED_MAINWIDGET_H

#include <QWidget>

class Canvas;

/**
 * @class MainWidget
 * @brief Main application widget
 * @see <a href="https://doc.qt.io/qt-5/qwidget.html">QWidget</a>
*/
class MainWidget : public QWidget
{
    friend class MainWindow;
    Q_OBJECT

public:
    /**
     * Constructs the main widget.
     * @param parent this widget's parent
    */
    MainWidget(QWidget *parent = nullptr);

    /**
     * Default destructor.
    */
    ~MainWidget() = default;
    
private:
    Canvas *m_canvas;   // should be destroyed when parent is destroyed
    //std::unique_ptr<PathTracingCanvas> m_ptcanvas;
 
    static constexpr int WIDTH = 900;
    static constexpr int HEIGHT = 900;
};

#endif // INCLUDED_MAINWIDGET_H