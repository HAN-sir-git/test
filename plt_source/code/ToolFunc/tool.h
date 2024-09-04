#ifndef TOOL_H
#define TOOL_H
#include <QList>
#include <QPointF>
#include <QLineF>
#include <QVector2D>

// 线的关系
enum LineRelation
{   
    // 无关系
    None = 1 << 0,
    // 平行
    Parallel = 1 << 1,
    // 垂直
    Vertical = 1 << 2,
    // 局部重合 
    Coincide = 1 << 3,
};


// 工具类
class Tool
{
public:
    Tool(){}
    ~Tool(){}

public:
    // 获取线集中所有线的交点
    static QList<QPointF> getIntersectionPoints(const QList<QLineF> &lines);
    // 判断点到线的垂直距离
    static double getDistanceToPoint(const QPointF &coord,const QLineF &line);
    // 获取向量  垂足点->坐标点
    static QVector2D getPerpendicularVector2D(const QPointF &coord,const QLineF &line);
    // 判断点在线内true和线外false
    static bool pointInsideLine(const QPointF &coord,const QLineF &line);
    // 获取点坐标投影到线上的坐标
    static QPointF getNearestPointOnEntity(const QPointF &coord,const QLineF &line);
    // 返回点集合距离coord最近的K个点
    static bool K_getNearestPoint(const QPointF &coord,const QList<QPointF> &points,const int &K, QList<QPointF> &res_points);

    /*****************************geometry relationship************************************************/
    static double angleBetweenVectors(const QVector2D& v1, const QVector2D& v2);
    
};

#endif // TOOL_H
