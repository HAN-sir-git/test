#include "customgraphicslineitem.h"

CustomGraphicsLineItem::CustomGraphicsLineItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{

}

CustomGraphicsLineItem::CustomGraphicsLineItem(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent)
{

}
