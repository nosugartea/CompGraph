#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMatrix4x4>
#include <vector>

struct Point3D {
    float x, y, z;
    Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

class BezierSurfaceWidget : public QWidget {
    Q_OBJECT

public:
    BezierSurfaceWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    // Обработка вращения по осям X и Y с помощью мыши
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void openFile();

private:
    std::vector<std::vector<QVector3D>> controlPoints; // Контрольные точки для поверхности Безье
    float angleX, angleY;                              // Углы поворота
    QPoint prevMousePos;                               // Предыдущая позиция мыши

    //QPointF projectPoint(const QVector3D &point, const QMatrix4x4 &transform);  // Проецирование 3D точки на 2D с учетом матричных преобразований
    QVector3D bezierSurface(float u, float v);                                  // Построение поверхности Безье
    void drawBezierSurface(QPainter &painter, const QMatrix4x4 &transform);     // Рисуем поверхность Безье
};

#endif // MAINWINDOW_H
