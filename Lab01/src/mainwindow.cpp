#include "headers/mainwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include "cmath"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    shape = new Circle(QPoint(this->width() / 2, this->height() / 2), 100);
    radiusChanger = new Circle(QPoint(shape->x(), shape->y() - shape->r()), 10);
    isDragging = isChangingRadius = false;
    clickCount = 0;
}

MainWindow::~MainWindow() {}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    shape->draw(painter);
    radiusChanger->draw(painter);
    if (clickCount == 1) {
        painter.setPen(Qt::red);
        painter.drawEllipse(pointTangent, 2, 2);
        drawTangentLine(painter, pointTangent);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (radiusChanger->contains(event->pos())) {
            isChangingRadius = true;
            changeStartPosition = event->pos();
        } else if (shape->contains(event->pos())) {
            isDragging = true;
            dragStartPosition = event->pos();
        } else {
            if (clickCount < 1) {
                pointTangent = event->pos();
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
        radiusChanger->moveBy(offset);
        dragStartPosition = event->pos();
        update();
    }
    if (isChangingRadius) {
        QPoint offset = event->pos() - changeStartPosition;
        radiusChanger->moveBy(offset);
        changeStartPosition = event->pos();
        shape->setRadius(QLineF(event->pos(), QPoint(shape->x(), shape->y())).length());
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        isChangingRadius = false;
    }
}

void MainWindow::drawTangentLine(QPainter &painter, const QPoint &point)
{
    QPoint center1 = QPoint(shape->x(), shape->y());
    QPoint center2 = QPoint((shape->x() + point.x())/2, (shape->y() + point.y())/2);

    qreal radius1 = shape->r();
    qreal distance = QLineF(center1, point).length();
    qreal radius2 = distance / 2;

    // Если точка лежит внутри окружности, касательные невозможны
    if (distance <= radius1) {
        return;
    }

    double d = sqrt((center2.x() - center1.x())*(center2.x() - center1.x()) + (center2.y() - center1.y())*(center2.y() - center1.y()));
    double a = (radius1 * radius1 - radius2 * radius2 + d * d)/ (2 * d);
    double h = sqrt(radius1 * radius1 - a * a);
    double Px = center1.x() + a*(center2.x() - center1.x()) / d;
    double Py = center1.y() + a*(center2.y() - center1.y()) / d;

    QPoint topLeft, topRight; // точки касательной к окружности
    topLeft.setX(Px + h * (center2.y() - center1.y()) / d);
    topLeft.setY(Py - h * (center2.x() - center1.x()) / d);
    topRight.setX(Px - h * (center2.y() - center1.y()) / d);
    topRight.setY(Py + h * (center2.x() - center1.x()) / d);

    painter.setPen(Qt::magenta);
    drawInfiniteLine(painter, point, topLeft);
    painter.setPen(Qt::darkMagenta);
    drawInfiniteLine(painter, point, topRight);
}

void MainWindow::drawInfiniteLine(QPainter &painter, const QPoint &p1, const QPoint &p2)
{
    int width = this->width();

    // Вычисляем уравнение прямой y = kx + b
    double k = (p2.y() - p1.y()) / static_cast<double>(p2.x() - p1.x());
    double b = p1.y() - k * p1.x();

    // Вычисляем точки пересечения прямой с границами окна
    QPoint topLeft(-width, k * -width + b);
    QPoint topRight(2 * width, k * (2 * width) + b);

    painter.drawLine(topLeft, topRight);
}
