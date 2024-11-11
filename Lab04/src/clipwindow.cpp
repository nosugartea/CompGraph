#include "headers/clipwindow.h"

ClipWindow::ClipWindow(const QVector<QPointF>& vertices) : vertices(vertices) {};

bool ClipWindow::clipLine(QPointF& p0, QPointF& p1) const
{
    if (vertices.isEmpty())
        return false;

    double tE = 0.0; // начальное значение параметра t
    double tL = 1.0; // конечное значение параметра t
    QPointF d = p1 - p0;

    for (int i = 0; i < vertices.size(); ++i) {
        QPointF edge = vertices[(i + 1) % vertices.size()] - vertices[i];
        QPointF normal(-edge.y(), edge.x());
        QPointF w = p0 - vertices[i];
        double DdotN = d.x() * normal.x() + d.y() * normal.y();
        double WdotN = w.x() * normal.x() + w.y() * normal.y();

        if (DdotN != 0) {
            double t = -WdotN / DdotN;
            if (DdotN > 0) {
                tE = std::max(tE, t);
            } else {
                tL = std::min(tL, t);
            }
            if (tE > tL) return false;
        } else if (WdotN < 0) {
            return false;
        }
    }

    p1 = p0 + d * tL;
    p0 = p0 + d * tE;
    return true;
}
