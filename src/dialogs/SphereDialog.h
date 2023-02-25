#ifndef INCLUDED_SPHERE_DIALOG_H
#define INCLUDED_SPHERE_DIALOG_H

#include <QDialog>
#include <memory>

class QLabel;
class QLineEdit;
class QPushButton;
class QColor;

/// @class SphereDialog
/// @brief Dialog for entering sphere parameters to create a single sphere
///        or a specified number of random spheres
class SphereDialog : public QDialog
{
    Q_OBJECT

public:
    /// @brief Constructs a sphere dialog
    /// @param parent this dialog's parent
    SphereDialog(QWidget *parent = nullptr);

    /// Default Destructor
    ~SphereDialog() = default;

    /// @brief Get the radius set for the sphere
    /// @return sphere radius
    float   getRadius() const { return m_radius; }

    /// @brief Get the center x-, y-, and z-coordinate for the sphere
    /// @return sphere center
    float   *getCenter() const { return m_center.get(); }

    /// @brief Get the color of the sphere
    /// @param out the color of the sphere is stored in out
    void    getColor(float *out);

    /// @brief Get the number of spheres to create. If number of spheres is greater
    ///        than 1, the radius, center, and color of the sphere will be generated
    ///        randomly.
    /// @return the number of spheres to create
    int     getCount() const { return m_count; }

private slots:
    void showColorDialog();
    void radiusEditingFinished();
    void centerXEditingFinished();
    void centerYEditingFinished();
    void centerZEditingFinished();
    void countEditingFinished();

private:
    void initUI();

    QLabel *m_countLabel;
    QLabel *m_radiusLabel;
    QLabel *m_centerLabel;
    QLabel *m_xLabel;
    QLabel *m_yLabel;
    QLabel *m_zLabel;
    QLabel *m_colorLabel;

    QLineEdit *m_countEdit;
    QLineEdit *m_radiusEdit;
    QLineEdit *m_xEdit;
    QLineEdit *m_yEdit;
    QLineEdit *m_zEdit;

    QPushButton *m_colorButton;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;

    int     m_count;
    float   m_radius;
    std::unique_ptr<float[]> m_center;
    std::unique_ptr<QColor> m_color;
};

#endif