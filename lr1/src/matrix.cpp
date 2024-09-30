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

// Углы для проекций
const double beta = 20.705 * M_PI / 180.0;  // перевод в радианы
const double gamma = 22.208 * M_PI / 180.0; // перевод в радианы

// Матрица поворота вокруг оси Y
std::array<std::array<double, 3>, 3> rotationMatrixY = {{
    {cos(gamma), 0.0, sin(gamma)},
    {0.0, 1.0, 0.0},
    {-sin(gamma), 0.0, cos(gamma)}
}};

// Матрица поворота вокруг оси X
std::array<std::array<double, 3>, 3> rotationMatrixX = {{
    {1.0, 0.0, 0.0},
    {0.0, cos(beta), -sin(beta)},
    {0.0, sin(beta), cos(beta)}
}};

// Умножение матрицы на вектор (матрица 3x3 на вектор 3D)
std::array<double, 3> multiplyMatrixVector(const std::array<std::array<float, 3>, 3>& matrix, const std::array<double, 3>& point) {
    std::array<double, 3> result = {0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
    return result;
}

std::array<double, 2> project3Dto2D(const std::array<double, 3>& point) {
    // double perspectiveDistance = 500.0;

    // double thetaY = M_PI / 12;  // Угол поворота камеры вокруг оси Y (15 градусов)
    // double thetaX = M_PI / 24;  // Угол поворота камеры вокруг оси X (7.5 градусов)

    // // Вращение вокруг оси Y
    // double xRotY = point3D[0] * cos(thetaY) - point3D[2] * sin(thetaY);
    // double zRotY = point3D[0] * sin(thetaY) + point3D[2] * cos(thetaY);

    // // Вращение вокруг оси X (для "подъема" оси Z над плоскостью XZ)
    // double yRotX = point3D[1] * cos(thetaX) - zRotY * sin(thetaX);
    // double zRotX = point3D[1] * sin(thetaX) + zRotY * cos(thetaX);

    // // Перспективная проекция
    // double x2D = xRotY / (1 + zRotX / perspectiveDistance);
    // double y2D = yRotX / (1 + zRotX / perspectiveDistance);

    std::array<double, 3> rotatedY = multiplyMatrixVector(rotationMatrixY, point);
    std::array<double, 3> rotatedYX = multiplyMatrixVector(rotationMatrixX, rotatedY);

    return {rotatedYX[0], rotatedYX[1]};
}
