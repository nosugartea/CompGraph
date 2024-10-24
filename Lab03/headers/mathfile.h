#ifndef MATHFILE_H
#define MATHFILE_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QPointF>
#include <QColor>

double binomialCoefficient(int a, int b);               // Функция для вычисления биномиального коэффициента C^b_a

QMatrix4x4 rotateX(float angle);                        // Вращение вокруг оси X
QMatrix4x4 rotateY(float angle);                        // Вращение вокруг оси Y
QMatrix4x4 applyRotation(float angleX, float angleY);   // Применение вращений для осей X и Y

QPointF projectPoint(const QVector3D &point, const QMatrix4x4 &transform,
                     const unsigned width, const unsigned height); // Перспективное проецирование 3D-точки на 2D-экран

float computeCurvature(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3);
QColor colorForCurvature(float curvature, float maxCurvature);

#endif // MATHFILE_H
