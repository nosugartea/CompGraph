#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers/circle.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    //void drawCoordinateSystem(QPainter &painter);
    void drawTangentLine(QPainter &painter, const QPoint &point);
    void drawInfiniteLine(QPainter &painter, const QPoint &p1, const QPoint &p2);
    QPoint moveRadiusChanger(const QPoint &point);

    Circle *shape, *radiusChanger;
    bool isDragging, isChangingRadius;
    QPoint dragStartPosition, changeStartPosition;

    QPoint pointTangent;
    int clickCount;
};
#endif // MAINWINDOW_H
