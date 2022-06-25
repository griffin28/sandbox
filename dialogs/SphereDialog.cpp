#include "SphereDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QHBoxLayout>

#include <limits>

SphereDialog::SphereDialog(QWidget *parent) : QDialog(parent)
{
    m_radius = 1.0f;
    
    m_center = new float[3];
    m_center[0] = 0.0f;
    m_center[1] = 0.0f;
    m_center[2] = 0.0f;

    m_color = new QColor(Qt::white);

    initUI();

    QGridLayout *layout = new QGridLayout();
    // radius
    layout->addWidget(m_radiusLabel, 0, 0, 1, 1);
    layout->addWidget(m_radiusEdit, 0, 1, 1, 1);
    // center
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addWidget(m_centerLabel);
    centerLayout->addWidget(m_xLabel);
    centerLayout->addWidget(m_xEdit);
    centerLayout->addWidget(m_yLabel);
    centerLayout->addWidget(m_yEdit);
    centerLayout->addWidget(m_zLabel);
    centerLayout->addWidget(m_zEdit);
    layout->addLayout(centerLayout, 1, 0, 1, 4);
    // Color
    layout->addWidget(m_colorLabel, 2, 0, 1, 1);
    layout->addWidget(m_colorButton, 2, 1, 1, 2);
    // Divider
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setFixedHeight(2);
    separator->setLineWidth(1);
    layout->addWidget(separator, 3, 0, 2, 4);
    // Ok/Cancel
    layout->addWidget(m_okButton, 5, 2, 1, 1);
    layout->addWidget(m_cancelButton, 5, 3, 1, 1);

    setLayout(layout);
    setWindowTitle(tr("Add Sphere"));
    setMinimumSize(200, 100);
    setMaximumSize(300, 200);
}

SphereDialog::~SphereDialog()
{
    if(m_center != nullptr)
    {
        delete [] m_center;
    }

    if(m_color != nullptr)
    {
        delete m_color;
    }
}

void
SphereDialog::initUI()
{
    // Sphere radius
    m_radiusLabel = new QLabel(tr("Radius: "), this);
    m_radiusEdit = new QLineEdit(this);
    m_radiusEdit->setPlaceholderText(QString::number(m_radius));
    m_radiusEdit->setValidator(new QDoubleValidator(std::numeric_limits<float>::min(), 
                                               std::numeric_limits<float>::max(),
                                               2, this));
    m_radiusLabel->setBuddy(m_radiusEdit);
    connect(m_radiusEdit, &QLineEdit::editingFinished, this, &SphereDialog::radiusEditingFinished);

    // Sphere center
    m_centerLabel = new QLabel(tr("Center: "));

    m_xLabel = new QLabel(tr("x "), this);
    m_xEdit = new QLineEdit(this);
    m_xEdit->setPlaceholderText(QString::number(m_center[0]));
    m_xLabel->setBuddy(m_xEdit);
    connect(m_xEdit, &QLineEdit::editingFinished, this, &SphereDialog::centerXEditingFinished);
    // m_xEdit->setValidator(new QDoubleValidator(std::numeric_limits<float>::min(), 
                                            //    std::numeric_limits<float>::max(),
                                            //    2, this));

    m_yLabel = new QLabel(tr("y "));
    m_yEdit = new QLineEdit();
    m_yEdit->setPlaceholderText(QString::number(m_center[1]));
    m_yLabel->setBuddy(m_yEdit);
    connect(m_yEdit, &QLineEdit::editingFinished, this, &SphereDialog::centerYEditingFinished);
    // m_yEdit->setValidator(new QDoubleValidator(std::numeric_limits<float>::min(), 
                                            //    std::numeric_limits<float>::max(),
                                            //    2, this));

    m_zLabel = new QLabel(tr("z "));
    m_zEdit = new QLineEdit();
    m_zEdit->setPlaceholderText(QString::number(m_center[2]));
    // m_zEdit->setValidator(new QDoubleValidator(std::numeric_limits<float>::min(), 
    //                                            std::numeric_limits<float>::max(),
    //                                            2, this));
    m_zLabel->setBuddy(m_zEdit);
    connect(m_zEdit, &QLineEdit::editingFinished, this, &SphereDialog::centerZEditingFinished);

    // Sphere color
    m_colorLabel = new QLabel(tr("Color: "));

    m_colorButton = new QPushButton();
    m_colorButton->setDefault(false);
    QPalette palette = m_colorButton->palette();
    palette.setColor(QPalette::Button, *m_color);
    m_colorButton->setAutoFillBackground(true);
    m_colorButton->setPalette(palette);
    m_colorButton->update();

    connect(m_colorButton, &QAbstractButton::pressed, this, &SphereDialog::showColorDialog);

    // Ok/Cancel 
    m_okButton = new QPushButton(tr("Ok"));
    m_okButton->setDefault(true);
    connect(m_okButton, &QAbstractButton::pressed, this, &SphereDialog::accept);

    m_cancelButton = new QPushButton(tr("Cancel"));
    m_cancelButton->setDefault(true);
    connect(m_cancelButton, &QAbstractButton::pressed, this, &SphereDialog::reject);
}

void
SphereDialog::getColor(float *color)
{
    color[0] = m_color->redF();
    color[1] = m_color->greenF();
    color[2] = m_color->blueF();
    color[3] = m_color->alphaF();
}

// SLOTS
//---------------------------------------------------------------------------------

void 
SphereDialog::showColorDialog()
{
    QColor color = QColorDialog::getColor(*m_color, this);
    
    if(color.isValid())
    {
        delete m_color;
        m_color = new QColor(color);

        // Update button color to match
        QPalette palette = m_colorButton->palette();
        palette.setColor(QPalette::Button, *m_color);
        m_colorButton->setPalette(palette);
        m_colorButton->update();
    }
}

void
SphereDialog::radiusEditingFinished()
{
    bool ok;
    float temp = m_radiusEdit->text().toFloat(&ok);    
    m_radius = ok ? temp : m_radius;
}

void
SphereDialog::centerXEditingFinished()
{
    bool ok;
    float temp = m_xEdit->text().toFloat(&ok);
    m_center[0] = ok ? temp : m_center[0];
}

void
SphereDialog::centerYEditingFinished()
{
    bool ok;
    float temp = m_yEdit->text().toFloat(&ok);
    m_center[1] = ok ? temp : m_center[1];
}

void
SphereDialog::centerZEditingFinished()
{
    bool ok;
    float temp = m_zEdit->text().toFloat(&ok);
    m_center[2] = ok ? temp : m_center[2];
}
