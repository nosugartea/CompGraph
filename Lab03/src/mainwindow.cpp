#include "headers/mainwindow.h"
#include "headers/mathfile.h"

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegularExpression>

BezierSurfaceWidget::BezierSurfaceWidget(QWidget *parent) : QWidget(parent), angleX(0), angleY(0)
{
    setWindowTitle("Lab03");
    setFixedSize(940, 600);
    QPushButton *button = new QPushButton("Выбрать набор точек", this);
    button->setGeometry(20, 20, 210, 70);
    connect(button, &QPushButton::clicked, this, &BezierSurfaceWidget::openFile);

    button->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "                                   stop:0 rgba(255, 0, 0, 0.7), "
        "                                   stop:1 rgba(0, 0, 255, 0.7));"
        "   color: white;"
        "   border-radius: 12px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "                                   stop:0 rgba(255, 0, 0, 0.5), "
        "                                   stop:1 rgba(0, 0, 255, 0.5));"
        "}"
    );
}

void BezierSurfaceWidget::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,
           "Выберете файл с вершинами",
           QDir::homePath(),
           "Bezie Files (*.bezier);;All Files (*)"); // Фильтр файлов

    if (fileName.isEmpty()) {
        QMessageBox::warning(nullptr, "Предупреждение", "Файл не выбран");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Предупреждение", "Не удалось открыть файл: " + fileName);
        return;
    }

    controlPoints.clear();

    QTextStream in(&file);
    std::vector<QVector3D> currentRow;
    QRegularExpression regex(R"(\(\s*(-?\d*\.?\d+)\s*,\s*(-?\d*\.?\d+)\s*,\s*(-?\d*\.?\d+)\s*\))");

    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatchIterator it = regex.globalMatch(line);

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                float x = match.captured(1).toFloat();
                float y = match.captured(2).toFloat();
                float z = match.captured(3).toFloat();
                currentRow.push_back(QVector3D(x, y, z));
            }
        }

        // Если собрали строку точек, добавляем её в итоговый вектор
        if (!currentRow.empty()) {
            controlPoints.push_back(currentRow);
            currentRow.clear();
        }
    }
    file.close();
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
    if (!controlPoints.empty())
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

// Функция вычисления поверхности Безье для данной точки s, t
QVector3D BezierSurfaceWidget::bezierSurface(float s, float t) {
    int m = controlPoints.size() - 1; // количество контрольных точек по s
    int n = controlPoints[0].size() - 1; // количество контрольных точек по t
    QVector3D point(0, 0, 0); // точка на поверхности

    // Проход по контрольным точкам и расчет взвешенной суммы
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            // Вычисляем биномиальные коэффициенты
            int Ci_m = binomialCoefficient(m, i);
            int Cj_n = binomialCoefficient(n, j);

            // Вычисляем веса по формуле: s^i (1-s)^(m-i) и t^j (1-t)^(n-j)
            float weightS = Ci_m * pow(s, i) * pow(1 - s, m - i);
            float weightT = Cj_n * pow(t, j) * pow(1 - t, n - j);

            // Взвешиваем текущую контрольную точку и добавляем к итоговой точке
            point += controlPoints[i][j] * weightS * weightT;
        }
    }
    return point;
}

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
            float s1 = static_cast<float>(i) / steps;
            float t1 = static_cast<float>(j) / steps;
            float s2 = static_cast<float>(i + 1) / steps;
            float t2 = static_cast<float>(j + 1) / steps;

            // Получаем точки поверхности для отрисовки
            QPointF p1 = projectPoint(bezierSurface(s1, t1), transform, width(), height());
            QPointF p2 = projectPoint(bezierSurface(s2, t1), transform, width(), height());
            QPointF p3 = projectPoint(bezierSurface(s2, t2), transform, width(), height());
            QPointF p4 = projectPoint(bezierSurface(s1, t2), transform, width(), height());

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

// void TInterface::openGraph()
// {
//     QString fileName = "";
//     fileName = QFileDialog::getOpenFileName(this, ("Open File"), "../Graphs", ("Graph (*.grh)"));
//     filePath->setText(fileName);

//     if (fileName == "")
//     {
//         errorName->setText("Ошибка: Вы не выбрали файл с графом.");
//         return;
//     }

//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//     {
//         errorName->setText("Ошибка: Файл не доступен к чтению.");
//         return;
// //     }
// QTextStream in(&file);

// int code = graph->setArc(in);
// switch (code)
// {
// case 1:
//     errorName->setText("Ошибка: Неизвестный символ вместо количества вершин графа.");
//     return;
// case 2:
//     errorName->setText("Ошибка: Отрицательное значение количества вершин графа.");
//     return;
// case 3:
//     errorName->setText("Ошибка: Неизвестный символ в матрице событий.");
//     return;
// case 4:
//     errorName->setText("Ошибка: Отрицательное значение или 0 в матрице событий.");
//     return;
// case 5:
//     errorName->setText("Ошибка: Указанное количество вершин отличается от фактического.");
//     return;
// case 0:
// default:
//     break;
// }
// file.close();

// canvas.show();
// }
