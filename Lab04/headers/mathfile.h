#ifndef MATHFILE_H
#define MATHFILE_H

#include <QVector>
#include <QPointF>

double polygonArea(const QVector<QPointF>& vertices);   // Нахождение ориентированной площади
void counterClockwiseOrder(QVector<QPointF>& vertices); // Изменение порядка вершин в случае обхода по часовой стрелки
double crossProduct(const QPointF& base, const QPointF& p1, const QPointF& p2); // Функция для вычисления векторного произведения двух векторов
bool isConvex(const QVector<QPointF>& polygon);         // Функция для проверки выпуклости многоугольника


#endif // MATHFILE_H
