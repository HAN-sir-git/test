#ifndef CUSTOMGRAPHICSITEMGROUP_H
#define CUSTOMGRAPHICSITEMGROUP_H

#include <QGraphicsItemGroup>
#include "customgraphicsitem.h"

class CustomGraphicsItemGroup : public QGraphicsItemGroup, public CustomGraphicsInterface 
{
public:
    CustomGraphicsItemGroup(QGraphicsItem *parent = 0);
    ~CustomGraphicsItemGroup();
public:
     void setPen(const QPen &pen);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

    QPointF lastMousePos;
};

#endif // CUSTOMGRAPHICSITEMGROUP_H
