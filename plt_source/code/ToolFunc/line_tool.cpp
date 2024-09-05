#include "line_tool.h"
#include "tool.h"
#include <QVector2D>
#include <QVector3D>
#include <math.h>
LineTool::LineTool()
{
}

std::pair<LinePositionalRelationship, QPointF> LineTool::GetPositionalRelationship(const QLineF &l1, const QLineF &l2, double parallelPrecision,double  coincidePrecision)
{
    // l1,l2转向量
    QVector2D v1(l1.p2() - l1.p1());
    QVector2D v2(l2.p2() - l2.p1());
    // 2维映射3维求解
    double sin_v1_v2 = QVector3D::crossProduct(v1, v2).z() / (v1.length() * v2.length());
    // SinPrecision 0.01
    if (fabs(sin_v1_v2) < 0.01)
    {
        // 获取v1,v2的距离
        double dis_v1_v2 = Tool::getDistanceToPoint(l1.p1(), l2);
        // DisPrecision 0.01
        if (dis_v1_v2 > parallelPrecision)
        {
            return std::make_pair(LinePositionalRelationship::PosParallel, QPointF());
        }
        else
        {
            QLineF l3 = l2;
            // 同向
            if (QVector2D::dotProduct(v1, v2) < .0)
            {
                l3 = QLineF(l2.p2(), l2.p1());
            }
            // 同向后求首尾点组成长度
            QVector2D v3(l3.p1() - l1.p1());
            QVector2D v4(l3.p2() - l1.p2());

            double cross_len = (v1.length() + v2.length() - v3.length() - v4.length()) * 0.5;

            if (fabs(cross_len) < parallelPrecision)
            {
                QPointF ePoint;
                // 求相交端点
                ePoint = ((QVector2D(l2.p1() - l1.p1()).length() < parallelPrecision) || QVector2D(l2.p1() - l1.p2()).length() < parallelPrecision)
                             ? l2.p1()
                             : l2.p2();
                return std::make_pair(LinePositionalRelationship::CollinearIntersect, ePoint);
            }
            else if (cross_len > parallelPrecision)
            {
                return std::make_pair(LinePositionalRelationship::CollinearOverlap, QPointF());
            }
            else
            {
                return std::make_pair(LinePositionalRelationship::CollinearStagger, QPointF());
            }
        }
    }
    else
    {
        QPointF interPoint;
        auto typeValue = l1.intersect(l2, &interPoint);
        if (typeValue == QLineF::NoIntersection)
            return std::make_pair(LinePositionalRelationship::PosNone, QPointF());

        // 判断两线是否垂直
        auto x = fabs(QVector2D::dotProduct(v1, v2));
        bool isVertical = fabs(QVector2D::dotProduct(v1, v2)) < 0.02;
        LinePositionalRelationship relationship = LinePositionalRelationship::PosNone;
        if(typeValue == QLineF::BoundedIntersection)
        {
            relationship = LinePositionalRelationship::IntersectCross;
        }else {
            relationship = LinePositionalRelationship::UnboundedIntersectionCross;
        }

        // 判断是普通交点还是双端点还是单端点
        // 在规定误差范围内，无界相交变有界相交
        if (QVector2D(interPoint - l1.p1()).length() < coincidePrecision || QVector2D(interPoint - l1.p2()).length() < coincidePrecision)
        {
            typeValue = QLineF::BoundedIntersection;
            relationship = LinePositionalRelationship::IntersectSingleEndPoint;
        }
        if (QVector2D(interPoint - l2.p1()).length() < coincidePrecision || QVector2D(interPoint - l2.p2()).length() < coincidePrecision)
        {
            typeValue = QLineF::BoundedIntersection;
            relationship = relationship == LinePositionalRelationship::IntersectSingleEndPoint ?
                LinePositionalRelationship::IntersectDoubleEndPoint:
                LinePositionalRelationship::IntersectSingleEndPoint;
        }

        if (isVertical)
        {
            if (relationship == LinePositionalRelationship::IntersectSingleEndPoint)
                relationship = LinePositionalRelationship::VerticalIntersectSingleEndPoint;
            else if (relationship == LinePositionalRelationship::IntersectDoubleEndPoint)
                relationship = LinePositionalRelationship::VerticalIntersectDoubleEndPoint;
            else
                relationship = LinePositionalRelationship::VerticalIntersectCross;
        }

        // QLineF::intersect对浮点判断精度bug
        if (typeValue == QLineF::UnboundedIntersection)
        {
            switch (relationship)
            {
            case LinePositionalRelationship::IntersectSingleEndPoint:
                relationship = LinePositionalRelationship::UnboundedIntersectionSingleEndPoint;
                break;
            case LinePositionalRelationship::IntersectDoubleEndPoint:  
                relationship = LinePositionalRelationship::UnboundedIntersectionDoubleEndPoint;
                break;
            case LinePositionalRelationship::IntersectCross:
                relationship = LinePositionalRelationship::UnboundedIntersectionCross;
                break;
            case LinePositionalRelationship::VerticalIntersectSingleEndPoint:
                relationship = LinePositionalRelationship::VerticalUnboundedIntersectionSingleEndPoint;
                break;
            case LinePositionalRelationship::VerticalIntersectDoubleEndPoint:
                relationship = LinePositionalRelationship::VerticalUnboundedIntersectionDoubleEndPoint;
                break;
            case LinePositionalRelationship::VerticalIntersectCross:
                relationship = LinePositionalRelationship::VerticalUnboundedIntersectionCross;
                break;
            default:
                break;
            }  
        }
        return std::make_pair(relationship, interPoint);

    }
}

double LineTool::CrossLength(const QLineF &l1, const QLineF &l2, QVector2D v)
{
    if (v.length() < 0.001)
        return 0.0;
    // 水平，竖直
    if (IsHorizontal(v))
    {
        return std::min(std::max(l1.p1().x(), l1.p2().x()), std::max(l2.p1().x(), l2.p2().x())) - std::max(std::min(l1.p1().x(), l1.p2().x()), std::min(l2.p1().x(), l2.p2().x()));
    }
    else if (IsVertical(v))
    {
        return std::min(std::max(l1.p1().y(), l1.p2().y()), std::max(l2.p1().y(), l2.p2().y())) - std::max(std::min(l1.p1().y(), l1.p2().y()), std::min(l2.p1().y(), l2.p2().y()));
    }
    else
    {
        // 单位化
        v.normalize();
        // l1,l2转向量
        QVector2D v1(l1.p2() - l1.p1());
        QVector2D v2(l2.p2() - l2.p1());
        QLineF l3 = l2;
        // 同向
        if (QVector2D::dotProduct(v1, v2) < .0)
            l3 = QLineF(l2.p2(), l2.p1());
        // 同向后求首尾点组成长度
        QVector2D v3(l3.p1() - l1.p1());
        QVector2D v4(l3.p2() - l1.p2());
        // 矢量点积 对v1,v2,v3,v4和v_unit做点积
        double dot_v1 = fabs(QVector2D::dotProduct(v1, v));
        double dot_v2 = fabs(QVector2D::dotProduct(v2, v));
        double dot_v3 = fabs(QVector2D::dotProduct(v3, v));
        double dot_v4 = fabs(QVector2D::dotProduct(v4, v));
        // 交叠长度  返回负数，则是两条线中间缺失的长度
        double cross_len = (dot_v1 + dot_v2 - dot_v3 - dot_v4) * 0.5;
        return cross_len;
    }
}

double LineTool::CrossLength(const QLineF &l1, const QLineF &l2)
{
    return LineTool::CrossLength(l1, l2, QVector2D(l1.p2() - l1.p1()));
}

bool LineTool::IsHorizontal(const QVector2D &v, double precision)
{
    return fabs(v.y()) < precision;
}

bool LineTool::IsVertical(const QVector2D &v,double precision)
{
    return fabs(v.x()) < precision;
}
