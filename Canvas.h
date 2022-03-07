#ifndef INCLUDED_CANVAS_H_
#define INCLUDED_CANVAS_H_

#include <QOpenGLWidget>

class QKeyEvent;
class SimpleScene;

class Canvas : public QOpenGLWidget {

	Q_OBJECT

public:
    Canvas(QWidget *parent=0);
    ~Canvas();
    
    static const int HEIGHT;
    static const int WIDTH;

    SimpleScene *getScene();

signals:
    void screenCoordsChanged(const int, const int); 
/* signals:
	void rotateX(int);
	void rotateY(int);
	void rotateZ(int);
*/

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int, int) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    SimpleScene *m_scene;

    // Mouse
    bool    m_mouseLeftDown;
    bool    m_mouseRightDown;
    int     m_posX;
    int     m_posY;

};

#endif /* CANVAS_H_ */
