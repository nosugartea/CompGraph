#include "headers/mathfile.h"

#include <cmath>

// Функция для вычисления биномиального коэффициента C^b_a
double binomialCoefficient(int a, int b)
{
    if (b > a - b)
        b = a - b;
    int c = 1;
    for (int i = 0; i < b; ++i) {
        c = c * (a - i) / (i + 1);
    }
    return c;
}

// Вращение вокруг оси X
QMatrix4x4 rotateX(float angle)
{
    double pi = 3.1415926535;
    float rad = angle * pi / 180.0f; // Конвертация в радианы
    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity(); // Устанавливка матрицы в единичную матрицу

    rotationMatrix(1, 1) = std::cos(rad);
    rotationMatrix(1, 2) = -std::sin(rad);
    rotationMatrix(2, 1) = std::sin(rad);
    rotationMatrix(2, 2) = std::cos(rad);

    return rotationMatrix;
}

// Вращение вокруг оси Y
QMatrix4x4 rotateY(float angle)
{
    double pi = 3.1415926535;
    float rad = angle * pi / 180.0f; // Конвертация в радианы
    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity(); // Устанавливка матрицы в единичную матрицу

    rotationMatrix(0, 0) = std::cos(rad);
    rotationMatrix(0, 2) = std::sin(rad);
    rotationMatrix(2, 0) = -std::sin(rad);
    rotationMatrix(2, 2) = std::cos(rad);

    return rotationMatrix;
}

// Применение вращений для осей X и Y
QMatrix4x4 applyRotation(float angleX, float angleY)
{
    QMatrix4x4 rotationMatrixX = rotateX(angleX); // Поворот вокруг X
    QMatrix4x4 rotationMatrixY = rotateY(angleY); // Поворот вокруг Y

    return rotationMatrixY * rotationMatrixX; // Сначала X, потом Y
}

Point3D projectPoint(const Point3D &point, const QMatrix4x4 &transform, const unsigned width, const unsigned height)
{
    QVector3D point_(point.x, point.y, point.z);
    QVector3D transformedPoint = transform.map(point_);
    // Используем простую перспективную проекцию
    float perspective = 3.0f / (3.0f + transformedPoint.z());
    Point3D rez(width / 2 + transformedPoint.x() * 100 * perspective, height / 2 - transformedPoint.y() * 100 * perspective, 0);
    return rez;
}

// Функция для проверки, является ли точка p вектором, лежащим слева от вектора (a -> b)
bool isLeft(const Point2D& a, const Point2D& b, const Point2D& p) {
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x) > 0;
}

// Функция для проверки, лежит ли точка p внутри треугольника (a, b, c)
bool isPointInTriangle(const Point2D& p, const Point2D& a, const Point2D& b, const Point2D& c) {
    bool b1 = isLeft(a, b, p);
    bool b2 = isLeft(b, c, p);
    bool b3 = isLeft(c, a, p);
    return (b1 == b2) && (b2 == b3);
}

// Алгоритм триангуляции с использованием обрезки уха (ear-clipping)
std::vector<std::vector<Point2D>> earClipTriangulate(std::vector<Point2D>& polygon) {
    std::vector<std::vector<Point2D>> triangles;

    while (polygon.size() > 3) {
        bool earFound = false;
        for (size_t i = 0; i < polygon.size(); ++i) {
            size_t prev = (i == 0) ? polygon.size() - 1 : i - 1;
            size_t next = (i == polygon.size() - 1) ? 0 : i + 1;

            // Проверяем, является ли треугольник (prev, i, next) ухо
            if (isLeft(polygon[prev], polygon[next], polygon[i])) {
                // Проверяем, нет ли точек внутри треугольника
                bool isEar = true;
                for (size_t j = 0; j < polygon.size(); ++j) {
                    if (j != prev && j != i && j != next) {
                        if (isPointInTriangle(polygon[j], polygon[prev], polygon[i], polygon[next])) {
                            isEar = false;
                            break;
                        }
                    }
                }

                // Если это ухо, разбиение на треугольник
                if (isEar) {
                    triangles.push_back({polygon[prev], polygon[i], polygon[next]});
                    polygon.erase(polygon.begin() + i); // Удаляем точку i
                    earFound = true;
                    break;
                }
            }
        }

        // Если не нашли ухо, прерываем (некорректный многоугольник или не выпуклый)
        if (!earFound) {
            break;
        }
    }

    // Добавляем последний треугольник, который остался
    if (polygon.size() == 3) {
        triangles.push_back({polygon[0], polygon[1], polygon[2]});
    }

    return triangles;
}
