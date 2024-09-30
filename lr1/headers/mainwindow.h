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
    void drawAxis(const std::array<double, 3> axisStart, const std::array<double, 3> axisEnd);
    std::array<int, 2> project3Dto2D(const std::array<double, 3>& point);

private:
    Shape *shape;
    bool isDragging;
    QPoint dragStartPosition;
    int clickCount;
    QPoint points[2];
};

#endif // MAINWINDOW_H
