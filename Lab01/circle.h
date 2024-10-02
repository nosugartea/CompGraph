#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape
{
public:
    Circle(QPoint center, int radius);
    ~Circle();

    int x() const { return center.x(); }
    int y() const { return center.y(); }
    int r() const { return radius; }

    void draw(QPainter &painter) const override;
    bool contains(const QPoint &point) const override;
    void moveBy(const QPoint &offset) override;

private:
    QPoint center;
    int radius;
};

#endif // CIRCLE_H
