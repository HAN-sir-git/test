#ifndef CGEOMETRYANALYSIS_H
#define CGEOMETRYANALYSIS_H

#include <QObject>
#include <QGraphicsScene>
#include <QMap>
#include <QSet>
#include <qmath.h>

using QGraphicsItemListPtr = QList<QGraphicsItem*>;
class QGraphicsScene;
class QGraphicsItem;

// 二维平面解析几何类
class CGeometryAnalysis: public QObject
{
    Q_OBJECT
public:
    CGeometryAnalysis(QGraphicsScene *scene);
    ~CGeometryAnalysis();

    // 对场景图元根据矩形面积进行排序 从大到小 并建立Map
    void sortItemsByBoundingRectAreaDescending(QList<QGraphicsItem *> &items, QMap<int, QGraphicsItemListPtr> &map,QList<QGraphicsItem *> &filteritemlist);

    // 将真正相交的图元进行聚类，返回聚类结果
    // items 为待聚类的图元,已经按照面积从大到小排序
    QMap<QGraphicsItem*,QGraphicsItemListPtr> intersectItemsSingleCluster(QList<QGraphicsItem *> &itemlist,QGraphicsScene *scene,QList<QGraphicsItem *> &filteritemlist);


    QMap<QGraphicsItem*,QGraphicsItemListPtr> intersectItemsLoopCluster(QList<QGraphicsItem *> &itemlist,QGraphicsScene *scene,QList<QGraphicsItem *> &filteritemlist);

    // 对一个图元进行单次聚类不外扩,所以需要最后做一次并集操作
    QGraphicsItemListPtr  singleCluster(QGraphicsItem *item, QGraphicsScene *scene,QSet<QGraphicsItem *>& clusteredItems);

    // 对一个图元进行循环聚类
    QGraphicsItemListPtr  loopCluster(QGraphicsItem *item, QGraphicsScene *scene,QSet<QGraphicsItem *>& clusteredItems);

    // 对一个图元进行单次聚类外扩
    QMap<QGraphicsItem*,QGraphicsItemListPtr>  singleClusterExpand(QMap<QGraphicsItem*,QGraphicsItemListPtr> singleClusterMap);


public:

    QGraphicsScene *m_scene;
    QMap<int, QGraphicsItemListPtr> m_Area2ItemMap;
    QMap<QGraphicsItem*,QGraphicsItemListPtr> m_singleClusterMap;
    QMap<QGraphicsItem*,QGraphicsItemListPtr> m_loopClusterMap;
};

#endif // CGEOMETRYANALYSIS_H
