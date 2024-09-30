#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "shape.h"

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void drawInfiniteLine(QPainter &painter, const QPoint &p1, const QPoint &p2);
    void drawCoordinateSystem(QPainter &painter);
    void drawAxis(QPainter &painter, const std::array<double, 3> &axisStart, const std::array<double, 3> &axisEnd);

private:
    Shape *shape;
    bool isDragging;
    QPoint dragStartPosition;
    int clickCount;
    QPoint points[2];
};

#endif // MAINWINDOW_H
