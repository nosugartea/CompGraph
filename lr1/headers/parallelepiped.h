#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include "parallelogram.h"

#include <array>

class Parallelepiped : public Shape
{
public:
    Parallelepiped(const std::array<Parallelogram, 2>& parallelograms);
    ~Parallelepiped();

    void draw(QPainter &painter) const override;
    bool contains(const QPoint &point) const override;
    void moveBy(const QPoint &offset) override;
    void rotate3D(Point dot) override;

private:
    std::array<Parallelogram, 2> parallelograms;
};

#endif // PARALLELEPIPED_H
