#ifndef CUSTOMGRAPHICSPATHITEM_H
#define CUSTOMGRAPHICSPATHITEM_H

#include <QGraphicsPathItem>
#include "customgraphicsitem.h"


class CustomGraphicsPathItem : public QGraphicsPathItem  , public CustomGraphicsInterface
{
public:
    CustomGraphicsPathItem(QGraphicsItem *parent = nullptr);
    CustomGraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent = nullptr);

public:
    QPainterPath getGlobalPath() const;
    
    QList<QLineF> getGlobalChildLine() const;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QVector<QLineF> pathToLines(const QPainterPath& path);
private:

    QPointF lastMousePos;
};

#endif // CUSTOMGRAPHICSPATHITEM_H
