#ifndef INCLUDED_HUD_H
#define INCLUDED_HUD_H

#include <QGraphicsView>

class HUD : public QGraphicsView 
{
    Q_OBJECT

public:
    HUD(QWidget *);
    ~HUD() = default;

    void    setRenderingMode(const char *);

public slots:
    void    setRenderTime(const float);
    
private:
    QRectF                  *m_position;
    QGraphicsScene          *m_scene;
    // border
    QGraphicsRectItem       *m_border;    
    // Rendering Mode
    QGraphicsSimpleTextItem *m_renderingModeTextItem;
    // FPS: 12, Frame Time: 87.88 ms
    QGraphicsSimpleTextItem *m_renderTimeTextItem;
    // Shape Count: 
};

#endif 