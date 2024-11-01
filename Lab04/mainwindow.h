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
            Vertex(100, 100), Vertex(200, 100), Vertex(250, 150),
            Vertex(200, 200), Vertex(100, 200), Vertex(50, 150)
        };

        clipWindow = {
            Vertex(150, 50), Vertex(300, 100), Vertex(250, 250), Vertex(100, 200)
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
            painter.drawLine(QPointF(polygon[i].x, polygon[i].y),
                             QPointF(polygon[next].x, polygon[next].y));
        }
    }
};

#endif // MAINWINDOW_H
