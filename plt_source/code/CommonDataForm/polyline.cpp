#include "polyline.h"

PolyLine::PolyLine(const QList<Point> &points, const bool closed)
{
    data.closed = closed;
    data.points = points;
}

const QVector<QPointF> PolyLine::getQPointFs()
{
    QVector<QPointF> points;
    for (auto &p : data.points) {
        points.append(p.getPointF() *0.1);
    }
    return points;
}

const QVector<QPoint> PolyLine::getQPoints()
{
    QVector<QPoint> points;
    for (auto &p : data.points) {
        points.append(p.getPointF().toPoint());
    }
    return points;
}

QRectF PolyLine::getBox() const
{
    QRectF rect;
    for(auto p: data.points)
    {
        if (p.x() < rect.left()) {
            rect.setLeft(p.x());
        }
        if (p.x() > rect.right()) {
            rect.setRight(p.x());
        }
        if (p.y() < rect.top()) {
            rect.setTop(p.y());
        }
        if (p.y() > rect.bottom()) {
            rect.setBottom(p.y());
        }
    }
    return rect;
}

void PolyLine::appendVertexs(const QList<Point> &vl)
{
    if(!vl.size()) return;
        data.points = vl;

    QPointF center;
    double r{0};
    double angle_value{0.0};
    QList<std::shared_ptr<Entity>> tmpChilds;
    for (int idx = 0; idx < data.points.size() - 1; idx++) {
        std::unique_ptr<Entity> vertex = createVertex(data.points[idx],data.points[idx+1],data.points[idx].getBulge());
        std::shared_ptr<Entity> sharedVertex = std::move(vertex);
        if(sharedVertex->rtti() == COMMON_TYPE::ARC)
        {

        }else {
            if(!tmpChilds.empty())
            {
                childs.append(tmpChilds);
                tmpChilds.clear();
                angle_value = 0.;
                r = 0.;
                center = QPointF();
            }
            childs.append(sharedVertex);
        }

    }
    if(angle_value > 358 && !tmpChilds.empty())
    {
        // 弧线成圆处理
    }else if(!tmpChilds.empty())
    {
        childs.append(tmpChilds);
    }
}

std::unique_ptr<Entity> PolyLine::createVertex(const Point &p, const Point &b, double bulge)
{
    std::unique_ptr<Entity> entity;
    if (std::abs(bulge)<COMMON_TOLERANCE || std::abs(bulge) >= COMMON_MAXDOUBLE) {
        entity = std::unique_ptr<Line>(new Line(p,b));
        entity->setEntityAttributes(attribute);
    }else {
        // 弧线计算处理
    }
    return entity;
}


PolylineData::PolylineData(const QList<Point> &points, const bool closed)
    : points(points), closed(closed) {}
