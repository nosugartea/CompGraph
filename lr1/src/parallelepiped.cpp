#include "parallelepiped.h"

Parallelepiped::Parallelepiped(const std::array<Parallelogram, 2>& parallelograms) : parallelograms(parallelograms) {}

Parallelepiped::~Parallelepiped() {}

void Parallelepiped::draw(QPainter &painter) const {
    for (const auto& parallelogram : parallelograms) {
        parallelogram.draw(painter);
    }
//     painter.drawLine(parallelograms[0].getVertex(0)[0] + 200, 200 - parallelograms[0].getVertex(0)[1],
//                      parallelograms[1].getVertex(0)[0] + 200, 200 - parallelograms[1].getVertex(0)[1]);
//     painter.drawLine(parallelograms[0].getVertex(1)[0] + 200, 200 - parallelograms[0].getVertex(1)[1],
//                      parallelograms[1].getVertex(1)[0] + 200, 200 - parallelograms[1].getVertex(1)[1]);
//     painter.drawLine(parallelograms[0].getVertex(2)[0] + 200, 200 - parallelograms[0].getVertex(2)[1],
//                      parallelograms[1].getVertex(2)[0] + 200, 200 - parallelograms[1].getVertex(2)[1]);
//     painter.drawLine(parallelograms[0].getVertex(3)[0] + 200, 200 - parallelograms[0].getVertex(3)[1],
//                      parallelograms[1].getVertex(3)[0] + 200, 200 - parallelograms[1].getVertex(3)[1]);
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
