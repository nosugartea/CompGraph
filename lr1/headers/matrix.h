#ifndef MATRIX_H
#define MATRIX_H

#include <array>

const double PI = 3.14159265358979323846;

struct Point
{
    double x = 0;
    double y = 0;
    double z = 0;
};

// Функция для умножения матриц 4x4
void multiplyMatrix(const std::array<std::array<double, 4>, 4>& A, const std::array<std::array<double, 4>, 4>& B, std::array<std::array<double, 4>, 4>& result);
// Функция для создания единичной матрицы
void identityMatrix(std::array<std::array<double, 4>, 4>& matrix);
// Функция для умножения матрицы 4x4 на вектор 4x1
void multiplyMatrixVector(const std::array<std::array<double, 4>, 4>& matrix, const std::array<double, 4>& vec, std::array<double, 4>& result);
// Функция для перевода углов в радианы
double toRadians(double degrees);
// Функция для создания матрицы переноса
void translationMatrix(double tx, double ty, double tz, std::array<std::array<double, 4>, 4>& matrix);
// Функция для создания матрицы поворота вокруг оси X
void rotationXMatrix(double theta, std::array<std::array<double, 4>, 4>& matrix);
// Функция для создания матрицы поворота вокруг оси Y
void rotationYMatrix(double theta, std::array<std::array<double, 4>, 4>& matrix);
// Функция для создания матрицы поворота вокруг оси Z
void rotationZMatrix(double theta, std::array<std::array<double, 4>, 4>& matrix);
// Основная функция для поворота вокруг произвольной оси
void rotateAroundAxis(Point origin, Point dot, double delta, std::array<double, 4>& point);

#endif // MATRIX_H
