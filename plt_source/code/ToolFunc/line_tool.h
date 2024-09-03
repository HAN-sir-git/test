#ifndef LINETOOL_H
#define LINETOOL_H

#include <QLineF>
#include <QVector2D>

enum LinePositionalRelationship {
    PosNone = 0,           //不相交，不平行，不共线
    PosParallel,           //平行不在同一直线上
    CollinearStagger,   //共线（在同一直线上）错开
    CollinearIntersect, //共线相交在端点处，端点相连，可以求唯一交点
    CollinearOverlap,   //共线重合
    IntersectCross,     //交叉相交,，可以求唯一交点
    IntersectSingleEndPoint,  //相交单端点处，可以求唯一交点
    IntersectDoubleEndPoint,  //相交双端点处，可以求唯一交点
    UnboundedIntersectionCross, //无界相交，可以求唯一交点
    UnboundedIntersectionSingleEndPoint, // 无界相交，相交单端点处，可以求唯一交点
    UnboundedIntersectionDoubleEndPoint, //无界相交，相交双端点处，可以求唯一交点
    VerticalIntersectCross,  //垂直相交，可以求唯一交点
    VerticalIntersectSingleEndPoint,    //垂直相交单端点处，可以求唯一交点
    VerticalIntersectDoubleEndPoint,    //垂直相交双端点处，可以求唯一交点
    VerticalUnboundedIntersectionCross, //垂直无界相交，可以求唯一交点
    VerticalUnboundedIntersectionSingleEndPoint, //垂直无界相交，相交单端点处，可以求唯一交点
    VerticalUnboundedIntersectionDoubleEndPoint, //垂直无界相交，相交双端点处，可以求唯一交点

};

class LineTool
{
public:
    LineTool();
    // 获取两条线的位置关系
    static std::pair<LinePositionalRelationship,QPointF> GetPositionalRelationship(const QLineF& l1,const QLineF& l2,
                                                                    double  parallelPrecision = 0.001,
                                                                    double  coincidePrecision = 0.001);
    // 两条线段在任一其他线段(v)投影的交叠距离
    static double CrossLength(const QLineF& l1,const QLineF& l2, QVector2D v);

    // 变形1 默认使用l1做投影向量
    static double CrossLength(const QLineF& l1,const QLineF& l2);

    // 判断向量水平
    static bool IsHorizontal(const QVector2D& v,double precision = 0.01);

    // 判断向量竖直
    static bool IsVertical(const QVector2D& v,double precision = 0.01);

};

#endif // LINE_TOOL_H
