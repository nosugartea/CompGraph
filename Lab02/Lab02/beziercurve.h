#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QVector>
#include <QPointF>

class BezierCurve
{
public:
    BezierCurve();
    QPointF polynomialFunction(QVector<QPointF> controlPoints, double t);

private:
    double binomialCoefficient(int n, int k);
};

#endif // BEZIERCURVE_H
