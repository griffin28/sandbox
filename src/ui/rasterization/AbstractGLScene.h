#ifndef INCLUDED_ABSTRACTGLSCENE_H
#define INCLUDED_ABSTRACTGLSCENE_H

#include <QOpenGLFunctions_4_3_Core>
//#include <QOpenGLFunctions_4_3_Compatibility>
#include <QOpenGLExtraFunctions>

class QOpenGLWidget;
class QOpenGLContext;

/**
 * @class AbstractGLScene
 * @brief Abstract class for creating an OpenGL scene
 *
 * This class needs to be subclassed by a concrete implementation
 * for creating an OpenGL scene.
 */

class AbstractGLScene : protected QOpenGLFunctions_4_3_Core 
{
public:
    AbstractGLScene(QOpenGLWidget *glWidget = nullptr);
    virtual ~AbstractGLScene() = default;

    /**
     * Initialize the OpenGL functions. This method needs to be called from a current OpenGL context.
     * @see <a href="https://doc.qt.io/qt-5/qopenglfunctions.html#initializeOpenGLFunctions">QOpenGLFunctions::initializeOpenGLFunctions</a>
     */
    virtual void initialize();

    /**
     * This pure virtual function is called whenever a painting operation is needed. 
     * Implement it in a subclass.
     */
    virtual void paint() = 0;

    /**
     * This pure virtual function is called whenever a resize operation is needed.
     * Implement in a subclass.
     * @param w width
     * @param h height
     */
    virtual void resize(int w, int h) = 0;

    /**
     * Determines if extension ext is supported by the current GL version.
     * @param ext the extension to test
     */
    bool isExtensionSupported(const char *ext);

    /**
     * Print the list of GL extensions available.
     */
    void printAvailableExtensions();

    /**
     * Updates the widget unless updates are disabled or the widget is hidden.
     * @see <a href="https://doc.qt.io/qt-5/qwidget.html#update">QWidget::update</a>
     */
    void update();

    /**
     * Returns the Open GL widget.
     */
    QOpenGLWidget *glWidget() const;

    //@{
    /**
     * Returns the OpenGL context
     * @see <a href="https://doc.qt.io/qt-5/qopenglwidget.html#context">QOpenGLWidget::context</a>
    */
    QOpenGLContext *context();
    const QOpenGLContext *context() const;
    //@}

private:    
    QOpenGLWidget *m_glWidget;     
};

#endif
