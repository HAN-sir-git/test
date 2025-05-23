#include "cgeometryanalysis.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
#include "CustomGraphicsItem/customgraphicsheader.h"
#include <cmath>
#include <algorithm>
#include "CommonDataForm/common_mathfunc.h"
#include "tool.h"

CGeometryAnalysis::CGeometryAnalysis(QGraphicsScene *scene)
{
    m_scene = scene;
    m_Area2ItemMap.clear();
}

void CGeometryAnalysis::setScene(QGraphicsScene *scene)
{
   m_scene = scene;
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
    for (int i = 0 ;i <  keys.size(); i++)
    {
        items.append(map[keys[i]]);
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


    /*********************************************************************/

    paths.clear();
    polygons.clear();
    /*********************************************************************/

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
    // 缩放因子
     const qreal scaleFactor = 1.0;
    // 获取与当前矩形相交的图元
    while(!loopItems.isEmpty())
    {
        QGraphicsItem *item = loopItems.takeFirst();
        QList<QGraphicsItem*> intersectingItems = scene->items(item->sceneBoundingRect(), Qt::IntersectsItemShape);
        QPainterPath itemShape = item->shape();
        auto itemRect = item->sceneBoundingRect();

        QTransform transform;
        transform.scale(scaleFactor, scaleFactor);
        itemShape = transform.map(itemShape);
        // 聚类的
        for (QGraphicsItem *intersectingItem : intersectingItems)
        {
            // 和矩形相交的图元可能聚类过了
            if (clusteredItems.contains(intersectingItem))
            {
                continue;
            }
            // 判断itemShape是否遮挡了intersectingItem
            // 获取放大后的intersectingItem的shape
            QPainterPath intersectingItemShape = intersectingItem->shape();
            intersectingItemShape = transform.map(intersectingItemShape);

            auto intersectingItemRect = intersectingItem->sceneBoundingRect();

            //if(itemShape.contains(intersectingItemShape))
            if(itemRect.contains(intersectingItemRect))
            {
                // 如果遮挡了，则将intersectingItem加入到item的覆盖图元中
                intersectingItem->setZValue(item->zValue()+1);
                coveredItems.append(intersectingItem);
                clusteredItems.insert(intersectingItem);

            }
            else if(itemShape.intersects(intersectingItemShape))
            //else if(Tool::calculateIntersectionPercentage(itemRect,intersectingItemRect) > 70)
            {
                auto intersectedPath = itemShape.intersected(intersectingItemShape);
               //auto subpaths = extractSubPaths(intersectedPath);
                //paths.append(subpaths);

                // 判断两个图元都是闭合图元
                auto CustomGraphicsPolygonItem = dynamic_cast<QGraphicsPolygonItem*>(item);
                auto CustomGraphicsPolygonItem1 = dynamic_cast<QGraphicsPolygonItem*>(intersectingItem);
                if(CustomGraphicsPolygonItem && CustomGraphicsPolygonItem1)
                {
                    auto pol = intersectedPath.toSubpathPolygons();
                    // 获取 pol 得面积
                    double areas = 0;
                    for (auto p : pol)
                    {
                        auto area = Tool::calculatePolygonArea(p);
                        areas += area;
                    }

                    auto p1 = CustomGraphicsPolygonItem->polygon();
                    auto p2 = CustomGraphicsPolygonItem1->polygon();
                    auto area1 = Tool::calculatePolygonArea(p1);
                    auto area2 = Tool::calculatePolygonArea(p2);
                    auto area = std::min(area1,area2);
                    if(areas > area * 0.3)
                    {
                        // 如果intersectingItem遮挡了item，则将item加入到intersectingItem的聚类中
                        loopItems.append(intersectingItem);
                        cluster.append(intersectingItem);
                        clusteredItems.insert(intersectingItem);
                    }
                    continue;
                }

                // 如果intersectingItem遮挡了item，则将item加入到intersectingItem的聚类中
                //loopItems.append(intersectingItem);
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
                lineList.append(p->getGlobalChildLine());
            }
        }

        // 将结果添加到新的 QMap 中
        lineMap.insert(keyItem, lineList);
    }

    return lineMap;
}

QList<QPainterPath> CGeometryAnalysis::extractSubPaths(const QPainterPath &intersectedPath)
{
    // 遍历 QPainterPath 的所有元素，提取子路径
    QList<QPainterPath> subpaths;
    QPainterPath currentSubpath;
    int elementCount = intersectedPath.elementCount();

    for (int i = 0; i < elementCount; ++i) {
        QPainterPath::Element element = intersectedPath.elementAt(i);

        switch (element.type) {
            case QPainterPath::MoveToElement:
                if (!currentSubpath.isEmpty()) {
                    subpaths.append(currentSubpath);
                    currentSubpath = QPainterPath();
                }
                currentSubpath.moveTo(element.x, element.y);
                break;

            case QPainterPath::LineToElement:
                currentSubpath.lineTo(element.x, element.y);
                break;

            case QPainterPath::CurveToElement:
                // CurveToElement is followed by CurveToDataElement(s)
                if (i + 2 < elementCount) {
                    const QPainterPath::Element& control1 = intersectedPath.elementAt(i);
                    const QPainterPath::Element& control2 = intersectedPath.elementAt(i + 1);
                    const QPainterPath::Element& endPoint = intersectedPath.elementAt(i + 2);
                    currentSubpath.cubicTo(control1.x, control1.y, control2.x, control2.y, endPoint.x, endPoint.y);
                    i += 2; // Skip the two control points and move to the next segment
                }
                break;

            default:
                break;
        }
    }

    if (!currentSubpath.isEmpty()) {
        subpaths.append(currentSubpath);
    }

    return subpaths;
}


QPointF CGeometryAnalysis::findIntersection(const QLineF &line1, const QLineF &line2, bool &intersect)
{
    QPointF intersectionPoint;
    auto type = line1.intersect(line2, &intersectionPoint);
    intersect = (type == QLineF::BoundedIntersection);
    if(!intersect)
    {
        intersect = COMMON_Math::areLinesEndpointConnected(line1,line2,intersectionPoint);
    }
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

QPointF CGeometryAnalysis::findBottomLeftPoint(const QList<QPointF> &points)
{
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

int CGeometryAnalysis::findBottomLeftPointIndex(const QList<QPointF> &points) {
    if (points.isEmpty()) {
        return -1; // 返回无效点或处理空情况
    }

    int  index = 0;
    QPointF bottomLeft = points.first();

    for (int i = 1 ; i < points.size(); i++) {
        auto point = points[i];
        // 如果 x 更小，更新左下角点
        if (point.x() < bottomLeft.x() ||
                (point.x() == bottomLeft.x() && point.y() < bottomLeft.y())) {
            bottomLeft = point;
            index = i;
        }
    }

    return index;
}

QList<int> CGeometryAnalysis::findOverCutPointIndex(const QList<QPointF> &points, qreal minAngle, qreal maxAngle)
{
    QList<int> overCutIndexs;
    int n = points.size();

    //记录 角度最大和最小的索引
    int maxIndex = 0;
    int minIndex = 0;
    qreal maxAngleValue = 0;
    qreal minAngleValue = 360;

    for (int i = 0; i < n; ++i) {
        if(i == 230)
        {
            auto a = 1;
        }
        QPointF p1 = points[i];
        QPointF p2 = points[(i - 1 + n) % n];
        QPointF p3 = points[(i + 1) % n];

        QVector2D v1(p2 - p1);
        QVector2D v2(p3 - p1);
        int angle = Tool::angleBetweenVectors(v1,v2);

        if(angle > maxAngleValue)
        {
            maxAngleValue = angle;
            maxIndex = i;
        }
        if(angle < minAngleValue)
        {
            minAngleValue = angle;
            minIndex = i;
        }
        if (angle > minAngle && angle < maxAngle) {
            overCutIndexs.append(i);
//            // 获取 p2 和 p3 的中点
//            QPointF midPoint = (p2 + p3) / 2;
            
//            // 判断 midPoint 是否在多边形内部
//            QPainterPath path;
//            path.addPolygon(QPolygonF(points.toVector()));
//            if (path.contains(midPoint)) {
//                overCutIndexs.append(i);
//            }

        }
    }

    if(overCutIndexs.isEmpty())
    {
        overCutIndexs.append(minIndex);
    }

    return overCutIndexs;

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

