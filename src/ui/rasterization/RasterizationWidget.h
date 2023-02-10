#ifndef INCLUDED_RASTERIZATIONWIDGET_H
#define INCLUDED_RASTERIZATIONWIDGET_H

#include <QWidget>

class GLCanvas;

/**
 * @class RasterizationWidget
 * @brief Main application widget
 * @see <a href="https://doc.qt.io/qt-5/qwidget.html">QWidget</a>
*/
class RasterizationWidget : public QWidget
{
    friend class MainWindow;
    Q_OBJECT

public:
    /**
     * Constructs the main widget.
     * @param parent this widget's parent
    */
    RasterizationWidget(QWidget *parent = nullptr);

    /**
     * Default destructor.
    */
    ~RasterizationWidget() = default;

private:
    GLCanvas *m_canvas;   // should be destroyed when parent is destroyed

    static constexpr int WIDTH = 900;
    static constexpr int HEIGHT = 900;
};

#endif // INCLUDED_RASTERIZATIONWIDGET_H