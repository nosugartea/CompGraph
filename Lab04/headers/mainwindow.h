#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QPair>
#include "mathfile.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Задаем исходный и отсеченный многоугольники
        subjectPolygon = {
            Vertex(50, 50), Vertex(200, 50), Vertex(125, 200)
        };

        clipWindow = {
            Vertex(150, 100), Vertex(300, 75), Vertex(200, 200)
        };
        clippedPolygon = weilerAthertonClip(subjectPolygon, clipWindow);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Рисуем исходный многоугольник
        painter.setPen(Qt::blue);
        drawPolygon(painter, subjectPolygon);

        // Рисуем отсекающее окно
        painter.setPen(Qt::red);
        drawPolygon(painter, clipWindow);

        // Рисуем отсеченный многоугольник
        painter.setPen(Qt::green);
        drawPolygon(painter, clippedPolygon);
    }

private:
    Polygon subjectPolygon;
    Polygon clipWindow;
    Polygon clippedPolygon;

    void drawPolygon(QPainter &painter, const Polygon &polygon) {
        for (int i = 0; i < polygon.size(); ++i) {
            int next = (i + 1) % polygon.size();
            qDebug() << polygon[i].x << polygon[i].y;
            painter.drawLine(QPointF(polygon[i].x, polygon[i].y),
                             QPointF(polygon[next].x, polygon[next].y));
        }
    }
};

#endif // MAINWINDOW_H
