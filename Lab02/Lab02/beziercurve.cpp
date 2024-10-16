#include "beziercurve.h"
#include "QDebug"

BezierCurve::BezierCurve() {};

double BezierCurve::binomialCoefficient(int n, int k)
{
    if (k > n - k)
        k = n - k;
    int c = 1;
    for (int i = 0; i < k; ++i) {
        c = c * (n - i) / (i + 1);
    }
    return c;
}

 QPointF BezierCurve::polynomialFunction(QVector<QPointF> controlPoints, double t)
{
    QPointF rezult;
    int degree = controlPoints.size() - 1;
    for (int i = 0; i <= degree; ++i) {
        double b = binomialCoefficient(degree, i) * std::pow(t, i) * std::pow(1 - t, degree - i);
        rezult.rx() += b * controlPoints[i].x();
        rezult.ry() += b * controlPoints[i].y();
    }

    return rezult;
}
