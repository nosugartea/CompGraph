#include "parallelogram.h"

Parallelogram::Parallelogram(const std::array<std::array<double, 3>, 4> &figure3D)
{
    for (size_t i = 0; i < figure3D.size(); ++i) {
        figure[i] = project3Dto2D(figure3D[i]);
    }
}

Parallelogram::~Parallelogram(){};

void Parallelogram::draw(QPainter &painter) const {
    painter.drawLine(figure[0][0] + 200, figure[0][1] + 200, figure[1][0] + 200, 200 + figure[1][1]);
    painter.drawLine(figure[1][0] + 200, figure[1][1] + 200, figure[2][0] + 200, 200 + figure[2][1]);
    painter.drawLine(figure[2][0] + 200, figure[2][1] + 200, figure[3][0] + 200, 200 + figure[3][1]);
    painter.drawLine(figure[3][0] + 200, figure[3][1] + 200, figure[0][0] + 200, 200 + figure[1][1]);
}

const std::array<double, 2> Parallelogram::getVertex(size_t point) const {
    return figure[point];
}

bool Parallelogram::contains(const QPoint &point) const {
    // QPolygon parallelogram;
    // parallelogram << p1 << p2 << p3 << p4;
    // return parallelogram.containsPoint(point, Qt::OddEvenFill);
}

void Parallelogram::moveBy(const QPoint &offset) {
    // p1 += offset;
    // p2 += offset;
    // p3 += offset;
    // p4 += offset;
}

void Parallelogram::rotate3D(Point dot) {

}
