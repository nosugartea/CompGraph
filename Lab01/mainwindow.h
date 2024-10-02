#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "circle.h"

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
    void drawTangentLine(QPainter &painter, const QPoint &point);
    void drawInfiniteLine(QPainter &painter, const QPoint &p1, const QPoint &p2);

    Circle *shape;
    bool isDragging;
    QPoint dragStartPosition;

    QPoint pointTangent;
    int clickCount;
};
#endif // MAINWINDOW_H
