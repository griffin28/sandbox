#include "AbstractGLScene.h"

#include <iostream>
#include <string>

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QDebug>

AbstractGLScene::AbstractGLScene(QOpenGLWidget *glWidget) : m_glWidget(glWidget) {}

//----------------------------------------------------------------------------------
void AbstractGLScene::initialize() 
{
    if(!this->initializeOpenGLFunctions()) 
    {
        qFatal("initializeOpenGLFunctions Failed!");
    }
}

//----------------------------------------------------------------------------------
void AbstractGLScene::update() 
{
    m_glWidget->update();
}

//----------------------------------------------------------------------------------
QOpenGLWidget *AbstractGLScene::glWidget() const 
{
	return m_glWidget;
}

//----------------------------------------------------------------------------------
QOpenGLContext *AbstractGLScene::context() 
{
	return m_glWidget ? m_glWidget->context() : nullptr;
}

//----------------------------------------------------------------------------------
const QOpenGLContext *AbstractGLScene::context() const 
{
	return m_glWidget ? m_glWidget->context() : nullptr;
}

//----------------------------------------------------------------------------------
bool AbstractGLScene::isExtensionSupported(const char *extension) 
{
    bool isSupported = false;
    int numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for(int i=0; i<numExtensions; i++) 
    {
        const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
        
        if(strcmp(extension, reinterpret_cast<const char *>(ext)) == 0) 
        {
            isSupported = true;
            break;
        }
    }

    return isSupported;
}

//----------------------------------------------------------------------------------
void AbstractGLScene::printAvailableExtensions() 
{
    std::cout << "Supported Extensions: " << std::endl;
    std::cout << "===================== " << std::endl;

    int numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for(int i=0; i<numExtensions; i++) 
    {
        const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
        std::cout << ext << std::endl;
//      string extStr(reinterpret_cast<const char *>(ext));
    }

    std::cout << "===================== " << std::endl;
}
