#ifndef INCLUDED_SIMPLESCENE_H
#define INCLUDED_SIMPLESCENE_H

#include "AbstractGLScene.h"
#include "sphere.h"

//#include <glm/vec3.hpp> // vec3
//#include <glm/mat4x4.hpp> // mat4
#include <glm/glm.hpp>

#include <QObject>
#include <QOpenGLShaderProgram>

class Canvas;

namespace mog {
    // Scoped enumeration
    enum class Shader {
        VERTEX = 0,
        TESS_CONTROL,
        TESS_EVAL,
        GEOMETRY,
        FRAGMENT,
        COMPUTE
    };
}

class SimpleScene: public QObject, public AbstractGLScene {
    Q_OBJECT

public:
    SimpleScene(Canvas *glWidget);
    ~SimpleScene();

    void initialize() override;
    void paint() override;
    void resize(int, int) override;

    void initGLSL(Shape * const);

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


    std::vector<Shape *> m_shapes;
    int         m_shapeSelectionIndex;
    
    // OpenGL
    std::vector<GLuint>          m_programs;  // for each shape
    // TODO: QOpenGLShaderProgram    m_shaderProgram;
    // std::vector<QOpenGLShaderProgram> m_shaderPrograms;
    

    // std::vector<GLuint> m_vectorArrayObjs;
    std::vector<GLuint>          m_vaos;     
    std::vector<GLuint>          m_positionBuffers;
    std::vector<GLuint>          m_indexBuffers;

    // Transformation
    float           m_angle;
    glm::mat4       m_projMatrix;
};

#endif
