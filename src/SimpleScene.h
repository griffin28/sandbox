#ifndef INCLUDED_SIMPLESCENE_H
#define INCLUDED_SIMPLESCENE_H

#include "AbstractGLScene.h"
#include "sphere.h"

#include <memory>

//#include <glm/vec3.hpp> // vec3
//#include <glm/mat4x4.hpp> // mat4
#include <glm/glm.hpp>

#include <QObject>
#include <QOpenGLShaderProgram>

class Canvas;

namespace mog 
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

   class SceneObject
   {
    public:
        SceneObject() : shape(nullptr),
                        program(0),
                        vertexArray(0),
                        positionBuffer(0),
                        indexBuffers{0,1} {}
        ~SceneObject() = default;

        std::unique_ptr<Shape>  shape;
        GLuint 	                program;
        GLuint 	                vertexArray;
        GLuint 	                positionBuffer;
        GLuint 	                indexBuffers[2];
   };
}

class SimpleScene: public QObject, public AbstractGLScene 
{
    Q_OBJECT

public:
    SimpleScene(Canvas *glWidget);
    ~SimpleScene();

    void initialize() override;
    void paint() override;
    void resize(int, int) override;

    void initGLSL(mog::SceneObject * const);

    void setShapeSelection(const int, const int);
    void addSphere(const float, 
                   float * const,
                   float * const, 
                   ModelType shading=ModelType::LAMBERT);

    static void GLAPIENTRY  DebugMessageCallback(GLenum, 
                                                 GLenum, 
                                                 GLuint, 
                                                 GLenum, 
                                                 GLsizei, 
                                                 const GLchar *, 
                                                 const void *);

    // Interactive Vars
    float   m_cameraDistance;
    float   m_mouseX;
    float   m_mouseY;
    float   m_cameraAngleX;
    float   m_cameraAngleY;

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

    void printShaderInfoLog(const GLuint * const, const mog::Shader);
    void printLinkerInfoLog(const GLuint);

    size_t         		                m_shapeSelectionIndex;
    std::vector<mog::SceneObject *>     m_sceneObjects;
    
    // Transformation
    float           m_angle;
    glm::mat4       m_projMatrix;
};

#endif
