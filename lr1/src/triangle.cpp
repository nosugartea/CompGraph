#include "headers/triangle.h"

Triangle::Triangle(const QPoint &p1, const QPoint &p2, const QPoint &p3)
    : p1(p1), p2(p2), p3(p3) {}

Triangle::~Triangle() {}

void Triangle::draw(QPainter &painter) const {
    QPolygon triangle;
    triangle << p1 << p2 << p3;
    painter.drawPolygon(triangle);
}

bool Triangle::contains(const QPoint &point) const {
    QPolygon triangle;
    triangle << p1 << p2 << p3;
    return triangle.containsPoint(point, Qt::OddEvenFill);
}

void Triangle::moveBy(const QPoint &offset) {
    p1 += offset;
    p2 += offset;
    p3 += offset;
}
