#include "RasterizationWidget.h"
#include "GLCanvas.h"

#include <QVBoxLayout>

//----------------------------------------------------------------------------------
RasterizationWidget::RasterizationWidget(QWidget *parent): QWidget(parent),
    m_canvas(nullptr)
{
	setMinimumSize(RasterizationWidget::WIDTH, RasterizationWidget::HEIGHT);

    // Add Rendering GLCanvas
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_canvas = new GLCanvas(RasterizationWidget::WIDTH, RasterizationWidget::HEIGHT);
    mainLayout->addWidget(m_canvas);

    setLayout(mainLayout);
}