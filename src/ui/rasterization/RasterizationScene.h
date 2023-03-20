#ifndef INCLUDED_RASTERIZATIONSCENE_H
#define INCLUDED_RASTERIZATIONSCENE_H

#include "AbstractGLScene.h"
#include "sphere.h"

#include <memory>

//#include <glm/vec3.hpp> // vec3
//#include <glm/mat4x4.hpp> // mat4
#include <glm/glm.hpp>

#include <QObject>
#include <QOpenGLShaderProgram>

class GLCanvas;
class ProjectionCamera;

namespace sandbox
{
    // Scoped enumeration
    enum class Shader {
        VERTEX = 0,
        TESS_CONTROL,
        TESS_EVAL,
        GEOMETRY,
        FRAGMENT,
        COMPUTE
    };

   /// @brief Collects all of the OpenGL scene related objects
   struct SceneObject
   {
        SceneObject() : shape(nullptr),
                        program(0),
                        vertexArray(0),
                        positionBuffer(0),
                        indexBuffers{0,1} {}

        Shape   *shape;
        GLuint  program;
        GLuint  vertexArray;
        GLuint  positionBuffer;
        GLuint  indexBuffers[2];
   };
} // namespace sandbox

/// @class RasterizationScene
/// @brief Renders a scene using OpenGL rasterization
///
/// This class creates a scene of objects and renders them using OpenGL's programmable
/// rasterization pipeline.
class RasterizationScene: public QObject, public AbstractGLScene
{
    Q_OBJECT

public:
    /// @brief Constructor
    /// @param glWidget
    RasterizationScene(GLCanvas *glWidget);

    /// Default destuctor
    ~RasterizationScene();

    void initialize() override;
    void paint() override;

    /// Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget
    /// has been resized (and also when it is shown for the first time because all newly
    /// created widgets get a resize event automatically).
    /// @see <a href="https://doc.qt.io/qt-6/qopenglwidget.html#resizeGL">QOpenGLWidget::resizeGL</a>
    /// @param w the width
    /// @param h the height
    void resize(int w, int h) override;

    void initGLSL(sandbox::SceneObject * const);

    void setShapeSelection(const int, const int);
    void addShape(Shape * const);
    void addShapes(Shape **, const size_t);

    //@{
    /// @brief Set/get the projection (perspecivte or orthographic) camera
    void setCamera(ProjectionCamera * const camera) { m_camera = camera; }
    ProjectionCamera *getCamera() const { return m_camera; }
    //@}

    static void GLAPIENTRY  DebugMessageCallback(GLenum,
                                                 GLenum,
                                                 GLuint,
                                                 GLenum,
                                                 GLsizei,
                                                 const GLchar *,
                                                 const void *);

signals:
    void statusUpdate(QString);

private:
    // GLuint compileShaders();
    GLuint createShaderProgram(const char * const *vs,
                               const char * const *fs,
                               const char * const *gs=nullptr,
                               const char * const *tcs=nullptr,
                               const char * const *tes=nullptr);
    GLuint  compileComputeShaders();
    void    updateShaderInputs(Shape const *, const GLuint);

    void printShaderInfoLog(const GLuint * const, const sandbox::Shader);
    void printLinkerInfoLog(const GLuint);

    int         		                    m_shapeSelectionIndex;
    std::vector<sandbox::SceneObject>       *m_sceneObjects;
    ProjectionCamera                        *m_camera;
};

#endif
