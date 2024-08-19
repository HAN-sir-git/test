#ifndef DATA_H
#define DATA_H

#include "point.h"
#include "line.h"
#include "polyline.h"

#include <QVector>
#include <QMap>
#include <QDebug>

using EntityPtr = std::shared_ptr<Entity>;
using PointPtr = std::shared_ptr<Point>;
using LinePtr = std::shared_ptr<Line>;
using PolylinePtr = std::shared_ptr<PolyLine>;

using EntityPtrList = QList<EntityPtr>;
using PointPtrList = QList<PointPtr>;
using LinePtrList =QList<LinePtr>;
using PolyLinePtrList = QList<PolylinePtr>;



struct ConvertData
{
    PointPtrList point_list;
    LinePtrList line_list;
    PolyLinePtrList polyline_list;
};

#endif // DATA_H
