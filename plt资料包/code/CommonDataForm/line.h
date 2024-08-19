#ifndef LINE_H
#define LINE_H

#include "entity.h"
#include "point.h"
#include <QLineF>

struct LineData
{
    LineData() = default;
    LineData(const Point& start, const Point& end) : start(start), end(end) {}
    LineData(const QLineF& line) : start(line.p1()), end(line.p2()) {}

    // 成员变量
    Point start;
    Point end;
    double lineWidth{0.0};
};

class Line : public Entity
{
public:
    Line() = default;
    Line(const LineData& data) : data(data) {}
    Line(const Point& start, const Point& end) : data(start, end) {}

    COMMON_TYPE::EntityType rtti() const { return COMMON_TYPE::LINE; }

    /**********************************SET*******************************************/
    void setStart(const Point& start) { data.start = start; }
    void setEnd(const Point& end) { data.end = end; }

public:
    /***********************************GET***************************************/
    const Point getStart() const { return data.start; }
    const Point getEnd() const { return data.end; }
    const QPointF getStartPointF() const { return data.start.getPointF(); }
    const QPointF getEndPointF() const { return data.end.getPointF(); }
    const QLineF getLineF() const { return QLineF(data.start.getPointF(), data.end.getPointF()); }
    const QRectF getBox()const {return  QRectF(data.start.getPointF(), data.end.getPointF());}

protected:
    LineData data;
};

#endif // LINE_H
