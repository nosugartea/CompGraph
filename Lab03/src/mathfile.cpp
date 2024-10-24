#include "headers/mathfile.h"

#include <cmath>

unsigned int M_PI = 3.1415926535;

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
    float rad = angle * M_PI / 180.0f; // Конвертация в радианы
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
    float rad = angle * M_PI / 180.0f; // Конвертация в радианы
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

// Проецирование 3D-точки на 2D-экран
QPointF projectPoint(const QVector3D &point, const QMatrix4x4 &transform, const unsigned width, const unsigned height)
{
    QVector3D transformedPoint = transform.map(point);
    // Используем простую перспективную проекцию
    float perspective = 3.0f / (3.0f + transformedPoint.z());
    return QPointF(width / 2 + transformedPoint.x() * 100 * perspective, height / 2 - transformedPoint.y() * 100 * perspective);
}


float computeCurvature(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3)
{
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p1;
    float angle = QVector3D::dotProduct(v1.normalized(), v2.normalized());
    return acos(angle);  // Кривизна, выраженная через угол между векторами
}

QColor colorForCurvature(float curvature, float maxCurvature)
{
    float ratio = curvature / maxCurvature;
    int colorValue = static_cast<int>(255 * ratio);
    return QColor(colorValue, 0, 255 - colorValue);
}
