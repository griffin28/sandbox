#include "SimpleScene.h"
#include "Canvas.h"

#include <glm/ext/matrix_transform.hpp>	 // translate, rotate, scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

using namespace std;

SimpleScene::SimpleScene(Canvas *glWidget):
    AbstractGLScene(glWidget),
    m_cameraDistance(20),
    m_mouseX(0),
    m_mouseY(0),
    m_cameraAngleX(0),
    m_cameraAngleY(0),
    m_sphere(new mog::Sphere(5.0f, 0.0f, 0.0f, 0.0f)),
    m_shapeSelectionIndex(-1),
    m_programs(),
    m_vaos(),
    m_positionBuffers(),
    m_indexBuffers(),
    m_angle(0.0f),
    m_projMatrix() {}

SimpleScene::~SimpleScene() 
{
    // Delete Vertex Array Object
    if(m_vaos.size() > 0)
    {
        glDeleteVertexArrays(m_vaos.size(), m_vaos.data());
    }

    if(m_positionBuffers.size() > 0)
    {
        glDeleteBuffers(m_positionBuffers.size(), m_positionBuffers.data());
    }
    
    if(m_indexBuffers.size() > 0)
    {
        glDeleteBuffers(m_indexBuffers.size(), m_indexBuffers.data());
    }    

    // Delete GLSL programs
    for(int i=0; i<m_programs.size(); i++)
    {
        glDeleteProgram(m_programs[i]);
    } 
}

//
// Static Methods
//

void GLAPIENTRY
SimpleScene::DebugMessageCallback(GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar *message,
                                  const void *userParam)
{
    // Message Source
    std::string sourceStr;
    switch(source) 
    {
        case(GL_DEBUG_SOURCE_API):
            sourceStr = "OpenGL API";
            break;
        case(GL_DEBUG_SOURCE_WINDOW_SYSTEM):
            sourceStr = "Window System API";
            break;
        case(GL_DEBUG_SOURCE_SHADER_COMPILER):
            sourceStr = "Shading Compiler";
            break;
        case(GL_DEBUG_SOURCE_THIRD_PARTY):
            sourceStr = "Third-party Application";
            break;
        case(GL_DEBUG_SOURCE_APPLICATION):
            sourceStr = "User";
            break;
        default:
            sourceStr = "Other";
    }

    // Message Type
    std::string typeStr;
    switch(type) 
    {
        case(GL_DEBUG_TYPE_ERROR):
            typeStr = "Error";
            break;
        case(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
            typeStr = "Deprecated";
            break;
        case(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
            typeStr = "Undefined";
            break;
        case(GL_DEBUG_TYPE_PORTABILITY):
            typeStr = "Portability";
            break;
        case(GL_DEBUG_TYPE_PERFORMANCE):
            typeStr = "Performance";
            break;
        default:
            typeStr = "Other";
    }

    // Message Severity
    std::string severityStr;
    switch(severity)
    {
        case(GL_DEBUG_SEVERITY_HIGH):
            severityStr = "HIGH";
            break;
        case(GL_DEBUG_SEVERITY_MEDIUM):
            severityStr = "MED";
            break;
        case(GL_DEBUG_SEVERITY_LOW):
            severityStr = "LOW";
            break;
        case(GL_DEBUG_SEVERITY_NOTIFICATION):
            severityStr = "INFO";
            break;
        default:
            severityStr = "UNK";
    }

    std::cout << "[GL Debug: " << sourceStr.c_str() << "] " << typeStr << ":" << severityStr << " => " << message << std::endl;
}

//
// Methods
//

void
SimpleScene::setShapeSelection(const int x, const int y)
{
    m_shapeSelectionIndex = m_shapeSelectionIndex == -1 ? 0 : -1;
    update();
    // TODO: m_shapeSelectionIndex = findShapeIntersection(x, y);
}

void
SimpleScene::updateShaderInputs(mog::Shape const *shapePtr, const GLuint program)
{
    // Material material = shapePtr->getMaterial();    

    // TODO: Create uniform blocks for lights and bind at global level to be shared
    // by all shaders
    GLint uniformLightPosition = glGetUniformLocation(program, "lightPosition"); // material.getLightPositionName
    GLint uniformLightAmbient = glGetUniformLocation(program, "lightAmbient");
    GLint uniformLightDiffuse = glGetUniformLocation(program, "lightDiffuse");
    GLint uniformLightSpecular = glGetUniformLocation(program, "lightSpecular");

    float lightPosition[] = {0.0f, 10.0f, -10.0f, 1.0f};
    float lightAmbient[] {0.3f, 0.3f, 0.3f, 1.0f};
    float lightDiffuse[]  = {0.7f, 0.7f, 0.7f, 1.0f};
    float lightSpecular[] = {0.5f, 0.5f, 1.0f, 1.0f};

    glUniform4fv(uniformLightPosition, 1, lightPosition); 
    glUniform4fv(uniformLightAmbient, 1, lightAmbient);
    glUniform4fv(uniformLightDiffuse, 1, lightDiffuse);
    glUniform4fv(uniformLightSpecular, 1, lightSpecular);

    // TODO: load textures

    // Retrieved from material
    GLint uniformMaterialAmbient = glGetUniformLocation(program, "materialAmbient");    // material.getAmbientName
    GLint uniformMaterialDiffuse = glGetUniformLocation(program, "materialDiffuse");
    GLint uniformMaterialSpecular = glGetUniformLocation(program, "materialSpecular");
    GLint uniformMaterialShininess = glGetUniformLocation(program, "materialShininess");
    GLint uniformMaterialColor = glGetUniformLocation(program, "materialColor");
    GLint uniformTextureUsed = glGetUniformLocation(program, "textureUsed");
    GLint uniformColorUsed = glGetUniformLocation(program, "colorUsed");

    float materialAmbient[]  = {1.0f, 0.5f, 0.5f, 1.0f};
    float materialDiffuse[]  = {1.0f, 0.7f, 0.7f, 1.0f};
    float materialSpecular[] = {0.4f, 0.4f, 0.4f, 1.0f};
    float materialShininess  = 5;
    float materialColor[] = {1.0f, 0.0f, 0.0f, 1.0f};

    glUniform4fv(uniformMaterialAmbient, 1, materialAmbient);
    glUniform4fv(uniformMaterialDiffuse, 1, materialDiffuse);
    glUniform4fv(uniformMaterialSpecular, 1, materialSpecular);
    glUniform1f(uniformMaterialShininess, materialShininess);
    glUniform4fv(uniformMaterialColor, 1, materialColor);
    glUniform1i(uniformTextureUsed, 0);
    glUniform1i(uniformColorUsed, 0);
}

void 
SimpleScene::initGLSL(mog::Shape * const shapePtr) 
{
    // TODO: loop through shapes
    // Material mat = shapePtr->getMaterial();

    // Init GLSL for all objects (sphere)
    const char *vertexShaderSource = m_sphere->getVertexShaderSource();     // mat.getVertexShaderSource();
    const char *fragmentShaderSource = m_sphere->getFragmentShaderSource(); // mat.getFragmentShaderSource();
    
    GLuint program1 = createShaderProgram(&vertexShaderSource, &fragmentShaderSource);
    m_programs.emplace_back(program1);
    glUseProgram(program1);

    GLuint vao1;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    m_vaos.emplace_back(vao1);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    m_positionBuffers.emplace_back(positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_sphere->getInterleavedVertexSize(), m_sphere->getInterleavedVertices(), GL_STATIC_DRAW);
    GLsizei stride = 8 * sizeof(float);
    
    // Position
    GLint positionLocation = glGetAttribLocation(program1, "position");
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, NULL);
    glEnableVertexAttribArray(positionLocation);
    // Normals
    GLint normalLocation = glGetAttribLocation(program1, "normal");
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, false, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(normalLocation);
    // Texture Coords
    GLint texCoordLocation = glGetAttribLocation(program1, "texCoord");
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, false, stride, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(texCoordLocation);

    // Indices
    GLuint indexBuffer1;
    glGenBuffers(1, &indexBuffer1);
    m_indexBuffers.emplace_back(indexBuffer1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sphere->sizeofIndices(), m_sphere->getIndices(), GL_STATIC_DRAW);

    // Line Indices
    GLuint indexBuffer2;
    glGenBuffers(1, &indexBuffer2);
    m_indexBuffers.emplace_back(indexBuffer2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sphere->sizeofLineIndices(), m_sphere->getLineIndices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
SimpleScene::initialize() 
{
    AbstractGLScene::initialize();
    // TODO: update progress
    // signal: sceneUpdated("Initializing...", 0)

    // Init OpenGL
//    cout << "GL_NV_depth_clamp supported: " << isExtensionSupported("GL_NV_depth_clamp") << endl;
    //glCullFace(GL_BACK);

    // Enable Debug Output
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(DebugMessageCallback, 0);

    // glShadeModel(GL_SMOOTH);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// Black
    glClearStencil(0);
    glClearDepth(1.0f);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3.0f);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDisable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);

    // TODO: loop through m_objects
    initGLSL(m_sphere);

    // TODO: For each shape??
    updateShaderInputs(m_sphere, m_programs[0]);
}

void
SimpleScene::resize(int width, int height) {
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    //d_projMatrix = vmath::perspective(45.0f, aspect, 0.1f, 1000.0f);
    m_projMatrix = glm::perspective(static_cast<float>(M_PI/4), aspect, 0.1f, 1000.0f);
}

void
SimpleScene::paint() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // For each shape
    glUseProgram(m_programs[0]);

    // Enable Vertex Array Object and buffers
    // glBindVertexArray(m_vao);   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[0]);

    GLint uniformColorUsed = glGetUniformLocation(m_programs[0], "colorUsed");
    glUniform1i(uniformColorUsed, 0);

    // Update Transforms
    // Column-major

    // Here's where you apply the interactive vars
    glm::mat4 mvMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_cameraDistance));
    mvMatrix = glm::rotate(mvMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mvMatrix = glm::rotate(mvMatrix, glm::radians(m_cameraAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    mvMatrix = glm::rotate(mvMatrix, glm::radians(m_cameraAngleX), glm::vec3(1.0f, 0.0f, 0.0f));

    // Normals are transformed by the inverse transpose of the matrix
    // See: https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
    glm::mat4 mvNormalMatrix = glm::transpose(glm::inverse(mvMatrix));    //mvMatrix;
    // glm::mat4 mvNormalMatrix = mvMatrix;
    // mvNormalMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    // mvMatrix = glm::rotate(mvMatrix, 1122245.0f * 17.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    // mvMatrix = glm::rotate(mvMatrix, 120.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    GLint mvLocation = glGetUniformLocation(m_programs[0], "mvMatrix");
    GLint mvpLocation = glGetUniformLocation(m_programs[0], "mvpMatrix");
    GLint projLocation = glGetUniformLocation(m_programs[0], "mvNormalMatrix");

    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(m_projMatrix * mvMatrix));
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(mvNormalMatrix));

    // std::cout << "Triangle count: " << m_sphere->getTriangleCount() << std::endl;

    // glDrawArrays(GL_TRIANGLES, 0, m_sphere->getTriangleCount());

    // Draw Sphere
    glDrawElements(GL_TRIANGLES,                // primitive type
                   m_sphere->getIndexCount(),   // # of indices
                   GL_UNSIGNED_INT,             // data type
                   (void*)0);                   // offset into the indices buffer

    // Draw selection outline
    if(m_shapeSelectionIndex == 0)
    {
        // Draw Lines
        // GLint uniformColorUsed = glGetUniformLocation(m_programs[0], "colorUsed");
        glUniform1i(uniformColorUsed, 1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[1]);

        glDrawElements(GL_LINES,
                    m_sphere->getLineIndexCount(),
                    GL_UNSIGNED_INT,
                    (void*)0);
    }
    // Triangle
    // glPatchParameteri(GL_PATCH_VERTICES, 16);
    // glDrawArrays(GL_PATCHES, 0, 16);

    // glDisableVertexAttribArray(m_attribIndex[0]);
    // glDisableVertexAttribArray(m_attribIndex[1]);
    // glDisableVertexAttribArray(m_attribIndex[2]);
    // glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void 
SimpleScene::printLinkerInfoLog(const GLuint program) {
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if(linkStatus == GL_FALSE) {
        GLint logLen;
        GLsizei msgLen;
        
        // Get log size
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        // Get linker info
        string msg;
        msg.reserve(logLen);
    
        glGetProgramInfoLog(program, logLen, &msgLen, const_cast<char *>(msg.c_str()));
        cout << "Linker Info: " << endl;
        cout << "============ " << endl;
        cout << msg.c_str() << endl;
    }
}

void
SimpleScene::printShaderInfoLog(const GLuint * const shader, const mog::Shader type) {
    GLint logLen;
    GLsizei msgLen;
    
    // Get log size
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLen);

    // Retrieve log info
    GLchar msg[logLen];
    glGetShaderInfoLog(*shader, logLen, &msgLen, msg);

    if(msgLen > 0) {
        switch(type) {
        case mog::Shader::VERTEX:
            cout << "Vertex Shader Info: " << endl;
            cout << "=================== " << endl;
            break;
        case mog::Shader::TESS_CONTROL:
            cout << "Tess Control Shader Info: " << endl;
            cout << "========================= " << endl;
            break;
        case mog::Shader::TESS_EVAL:
            cout << "Tess Eval Shader Info: " << endl;
            cout << "====================== " << endl;
            break;
        case mog::Shader::GEOMETRY:
            cout << "Geometry Shader Info: " << endl;
            cout << "===================== " << endl;
            break;
        case mog::Shader::FRAGMENT:
            cout << "Fragment Shader Info: " << endl;
            cout << "===================== " << endl;
            break;
        case mog::Shader::COMPUTE:
            cout << "Compute Shader Info: " << endl;
            cout << "==================== " << endl;
            break;
        }
            
        cout << msg << endl;
    }
}

GLuint
SimpleScene::compileComputeShaders()
{
    static const GLchar *computeShaderSource[] = 
    {
    "#version 430 core \n"
    "\n"
    "layout (local_size_x=32, local_size_y=32) in; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "    // Do nothing \n"
    "} \n"
    };

    auto computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, computeShaderSource, nullptr);
    printShaderInfoLog(&computeShader, mog::Shader::COMPUTE);

    // Create Program
    auto program = glCreateProgram();
    glAttachShader(program, computeShader);

    glLinkProgram(program);
    printLinkerInfoLog(program);

    int size[3];
    glGetProgramiv(program, GL_COMPUTE_WORK_GROUP_SIZE, size);
    cout << "Work group size is " << size[0] << " x " << size[1] << " x " << size[2] << "items." << endl;

    // Delete shaders as the program has them now
    glDeleteShader(computeShader);

    return program;
}

GLuint 
SimpleScene::createShaderProgram(const char * const *vs,
                                 const char * const *fs,
                                 const char * const *gs,
                                 const char * const *tcs,
                                 const char * const *tes)
{
    // Create Program
    GLuint program = glCreateProgram();

    // Create and compile vertex shader
    if(vs != nullptr) {        
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, vs, NULL);
        glCompileShader(vertexShader);
        printShaderInfoLog(&vertexShader, mog::Shader::VERTEX);

        glAttachShader(program, vertexShader);
        glDeleteShader(vertexShader);
    }

    // Create and compile tessellation control shader
    if(tcs != nullptr) {
        GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControlShader, 1, tcs, NULL);
        glCompileShader(tessControlShader);
        printShaderInfoLog(&tessControlShader, mog::Shader::TESS_CONTROL);

        glAttachShader(program, tessControlShader);
        glDeleteShader(tessControlShader);
    }

    // Create and compile tessellation evaluation shader
    if(tes != nullptr) {
        GLuint tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEvaluationShader, 1, tes, NULL);
        glCompileShader(tessEvaluationShader);
        printShaderInfoLog(&tessEvaluationShader, mog::Shader::TESS_EVAL);

        glAttachShader(program, tessEvaluationShader);
        glDeleteShader(tessEvaluationShader);
    }

    // Create and compile Geometry Shader
    if(gs != nullptr) {
        GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, gs, NULL);
        glCompileShader(geometryShader);
        printShaderInfoLog(&geometryShader, mog::Shader::GEOMETRY);

        glAttachShader(program, geometryShader);
        glDeleteShader(geometryShader);
    }

    // Create and compile fragment shader
    if(fs != nullptr) {
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fs, NULL);
        glCompileShader(fragmentShader);
        printShaderInfoLog(&fragmentShader, mog::Shader::FRAGMENT);

        glAttachShader(program, fragmentShader);
        glDeleteShader(fragmentShader);
    }

    // Modify program parameters before linking
//    glProgramParameteri(program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
    glLinkProgram(program);
    printLinkerInfoLog(program);

    return program;
}






