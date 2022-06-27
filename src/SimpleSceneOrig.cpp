#include "SimpleScene.h"
#include "Canvas.h"

#include <iostream>

#include <QTimer>

using namespace std;

SimpleScene::SimpleScene(Canvas *glWidget):
    AbstractGLScene(glWidget),
    d_angle(0.0f),
    d_program(0),
    d_vao(0),
    d_buffer(0),
    d_indexBuffer(0),
    d_mvLocation(0),
    d_mvLocation1(0),
    d_projLocation(0),
    d_projMatrix(),
    d_timer(nullptr) { }

SimpleScene::~SimpleScene() {
    glDeleteVertexArrays(1, &d_vao);
    glDeleteProgram(d_program);
}

//
// Slots
//

void
SimpleScene::handleTimer() {
//    cout << "Timer called" << endl;
    if(d_angle >= 355.0f)
        d_angle = 0.0f;
    else
        d_angle += 5.0f;
    this->update();
    emit sceneUpdated();
}

//
// Methods
//

void
SimpleScene::initialize() {
    AbstractGLScene::initialize();
//    cout << "GL_NV_depth_clamp supported: " << isExtensionSupported("GL_NV_depth_clamp") << endl;
    //glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// Black
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    d_program = compileShaders();

    // Uniform locations
    d_mvLocation = glGetUniformLocation(d_program, "mv_matrix");
    d_mvLocation1 = glGetUniformLocation(d_program, "mv_matrix1");
    d_projLocation = glGetUniformLocation(d_program, "proj_matrix");

    glGenVertexArrays(1, &d_vao);
    glBindVertexArray(d_vao);
/*
    const GLfloat data[] = {
        -0.25f,  0.25f, -0.25f,
        -0.25f, -0.25f, -0.25f,
         0.25f, -0.25f, -0.25f,

         0.25f, -0.25f, -0.25f,
         0.25f,  0.25f, -0.25f,
        -0.25f,  0.25f, -0.25f,

         0.25f, -0.25f, -0.25f,
         0.25f, -0.25f,  0.25f,
         0.25f,  0.25f, -0.25f,

         0.25f, -0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,
         0.25f,  0.25f, -0.25f,

         0.25f, -0.25f,  0.25f,
        -0.25f, -0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,

        -0.25f, -0.25f,  0.25f,
        -0.25f,  0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,

        -0.25f, -0.25f,  0.25f,
        -0.25f, -0.25f, -0.25f,
        -0.25f,  0.25f,  0.25f,

        -0.25f, -0.25f, -0.25f,
        -0.25f,  0.25f, -0.25f,
        -0.25f,  0.25f,  0.25f,

        -0.25f, -0.25f,  0.25f,
         0.25f, -0.25f,  0.25f,
         0.25f, -0.25f, -0.25f,

         0.25f, -0.25f, -0.25f,
        -0.25f, -0.25f, -0.25f,
        -0.25f, -0.25f,  0.25f,

        -0.25f,  0.25f, -0.25f,
         0.25f,  0.25f, -0.25f,
         0.25f,  0.25f,  0.25f,

         0.25f,  0.25f,  0.25f,
        -0.25f,  0.25f,  0.25f,
        -0.25f,  0.25f, -0.25f
    };
*/

    const GLfloat data[] = {
        -0.25f, -0.25f, -0.25f,
        -0.25f, 0.25f, -0.25f,
        0.25f, -0.25f, -0.25f,
        0.25f, 0.25f, -0.25f,
        0.25f, -0.25f, 0.25f,
        0.25f, 0.25f, 0.25f,
        -0.25f, -0.25f, 0.25f,
        -0.25f, 0.25f, 0.25f,
    };

    const GLfloat instancePositions[] = {
         0.0f,  0.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    };

    const GLushort vertex_indices[] = {
        0, 1, 2,
        2, 1, 3,
        2, 3, 4,
        4, 3, 5,
        4, 5, 6,
        6, 5, 7,
        6, 7, 0,
        0, 7, 1,
        6, 0, 2,
        2, 4, 6,
        7, 5, 3,
        7, 3, 1
    };

//    glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), data);

    // Array Buffer
    glGenBuffers(1, &d_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, d_buffer); 
//    glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), data, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    // Allocate buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(data) + sizeof(instancePositions), NULL, GL_STATIC_DRAW);
    
    // Add vertex data to buffer
    GLuint offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(data), data);

    // Add instance data to buffer
    offset += sizeof(data);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instancePositions), instancePositions);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(data)));
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(1, 1);
 
    // Index Buffer
    glGenBuffers(1, &d_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    // Start the update timer
    if(d_timer == nullptr) {
    	d_timer = new QTimer(this);
    	connect(d_timer, &QTimer::timeout, this, &SimpleScene::handleTimer);
        d_timer->start(50);
    } else {
        d_timer->start(50);
    }
}

void
SimpleScene::resize(int width, int height) {
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    d_projMatrix = vmath::perspective(45.0f, aspect, 0.1f, 1000.0f);
}

void
SimpleScene::paint() {
//	float red[] = {d_intensity, 0.0f, 0.0f, 1.0f};
//	this->glClearBufferfv(GL_COLOR, 0, red);

//    GLDouble attrib[] = {0, d_offset, 0, 0};
//    GLfloat attrib[] = {0.0f, 0.0f, 0.0f, 0.0f};
//    glVertexAttribL4dv(0, attrib);
//    glVertexAttrib4fv(0, attrib);
//    glPointSize(15.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(d_program);

    glUniformMatrix4fv(d_projLocation, 1, GL_FALSE, d_projMatrix);

    // Create model-view matrix
    vmath::mat4 mvMatrix = vmath::translate(0.0f, 0.0f, -4.0f) *
                           vmath::translate(sinf(2.1f) * 0.5f,
                                            cosf(1.7f) * 0.5f,
                                            sinf(1.3f) * cosf(1.5f) * 2.0f) *
                           vmath::rotate(d_angle, 0.0f, 1.0f, 0.0f);
    glUniformMatrix4fv(d_mvLocation, 1, GL_FALSE, mvMatrix);

    vmath::mat4 mvMatrix1 = vmath::translate(0.0f, 0.0f, -4.0f) *
                           vmath::translate(sinf(2.1f) * 0.5f,
                                            cosf(1.7f) * 0.5f,
                                            sinf(1.3f) * cosf(1.5f) * 2.0f) *
                           vmath::rotate(-d_angle, 0.0f, 1.0f, 0.0f);
    glUniformMatrix4fv(d_mvLocation1, 1, GL_FALSE, mvMatrix1);

    // Triangle
//    glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, nullptr);
//    glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);
//    glPatchParameteri(GL_PATCH_VERTICES, 3);
//    glDrawArrays(GL_PATCHES, 0, 3);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr); // try nullptr
    glDrawElementsInstanced(GL_PATCHES, 36, GL_UNSIGNED_SHORT, 0, 5);
}

void 
SimpleScene::printLinkerInfoLog(const GLuint program) {
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

void
SimpleScene::printShaderInfoLog(const GLuint * const shader, const Shader type) {
    GLint logLen;
    GLsizei msgLen;
    
    // Get log size
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLen);

    // Retrieve log info
    GLchar msg[logLen];
    glGetShaderInfoLog(*shader, logLen, &msgLen, msg);

    if(msgLen > 0) {
        switch(type) {
        case Shader::VERTEX:
            cout << "Vertex Shader Info: " << endl;
            cout << "=================== " << endl;
            break;
        case Shader::TESS_CONTROL:
            cout << "Tess Control Shader Info: " << endl;
            cout << "========================= " << endl;
            break;
        case Shader::TESS_EVAL:
            cout << "Tess Eval Shader Info: " << endl;
            cout << "====================== " << endl;
            break;
        case Shader::GEOMETRY:
            cout << "Geometry Shader Info: " << endl;
            cout << "===================== " << endl;
            break;
        case Shader::FRAGMENT:
            cout << "Fragment Shader Info: " << endl;
            cout << "===================== " << endl;
            break;
        }
            
        cout << msg << endl;
    }
}

GLuint
SimpleScene::compileShaders() {
    GLuint vertexShader;
    GLuint tessControlShader;
    GLuint tessEvaluationShader;
    GLuint geometryShader;
    GLuint fragmentShader;
    GLuint program;

    // Source code for vertex shader
//    "#version 430 core \n"
//	"\n"
//	"void main() \n"
//	"{ \n"
//    "    const vec4 vertices[3] = vec4[3](vec4(0.25,-0.25,0.5,1.0), vec4(-0.25,-0.25,0.5,1.0), vec4(0.25,0.25,0.5,1.0)); \n"
//	"    gl_Position = vertices[gl_VertexID]; \n"
//	"} \n"
    //"layout (location = 0) in vec4 offset; \n"
    static const GLchar *vertexShaderSource[] = 
    {
	"#version 430 core \n"
    "\n"
    "layout (location = 0) in vec4 position; \n"
    "layout (location = 1) in vec4 instance_position; \n"
    "\n"
    "out VS_OUT \n"
    "{ \n"
    "     vec4 color; \n"
    "} vs_out; \n"
	"\n"
    "uniform mat4 mv_matrix; \n"
    "uniform mat4 mv_matrix1; \n"
    "uniform mat4 proj_matrix; \n"
    "\n"
	"void main() \n"
	"{ \n"
    "    if(gl_InstanceID == 0) \n"
    "    { \n"
    "    gl_Position = proj_matrix * mv_matrix * (position + instance_position); \n"
    "    } \n"
    "    else \n"
    "    { \n"
    "    gl_Position = proj_matrix * mv_matrix1 * (position + instance_position); \n"
    "    } \n"
    "    vs_out.color = position * 2.0 + vec4(0.5,0.5,0.5,0.0); \n"
	"} \n"
    };

    // Source code for tessellation control shader
    // Responsible for: 1. Determining the level of tessellation; 2. Generation of data sent to tess eval shader
    // Max control points guaranteed to be 32
    static const GLchar *tessellationControlShaderSource[] =
    {
    "#version 430 core \n"
    "layout (vertices = 3) out; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "    if(gl_InvocationID == 0) \n"
    "    { \n"
    "        gl_TessLevelInner[0] = 5.0; \n"
    "        gl_TessLevelOuter[0] = 10.0; \n"
    "        gl_TessLevelOuter[1] = 10.0; \n"
    "        gl_TessLevelOuter[2] = 10.0; \n"
    "    } \n"
    "    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; \n"
    "} \n"
    };

    // Source code for tessellation evaluation shader
    static const GLchar *tessellationEvaluationShaderSource[] =
    {
    "#version 430 core \n"
    "\n"
    "layout (triangles) in; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + (gl_TessCoord.y * gl_in[1].gl_Position) + (gl_TessCoord.z * gl_in[2].gl_Position); \n"
    "} \n"
    };

    static const GLchar *geometryShaderSource[] =
    {
	"#version 430 core \n"
 	"\n"
	"layout (triangles) in; \n"
	"layout (points, max_vertices = 3) out; \n"
	"\n"
	"void main(void) \n"
	"{ \n"
        "  int i; \n"
        "  for(i=0; i < gl_in.length(); i++) \n"
        "  { \n"
	"      gl_Position = gl_in[i].gl_Position; \n"
	"      EmitVertex(); \n"
        "  } \n"
	"} \n"

    };

    // Source code for fragment shader
    static const GLchar *fragmentShaderSource[] = 
    {
	"#version 430 core \n"
 	"\n"
	"out vec4 color; \n"
	"\n"
   // "in TESS_OUT                                                          \n"
   // "{                                                                  \n"
   // "    vec4 color;                                                    \n"
    //"} fs_in; \n"
    "\n"
	"void main(void) \n"
	"{ \n"
    //"    color = fs_in.color; \n"
        "  color = vec4(1.0,0.0,0.0,1.0); \n"
	"} \n"
    };

    // Create and compile vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    printShaderInfoLog(&vertexShader, Shader::VERTEX);

    // Create and compile tessellation control shader
    tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessControlShader, 1, tessellationControlShaderSource, NULL);
    glCompileShader(tessControlShader);
    printShaderInfoLog(&tessControlShader, Shader::TESS_CONTROL);

    // Create and compile tessellation evaluation shader
    tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessEvaluationShader, 1, tessellationEvaluationShaderSource, NULL);
    glCompileShader(tessEvaluationShader);
    printShaderInfoLog(&tessEvaluationShader, Shader::TESS_EVAL);

    // Create and compile Geometry Shader
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, geometryShaderSource, NULL);
    glCompileShader(geometryShader);
    printShaderInfoLog(&geometryShader, Shader::GEOMETRY);

    // Create and compile fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    printShaderInfoLog(&fragmentShader, Shader::FRAGMENT);

    // Create Program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, tessControlShader);
    glAttachShader(program, tessEvaluationShader);
//    glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);

    // Modify program parameters before linking
//    glProgramParameteri(program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

    glLinkProgram(program);
    printLinkerInfoLog(program);

    // Delete shaders as the program has them now
    glDeleteShader(vertexShader);
    glDeleteShader(tessControlShader);
    glDeleteShader(tessEvaluationShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    return program;
}







