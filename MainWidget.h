#ifndef INCLUDED_MAINWIDGET_H
#define INCLUDED_MAINWIDGET_H

#include <QWidget>

class Canvas;

class MainWidget : public QWidget
{
    friend class MainWindow;
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

/*
private slots:
    void nearValueChanged(int);
*/

private:
    Canvas *d_canvas;

    void initialize();
    void setupLayout();
};

#endif // INCLUDED_MAINWIDGET_H
