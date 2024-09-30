#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape {
public:
    Triangle(const QPoint &p1, const QPoint &p2, const QPoint &p3);
    ~Triangle();

    void draw(QPainter &painter) const override;
    bool contains(const QPoint &point) const override;
    void moveBy(const QPoint &offset) override;

private:
    QPoint p1, p2, p3;
};

#endif // TRIANGLE_H
