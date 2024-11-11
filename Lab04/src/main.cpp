#include <QApplication>
#include "headers/clipwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ClipWidget widget;
    widget.show();
    return app.exec();
}
