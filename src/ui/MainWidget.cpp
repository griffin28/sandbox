#include "MainWidget.h"
#include "Canvas.h"

#include <QVBoxLayout>

//----------------------------------------------------------------------------------
MainWidget::MainWidget(QWidget *parent): QWidget(parent),
    m_canvas(new Canvas(MainWidget::WIDTH, MainWidget::HEIGHT, this)),
    m_currentRendererType(sandbox::RendererType::RASTERIZATION)
{
	setMinimumSize(MainWidget::WIDTH, MainWidget::HEIGHT);
	
    // Add Rendering Canvas
    QVBoxLayout *mainLayout = new QVBoxLayout;

	//vlayout1->addStretch(1);
    mainLayout->addWidget(m_canvas);
    setLayout(mainLayout);
}