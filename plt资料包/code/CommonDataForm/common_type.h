#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

#define COMMON_MAXDOUBLE 1.0E+10
#define COMMON_MINDOUBLE -1.0E+10

#define COMMON_TOLERANCE 1.0e-10

#define COMMON_TOLERANCE15 1.5e-15
#define COMMON_TOLERANCE2 1.0e-20
#define COMMON_TOLERANCE_ANGLE 1.0e-8

namespace  {
    constexpr double MIN_Scale_Factor = 1.0e-6;
}

namespace COMMON_TYPE {
    enum EntityType :unsigned
    {
        UNKNOWN,
        ENTITY,
        BLOCK,
        POINT,
        LINE,
        CIRCLE,
        ARC,
        ELLIPSE,
        HATCH,
        TEXT,
        SOLID,
        TRACE,
        LWPOLYLINE,
        POLYLINE,
        MTEXT,
        SPLINE,
        PAINTERPATH,
        INSERT,
    };
}

#endif // COMMON_TYPE_H
