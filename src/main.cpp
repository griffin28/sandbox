#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set global OpenGL format
//    QSurfaceFormat format;
//    //format.setDepthBufferSize(24);
//    //format.setStencilBufferSize(8);
//    format.setVersion(4, 3);
//    format.setProfile(QSurfaceFormat::CoreProfile);
//    format.setSamples(16);
//    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();
    return a.exec();
}
