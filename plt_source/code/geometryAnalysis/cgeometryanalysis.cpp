#include "cgeometryanalysis.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
CGeometryAnalysis::CGeometryAnalysis(QGraphicsScene *scene)
{
    m_scene = scene;
    m_Area2ItemMap.clear();
}

CGeometryAnalysis::~CGeometryAnalysis()
{
}

void CGeometryAnalysis::sortItemsByBoundingRectAreaDescending(QList<QGraphicsItem *> &items, QMap<int, QGraphicsItemListPtr> &map)
{
    map.clear();
    for (QGraphicsItem *item : items)
    {
        map[qCeil(item->boundingRect().width() * item->boundingRect().height())].append(item);
    }
    QList<int> keys = map.keys();
    qSort(keys.begin(), keys.end(), qGreater<int>());
    items.clear();
    for (auto  key: keys)
    {
        items.append(map[key]);
    }
}

QMap<QGraphicsItem*,QGraphicsItemListPtr> CGeometryAnalysis::intersectItemsSingleCluster(QList<QGraphicsItem *> &itemlist,QGraphicsScene *scene)
{
    // 用于过滤掉已经聚类的图元
    QSet<QGraphicsItem *> clusteredItems;

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

QMap<QGraphicsItem *, QGraphicsItemListPtr> CGeometryAnalysis::intersectItemsLoopCluster(QList<QGraphicsItem *> & itemlist, QGraphicsScene * scene)
{
        // 用于过滤掉已经聚类的图元
    QSet<QGraphicsItem *> clusteredItems;

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

