#include "customgraphicspolygonitem.h"

CustomGraphicsPolygonItem::CustomGraphicsPolygonItem(QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{

}

CustomGraphicsPolygonItem::CustomGraphicsPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsPolygonItem(polygon, parent)
{

}
