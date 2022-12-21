#include "MainWidget.h"
#include "Canvas.h"

#include <QVBoxLayout>

const int WIDTH = 900;
const int HEIGHT = 900;

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
	setMinimumSize(WIDTH, HEIGHT);
	//setMaximumSize(900, 800);

	initialize();
	setupLayout();

	//connect(nearSpinbox, SIGNAL(valueChanged(int)), this, SLOT(nearValueChanged(int)));
}

MainWidget::~MainWidget()
{
    if(m_canvas != NULL)
    {
        delete m_canvas;
    }
    
    // Having widgets in a parent widget allows them to get deleted by the parent
}

void 
MainWidget::initialize() 
{
    m_canvas = new Canvas(WIDTH, HEIGHT, this);
}

void 
MainWidget::setupLayout() 
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Canvas
	//vlayout1->addStretch(1);
    mainLayout->addWidget(m_canvas);

    setLayout(mainLayout);
}
