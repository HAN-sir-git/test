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

    // 对一个图元进行单次聚类不外扩,所以需要最后做一次并集操作（可多线程）
    QGraphicsItemListPtr  singleCluster(QGraphicsItem *item, QGraphicsScene *scene,QSet<QGraphicsItem *>& clusteredItems);

    // 对一个图元进行单次聚类外扩（并集操作）
    QMap<QGraphicsItem*,QGraphicsItemListPtr>  singleClusterExpand(QMap<QGraphicsItem*,QGraphicsItemListPtr> singleClusterMap);

    // 对一个图元进行循环聚类
    QGraphicsItemListPtr  loopCluster(QGraphicsItem *item, QGraphicsScene *scene,QSet<QGraphicsItem *>& clusteredItems);

    QMap<QGraphicsItem*, QList<QLineF>> convertToLineMap(const QMap<QGraphicsItem*, QGraphicsItemListPtr>& inputMap);

    /************************************************分开实现******************************************************/

    // 对线段集合打断返回新的集合并构建点的邻域关系
    QList<QLineF> breakLinesByIntersections(const QList<QLineF> &lines, QMap<QPointF, QSet<QPointF> > &neighborhood);

    // 计算线段的交点
    QPointF findIntersection(const QLineF &line1, const QLineF &line2, bool &intersect);

    // 找到所有交点
    void findAllIntersections(const QList<QLineF>& lines, QVector<QList<QPointF>>& intersections);

    // 对线段进行打断并记录新线段
    void splitLines(const QList<QLineF>& lines, const QVector<QList<QPointF>>& intersections, QList<QLineF>& brokenLines);

    // 构建邻域关系
    void updateNeighborhood(const QList<QLineF>& lines, QMap<QPointF, QSet<QPointF>>& neighborhood);

    // 通过子函数构建的打断线集合和点的邻域关系
    QList<QLineF> breakLinesByIntersectionFromFuncs(const QList<QLineF>& lines, QMap<QPointF, QSet<QPointF>>& neighborhood);

    /*************************************************************************************************************/

    // 查找轮廓中的左下角点
    QPointF findBottomLeftPoint(const QList<QPointF>& points);

    qreal angleBetweenLists(const QPointF& referenceList, const QPointF& List);

    // 从指定起始点(左下角点)开始，找到逆时针方向上夹角最大的点，并依次更新参考向量
    QList<QPointF> findContour(const QPointF& startPoint, const QMap<QPointF, QSet<QPointF>>& pointNeighborhood);



public:

    QGraphicsScene *m_scene;
    QMap<int, QGraphicsItemListPtr> m_Area2ItemMap;
    QMap<QGraphicsItem*,QGraphicsItemListPtr> m_singleClusterMap;
    QMap<QGraphicsItem*,QGraphicsItemListPtr> m_loopClusterMap;

};

#endif // CGEOMETRYANALYSIS_H
