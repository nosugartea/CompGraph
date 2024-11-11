#include "headers/clipwidget.h"

#include "ctime"
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>

#include "headers/mathfile.h"

ClipWidget::ClipWidget(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Lab04");
    setFixedSize(800, 600);

    setButtons();    // Создание и оформление кнопок
    generateLines(); // Инициализация случайных отрезков

    // Задание вершин начального окна отсечения
    QVector<QPointF> vertices = {
        QPointF(150, 150), QPointF(300, 100),
        QPointF(350, 200), QPointF(250, 300),
        QPointF(100, 250)
    };
    clipWindow = new ClipWindow(vertices);
}

void ClipWidget::setButtons()
{
    buttonGenerate = new QPushButton("Generate Lines", this);
    buttonGenerate->setGeometry(10, 15, 90, 40);
    connect(buttonGenerate, &QPushButton::clicked, this, &ClipWidget::generateLines);

    buttonGenerate->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "                                   stop:0 rgba(253,61,181, 0.7), "
        "                                   stop:1 rgba(0, 139, 139, 0.7));"
        "   color: white;"
        "   border-radius: 45px;"
        "   min-width: 90px;"
        "   min-height: 90px;"
        "   max-width: 90px;"
        "   max-height: 90px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "                                   stop:0 rgba(253,61,181, 0.5), "
        "                                   stop:1 rgba(0, 139, 139, 0.5));"
        "}"
        );

    buttonClearClip = new QPushButton("Clear Clip", this);
    buttonClearClip->setGeometry(10, 120, 90, 40);
    connect(buttonClearClip, &QPushButton::clicked, this, &ClipWidget::clearClip);

    buttonClearClip->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "                                   stop:0 rgba(253,61,181, 0.7), "
        "                                   stop:1 rgba(0, 139, 139, 0.7));"
        "   color: white;"
        "   border-radius: 45px;"
        "   min-width: 90px;"
        "   min-height: 90px;"
        "   max-width: 90px;"
        "   max-height: 90px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "                                   stop:0 rgba(253,61,181, 0.5), "
        "                                   stop:1 rgba(0, 139, 139, 0.5));"
        "}"
        );
}

void ClipWidget::generateLines()
{
    if (!lines.isEmpty())
        lines.clear();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < 10; ++i) {
        QPointF p1(rand() % 800, rand() % 600);
        QPointF p2(rand() % 800, rand() % 600);
        lines.append(QLineF(p1, p2));
    }
}

void ClipWidget::clearClip()
{
    clipWindow->vertices.clear();
    clipLines.clear();
    tempVertices.clear();
}


void ClipWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF clickPos = event->pos();

        for (const QPointF &point : tempVertices) {
            // Если расстояние между точками меньше определенного радиуса, считаем это нажатие на существующую точку
            if (QLineF(clickPos, point).length() < 5) {
                int connectionCount = 0;

                // Проверка количества соединений точки с другими точками
                for (const QLineF &line : clipLines) {
                    if (line.p1() == point || line.p2() == point) {
                        connectionCount++;
                    }
                }

                // Если соединена с одной точкой, замыкаем многоугольник
                if (connectionCount == 1) {
                    counterClockwiseOrder(tempVertices);
                    if (!isConvex(tempVertices)) {
                        QMessageBox::warning(nullptr, "Ошибка", "Многоугольник не является выпуклым!");
                        tempVertices.clear();
                        clipLines.clear();
                        return;
                    }
                    clipWindow->vertices = tempVertices;
                    tempVertices.clear();
                    update();
                    return;
                }

                if (connectionCount == 2) {
                    return;
                }
            }
        }

        tempVertices.append(clickPos);

        if (tempVertices.size() > 1) {
            QLineF newLine(tempVertices[tempVertices.size() - 2], tempVertices.last());
            clipLines.append(newLine);
        }
    }
}

void ClipWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (clipWindow->vertices.isEmpty()) {
        // Отображение процесса построения окна отсечения
        painter.setPen(QPen(Qt::blue));

        for (const QPointF &point : tempVertices) {
            painter.drawEllipse(point, 3, 3);
        }

        painter.setPen(QPen(Qt::magenta, 1, Qt::DashLine));
        for (const QLineF &line : clipLines) {
            painter.drawLine(line);
        }
    } else {
        tempVertices.clear();
        clipLines.clear();
    }

    // Отображение окна отсечения
    QPolygonF polygon;
    for (const auto& vertex : clipWindow->vertices) {
        polygon << vertex;
    }
    painter.setPen(QPen(Qt::magenta, 2));
    painter.drawPolygon(polygon);

    // Отображение случайных отрезков до отсечения
    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    for (const auto& line : lines) {
        painter.drawLine(line);
    }

    // Отображение отсеченных отрезков
    painter.setPen(QPen(Qt::darkCyan, 2));
    for (auto line : lines) {
        QPointF p0 = line.p1();
        QPointF p1 = line.p2();
        if (clipWindow->clipLine(p0, p1)) {
            painter.drawLine(QLineF(p0, p1));
        }
    }
    update();
}
