#ifndef INCLUDED_RASTERIZATIONSCENE_H
#define INCLUDED_RASTERIZATIONSCENE_H

#include "AbstractGLScene.h"
#include "sphere.h"

#include <memory>
#include <glm/glm.hpp>

#include <QObject>
#include <QOpenGLShaderProgram>

class GLCanvas;
class ProjectionCamera;
class BVH;

namespace sandbox
{
    // Scoped enumeration
    enum class Shader
    {
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
        SceneObject() : shape(),
                        program(0),
                        vertexArray(0),
                        positionBuffer(0),
                        boundingBoxBuffer(0),
                        indexBuffers{0,1,2} {}

        std::shared_ptr<Shape> shape;
        GLuint  program;
        GLuint  vertexArray;
        GLuint  positionBuffer;
        GLuint  boundingBoxBuffer;
        GLuint  indexBuffers[3];
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

    /// @brief Initialize the OpenGL functions. This method needs to be called from a current OpenGL context.
    /// @see <a href="https://doc.qt.io/qt-5/qopenglfunctions.html#initializeOpenGLFunctions">QOpenGLFunctions::initializeOpenGLFunctions</a>
    void initialize() override;

    /// @brief This function is called whenever a painting operation is needed.
    void paint() override;

    /// Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget
    /// has been resized (and also when it is shown for the first time because all newly
    /// created widgets get a resize event automatically).
    /// @see <a href="https://doc.qt.io/qt-6/qopenglwidget.html#resizeGL">QOpenGLWidget::resizeGL</a>
    /// @param w the width
    /// @param h the height
    void resize(int w, int h) override;

    //@{
    /// @brief Set/get the shape selection index
    /// @param x the x-coordinate of the mouse
    /// @param y the y-coordinate of the mouse
    void setShapeSelection(const int x, const int y);
    int getShapeSelection() { return m_shapeSelectionIndex; }
    //@}

    /// @brief Add shape to the scene
    /// @param  shape the shape to add
    void addShape(const std::shared_ptr<Shape> &shape);

    /// @brief Add the shapes to the scene
    /// @param shapes the shapes to add to the scene
    void addShapes(const std::vector<std::shared_ptr<Shape>> &shapes);

    //@{
    /// @brief Set/get the projection (perspecivte or orthographic) camera
    void setCamera(const std::shared_ptr<ProjectionCamera> &camera) { m_camera = camera; }
    std::shared_ptr<ProjectionCamera> getCamera() const { return m_camera; }
    //@}

    /// @brief Get the currently selected shape.
    /// @return the selected shape
    std::shared_ptr<Shape> getSelectedShape();

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
    void initGLSL(sandbox::SceneObject * const);

    // GLuint compileShaders();
    GLuint createShaderProgram(const char * const *vs,
                               const char * const *fs,
                               const char * const *gs=nullptr,
                               const char * const *tcs=nullptr,
                               const char * const *tes=nullptr);
    GLuint  compileComputeShaders();
    void    updateShaderInputs(std::shared_ptr<Shape>, const GLuint);

    void printShaderInfoLog(const GLuint * const, const sandbox::Shader);
    void printLinkerInfoLog(const GLuint);

    long        		                m_shapeSelectionIndex;
    std::vector<sandbox::SceneObject *> m_sceneObjects;
    std::shared_ptr<ProjectionCamera>   m_camera;
    std::unique_ptr<BVH>                m_bvh;
};

#endif
