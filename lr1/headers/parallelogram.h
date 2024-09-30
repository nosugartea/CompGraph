#ifndef PARALLELOGRAM_H
#define PARALLELOGRAM_H

#include "shape.h"

class Parallelogram : public Shape
{
public:
    Parallelogram(const std::array<std::array<double, 3>,4> &figure3D);
    ~Parallelogram();

    void draw(QPainter &painter) const override;
    bool contains(const QPoint &point) const override;
    void moveBy(const QPoint &offset) override;
    void rotate3D(Point dot) override;

    const std::array<double, 2> getVertex(size_t point) const;

private:
    std::array<std::array<double, 2>,4> figure;
};

#endif // PARALLELOGRAM_H
