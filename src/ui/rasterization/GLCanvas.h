#ifndef INCLUDED_GLCANVAS_H_
#define INCLUDED_GLCANVAS_H_

#include <QOpenGLWidget>

class QKeyEvent;
class RasterizationScene;

/**
 * @class GLCanvas
 * @brief Creates a widget for displaying OpenGL graphics
 * @see <a href="https://doc.qt.io/qt-5/qopenglwidget.html">OpenGLWidget</a>
 *
 * Provides functionality for displaying OpenGL graphics. You have the choice between using QPainter
 * and standard OpenGL rendering commands.
 *
*/
class GLCanvas : public QOpenGLWidget {

	Q_OBJECT

public:
    /**
     * Constructs an OpenGL widget as a child of parent.
     * @param w the widget width
     * @param h the widget height
     * @param parent this widget's parent
     *
    */
    GLCanvas(int w, int h, QWidget *parent = nullptr);

    /**
     * Destroys the widget.
    */
    ~GLCanvas();

    /**
     * Get the OpenGL scene.
    */
    RasterizationScene *getScene() const { return m_scene; }

signals:
    /**
     * This signal is emitted when the mouse position on the screen has changed.
     * @param x the x position of the mouse cursor
     * @param y the y position of the mouse cursor
     *
    */
    void screenCoordsChanged(const int x, const int y);

    /**
     * This signal is emitted when a new time is calculated for rendering a frame
     * @param elapsed the time elapsed for rendering a frame
    */
    void frameRenderTimeChanged(long long int elapsed);

protected:
    /**
     * @see <a href="https://doc.qt.io/qt-5/qopenglwidget.html#initializeGL">OpenGLWidget::initializeGL</a>
    */
    void initializeGL() override;

    /**
     * @see <a href="https://doc.qt.io/qt-5/qopenglwidget.html#paintGL">OpenGLWidget::paintGL</a>
    */
    void paintGL() override;

    /**
     * @see <a href="https://doc.qt.io/qt-5/qopenglwidget.html#resizeGL">OpenGLWidget::resizeGL</a>
    */
    void resizeGL(int, int) override;

    /**
     * @see <a href="https://doc.qt.io/qt-5/qwidget.html#keyPressEvent">QWidget::keyPressEvent</a>
    */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @see <a href="https://doc.qt.io/qt-5/qwidget.html#mouseMoveEvent">QWidget::mouseMoveEvent</a>
    */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @see <a href="https://doc.qt.io/qt-5/qwidget.html#mousePressEvent">QWidget::mousePressEvent</a>
    */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @see <a href="https://doc.qt.io/qt-5/qwidget.html#mouseReleaseEvent">QWidget::mouseReleaseEvent</a>
    */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_width;
    int m_height;

    RasterizationScene *m_scene;

    // Mouse
    bool    m_mouseLeftDown;
    bool    m_mouseRightDown;
    int     m_posX;
    int     m_posY;
};

#endif /* GLCANVAS_H_ */
