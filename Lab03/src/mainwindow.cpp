#include "headers/mainwindow.h"

#include <cmath>

float computeCurvature(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3) {
    QVector3D v1 = p2 - p1;
    QVector3D v2 = p3 - p1;
    float angle = QVector3D::dotProduct(v1.normalized(), v2.normalized());
    return acos(angle);  // Кривизна, выраженная через угол между векторами
}

QColor colorForCurvature(float curvature, float maxCurvature) {
    float ratio = curvature / maxCurvature;
    int colorValue = static_cast<int>(255 * ratio);
    return QColor(colorValue, 0, 255 - colorValue);
}

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
QMatrix4x4 rotateX(float angle) {
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
QMatrix4x4 rotateY(float angle) {
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
QMatrix4x4 applyRotation(float angleX, float angleY) {
    QMatrix4x4 rotationMatrixX = rotateX(angleX); // Поворот вокруг X
    QMatrix4x4 rotationMatrixY = rotateY(angleY); // Поворот вокруг Y

    return rotationMatrixY * rotationMatrixX; // Сначала X, потом Y
}

BezierSurfaceWidget::BezierSurfaceWidget(QWidget *parent) : QWidget(parent), angleX(0), angleY(0)
{
    controlPoints = {
        { QVector3D(-1, -1, 0), QVector3D(0, -1, 2), QVector3D(1, -1, 0) },
        { QVector3D(-1, 0, 3), QVector3D(0, 0, 5), QVector3D(1, 0, 3) },
        { QVector3D(-1, 1, 0), QVector3D(0, 1, 2), QVector3D(1, 1, 0) }
    };

    // Простой плоский прямоугольник
    // controlPoints = {
    //     { QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(2, 0, 0) },
    //     { QVector3D(0, 1, 0), QVector3D(1, 1, 0), QVector3D(2, 1, 0) },
    //     { QVector3D(0, 2, 0), QVector3D(1, 2, 0), QVector3D(2, 2, 0) }
    // };

    // Гиперболический купол
    // controlPoints = {
    //     { QVector3D(0, 0, 0), QVector3D(1, 0, 0.5), QVector3D(2, 0, 0) },
    //     { QVector3D(0, 1, 0.5), QVector3D(1, 1, 1), QVector3D(2, 1, 0.5) },
    //     { QVector3D(0, 2, 0), QVector3D(1, 2, 0.5), QVector3D(2, 2, 0) }
    // };

    // Седловидная поверхность
    // controlPoints = {
    //     { QVector3D(0, 0, 1), QVector3D(1, 0, 0), QVector3D(2, 0, 1) },
    //     { QVector3D(0, 1, 0), QVector3D(1, 1, -1), QVector3D(2, 1, 0) },
    //     { QVector3D(0, 2, 1), QVector3D(1, 2, 0), QVector3D(2, 2, 1) }
    // };

    // Волнообразная поверхность
    // controlPoints = {
    //     { QVector3D(0, 0, 0), QVector3D(1, 0, 1), QVector3D(2, 0, 0), QVector3D(3, 0, 1) },
    //     { QVector3D(0, 1, 1), QVector3D(1, 1, 0), QVector3D(2, 1, 1), QVector3D(3, 1, 0) },
    //     { QVector3D(0, 2, 0), QVector3D(1, 2, 1), QVector3D(2, 2, 0), QVector3D(3, 2, 1) },
    //     { QVector3D(0, 3, 1), QVector3D(1, 3, 0), QVector3D(2, 3, 1), QVector3D(3, 3, 0) }
    // };
}

void BezierSurfaceWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white);

    // Получаем текущую матрицу для отображения
    QMatrix4x4 transformMatrix;
    transformMatrix = applyRotation(angleX, angleY);

    // Рисуем поверхность Безье
    drawBezierSurface(painter, transformMatrix);
}

// Обработка вращения по осям
void BezierSurfaceWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        angleX += event->pos().y() - prevMousePos.y();
        angleY += event->pos().x() - prevMousePos.x();
        prevMousePos = event->pos();
        update();
    }
}

void BezierSurfaceWidget::mousePressEvent(QMouseEvent *event)
{
    prevMousePos = event->pos();
}


// Функция вычисления поверхности Безье для данной точки u, v
QVector3D BezierSurfaceWidget::bezierSurface(float u, float v) {
    int m = controlPoints.size() - 1; // количество контрольных точек по u
    int n = controlPoints[0].size() - 1; // количество контрольных точек по v
    QVector3D point(0, 0, 0); // точка на поверхности

    // Проход по контрольным точкам и расчет взвешенной суммы
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            // Вычисляем биномиальные коэффициенты
            int Ci_m = binomialCoefficient(m, i);
            int Cj_n = binomialCoefficient(n, j);

            // Вычисляем веса по формуле: s^i (1-s)^(m-i) и t^j (1-t)^(n-j)
            float weightU = Ci_m * pow(u, i) * pow(1 - u, m - i);
            float weightV = Cj_n * pow(v, j) * pow(1 - v, n - j);

            // Взвешиваем текущую контрольную точку и добавляем к итоговой точке
            point += controlPoints[i][j] * weightU * weightV;
        }
    }

    return point;
}

// Проецирование 3D-точки на 2D-экран
QPointF BezierSurfaceWidget::projectPoint(const QVector3D &point, const QMatrix4x4 &transform) {
    QVector3D transformedPoint = transform.map(point);
    // Используем простую перспективную проекцию
    float perspective = 3.0f / (3.0f + transformedPoint.z());
    return QPointF(width() / 2 + transformedPoint.x() * 100 * perspective, height() / 2 - transformedPoint.y() * 100 * perspective);
}

// // Функция для рисования поверхности Безье
// void BezierSurfaceWidget::drawBezierSurface(QPainter &painter, const QMatrix4x4 &transform) {
//     const int steps = 20; // Количество шагов для дискретизации

//     for (int i = 0; i < steps; ++i) {
//         for (int j = 0; j < steps; ++j) {
//             // Параметры u и v для углов ячеек
//             float u1 = static_cast<float>(i) / steps;
//             float v1 = static_cast<float>(j) / steps;
//             float u2 = static_cast<float>(i + 1) / steps;
//             float v2 = static_cast<float>(j + 1) / steps;

//             // Вычисляем точки на поверхности Безье
//             QPointF p1 = projectPoint(bezierSurface(u1, v1), transform);
//             QPointF p2 = projectPoint(bezierSurface(u2, v1), transform);
//             QPointF p3 = projectPoint(bezierSurface(u2, v2), transform);
//             QPointF p4 = projectPoint(bezierSurface(u1, v2), transform);

//             // Рисуем сетку поверхности
//             painter.drawLine(p1, p2);
//             painter.drawLine(p2, p3);
//             painter.drawLine(p3, p4);
//             painter.drawLine(p4, p1);
//         }
//     }
// }

void BezierSurfaceWidget::drawBezierSurface(QPainter &painter, const QMatrix4x4 &transform) {
    const int steps = 20;
    float maxCurvature = 0.0f;
    QVector<float> curvatures;

    // Первый проход для расчета кривизны и определения максимальной кривизны
    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < steps; ++j) {
            float u1 = static_cast<float>(i) / steps;
            float v1 = static_cast<float>(j) / steps;
            float u2 = static_cast<float>(i + 1) / steps;
            float v2 = static_cast<float>(j + 1) / steps;

            QVector3D curvature1 = bezierSurface(u1, v1);
            QVector3D curvature2 = bezierSurface(u2, v1);
            QVector3D curvature3 = bezierSurface(u2, v2);

            // Вычисляем кривизну на основании трёх точек
            float curvature = computeCurvature(curvature1, curvature2, curvature3);
            curvatures.append(curvature);

            if (curvature > maxCurvature) {
                maxCurvature = curvature;
            }
        }
    }

    // Второй проход для отрисовки поверхности с раскраской
    int index = 0;
    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < steps; ++j) {
            float u1 = static_cast<float>(i) / steps;
            float v1 = static_cast<float>(j) / steps;
            float u2 = static_cast<float>(i + 1) / steps;
            float v2 = static_cast<float>(j + 1) / steps;

            // Получаем точки поверхности для отрисовки
            QPointF p1 = projectPoint(bezierSurface(u1, v1), transform);
            QPointF p2 = projectPoint(bezierSurface(u2, v1), transform);
            QPointF p3 = projectPoint(bezierSurface(u2, v2), transform);
            QPointF p4 = projectPoint(bezierSurface(u1, v2), transform);

            // Получаем кривизну для текущей ячейки
            float curvature = curvatures[index++];
            QColor color = colorForCurvature(curvature, maxCurvature);

            // Устанавливаем цвет для текущей ячейки
            QPen pen(color);
            painter.setPen(pen);

            // Рисуем ячейку сетки
            painter.drawLine(p1, p2);
            painter.drawLine(p2, p3);
            painter.drawLine(p3, p4);
            painter.drawLine(p4, p1);
        }
    }
}
