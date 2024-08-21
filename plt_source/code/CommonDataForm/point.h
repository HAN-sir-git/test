#ifndef POINT_H
#define POINT_H

#include "common_type.h"
#include "entity.h"
#include <QPointF>
#include <cmath>
struct PointData
{
    PointData()  = default;
    PointData(const QPointF& point) : point(point) {}
    PointData(const double& x,const double& y): point(x,y){}
    PointData(const double& x,const double& y,double stawidth,double endwidth,double bulge)
        : point(x,y),stawidth(stawidth),endwidth(endwidth),bulge(bulge){}

    // 变量
    QPointF point;
    double stawidth=0.;         // 多段线起始宽度
    double endwidth=0.;         // 多段线终止宽度
    double bulge=0.;            // 多段线凸度

};

class Point: public Entity
{
public:
    Point(const PointData& data,const bool valid = true) : data(data), valid(valid) {}
    Point(const QPointF& point, const bool valid = true) : data(point), valid(valid) {}
    Point(const double& x,const double& y,const bool valid = true) : data(x,y), valid(valid) {}
    Point(const bool valid = false): valid(valid) {}
    Point(const double angle,const bool vaild = true): data(PointData{std::cos(angle),std::sin(angle)}),valid(vaild){}

    //虚析构函数
    virtual ~Point(){}
    virtual COMMON_TYPE::EntityType rtti() const { return COMMON_TYPE::POINT; }

public:
    /***************************GET**************************************/
    QPointF getPointF() const { return data.point; }
    double getX() const { return data.point.x(); }
    double getY() const { return data.point.y(); }
    QRectF getBox() const { return QRectF(data.point,data.point); }
    double getStaWidth() const { return data.stawidth; }
    double getEndWidth() const { return data.endwidth; }
    double getBulge() const { return data.bulge; }
    bool isValid() const { return valid; }

public:
    /***************************SET**************************************/
    void setPointF(const QPointF& point) { data.point = point; }
    void setX(const double& x) { data.point.setX(x); }
    void setY(const double& y) { data.point.setY(y); }
    void setStaWidth(const double& stawidth) { data.stawidth = stawidth; }
    void setEndWidth(const double& endwidth) { data.endwidth = endwidth; }
    void setBulge(const double& bulge) { data.bulge = bulge; }
    double getAngle() const { return COMMON_Math::correctAngle(std::atan2(data.point.y(),data.point.x()));}
    void setValid(const bool& valid) { this->valid = valid; }

public:
    // 计算向量长度
    double  magnitude(){
        if(valid)
            return std::hypot(data.point.x(),data.point.y());
        return 0.0;
    }

    static double magnitude(const QPoint& point) {
        return std::hypot(point.x(), point.y());
    }

    // 从极坐标（rho 和 theta）创建一个二维平面上的点 rho 表示向量的长度，theta 表示角度（以弧度为单位）
    static Point polar(double rho,double theta){ return Point(rho * std::cos(theta) , rho * std::sin(theta)); }

public:
    PointData data;
    bool valid;
};

#endif // POINT_H
