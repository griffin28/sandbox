#include "AbstractGLScene.h"

#include <iostream>
#include <string>

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QDebug>

using namespace std;

AbstractGLScene::AbstractGLScene(QOpenGLWidget *glWidget) : d_glWidget(glWidget) { }

AbstractGLScene::~AbstractGLScene() { }

void
AbstractGLScene::initialize() {
	if(!this->initializeOpenGLFunctions()) {
		qFatal("initializeOpenGLFunctions Failed!");
	}
}

void
AbstractGLScene::update() {
    d_glWidget->update();
}

QOpenGLWidget *
AbstractGLScene::glWidget() const {
	return d_glWidget;
}

QOpenGLContext *
AbstractGLScene::context() {
	return d_glWidget ? d_glWidget->context() : nullptr;
}

const QOpenGLContext *
AbstractGLScene::context() const {
	return d_glWidget ? d_glWidget->context() : nullptr;
}

bool
AbstractGLScene::isExtensionSupported(const char *extension) {
    bool isSupported = false;
    int numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for(int i=0; i<numExtensions; i++) {
        const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
        if(strcmp(extension, reinterpret_cast<const char *>(ext)) == 0) {
            isSupported = true;
            break;
        }
    }

    return isSupported;
}

void
AbstractGLScene::printAvailableExtensions() {
    cout << "Supported Extensions: " << endl;
    cout << "===================== " << endl;

    int numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for(int i=0; i<numExtensions; i++) {
        const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
        cout << ext << endl;
//      string extStr(reinterpret_cast<const char *>(ext));
    }

    cout << "===================== " << endl;
}
