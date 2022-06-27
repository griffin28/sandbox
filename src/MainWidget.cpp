#include "MainWidget.h"
#include "Canvas.h"

#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent): QWidget(parent)
{
	setMinimumSize(Canvas::WIDTH, Canvas::HEIGHT);
	//setMaximumSize(900, 800);

	initialize();
	setupLayout();

	//connect(nearSpinbox, SIGNAL(valueChanged(int)), this, SLOT(nearValueChanged(int)));
}

MainWidget::~MainWidget()
{
    if(d_canvas != NULL)
        delete d_canvas;
    
    // Having widgets in a parent widget allows them to get deleted by the parent
}

void 
MainWidget::initialize() {
    d_canvas = new Canvas(this);
}

void 
MainWidget::setupLayout() {
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Canvas
	//vlayout1->addStretch(1);
    mainLayout->addWidget(d_canvas);

    setLayout(mainLayout);
}
