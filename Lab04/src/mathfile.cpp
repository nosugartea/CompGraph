#include "headers/mathfile.h"

// Нахождение ориентированной площади
double polygonArea(const QVector<QPointF>& vertices)
{
    double area = 0.0;
    int n = vertices.size();
    for (int i = 0; i < n; ++i) {
        QPointF p1 = vertices[i];
        QPointF p2 = vertices[(i + 1) % n];
        area += (p1.x() * p2.y() - p2.x() * p1.y());
    }
    return area / 2.0;
}

// Изменение порядка вершин в случае обхода по часовой стрелки
void counterClockwiseOrder(QVector<QPointF>& vertices)
{
    if (polygonArea(vertices) < 0) {
        std::reverse(vertices.begin(), vertices.end());
    }
}

double crossProduct(const QPointF& base, const QPointF& p1, const QPointF& p2)
{
    QPointF v1 = p1 - base;
    QPointF v2 = p2 - base;
    return v1.x() * v2.y() - v1.y() * v2.x();
}

// Функция для проверки выпуклости многоугольника
bool isConvex(const QVector<QPointF>& polygon)
{
    if (polygon.size() < 3) {
        return false;
    }

    QPointF base = polygon[0];

    // Вычисляем векторные произведения для всех пар векторов
    bool isPositive = false;
    bool isNegative = false;

    for (int i = 1; i < polygon.size(); ++i) {
        int next = (i + 1) % polygon.size();
        double cp = crossProduct(base, polygon[i], polygon[next]);

        if (cp > 0) {
            isPositive = true;
        } else if (cp < 0) {
            isNegative = true;
        }

        // Если уже встретились и положительные, и отрицательные значения,
        // то многоугольник не выпуклый
        if (isPositive && isNegative) {
            return false;
        }
    }
    return true;
}
