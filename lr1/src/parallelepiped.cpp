#include "headers/parallelepiped.h"

Parallelepiped::Parallelepiped(const std::array<Parallelogram, 2>& parallelograms) : parallelograms(parallelograms) {}

Parallelepiped::~Parallelepiped() {}

void Parallelepiped::draw(QPainter &painter) const {
    for (const auto& parallelogram : parallelograms) {
        parallelogram.draw(painter);
    }
    for (int i = 0; i < 4; ++i)
    {
        painter.drawLine(parallelograms[0].getVertex(i)[0] + 200, 200 + parallelograms[0].getVertex(i)[1],
                         parallelograms[1].getVertex(i)[0] + 200, 200 + parallelograms[1].getVertex(i)[1]);
    }
}

bool Parallelepiped::contains(const QPoint &point) const {
    for (const auto& parallelogram : parallelograms) {
        if (parallelogram.contains(point)) {
            return true;
        }
    }
    return false;
}

void Parallelepiped::moveBy(const QPoint &offset) {
    for (auto& parallelogram : parallelograms) {
        parallelogram.moveBy(offset);
    }
}

void Parallelepiped::rotate3D(Point dot) {

}
