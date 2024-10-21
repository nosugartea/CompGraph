#include "headers/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BezierSurfaceWidget widget;
    widget.resize(800, 600);
    widget.show();

    return app.exec();
}
