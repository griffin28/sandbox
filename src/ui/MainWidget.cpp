#include "MainWidget.h"
#include "Canvas.h"

#include <QVBoxLayout>

//----------------------------------------------------------------------------------
MainWidget::MainWidget(QWidget *parent): QWidget(parent),
    m_canvas(new Canvas(MainWidget::WIDTH, MainWidget::HEIGHT, this)),
    m_currentRendererType(RendererType::RASTERIZATION)
{
	setMinimumSize(MainWidget::WIDTH, MainWidget::HEIGHT);
	
    // Add Rendering Canvas
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_canvas);

    setLayout(mainLayout);
}

//----------------------------------------------------------------------------------
void MainWidget::setRendererType(const RendererType type)
{
    if(m_currentRendererType != type)
    {
        m_currentRendererType = type;
        // QLayout *oldLayout = layout();

        // if(oldLayout != nullptr)
        // {
        //     delete oldLayout;
        // }

        // QVBoxLayout *newLayout = new QVBoxLayout;

        // if(type == RendererType::RASTERIZATION)
        // {
        //     newLayout->addWidget(m_canvas);
        //     setLayout(newLayout);
        // }
        // else if(type == RendererType::PATHTRACING)
        // {
        //     setLayout(newLayout);
        // }
    }
}