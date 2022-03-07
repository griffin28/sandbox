#ifndef INCLUDED_ABSTRACTGLSCENE_H
#define INCLUDED_ABSTRACTGLSCENE_H

#include <QOpenGLFunctions_4_3_Core>
//#include <QOpenGLFunctions_4_3_Compatibility>
#include <QOpenGLExtraFunctions>

class QOpenGLWidget;
class QOpenGLContext;

class AbstractGLScene : protected QOpenGLFunctions_4_3_Core {
public:
    AbstractGLScene(QOpenGLWidget *glWidget = nullptr);
    virtual ~AbstractGLScene();

    virtual void initialize();
    virtual void paint() = 0;
    virtual void resize(int, int) = 0;

    bool isExtensionSupported(const char *);
    void printAvailableExtensions();
    void update();

    QOpenGLWidget *glWidget() const;
    QOpenGLContext *context();
    const QOpenGLContext *context() const;

private:    
    QOpenGLWidget *d_glWidget;     
};

#endif
