#include "GLCanvas.h"
#include "RasterizationScene.h"

#include <iostream>

#include <QtGui>
#include <QKeyEvent>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLContext>
#include <QElapsedTimer>

//const GLfloat GLCanvas::RED[] = {1.0f, 0.0f, 0.0f, 1.0f};
//----------------------------------------------------------------------------------
GLCanvas::GLCanvas(int w, int h, QWidget *parent) : QOpenGLWidget(parent),
                                                m_width(w),
												m_height(h),
					        					m_scene(new RasterizationScene(this)),
												m_mouseLeftDown(false),
												m_mouseRightDown(false),
												m_posX(0),
												m_posY(0)
{
    setMinimumSize(w, h);
    setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
}

//----------------------------------------------------------------------------------
GLCanvas::~GLCanvas()
{
    // Make sure the context is current and then explicitly
    // destroy all underlying OpenGL resources.
    makeCurrent();
    delete m_scene;
    doneCurrent();
}

//----------------------------------------------------------------------------------
RasterizationScene *
GLCanvas::getScene()
{
    return m_scene;
}

//----------------------------------------------------------------------------------
void
GLCanvas::mousePressEvent(QMouseEvent *event)
{
	switch(event->button())
	{
	case Qt::LeftButton:
		m_mouseLeftDown = true;
		m_scene->setShapeSelection(event->x(), event->y());
		break;
	case Qt::RightButton:
		m_mouseRightDown = true;
		break;
	default:
		QOpenGLWidget::mousePressEvent(event);
	}
}

//----------------------------------------------------------------------------------
void
GLCanvas::mouseReleaseEvent(QMouseEvent *event)
{
	switch(event->button())
	{
	case Qt::LeftButton:
		m_mouseLeftDown = false;

		break;
	case Qt::RightButton:
		m_mouseRightDown = false;
		break;
	default:
		QOpenGLWidget::mouseReleaseEvent(event);
	}
}

//----------------------------------------------------------------------------------
void
GLCanvas::mouseMoveEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();

	emit screenCoordsChanged(x, y);

	if(m_mouseLeftDown)
	{
		m_scene->m_cameraAngleX += (x - m_posX);
		m_scene->m_cameraAngleY += (y - m_posY);

		m_posX = x;
		m_posY = y;

		m_scene->update();
	}
	else if(m_mouseRightDown)
	{
		m_scene->m_cameraDistance -= (y - m_posY) * 0.2f;
		m_posY = y;

		m_scene->update();
	}
	else
	{
		QOpenGLWidget::mouseMoveEvent(event);
	}
}

//----------------------------------------------------------------------------------
void
GLCanvas::keyPressEvent(QKeyEvent *event) {
//	QOpenGLWidgetGLWidget::keyPressEvent(event);
//	double offset;

//	switch(event->key()) {
//	case Qt::Key_Up:
////		std::cout << "Key_up" << std::endl;
//		offset = d_scene->getOffset() + 0.05;

//		if(offset > 1) {
//			offset = 1;
//		}

//		d_scene->setOffset(offset);
//		break;
//	case Qt::Key_Down:
////		std::cout << "Key_down" << std::endl;
//		offset = d_scene->getOffset() - 0.05;

//		if(offset < 0) {
//			offset = 0;
//		}

//		d_scene->setOffset(offset);
//		break;
//	default:
//		QOpenGLWidget::keyPressEvent(event);
//	}
}

//----------------------------------------------------------------------------------
// Sets up the OpenGL resources and state. Gets called once before the first time resizeGL() or paintGL() is called.
void
GLCanvas::initializeGL()
{
    m_scene->initialize();
}

//----------------------------------------------------------------------------------
//Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized
//(and also when it is shown for the first time because all newly created widgets get a resize event automatically).
void
GLCanvas::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings
    m_scene->resize(w, h);
}

//----------------------------------------------------------------------------------
// Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
void
GLCanvas::paintGL()
{
	QElapsedTimer timer;
	timer.start();
    m_scene->paint();
	emit frameRenderTimeChanged(timer.nsecsElapsed());
}
