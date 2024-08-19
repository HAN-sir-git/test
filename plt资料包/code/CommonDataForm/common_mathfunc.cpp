#include "common_mathfunc.h"
#include <cmath>
#include <QVector2D>
#include <numeric>
#include <cmath>



double COMMON_Math::angleTo(const QPointF p1, const QPointF p2)
{
   QVector2D v = QVector2D(p1-p2);
    return correctAngle(std::atan2(v.y(),v.x()));
}

double COMMON_Math::correctAngle(const double &angle)
{
   return std::fmod(M_PI + std::remainder(angle - M_PI, M_PI*2), M_PI*2);
}

double COMMON_Math::correctAngle2(const double &angle)
{
   return std::remainder(angle, M_PI*2);
}

double COMMON_Math::correctAngleU(const double &angle)
{
   return std::abs(std::remainder(angle, M_PI*2));
}

double COMMON_Math::getAngleDifference(double a1, double a2, bool reversed)
{
    if(reversed) std::swap(a1, a2);
    return correctAngle(a2 - a1);
}

bool COMMON_Math::isAngleReadable(double angle)
{
    const double tolerance=0.001;
    if (angle > M_PI_2)
        return std::abs(std::remainder(angle, M_PI*2)) < (M_PI_2 - tolerance);
    else
        return std::abs(std::remainder(angle, M_PI*2)) < (M_PI_2 + tolerance);
}

double COMMON_Math::makeAngleReadable(double angle, bool readable, bool *corrected)
{
    double ret = correctAngle(angle);

    bool cor = isAngleReadable(ret) ^ readable;

    if (cor)
        ret = correctAngle(ret + M_PI);

    if (corrected)
        *corrected = cor;

    return ret;
}

double COMMON_Math::getAngle(const QPointF p)
{
    return COMMON_Math::correctAngle(std::atan2(p.y(),p.x()));
}
