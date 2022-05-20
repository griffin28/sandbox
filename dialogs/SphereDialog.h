#ifndef INCLUDED_SPHERE_DIALOG_H
#define INCLUDED_SPHERE_DIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QColor;

class SphereDialog : public QDialog
{
    Q_OBJECT

public:
    SphereDialog(QWidget *parent = nullptr);
    ~SphereDialog();

    void initUI();

    float getRadius() const { return m_radius; }
    float *getCenter() const { return m_center; }
    void getColor(float *, float *, float *, float *);

public slots:
    void showColorDialog();
    void radiusEditingFinished();
    void centerXEditingFinished();
    void centerYEditingFinished();
    void centerZEditingFinished();

private:
    QLabel *m_radiusLabel;
    QLabel *m_centerLabel;
    QLabel *m_xLabel;
    QLabel *m_yLabel;
    QLabel *m_zLabel;
    QLabel *m_colorLabel;

    QLineEdit *m_radiusEdit;
    QLineEdit *m_xEdit;
    QLineEdit *m_yEdit;
    QLineEdit *m_zEdit;

    QPushButton *m_colorButton;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;

    float m_radius;
    float *m_center;
    QColor *m_color;
};


#endif