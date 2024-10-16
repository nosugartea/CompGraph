#include "headers/mygraphicview.h"
#include "headers/beziercurve.h"

MyGraphicView::MyGraphicView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setFixedSize(800, 600);

    // Настраиваем виджет
    this->setRenderHint(QPainter::Antialiasing);  // Включаем сглаживание
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);  // Режим обновления

    // Отключаем масштабирование и перемещение
    this->setDragMode(QGraphicsView::NoDrag);
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->setResizeAnchor(QGraphicsView::NoAnchor);
    this->setInteractive(false);

    // Создаем сцену
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    scene->setSceneRect(0, 0, 800, 600);

    currentPoint = nullptr;
}

MyGraphicView::~MyGraphicView()
{
    cleanAll();
    delete scene;
}

void MyGraphicView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Получаем позицию нажатия
        QPointF scenePos = mapToScene(event->pos());

        for (QGraphicsEllipseItem *point : points) {
            if (point->contains(point->mapFromScene(scenePos))) {
                currentPoint = point;
                return;
            }
        }
        QGraphicsEllipseItem *point = scene->addEllipse(scenePos.x() - 4, scenePos.y() - 4, 8, 8, QPen(Qt::NoPen), QBrush(Qt::magenta));
        points.append(point);
        connectPointsWithDashedLines();
    } else if (event->button() == Qt::RightButton) {
        // Удаляем последнюю поставленную точку
        if (!points.isEmpty()) {
            QGraphicsEllipseItem *lastPoint = points.takeLast();
            scene->removeItem(lastPoint);
            delete lastPoint;
        }
        if (!lines.isEmpty()) {
            QGraphicsLineItem *lastLine = lines.takeLast();
            scene->removeItem(lastLine);
            delete lastLine;
        }
    }
}

void MyGraphicView::connectPointsWithDashedLines()
{
    if (points.size() > 1) {
        QGraphicsEllipseItem *newPoint = points[points.size() - 1];
        QGraphicsEllipseItem *prevPoint = points[points.size() - 2];
        QPointF prevCenter = prevPoint->boundingRect().center();
        QPointF newCenter = newPoint->boundingRect().center();

        QGraphicsLineItem *line = scene->addLine(prevCenter.x(), prevCenter.y(), newCenter.x(), newCenter.y());
        QPen pen(Qt::DashLine);
        line->setPen(pen);
        lines.append(line);
    }
}

void MyGraphicView::mouseMoveEvent(QMouseEvent *event)
{
    if (currentPoint) {
        QPointF scenePos = mapToScene(event->pos());
        currentPoint->setRect(scenePos.x() - 3, scenePos.y() - 3, 6, 6);

        int pointIndex = points.indexOf(currentPoint);
        if (pointIndex != -1) {
            updateLines(pointIndex);  // Обновляем только связанные линии
        }
    }
}

void MyGraphicView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && currentPoint) {
        currentPoint = nullptr;
    }
}

void MyGraphicView::updateLines(int pointIndex)
{
    if (pointIndex > 0 && pointIndex < points.size()) {
        // Обновляем линию, которая соединяет текущую точку с предыдущей
        QPointF prevCenter = points[pointIndex - 1]->boundingRect().center();
        QPointF currentCenter = points[pointIndex]->boundingRect().center();
        lines[pointIndex - 1]->setLine(prevCenter.x(), prevCenter.y(), currentCenter.x(), currentCenter.y());
    }

    if (pointIndex < points.size() - 1) {
        // Обновляем линию, которая соединяет текущую точку с последующей
        QPointF currentCenter = points[pointIndex]->boundingRect().center();
        QPointF nextCenter = points[pointIndex + 1]->boundingRect().center();
        lines[pointIndex]->setLine(currentCenter.x(), currentCenter.y(), nextCenter.x(), nextCenter.y());
    }
}

void MyGraphicView::makeBezierCurve()
{
    if (!bezierPoints.isEmpty()) {
        bezierPoints.clear();

        if (!bezierCurve.isEmpty()){
            for (auto* it : bezierCurve) {
                delete it;
            }
            bezierCurve.clear();
        }
    }
    QVector<QPointF> part;
    for (qsizetype i = 0; i < points.size(); ++i) {
        QGraphicsEllipseItem *newPoint = points[i];
        part.push_back(newPoint->boundingRect().center());

        if (part.size() == 3) {
            if (i == points.size() - 1) {
                drawBezierCurve(part);
                part.clear();
            } else {
                int minX = std::min(points[i]->boundingRect().center().x(), points[i + 1]->boundingRect().center().x()),
                    minY = std::min(points[i]->boundingRect().center().y(), points[i + 1]->boundingRect().center().y());

                QPointF additionalPoint = QPointF(minX + (abs(points[i]->boundingRect().center().x() - points[i + 1]->boundingRect().center().x())) / 2,
                                                  minY + (abs(points[i]->boundingRect().center().y() - points[i + 1]->boundingRect().center().y())) / 2);

                part.push_back(additionalPoint);
                drawBezierCurve(part);
                part.clear();
                part.push_back(additionalPoint);
            }
        }
    }
}

void MyGraphicView::drawBezierCurve(QVector<QPointF> part)
{
    BezierCurve bc;
    for (double t = 0.0; t <=1.0; t += 0.05) {
        bezierPoints.push_back(bc.polynomialFunction(part, t));
    }

    for (int i = 0; i < bezierPoints.size() - 1; ++i){
        QGraphicsLineItem *line = scene->addLine(bezierPoints[i].x(), bezierPoints[i].y(), bezierPoints[i + 1].x(), bezierPoints[i + 1].y());
        QPen pen(Qt::red);
        line->setPen(pen);
        bezierCurve.append(line);
    }
}

void MyGraphicView::cleanAll()
{
    for (auto* it : points) {
        delete it;
    }
    points.clear();

    for (auto* it : lines) {
        delete it;
    }
    lines.clear();

    for (auto* it : bezierCurve) {
        delete it;
    }
    bezierCurve.clear();

    delete currentPoint;
    bezierPoints.clear();
}
