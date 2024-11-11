#ifndef CLIPWIDGET_H
#define CLIPWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QPushButton>
#include <QVector>

#include "headers/clipwindow.h"

class ClipWidget : public QWidget
{
public:
    ClipWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPushButton *buttonGenerate;    // Для создания нового набора случацных отрезков
    QPushButton *buttonClearClip;   // Для очистки существующего окна отсечения

    void setButtons();              // Создание и оформление кнопок
    void generateLines();           // Инициализация случайных отрезков
    void clearClip();

    QVector<QPointF> tempVertices;  // Точки нового окна отсечения
    QVector<QLineF> clipLines;      // Отрезки соединяющие точки нового окна отсечения
    QVector<QLineF> lines;          // Набор случайных отрезков
    ClipWindow* clipWindow;         // Выпуклое окно для отсечения
};

#endif // CLIPWIDGET_H
