#include "headers/circle.h"

Circle::Circle(QPoint center, int radius) : center(center), radius(radius) {}

Circle::~Circle() {}

void Circle::draw(QPainter &painter) const
{
    painter.drawEllipse(center, 1, 1);
    painter.drawEllipse(center, radius, radius);
}

bool Circle::contains(const QPoint &point) const
{
    // Расстояние от точки до центра круга
    int dx = point.x() - center.x();
    int dy = point.y() - center.y();
    int distanceSquared = dx * dx + dy * dy;

    // Проверка, находится ли точка внутри круга
    return distanceSquared <= radius * radius;
}

void Circle::moveBy(const QPoint &offset)
{
    center += offset;
}
