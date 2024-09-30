#include "mainwindow.h"
#include "parallelepiped.h"

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

    std::array<double, 3> axisStart = {-50, -50, -50};
    std::array<double, 3> axisEnd = {50, 50, 50};
    drawAxis(axisStart, axisEnd);

    painter.setPen(Qt::black);

    shape->draw(painter);

    for (int i = 0; i < clickCount; ++i) {
        painter.drawEllipse(points[i], 5, 5);
    }
    if (clickCount > 1) {
        drawInfiniteLine(painter, points[clickCount - 2], points[clickCount - 1]);
    }
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

void MainWindow::drawAxis(QPainter, const std::array<double, 3> axisStart, const std::array<double, 3> axisEnd) {
    auto projectedAxisStart = project3Dto2D(axisStart);
    auto projectedAxisEnd = project3Dto2D(axisEnd);

    painter.setPen(Qt::red);
    painter.drawLine(projectedAxisStart[0] + 200, projectedAxisStart[1] + 200, projectedAxisEnd[0] + 200, projectedAxisEnd[1] + 200);
}

std::array<int, 2> MainWindow::project3Dto2D(const std::array<double, 3>& point) {
    int x2D = static_cast<int>(point[0] - point[2] * std::cos(M_PI / 6));
    int y2D = static_cast<int>(point[1] - point[2] * std::sin(M_PI / 6));
    return {x2D, y2D};
}
