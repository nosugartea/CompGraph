#ifndef MATHFILE_H
#define MATHFILE_H

#include <QVector>

struct Vertex {
    double x, y;
    bool isInside = false; // Флаг, находится ли вершина внутри окна
    bool isIntersection = false; // Флаг, является ли точка точкой пересечения
    bool isProcessed = false; // Флаг, была ли точка уже обработана
    Vertex() = default;
    Vertex(double _x, double _y) : x(_x), y(_y) {};
    // Оператор равенства для сравнения двух вершин
    bool operator==(const Vertex& other) const {
        return x == other.x && y == other.y &&
               isInside == other.isInside &&
               isIntersection == other.isIntersection &&
               isProcessed == other.isProcessed;
    }
};

using Polygon = QVector<Vertex>;

struct Edge {
    Vertex start, end;
    Edge(Vertex _start, Vertex _end) : start(_start), end(_end) {}
};

bool getIntersection(const Edge &edge1, const Edge &edge2, Vertex &intersection);
bool isInside(const Vertex &point, const Polygon &clipWindow);
bool getIntersection(const Vertex &p1, const Vertex &p2, const Vertex &q1, const Vertex &q2, Vertex &intersection);
Polygon weilerAthertonClip(const Polygon &subject, const Polygon &clipWindow);

#endif // MATHFILE_H
