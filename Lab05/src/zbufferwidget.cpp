#include "headers/zbufferwidget.h"

#include <limits>
#include <algorithm>

ZBufferWidget::ZBufferWidget(QWidget *parent) : QWidget(parent), width(800), height(600)
{
    setFixedSize(width, height);

    z_min = std::numeric_limits<double>::lowest();
    zBuffer = std::vector<std::vector<double>>(width, std::vector<double>(height, z_min));

    // Параллелепипед (6 граней)
    polygons.push_back({{{300, 250, -20}, {400, 250, -20}, {400, 350, -20}, {300, 350, -20}}, Qt::red});
    polygons.push_back({{{300, 250, 20}, {400, 250, 20}, {400, 350, 20}, {300, 350, 20}}, Qt::red});
    polygons.push_back({{{300, 250, -20}, {300, 250, 20}, {300, 350, 20}, {300, 350, -20}}, Qt::red});
    polygons.push_back({{{400, 250, -20}, {400, 250, 20}, {400, 350, 20}, {400, 350, -20}}, Qt::red});
    polygons.push_back({{{300, 250, -20}, {400, 250, -20}, {400, 250, 20}, {300, 250, 20}}, Qt::red});
    polygons.push_back({{{300, 350, -20}, {400, 350, -20}, {400, 350, 20}, {300, 350, 20}}, Qt::red});

    // Пирамида (основание + 4 треугольные грани)
    polygons.push_back({{{350, 300, -40}, {450, 300, -40}, {400, 400, -40}}, Qt::green});
    polygons.push_back({{{350, 300, -40}, {450, 300, -40}, {400, 300, 40}}, Qt::green});
    polygons.push_back({{{450, 300, -40}, {400, 400, -40}, {400, 300, 40}}, Qt::green});
    polygons.push_back({{{400, 400, -40}, {350, 300, -40}, {400, 300, 40}}, Qt::green});
    polygons.push_back({{{350, 300, -40}, {400, 400, -40}, {400, 300, 40}}, Qt::green});

    // Параллелепипед
    polygons.push_back({{{250, 200, -60}, {350, 200, -60}, {330, 280, -60}, {230, 280, -60}}, Qt::blue});
    polygons.push_back({{{250, 200, 60}, {350, 200, 60}, {330, 280, 60}, {230, 280, 60}}, Qt::blue});
    polygons.push_back({{{250, 200, -60}, {250, 200, 60}, {230, 280, 60}, {230, 280, -60}}, Qt::blue});
    polygons.push_back({{{350, 200, -60}, {350, 200, 60}, {330, 280, 60}, {330, 280, -60}}, Qt::blue});
    polygons.push_back({{{250, 200, -60}, {350, 200, -60}, {350, 200, 60}, {250, 200, 60}}, Qt::blue});
    polygons.push_back({{{230, 280, -60}, {330, 280, -60}, {330, 280, 60}, {230, 280, 60}}, Qt::blue});

    // Пирамида (основание + 4 треугольные грани)
    polygons.push_back({{{300, 400, -30}, {350, 450, -30}, {300, 500, -30}, {250, 450, -30}}, Qt::yellow});
    polygons.push_back({{{300, 400, -30}, {350, 450, -30}, {300, 450, 30}}, Qt::yellow});
    polygons.push_back({{{350, 450, -30}, {300, 500, -30}, {300, 450, 30}}, Qt::yellow});
    polygons.push_back({{{300, 500, -30}, {250, 450, -30}, {300, 450, 30}}, Qt::yellow});
    polygons.push_back({{{250, 450, -30}, {300, 400, -30}, {300, 450, 30}}, Qt::yellow});

    // Пирамида (основание + 4 треугольные грани)
    polygons.push_back({{{450, 250, -50}, {550, 300, -50}, {500, 350, -50}}, Qt::magenta});
    polygons.push_back({{{450, 250, -50}, {550, 300, -50}, {500, 300, 50}}, Qt::magenta});
    polygons.push_back({{{550, 300, -50}, {500, 350, -50}, {500, 300, 50}}, Qt::magenta});
    polygons.push_back({{{500, 350, -50}, {450, 250, -50}, {500, 300, 50}}, Qt::magenta});
}

void ZBufferWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Сброс Z-буфера перед отрисовкой
    std::fill(zBuffer.begin(), zBuffer.end(), std::vector<double>(height, z_min));
    painter.fillRect(0, 0, width, height, Qt::black);

    for (const auto& poly : polygons) {
        renderPolygon(poly, painter);
    }
}

void ZBufferWidget::renderPolygon(const Polygon& poly, QPainter& painter)
{
    if (poly.vertices.size() < 3) return;

    QMatrix4x4 rotationMatrix = applyRotation(angleX, angleY);

    std::vector<Point3D> rotatedVertices;
    for (const auto& vertex : poly.vertices) {
        // Поворачиваем координаты вершины
        QVector4D rotated = rotationMatrix * QVector4D(vertex.x, vertex.y, vertex.z, 1);
        rotatedVertices.push_back({rotated.x(), rotated.y(), rotated.z()});
    }

    Polygon rotatedPolygon = poly;
    rotatedPolygon.vertices = rotatedVertices;

    // Разбиваем на треугольники для обработки
    auto triangles = triangulatePolygon(rotatedPolygon);

    for (const auto& triangle : triangles) {
        renderTriangle(triangle, rotatedPolygon.color, painter);
    }
}

std::vector<std::vector<Point3D>> ZBufferWidget::triangulatePolygon(const Polygon& poly)
{
    std::vector<std::vector<Point3D>> triangles;
    for (size_t i = 1; i < poly.vertices.size() - 1; i++) {
        triangles.push_back({poly.vertices[0], poly.vertices[i], poly.vertices[i + 1]});
    }
    return triangles;
}

void ZBufferWidget::renderTriangle(const std::vector<Point3D>& triangle, const QColor& color, QPainter& painter)
{
    double a, b, c, d;
    calculatePlaneCoefficients(triangle, a, b, c, d);

    // Определяем границы треугольника
    int minX = width, maxX = 0, minY = height, maxY = 0;
    for (const auto& vertex : triangle) {
        minX = std::max(0, std::min(minX, static_cast<int>(vertex.x)));
        maxX = std::min(width - 1, std::max(maxX, static_cast<int>(vertex.x)));
        minY = std::max(0, std::min(minY, static_cast<int>(vertex.y)));
        maxY = std::min(height - 1, std::max(maxY, static_cast<int>(vertex.y)));
    }

    // Проверяем каждый пиксель в границах треугольника
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            if (pointInTriangle(x, y, triangle)) {
                // Вычисляем Z-координату для текущего пикселя
                double z = -(a * x + b * y + d) / c;

                // Обновляем пиксель, если он ближе к камере
                if (z > zBuffer[x][y]) {
                    zBuffer[x][y] = z;
                    painter.setPen(QPen(color));
                    painter.drawPoint(x, y);
                }
            }
        }
    }
}

void ZBufferWidget::calculatePlaneCoefficients(const std::vector<Point3D>& triangle, double& a, double& b, double& c, double& d)
{
    // Коэффициенты плоскости через векторное произведение
    Point3D p1 = triangle[0];
    Point3D p2 = triangle[1];
    Point3D p3 = triangle[2];
    a = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
    b = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
    c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    d = -(a * p1.x + b * p1.y + c * p1.z);
}

bool ZBufferWidget::pointInTriangle(int px, int py, const std::vector<Point3D>& triangle)
{
    auto sign = [](const Point3D& p1, const Point3D& p2, const Point3D& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    bool b1 = sign({static_cast<double>(px), static_cast<double>(py), 0}, triangle[0], triangle[1]) < 0.0f;
    bool b2 = sign({static_cast<double>(px), static_cast<double>(py), 0}, triangle[1], triangle[2]) < 0.0f;
    bool b3 = sign({static_cast<double>(px), static_cast<double>(py), 0}, triangle[2], triangle[0]) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

void ZBufferWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        angleX += event->pos().y() - prevMousePos.y();
        angleY += event->pos().x() - prevMousePos.x();
        prevMousePos = event->pos();
        update();
    }
}

void ZBufferWidget::mousePressEvent(QMouseEvent *event)
{
    prevMousePos = event->pos();
}
