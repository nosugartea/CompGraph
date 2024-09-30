#ifndef LINEWIDGET_H
#define LINEWIDGET_H

//#include <QApplication>
#include <QWidget>
#include <QPainter>

class LineWidget : public QWidget {
protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.setPen(Qt::black);

        int x1 = 50;
        int y1 = 50;
        int x2 = 150;
        int y2 = 150;

        painter.drawLine(x1, y1, x2, y2);
    }
};

#endif // LINEWIDGET_H
