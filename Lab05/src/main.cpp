#include <QApplication>
#include "headers/zbufferwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ZBufferWidget window;
    window.setWindowTitle("3D Rotation with Z-buffer");
    window.show();

    return app.exec();
}
