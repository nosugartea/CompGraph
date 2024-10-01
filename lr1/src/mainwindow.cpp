#include "headers/mainwindow.h"
#include "headers/parallelepiped.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    //shape = new Parallelogram(QPoint(50, 50), QPoint(150, 50), QPoint(170, 150), QPoint(70, 150));
    std::array<Parallelogram, 2> parallelograms = {
        Parallelogram({{
            {-50.0, -50.0, -50.0},
            {50.0, -50.0, -50.0},
            {50.0, 50.0, -50.0},
            {-50.0, 50.0, -50.0}
        }}),
        Parallelogram({{
            {-50.0, -50.0, 50.0},
            {50.0, -50.0, 50.0},
            {50.0, 50.0, 50.0},
            {-50.0, 50.0, 50.0}
        }})
    };
    shape = new Parallelepiped(parallelograms);
    isDragging = false;
    clickCount = 0;
}

MainWindow::~MainWindow() {
    delete shape;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    drawCoordinateSystem(painter);
    // std::array<double, 3> axisStart = {-50, -50, -50};
    // std::array<double, 3> axisEnd = {50, 50, 50};
    // drawAxis(painter, axisStart, axisEnd);

    painter.setPen(Qt::black);
    shape->draw(painter);

    // for (int i = 0; i < clickCount; ++i) {
    //     painter.drawEllipse(points[i], 5, 5);
    // }
    // if (clickCount > 1) {
    //     drawInfiniteLine(painter, points[clickCount - 2], points[clickCount - 1]);
    // }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (shape->contains(event->pos())) {
            isDragging = true;
            dragStartPosition = event->pos();
        } else {
            if (clickCount < 2) {
                points[clickCount] = event->pos();
                clickCount++;
                update();
            }
        }
    } else if (event->button() == Qt::RightButton) {
        if (clickCount > 0) {
            clickCount--;
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        QPoint offset = event->pos() - dragStartPosition;
        shape->moveBy(offset);
        dragStartPosition = event->pos();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

void MainWindow::drawInfiniteLine(QPainter &painter, const QPoint &p1, const QPoint &p2) {
    int width = this->width();

    // Вычисляем уравнение прямой y = kx + b
    double k = (p2.y() - p1.y()) / static_cast<double>(p2.x() - p1.x());
    double b = p1.y() - k * p1.x();

    // Вычисляем точки пересечения прямой с границами окна
    QPoint topLeft(-width, k * -width + b);
    QPoint topRight(2 * width, k * (2 * width) + b);

    painter.drawLine(topLeft, topRight);
}

void MainWindow::drawCoordinateSystem(QPainter &painter) {
    std::array<double, 3> origin = {0, 0, 0}; // Начало координат
    std::array<double, 3> xAxisEnd = {100, 0, 0};  // Ось X
    std::array<double, 3> yAxisEnd = {0, 100, 0};  // Ось Y
    std::array<double, 3> zAxisEnd = {0, 0, 100};  // Ось Z

    // Проецируем точки на 2D
    auto projectedOrigin = project3Dto2D(origin);
    auto projectedXEnd = project3Dto2D(xAxisEnd);
    auto projectedYEnd = project3Dto2D(yAxisEnd);
    auto projectedZEnd = project3Dto2D(zAxisEnd);

    int centerX = width() / 2;
    int centerY = height() / 2;

    // Рисуем оси (X - красный, Y - зеленый, Z - синий)
    painter.setPen(Qt::red);
    painter.drawLine(centerX + projectedOrigin[0], centerY - projectedOrigin[1],
                     centerX + projectedXEnd[0], centerY - projectedXEnd[1]);

    painter.setPen(Qt::green);
    painter.drawLine(centerX + projectedOrigin[0], centerY - projectedOrigin[1],
                     centerX + projectedYEnd[0], centerY - projectedYEnd[1]);

    painter.setPen(Qt::blue);
    painter.drawLine(centerX + projectedOrigin[0], centerY - projectedOrigin[1],
                     centerX + projectedZEnd[0], centerY - projectedZEnd[1]);
}

void MainWindow::drawAxis(QPainter &painter, const std::array<double, 3> &axisStart, const std::array<double, 3> &axisEnd) {
    auto projectedAxisStart = project3Dto2D(axisStart);
    auto projectedAxisEnd = project3Dto2D(axisEnd);

    painter.setPen(Qt::red);
    painter.drawLine(projectedAxisStart[0] + 200, projectedAxisStart[1] + 200, projectedAxisEnd[0] + 200, projectedAxisEnd[1] + 200);
}
