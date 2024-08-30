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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:

    QPointF lastMousePos;
};





#endif // CUSTOMGRAPHICSPOLYGONITEM_H
