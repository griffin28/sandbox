#ifndef INCLUDED_SIMPLESCENE_H
#define INCLUDED_SIMPLESCENE_H

#include "AbstractGLScene.h"
#include "vmath.h"

#include <QObject>

class Canvas;
class QTimer;

class SimpleScene: public QObject, public AbstractGLScene {
    Q_OBJECT

    // Scoped enumeration
    enum class Shader {
        VERTEX = 0,
        TESS_CONTROL,
        TESS_EVAL,
        GEOMETRY,
        FRAGMENT
    };

public:
    SimpleScene(Canvas *glWidget);
    ~SimpleScene();

    void initialize() override;
    void paint() override;
    void resize(int, int) override;

signals:
    void sceneUpdated();

public slots:
    void handleTimer();

private:
    GLuint compileShaders();
    void printShaderInfoLog(const GLuint * const, const Shader);
    void printLinkerInfoLog(const GLuint);

    float d_angle;
    GLuint d_program;
    GLuint d_vao;
    GLuint d_buffer;
    GLuint d_indexBuffer;
    GLint d_mvLocation;
    GLint d_mvLocation1;
    GLint d_projLocation;
    vmath::mat4 d_projMatrix;

    QTimer *d_timer;
};

#endif
