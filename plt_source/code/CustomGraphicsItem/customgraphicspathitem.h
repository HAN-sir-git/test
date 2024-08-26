#ifndef CUSTOMGRAPHICSPATHITEM_H
#define CUSTOMGRAPHICSPATHITEM_H

#include <QGraphicsPathItem>
#include "customgraphicsitem.h"


class CustomGraphicsPathItem : public QGraphicsPathItem  , public CustomGraphicsInterface
{
public:
    CustomGraphicsPathItem(QGraphicsItem *parent = nullptr);
    CustomGraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent = nullptr);

    QVector<QLineF> pathToLines(const QPainterPath& path);

};

#endif // CUSTOMGRAPHICSPATHITEM_H
