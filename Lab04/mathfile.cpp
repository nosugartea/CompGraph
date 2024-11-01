#include "mathfile.h"

bool getIntersection(const Edge &edge1, const Edge &edge2, Vertex &intersection)
{
    // Решение системы уравнений для нахождения пересечения edge1 и edge2
    double dx1 = edge1.end.x - edge1.start.x;
    double dy1 = edge1.end.y - edge1.start.y;
    double dx2 = edge2.end.x - edge2.start.x;
    double dy2 = edge2.end.y - edge2.start.y;

    double denominator = dx1 * dy2 - dy1 * dx2;
    if (denominator == 0) return false; // Отрезки параллельны

    double s = ((edge1.start.x - edge2.start.x) * dy2 - (edge1.start.y - edge2.start.y) * dx2) / denominator;
    double t = ((edge1.start.x - edge2.start.x) * dy1 - (edge1.start.y - edge2.start.y) * dx1) / denominator;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        intersection = Vertex(edge1.start.x + s * dx1, edge1.start.y + s * dy1);
        return true;
    }
    return false;
}

// Проверка, находится ли точка внутри многоугольника
bool isInside(const Vertex &point, const Polygon &clipWindow) {
    int intersections = 0;
    for (int i = 0; i < clipWindow.size(); ++i) {
        Vertex start = clipWindow[i];
        Vertex end = clipWindow[(i + 1) % clipWindow.size()];
        if (((start.y > point.y) != (end.y > point.y)) &&
            (point.x < (end.x - start.x) * (point.y - start.y) / (end.y - start.y) + start.x)) {
            intersections++;
        }
    }
    return (intersections % 2 != 0);
}

// Находим точку пересечения отрезков
bool getIntersection(const Vertex &p1, const Vertex &p2, const Vertex &q1, const Vertex &q2, Vertex &intersection) {
    double dx1 = p2.x - p1.x, dy1 = p2.y - p1.y;
    double dx2 = q2.x - q1.x, dy2 = q2.y - q1.y;

    double denominator = dx1 * dy2 - dy1 * dx2;
    if (denominator == 0) return false; // Отрезки параллельны

    double s = ((p1.x - q1.x) * dy2 - (p1.y - q1.y) * dx2) / denominator;
    double t = ((p1.x - q1.x) * dy1 - (p1.y - q1.y) * dx1) / denominator;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        intersection = Vertex(p1.x + s * dx1, p1.y + s * dy1);
        intersection.isIntersection = true;
        return true;
    }
    return false;
}

// Реализация алгоритма Уэйлера-Азертона
Polygon weilerAthertonClip(const Polygon &subject, const Polygon &clipWindow) {
    Polygon result;

    // Копируем исходный многоугольник для добавления пересечений
    Polygon clippedSubject = subject;
    for (Vertex &vertex : clippedSubject) {
        vertex.isInside = isInside(vertex, clipWindow);
    }

    // Находим точки пересечения и добавляем их в многоугольник
    QVector<QPair<Vertex, Vertex>> intersections;
    for (int i = 0; i < clippedSubject.size(); ++i) {
        Vertex p1 = clippedSubject[i];
        Vertex p2 = clippedSubject[(i + 1) % clippedSubject.size()];

        for (int j = 0; j < clipWindow.size(); ++j) {
            Vertex q1 = clipWindow[j];
            Vertex q2 = clipWindow[(j + 1) % clipWindow.size()];

            Vertex intersection;
            if (getIntersection(p1, p2, q1, q2, intersection)) {
                intersections.append(qMakePair(p1, intersection));
                intersections.append(qMakePair(q1, intersection));
                clippedSubject.insert(i + 1, intersection);
                i++; // Смещаем индекс, так как добавили новую точку
            }
        }
    }

    // Обход пересечений, чтобы сформировать отсеченный многоугольник
    for (int i = 0; i < clippedSubject.size(); ++i) {
        Vertex start = clippedSubject[i];

        if (start.isInside && !start.isProcessed) {
            Polygon part;
            part.append(start);

            while (true) {
                start.isProcessed = true;
                Vertex next;

                // Следующая точка: либо вершина, либо точка пересечения
                if (start.isIntersection) {
                    int clipIndex = clipWindow.indexOf(start);
                    next = clipWindow[(clipIndex + 1) % clipWindow.size()];
                } else {
                    int subjIndex = clippedSubject.indexOf(start);
                    next = clippedSubject[(subjIndex + 1) % clippedSubject.size()];
                }

                if (next.isProcessed) break; // Завершаем обход

                part.append(next);
                start = next;
            }
            result += part;
        }
    }
    return result;
}

