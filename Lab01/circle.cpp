#include "circle.h"

Circle::Circle(QPoint center, int radius) : center(center), radius(radius) {}

Circle::~Circle() {}

void Circle::draw(QPainter &painter) const
{
    QPen pen;
    pen.setWidth(4);
    painter.setPen(pen);
    painter.setPen(Qt::darkCyan);
    painter.drawEllipse(center, 2, 2);
    painter.drawEllipse(center, radius, radius);
}

bool Circle::contains(const QPoint &point) const
{
    // Небольшой отступ от окружности, чтобы точка не лежала на ней
    int diff = 5;
    // Расстояние от точки до центра круга
    int dx = point.x() - center.x() + diff;
    int dy = point.y() - center.y() + diff;
    int distanceSquared = dx * dx + dy * dy;

    // Проверка, находится ли точка внутри круга
    return distanceSquared <= radius * radius;
}

void Circle::moveBy(const QPoint &offset)
{
    center += offset;
}
