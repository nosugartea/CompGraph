#ifndef CLIPWINDOW_H
#define CLIPWINDOW_H

#include <QVector>
#include <QPointF>

class ClipWindow
{
public:
    QVector<QPointF> vertices; // Вершины выпуклого многоугольника окна

    ClipWindow(const QVector<QPointF>& vertices);
    bool clipLine(QPointF& p0, QPointF& p1) const; // Алгоритм Кируса-Бека
};

#endif // CLIPWINDOW_H
