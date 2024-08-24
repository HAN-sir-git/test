#include "customgraphicspathitem.h"

CustomGraphicsPathItem::CustomGraphicsPathItem(QGraphicsItem *parent): QGraphicsPathItem(parent)
{
}

CustomGraphicsPathItem::CustomGraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent)
    : QGraphicsPathItem(path, parent)
{
}
 
