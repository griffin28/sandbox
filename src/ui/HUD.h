#ifndef INCLUDED_HUD_H
#define INCLUDED_HUD_H

#include <QGraphicsView>

class QElapsedTimer;

/**
 * @class HUD
 * @brief Heads-up Display
 * 
 * Semi-transparent overlay displaying rendering details like rendering mode and 
 * frames-per-second.
 * 
*/
class HUD : public QGraphicsView 
{
    Q_OBJECT

public:
    /**
     * Constructs a Heads-up Display.
     * @param parent this widget's parent
    */
    HUD(QWidget *parent);

    /**
     * Default destructor.
    */
    ~HUD() = default;

    /**
     * Sets the current rendering mode.
     * @param mode the current rendering mode (rasterization, pathtracing, etc.)
    */
    void    setRenderingMode(const char *mode);
public slots:
    /**
     * Update the frames per second (FPS) and the frame render time in the HUD.
     * @param nsecs the number of nanoseconds to render the frame 
     * 
    */
    void    updateFrameRenderTime(long long int nsecs);
    
private:
    QElapsedTimer           *m_fpsTimer;
    QRectF                  *m_position;
    QGraphicsScene          *m_scene;
    QGraphicsRectItem       *m_border;    
    QGraphicsSimpleTextItem *m_renderingModeTextItem;
    QGraphicsSimpleTextItem *m_renderTimeTextItem;
};

#endif 