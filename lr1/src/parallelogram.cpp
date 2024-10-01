#include "headers/parallelogram.h"

Parallelogram::Parallelogram(const std::array<std::array<double, 3>, 4> &figure3D)
{
    for (size_t i = 0; i < figure3D.size(); ++i) {
        figure[i] = project3Dto2D(figure3D[i]);
    }
}

Parallelogram::~Parallelogram(){}

void Parallelogram::draw(QPainter &painter) const {
    for (int i = 0; i < figure.size(); ++i)
    {
        painter.drawLine(figure[i % 4][0] + 200, figure[i % 4][1] + 200,
                         figure[(i + 1) % 4][0] + 200, 200 + figure[(i + 1) % 4][1]);
        painter.drawEllipse(QPoint(figure[i][0] + 200, figure[i][1] + 200), 5, 5);
    }
}

const std::array<double, 2> Parallelogram::getVertex(size_t point) const {
    return figure[point];
}

bool Parallelogram::contains(const QPoint &point) const {
    // QPolygon parallelogram;
    // parallelogram << p1 << p2 << p3 << p4;
    // return parallelogram.containsPoint(point, Qt::OddEvenFill);
    return 0;
}

void Parallelogram::moveBy(const QPoint &offset) {
    // p1 += offset;
    // p2 += offset;
    // p3 += offset;
    // p4 += offset;
}

void Parallelogram::rotate3D(Point dot) {

}
