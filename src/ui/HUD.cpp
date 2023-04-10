#include "HUD.h"

#include <iostream>

#include <QGraphicsRectItem>
#include <QElapsedTimer>

//----------------------------------------------------------------------------------
HUD::HUD(QWidget *parent): QGraphicsView(parent)
{
    float width = static_cast<float>(parent->width()) / 4.0f;
    float height = static_cast<float>(parent->height()) / 6.0f;

    // Scene
    m_scene = new QGraphicsScene(0, 0, width, height);
    // m_scene->setForegroundBrush(Qt::white);
    // m_scene->setBackgroundBrush(QColor(255,255,255,10));

    // Border
    m_border = m_scene->addRect(2, 2, width-4, height-4);
    QPen borderPen(Qt::black);
    borderPen.setWidth(1);
    m_border->setPen(borderPen);

    // Text
    QPen textPen(Qt::white);
    textPen.setWidth(1);

    QFont textFont("Arial", 9, QFont::Thin);

    m_renderingModeTextItem = m_scene->addSimpleText("Render Mode: Rasterization", textFont);
    m_renderingModeTextItem->setParentItem(m_border);
    m_renderingModeTextItem->setPos(4, 4);
    m_renderingModeTextItem->setBrush(Qt::white);
    // m_renderingModeTextItem->setPen(textPen);

    m_cameraTextItem = m_scene->addSimpleText("Camera: Perspective", textFont);
    m_cameraTextItem->setParentItem(m_border);
    QRectF br1 = m_renderingModeTextItem->sceneBoundingRect();
    auto h1 = br1.height() + 4;
    m_cameraTextItem->setPos(4, h1);
    m_cameraTextItem->setBrush(Qt::white);

    QString renderTimeStr = "FPS: 0 Frame time: 0 ms";
    m_renderTimeTextItem = m_scene->addSimpleText(renderTimeStr, textFont);
    m_renderTimeTextItem->setParentItem(m_border);
    QRectF br2 = m_cameraTextItem->sceneBoundingRect();
    auto h2 = br1.height() + br2.height() + 4;
    m_renderTimeTextItem->setPos(4, h2);
    m_renderTimeTextItem->setBrush(Qt::white);

    this->setScene(m_scene);

    // Customize view
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgba(0, 0, 0, 0.3)");
}

//----------------------------------------------------------------------------------
void
HUD::setRenderingMode(const char *mode)
{
    QString modeStr = "Render Mode: " + QString(mode);
    m_renderingModeTextItem->setText(modeStr);
}

//----------------------------------------------------------------------------------
void
HUD::setCameraType(const char *type)
{
    QString typeStr = "Camera: " + QString(type);
    m_cameraTextItem->setText(typeStr);
}

// SLOTS
//----------------------------------------------------------------------------------
void
HUD::updateFrameRenderTime(long long int nsecs)
{
    float elapsedTime = static_cast<float>(nsecs) / 1e+6;   // milliseconds
    float fps = (1.0f / elapsedTime) * 1000.0f;

    QString timeText = "FPS: " + QString::number(fps, 'f', 2) + " Frame time: " + QString::number(elapsedTime, 'f', 2) + " ms";
    m_renderTimeTextItem->setText(timeText);
}