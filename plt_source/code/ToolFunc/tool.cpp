#include "tool.h"
#include <qmath.h>
#include "ToolFunc/kdtree.hpp"

QPointF Tool::getNearestPointOnEntity(const QPointF& coord,const QLineF &line)
{
    QVector2D P = QVector2D(coord);
    QVector2D A = QVector2D(line.p1());
    QVector2D B = QVector2D(line.p2());
    QVector2D AB = B - A;
    QVector2D AP = P - A;
    double length_2 = AB.lengthSquared();
    const double t = QVector2D::dotProduct(AP, AB) / length_2;
    QVector2D vpc = A + t * AB;
    return vpc.toPointF();
}

bool Tool::K_getNearestPoint(const QPointF &coord, const QList<QPointF> &points, const int &K, QList<QPointF> &res_points)
{
    if(points.empty() || points.size() < K)
        return false;   
    Kdtree::KdNodeVector nodes;
    for (int i = 0; i < points.size(); ++i)
    {
        std::vector<double> point(2);
        point[0] = points[i].x();
        point[1] = points[i].y();
        nodes.push_back(Kdtree::KdNode(point));
    }
    Kdtree::KdTree tree(&nodes);
    Kdtree::KdNodeVector result;
    std::vector<double> coord_point(2);
    coord_point[0] = coord.x();
    coord_point[1] = coord.y();
    tree.k_nearest_neighbors(coord_point, K, &result);

    for (int i = 0; i < result.size(); ++i)
    {
        res_points.push_back(QPointF(result[i].point[0], result[i].point[1]));
    }
    return true;
}

double Tool::angleBetweenVectors(const QVector2D &v1, const QVector2D &v2)
{
    qreal dotProduct = QVector2D::dotProduct(v1, v2);
        qreal lengthV1 = v1.length();
        qreal lengthV2 = v2.length();

        // 避免除零错误
        if (lengthV1 == 0 || lengthV2 == 0) {
            return 0.0;
        }

        // 计算夹角的余弦值
        qreal cosTheta = dotProduct / (lengthV1 * lengthV2);

        // 使用反余弦函数获取夹角（弧度）
        qreal radianAngle = std::acos(cosTheta);

        // 将弧度转换为角度
        qreal degreeAngle = radianAngle * 180.0 / M_PI;

        return degreeAngle;
}

qreal Tool::calculateIntersectionPercentage(const QRectF &rect1, const QRectF &rect2) {
    // 获取相交部分的矩形
    QRectF intersection = rect1.intersected(rect2);

    // 如果没有交集，直接返回 0
    if (intersection.isEmpty()) {
        return 0.0;
    }

    // 计算相交部分的面积
    qreal intersectionArea = intersection.width() * intersection.height();

    // 计算两个矩形的面积
    qreal area1 = rect1.width() * rect1.height();
    qreal area2 = rect2.width() * rect2.height();

    // 找出较小矩形的面积
    qreal minArea = qMin(area1, area2);

    // 计算百分比
    qreal percentage = (intersectionArea / minArea) * 100.0;

    return percentage;
}

qreal Tool::calculatePolygonArea(const QPolygonF &polygon) {
    qreal area = 0.0;
    int n = polygon.size();
    if (n < 3) return 0.0; // 多边形至少需要三个点

    for (int i = 0; i < n; ++i) {
        QPointF p1 = polygon[i];
        QPointF p2 = polygon[(i + 1) % n];
        area += p1.x() * p2.y() - p2.x() * p1.y();
    }

    area = std::abs(area) / 2.0;
    return area;
}

bool Tool::pointInsideLine(const QPointF &coord, const QLineF &line)
{
    QVector2D P = QVector2D(coord);
    QVector2D A = QVector2D(line.p1());
    QVector2D B = QVector2D(line.p2());
    QVector2D AB = B - A;
    QVector2D AP = P - A;
    double dot_product = QVector2D::dotProduct(AP, AB);
    if(dot_product < 0 || dot_product > QVector2D::dotProduct(AB, AB))
        return false;
    return true;
}

QVector2D Tool::getPerpendicularVector2D(const QPointF &coord,const QLineF & line)
{
    QVector2D P = QVector2D(coord);
    QVector2D A = QVector2D(line.p1());
    QVector2D B = QVector2D(line.p2());
    QVector2D AB = B - A;
    QVector2D AP = P - A;

    QVector2D v = QVector2D(-AB.y(), AB.x());

    QVector2D APv = QVector2D::dotProduct(AP, v) / v.lengthSquared() * v;

    return APv;
}
double Tool::getDistanceToPoint(const QPointF &coord,const QLineF & line)
{
    QPointF A = line.p1();
    QPointF B = line.p2();
    QPointF P = coord;
    double APx = P.x() - A.x();
    double APy = P.y() - A.y();
    double ABx = B.x() - A.x();
    double ABy = B.y() - A.y();

    double crossProduct = qAbs(APx * ABy - APy * ABx);

    double lengthAB = qSqrt(ABx * ABx + ABy * ABy);

    double distance = qAbs(crossProduct / lengthAB);

    return distance;
}

QList<QPointF> Tool::getIntersectionPoints(const QList<QLineF> &lines)
{
    QList<QPointF> intersectionPoints;

    for (int i = 0; i < lines.size(); ++i) {
        for (int j = i + 1; j < lines.size(); ++j) {
            QPointF intersectionPoint;
            QLineF::IntersectType intersectType = lines[i].intersect(lines[j], &intersectionPoint);
            if (intersectType == QLineF::BoundedIntersection) {
                intersectionPoints.append(intersectionPoint);
            }
        }
    }
    return intersectionPoints;
}
