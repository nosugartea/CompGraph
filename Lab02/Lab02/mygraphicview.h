#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>

class MyGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicView(QWidget *parent = 0);
    ~MyGraphicView();

    void makeBezierCurve();
    void cleanAll();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QGraphicsScene *scene;     // Объявляем сцену для отрисовки
    QList<QGraphicsEllipseItem*> points; // Список для хранения точек
    QList<QGraphicsLineItem*> lines; // Список для хранения линий
    QList<QGraphicsLineItem*> bezierCurve;
    QList<QPointF> bezierPoints;
    QGraphicsEllipseItem *currentPoint; // Текущая перемещаемая точка

    void connectPointsWithDashedLines();
    void updateLines(int pointIndex);
    void drawBezierCurve(QVector<QPointF> curve);
};

#endif // MYGRAPHICVIEW_H
