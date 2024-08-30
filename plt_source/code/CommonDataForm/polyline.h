#ifndef POLYLINE_H
#define POLYLINE_H

#include "line.h"
#include <QList>


struct PolylineData
{
    /* data */
    PolylineData() = default;

    PolylineData(const QList<Point>& points, const bool closed = false);


    QList<Point> points;
    bool closed = false;
    double width;
};

class PolyLine: public Entity
{
public:
    PolyLine() = default;
    PolyLine(const PolylineData& data) : data(data) {}
    PolyLine(const QList<Point>& points, const bool closed = false);
    ~PolyLine(){}

    virtual COMMON_TYPE::EntityType rtti() const { return COMMON_TYPE::POLYLINE; }

public:
    /**********************SET*************************/
    void setClosed(bool closed) { data.closed = closed; }
    void setWidth(double width) { data.width  = width;  }

public:
    const QList<Point>& getPoints() const { return data.points; }
    const QVector<QPointF> getQPointFs();
    const QVector<QPoint> getQPoints();
    bool getClosed() const { return data.closed; }
    double getWidth() const {return data.width;}
    QList<std::shared_ptr<Entity>> getChilds() const { return childs; }
    QRectF getBox() const ;

public:
    // dxf中多段线不全是直线，会有弧线，需要弧度计算，弧线结构体未定义，先实现直线的
    void appendVertexs(const QList<Point>& vl);
    std::unique_ptr<Entity> createVertex(const Point& p,const Point& b,double bulge=0.0);

public:
    // 有序列表
    QList<std::shared_ptr<Entity>> childs;
public:
    PolylineData data;
};

#endif // POLYLINE_H
