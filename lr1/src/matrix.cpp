#include "matrix.h"

#include <cmath>

void multiplyMatrix(const std::array<std::array<double, 4>, 4>& A, const std::array<std::array<double, 4>, 4>& B, std::array<std::array<double, 4>, 4>& result) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void identityMatrix(std::array<std::array<double, 4>, 4>& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void multiplyMatrixVector(const std::array<std::array<double, 4>, 4>& matrix, const std::array<double, 4>& vec, std::array<double, 4>& result) {
    for (int i = 0; i < 4; ++i) {
        result[i] = 0;
        for (int j = 0; j < 4; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
}

double toRadians(double degrees) {
    return degrees * PI / 180.0;
}

void translationMatrix(double tx, double ty, double tz, std::array<std::array<double, 4>, 4>& matrix) {
    identityMatrix(matrix);
    matrix[0][3] = tx;
    matrix[1][3] = ty;
    matrix[2][3] = tz;
}

void rotationXMatrix(double theta, std::array<std::array<double, 4>, 4>& matrix) {
    identityMatrix(matrix);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    matrix[1][1] = cosTheta;
    matrix[1][2] = -sinTheta;
    matrix[2][1] = sinTheta;
    matrix[2][2] = cosTheta;
}

void rotationYMatrix(double theta, std::array<std::array<double, 4>, 4>& matrix) {
    identityMatrix(matrix);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    matrix[0][0] = cosTheta;
    matrix[0][2] = sinTheta;
    matrix[2][0] = -sinTheta;
    matrix[2][2] = cosTheta;
}

void rotationZMatrix(double theta, std::array<std::array<double, 4>, 4>& matrix) {
    identityMatrix(matrix);
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    matrix[0][0] = cosTheta;
    matrix[0][1] = -sinTheta;
    matrix[1][0] = sinTheta;
    matrix[1][1] = cosTheta;
}

void rotateAroundAxis(Point origin, Point dot, double delta, std::array<double, 4>& point) {
    // Шаг 1: Перенос начала оси вращения в начало координат
    std::array<std::array<double, 4>, 4> translationMatrixToOrigin;
    translationMatrix(-origin.x, -origin.y, -origin.z, translationMatrixToOrigin);

    // Шаг 2: Нормализация оси вращения
    double axisLength = sqrt(dot.x * dot.x + dot.y * dot.y + dot.z * dot.z);
    dot.x /= axisLength;
    dot.y /= axisLength;
    dot.z /= axisLength;

    // Поворот, чтобы ось совпала с осью Z
    double d = sqrt(dot.y * dot.y + dot.z * dot.z);
    std::array<std::array<double, 4>, 4> rotateAroundX;
    rotationXMatrix(atan2(dot.y, dot.z), rotateAroundX);

    std::array<std::array<double, 4>, 4> rotateAroundY;
    rotationYMatrix(-atan2(dot.x, d), rotateAroundY);

    // Шаг 3: Поворот вокруг оси Z на угол delta
    std::array<std::array<double, 4>, 4> rotateAroundZ;
    rotationZMatrix(toRadians(delta), rotateAroundZ);

    // Шаг 4: Обратные преобразования
    std::array<std::array<double, 4>, 4> inverseRotateAroundY;
    rotationYMatrix(atan2(dot.x, d), inverseRotateAroundY);

    std::array<std::array<double, 4>, 4> inverseRotateAroundX;
    rotationXMatrix(-atan2(dot.y, dot.z), inverseRotateAroundX);

    // Шаг 5: Обратный перенос
    std::array<std::array<double, 4>, 4> translationMatrixBack;
    translationMatrix(origin.x, origin.y, origin.z, translationMatrixBack);

    // Применение всех матриц к точке
    std::array<std::array<double, 4>, 4> tempMatrix;
    std::array<std::array<double, 4>, 4> combinedMatrix;

    multiplyMatrix(rotateAroundX, translationMatrixToOrigin, tempMatrix);
    multiplyMatrix(rotateAroundY, tempMatrix, combinedMatrix);
    multiplyMatrix(rotateAroundZ, combinedMatrix, tempMatrix);
    multiplyMatrix(inverseRotateAroundY, tempMatrix, combinedMatrix);
    multiplyMatrix(inverseRotateAroundX, combinedMatrix, tempMatrix);
    multiplyMatrix(translationMatrixBack, tempMatrix, combinedMatrix);

    // Применение итоговой матрицы к вектору точки
    std::array<double, 4> resultPoint;
    multiplyMatrixVector(combinedMatrix, point, resultPoint);

    // Обновление координат точки
    point = resultPoint;
}
