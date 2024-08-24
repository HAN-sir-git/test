#ifndef CUSTOMGRAPHICSPOLYGONITEM_H
#define CUSTOMGRAPHICSPOLYGONITEM_H

#include <QGraphicsPolygonItem>
#include "customgraphicsitem.h"


class CustomGraphicsPolygonItem : public QGraphicsPolygonItem , public CustomGraphicsInterface
{
public:
    CustomGraphicsPolygonItem(QGraphicsItem *parent = nullptr);
    CustomGraphicsPolygonItem(const QPolygonF &polygon,
                              QGraphicsItem *parent = nullptr);

};

#endif // CUSTOMGRAPHICSPOLYGONITEM_H
