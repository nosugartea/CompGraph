#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPoint>
#include <QPainter>

class Circle
{
public:
    Circle(QPoint center, int radius);
    ~Circle();

    int x() const { return center.x(); }
    int y() const { return center.y(); }
    int r() const { return radius; }
    void setRadius(int r) { radius = r; }

    void draw(QPainter &painter) const;
    bool contains(const QPoint &point) const;
    void moveBy(const QPoint &offset);

private:
    QPoint center;
    int radius;
};

#endif // CIRCLE_H
