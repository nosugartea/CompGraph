#ifndef MATHFILE_H
#define MATHFILE_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QPointF>
#include <QColor>

// Структура для представления точек в 2D
struct Point2D {
    double x, y;
};

// Структура для представления точки в 3D
struct Point3D {
    int x, y;
    double z;
    Point3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// Структура для представления многоугольника
struct Polygon {
    std::vector<Point3D> vertices;
    QColor color;
};

double binomialCoefficient(int a, int b);               // Функция для вычисления биномиального коэффициента C^b_a

QMatrix4x4 rotateX(float angle);                        // Вращение вокруг оси X
QMatrix4x4 rotateY(float angle);                        // Вращение вокруг оси Y
QMatrix4x4 applyRotation(float angleX, float angleY);   // Применение вращений для осей X и Y
Point3D projectPoint(const Point3D &point, const QMatrix4x4 &transform,
                     const unsigned width, const unsigned height); // Перспективное проецирование 3D-точки на 2D-экран

bool isLeft(const Point2D& a, const Point2D& b, const Point2D& p);
bool isPointInTriangle(const Point2D& p, const Point2D& a, const Point2D& b, const Point2D& c);
std::vector<std::vector<Point2D>> earClipTriangulate(std::vector<Point2D>& polygon);


#endif // MATHFILE_H
