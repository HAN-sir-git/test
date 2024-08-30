#ifndef CUSTOMGRAPHICSLINEITEM_H
#define CUSTOMGRAPHICSLINEITEM_H

#include  "customgraphicsitem.h"

#include <QGraphicsLineItem>

class CustomGraphicsLineItem : public QGraphicsLineItem, public CustomGraphicsInterface 
{
public:
    CustomGraphicsLineItem(QGraphicsItem *parent = nullptr);
    CustomGraphicsLineItem(const QLineF &line, QGraphicsItem *parent = nullptr);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:

    QPointF lastMousePos;


};

#endif // CUSTOMGRAPHICSLINEITEM_H
