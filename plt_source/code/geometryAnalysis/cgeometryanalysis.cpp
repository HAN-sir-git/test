#include "cgeometryanalysis.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
#include "CustomGraphicsItem/customgraphicsitem.h"
#include <cmath>
#include <algorithm>


bool operator<(const QPointF& p1, const QPointF& p2) {
    if (p1.x() < p2.x()) return true;
    if (p1.x() > p2.x()) return false;
    return p1.y() < p2.y();
}

uint qHash(const QPointF &key, uint seed = 0) {
    return qHash(qRound(key.x()), seed) ^ qHash(qRound(key.y()), seed);
}

bool pointsAreClose(const QPointF& p1, const QPointF& p2, qreal epsilon = 0.0001) {
    return std::abs(p1.x() - p2.x()) < epsilon && std::abs(p1.y() - p2.y()) < epsilon;
}

CGeometryAnalysis::CGeometryAnalysis(QGraphicsScene *scene)
{
    m_scene = scene;
    m_Area2ItemMap.clear();
}

CGeometryAnalysis::~CGeometryAnalysis()
{
}

bool CGeometryAnalysis::isLineCollinearWithRectangleEdge(const QLineF &line, const QRectF &rect) {
    // 获取矩形的四条边的y或x坐标
    double topY = rect.top();
    double bottomY = rect.bottom();
    double leftX = rect.left();
    double rightX = rect.right();

    // 检查线段是否水平且共线
    if (line.dy() == 0) {
        if (line.y1() == topY || line.y1() == bottomY) {
            return true;
        }
    }

    // 检查线段是否竖直且共线
    if (line.dx() == 0) {
        if (line.x1() == leftX || line.x1() == rightX) {
            return true;
        }
    }

    return false;
}

void CGeometryAnalysis::sortItemsByBoundingRectAreaDescending(QList<QGraphicsItem *> &items, QMap<int, QGraphicsItemListPtr> &map, QList<QGraphicsItem *> &filteritemlist, QRectF box)
{
    map.clear();
    auto maxArea = m_scene->sceneRect().width() * m_scene->sceneRect().height() * 0.9;
    auto rect = m_scene->sceneRect();
    for (QGraphicsItem *item : items)
    {

        auto *lineItem = qgraphicsitem_cast<QGraphicsLineItem *>(item);
        if(lineItem )
        {
            auto l = lineItem->line();
            auto b = isLineCollinearWithRectangleEdge(l,box);
            if(b || l.length() > box.width()*0.8)
            {
                filteritemlist.append(item);
                continue;
            }
        }

        auto area = qCeil(item->boundingRect().width() * item->boundingRect().height());
        if(area >= maxArea * 0.9)
        {
            filteritemlist.append(item);
            continue;
        }
        map[area].append(item);
    }
    QList<int> keys = map.keys();
    qSort(keys.begin(), keys.end(), qGreater<int>());
    items.clear();
    for (auto  key: keys)
    {
        items.append(map[key]);
    }
}

QMap<QGraphicsItem*,QGraphicsItemListPtr> CGeometryAnalysis::intersectItemsSingleCluster(QList<QGraphicsItem *> &itemlist,
                                                                                           QGraphicsScene *scene,QList<QGraphicsItem *> &filteritemlist)
{
    // 用于过滤掉已经聚类的图元
    QSet<QGraphicsItem *> clusteredItems = filteritemlist.toSet();

    // 结果集合
    QMap<QGraphicsItem*,QGraphicsItemListPtr> clusters;
    for (QGraphicsItem *item : itemlist)
    {
        // 如果已经聚类过了，则跳过
        if (clusteredItems.contains(item))
        {
            continue;
        }
        // 这里的singleCluster和loopCluster是两个不同的聚类方法
        auto cluster = singleCluster(item, scene, clusteredItems);
        // 将本次聚类结果加入到结果集合中
        clusters[item] = cluster;
    }
    return clusters;
}

QMap<QGraphicsItem *, QGraphicsItemListPtr> CGeometryAnalysis::intersectItemsLoopCluster(QList<QGraphicsItem *> & itemlist,
                                                                                         QGraphicsScene * scene,QList<QGraphicsItem *> &filteritemlist)
{
        // 用于过滤掉已经聚类的图元
    QSet<QGraphicsItem *> clusteredItems = filteritemlist.toSet();

    // 结果集合
    QMap<QGraphicsItem*,QGraphicsItemListPtr> clusters;
    for (QGraphicsItem *item : itemlist)
    {
        // 如果已经聚类过了，则跳过
        if (clusteredItems.contains(item))
        {
            continue;
        }
        // 这里的singleCluster和loopCluster是两个不同的聚类方法
        auto cluster = loopCluster(item, scene, clusteredItems);
        // 将本次聚类结果加入到结果集合中
        clusters[item] = cluster;
    }
    return clusters;
}

QGraphicsItemListPtr CGeometryAnalysis::singleCluster(QGraphicsItem *item, QGraphicsScene *scene , QSet<QGraphicsItem *>& clusteredItems)
{
    clusteredItems.insert(item);

    // 聚类找到的图元
    QGraphicsItemListPtr cluster = {item};
    
    // 被完全覆盖的图元，不可能是轮廓图元
    QGraphicsItemListPtr coveredItems = {};

    // 获取与当前矩形相交的图元
    QList<QGraphicsItem*> intersectingItems = scene->items(item->boundingRect(), Qt::ContainsItemShape);
    QPainterPath itemShape = item->shape();
    // 聚类的
    for (auto intersectingItem : intersectingItems)
    {
        // 和矩形相交的图元可能聚类过了
        if (clusteredItems.contains(intersectingItem))
        {
            continue;
        }
        // 判断itemShape是否遮挡了intersectingItem
        if( itemShape.contains(intersectingItem->shape()))
        {
            // 如果遮挡了，则将intersectingItem加入到item的聚类中
            coveredItems.append(intersectingItem);
            clusteredItems.insert(intersectingItem);
            continue;
        }
        else if(itemShape.intersects(intersectingItem->shape()))
        {
            // 如果intersectingItem遮挡了item，则将item加入到intersectingItem的聚类中
            cluster.append(intersectingItem);
            clusteredItems.insert(intersectingItem);
        }
    }
    return cluster;
}

QGraphicsItemListPtr CGeometryAnalysis::loopCluster(QGraphicsItem * item, QGraphicsScene * scene , QSet<QGraphicsItem *>& clusteredItems)
{
    clusteredItems.insert(item);
    // 聚类找到的图元
    QGraphicsItemListPtr cluster = {item};
    // 需要循环聚类的图元
    QGraphicsItemListPtr loopItems = {item};
    // 被完全覆盖的图元，不可能是轮廓图元
    QGraphicsItemListPtr coveredItems = {};
    // 获取与当前矩形相交的图元
    while(!loopItems.isEmpty())
    {
        QGraphicsItem *item = loopItems.takeFirst();
        QList<QGraphicsItem*> intersectingItems = scene->items(item->boundingRect(), Qt::IntersectsItemShape);
        QPainterPath itemShape = item->shape();
        // 聚类的
        for (QGraphicsItem *intersectingItem : intersectingItems)
        {
            // 和矩形相交的图元可能聚类过了
            if (clusteredItems.contains(intersectingItem))
            {
                continue;
            }
            // 判断itemShape是否遮挡了intersectingItem

            if(itemShape.contains(intersectingItem->shape()))
            {
                // 如果遮挡了，则将intersectingItem加入到item的覆盖图元中
                coveredItems.append(intersectingItem);
                clusteredItems.insert(intersectingItem);
                continue;
            }
            else if(itemShape.intersects(intersectingItem->shape()))
            {
                // 如果intersectingItem遮挡了item，则将item加入到intersectingItem的聚类中
                loopItems.append(intersectingItem);
                cluster.append(intersectingItem);
                clusteredItems.insert(intersectingItem);
            }
        }
    }
    return cluster;
}

QMap<QGraphicsItem *, QList<QLineF> > CGeometryAnalysis::convertToLineMap(const QMap<QGraphicsItem *, QGraphicsItemListPtr> &inputMap) {
    QMap<QGraphicsItem*, QList<QLineF>> lineMap;

    // 遍历输入的 QMap
    for (auto it = inputMap.constBegin(); it != inputMap.constEnd(); ++it) {
        QGraphicsItem* keyItem = it.key();
        QGraphicsItemListPtr itemListPtr = it.value();

        QList<QLineF> lineList;

        // 遍历 QGraphicsItemListPtr 中的每个 QGraphicsItem
        for (QGraphicsItem* item : itemListPtr) {
            if (item && dynamic_cast<CustomGraphicsInterface*>(item)) {
                CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
                lineList.append(p->childLine());
            }
        }

        // 将结果添加到新的 QMap 中
        lineMap.insert(keyItem, lineList);
    }

    return lineMap;
}


QPointF CGeometryAnalysis::findIntersection(const QLineF &line1, const QLineF &line2, bool &intersect)
{
    QPointF intersectionPoint;
    QLineF::IntersectionType type = line1.intersect(line2, &intersectionPoint);
    intersect = (type == QLineF::BoundedIntersection);
    return intersectionPoint;
}

void CGeometryAnalysis::findAllIntersections(const QList<QLineF> &lines, QVector<QList<QPointF> > &intersections) {
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = i + 1; j < lines.size(); ++j) {
            bool intersect = false;
            QPointF intersection = findIntersection(lines[i], lines[j], intersect);
            if (intersect) {
                intersections[i].append(intersection);
                intersections[j].append(intersection);
            }
        }
    }
}

void CGeometryAnalysis::splitLines(const QList<QLineF> &lines, const QVector<QList<QPointF> > &intersections, QList<QLineF> &brokenLines) {
    for (int i = 0; i < lines.size(); ++i) {
        QList<QPointF> points = { lines[i].p1(), lines[i].p2() };
        points.append(intersections[i]);

        // 对点进行排序
        std::sort(points.begin(), points.end(), [](const QPointF& a, const QPointF& b) {
            return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
        });

        // 创建新的线段
        for (int k = 0; k < points.size() - 1; ++k) {
            if (points[k] != points[k + 1]) {
                QLineF newLine(points[k], points[k + 1]);
                brokenLines.append(newLine);
            }
        }
    }
}

void CGeometryAnalysis::updateNeighborhood(const QList<QLineF> &lines, QMap<QPointF, QSet<QPointF> > &neighborhood) {
    neighborhood.clear();
    // 更新连通性
    for (const QLineF& segment : lines) {
        neighborhood[segment.p1()].insert(segment.p2());
        neighborhood[segment.p2()].insert(segment.p1());
    }

}

QList<QLineF> CGeometryAnalysis::breakLinesByIntersectionFromFuncs(const QList<QLineF> &lines, QMap<QPointF, QSet<QPointF> > &neighborhood) {
    QList<QLineF> brokenLines;
    QVector<QList<QPointF>> intersections(lines.size());
    QMap<QPointF, QSet<QPointF>> pointMap;

    // 找到所有交点
    findAllIntersections(lines, intersections);

    // 对线段进行打断并记录新线段
    splitLines(lines, intersections, brokenLines);

    // 更新邻域关系
    updateNeighborhood(brokenLines, neighborhood);

    return brokenLines;
}

QPointF CGeometryAnalysis::findBottomLeftPoint(const QList<QPointF> &points) {
    if (points.isEmpty()) {
        return QPointF(); // 返回无效点或处理空情况
    }

    QPointF bottomLeft = points.first();

    for (const QPointF& point : points) {
        // 如果 x 更小，更新左下角点
        if (point.x() < bottomLeft.x() ||
                (point.x() == bottomLeft.x() && point.y() < bottomLeft.y())) {
            bottomLeft = point;
        }
    }

    return bottomLeft;
}

qreal CGeometryAnalysis::angleBetweenLists(const QPointF &referenceList, const QPointF &List) {
    // 计算点积
        qreal dotProduct = referenceList.x() * List.x() + referenceList.y() * List.y();

        // 计算向量的模长
        qreal magnitude1 = std::sqrt(referenceList.x() * referenceList.x() + referenceList.y() * referenceList.y());
        qreal magnitude2 = std::sqrt(List.x() * List.x() + List.y() * List.y());

        // 计算余弦值
        qreal cosTheta = dotProduct / (magnitude1 * magnitude2);
        cosTheta = std::max(-1.0, std::min(cosTheta, 1.0));  // 防止浮点误差

        // 计算角度（弧度）
        qreal angle = std::acos(cosTheta);

        // 计算向量的叉积
        qreal crossProduct = referenceList.x() * List.y() - referenceList.y() * List.x();

        // 判断角度的方向，调整为0到360度的逆时针夹角
        qreal angleInDegrees = qRadiansToDegrees(angle);
        if (crossProduct < 0) {
            angleInDegrees = 360.0 - angleInDegrees;
        }

        return angleInDegrees;
}

QList<QPointF> CGeometryAnalysis::findContour(const QPointF &startPoint, const QMap<QPointF, QSet<QPointF> > &pointNeighborhood) {
    QList<QPointF> contourPoints;
    QSet<QPointF> filterPoints;
    QPointF currentPoint = startPoint;
    QPointF old_currentPoint = QPointF(startPoint.x() -1,startPoint.y());
    contourPoints.append(startPoint);
    filterPoints.insert(startPoint);
    // 初始参考向量
    QPointF referenceList(old_currentPoint- startPoint); // 从左下角点指向 (0, 0)

    int i = 0;
    bool end = false;
    while (pointNeighborhood.contains(currentPoint) && !end) {
        QPointF maxPoint;
        qreal maxAngle = 361;
        // 遍历邻域点，计算角度
        for (const QPointF& neighbor : pointNeighborhood.value(currentPoint)) {
            if (neighbor == currentPoint) {
                continue; // 忽略自己
            }
            // 忽略找过的方向,但是最后会找回来，此时咋办
            if (filterPoints.contains(neighbor))
            {
                if(neighbor == startPoint && neighbor != old_currentPoint)
                {
                     end = true;
                     contourPoints.append(startPoint);
                     break;
                }
                continue;
            }

            QPointF List = neighbor - currentPoint;
            qreal angle = angleBetweenLists(referenceList, List);

            // 更新最大角度向量
            if (angle < maxAngle && pointNeighborhood.contains(neighbor)) {
                maxAngle = angle;
                maxPoint = neighbor;
            }
        }

        if (end) {
            break;
        }
        // 回退向量
        if (maxPoint.isNull()) {
            if(contourPoints.size() > 2)
            {
                contourPoints.removeLast();
                currentPoint = contourPoints[contourPoints.size()-1];
                referenceList = contourPoints[contourPoints.size()-2] - currentPoint;
            }else {
                break;
            }

        }else
        {
            // 添加到轮廓点
            contourPoints.append(maxPoint);
            filterPoints.insert(maxPoint);
            // 更新当前点和参考向量
            referenceList = currentPoint - maxPoint; // 从新点指向当前点的向量
            old_currentPoint = currentPoint;
            currentPoint = maxPoint; // 更新当前点为新点
        }
        i++;
    }

    return contourPoints;
}

QList<QLineF> CGeometryAnalysis::breakLinesByIntersections(const QList<QLineF> &lines, QMap<QPointF, QSet<QPointF> > &neighborhood)
{
    QList<QLineF> brokenLines;
    QMap<QPointF, QSet<QPointF>> pointMap;
    QVector<QList<QPointF>> intersections(lines.size());
    // 找到所有交点
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = i + 1; j < lines.size(); ++j) {
            bool intersect = false;
            QPointF intersection = findIntersection(lines[i], lines[j], intersect);
            if (intersect) {
                intersections[i].append(intersection);
                intersections[j].append(intersection);
            }
        }
    }

    // 对每条线段打断
    for (int i = 0; i < lines.size(); ++i) {
        QList<QPointF> points = { lines[i].p1(), lines[i].p2() };
        points.append(intersections[i]);

        // 对点进行排序
        std::sort(points.begin(), points.end(), [](const QPointF& a, const QPointF& b) {
            return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
        });

        // 创建新的线段
        for (int k = 0; k < points.size() - 1; ++k) {
            if (points[k] != points[k + 1]) {
                QLineF newLine(points[k], points[k + 1]);
                brokenLines.append(newLine);

                // 更新邻域关系
                pointMap[points[k].toPoint()].insert(points[k + 1].toPoint());
                pointMap[points[k + 1].toPoint()].insert(points[k].toPoint());
            }
        }
    }
    neighborhood = pointMap;
    return brokenLines;
}

QMap<QGraphicsItem *, QGraphicsItemListPtr> CGeometryAnalysis::singleClusterExpand(QMap<QGraphicsItem *, QGraphicsItemListPtr> singleClusterMap)
{
    QMap<QGraphicsItem*,QGraphicsItemListPtr> expandClusterMap;

    // 记录扩展过的item
    QSet<QGraphicsItem *> expandedItems;

    // 记录需要循环扩展的item
    for(auto item : singleClusterMap.keys())
    {
        if(expandedItems.contains(item))
        {
            continue;
        }
        QGraphicsItemListPtr& cluster = expandClusterMap[item];
        QGraphicsItemListPtr loopItems = {item};
        while(!loopItems.isEmpty())
        {
            QGraphicsItem *item1 = loopItems.takeFirst();
            if(expandedItems.contains(item1))
            {
                continue;
            }
            expandedItems.insert(item);
            // cluster和singleClusterMap[item1]的并集
            cluster = (cluster.toSet() | singleClusterMap[item1].toSet()).toList();
            loopItems = (loopItems.toSet() | singleClusterMap[item1].toSet()).toList();
            expandedItems.insert(item1);
        }
    }
    return expandClusterMap;
}

