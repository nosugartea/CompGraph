#ifndef ZBUFFERWIDGET_H
#define ZBUFFERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>

#include "headers/mathfile.h"

class ZBufferWidget : public QWidget {
public:
    ZBufferWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void renderPolygon(const Polygon& poly, QPainter& painter);
    std::vector<std::vector<Point3D>> triangulatePolygon(const Polygon& poly);
    void renderTriangle(const std::vector<Point3D>& triangle, const QColor& color, QPainter& painter);
    void calculatePlaneCoefficients(const std::vector<Point3D>& triangle, double& a, double& b, double& c, double& d);
    bool pointInTriangle(int px, int py, const std::vector<Point3D>& triangle);

    int width;      // ширина экрана
    int height;     // высота экрана
    double z_min;   // начальные значения буфера

    std::vector<Polygon> polygons;              // список граней
    std::vector<std::vector<double>> zBuffer;   // Z-буфер

    float angleX = 0, angleY = 0;               // углы для вращения вокруг осей X и Y
    QPoint prevMousePos;                        // координаты нажатия ЛКМ
};

#endif // ZBUFFERWIDGET_H
