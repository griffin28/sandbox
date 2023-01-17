#include "MainWidget.h"
#include "Canvas.h"

#include <QVBoxLayout>

//----------------------------------------------------------------------------------
MainWidget::MainWidget(QWidget *parent): QWidget(parent),
    m_canvas(nullptr)
{
	setMinimumSize(MainWidget::WIDTH, MainWidget::HEIGHT);
	
    // Add Rendering Canvas
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_canvas = new Canvas(MainWidget::WIDTH, MainWidget::HEIGHT);
    mainLayout->addWidget(m_canvas);

    setLayout(mainLayout);
}