#include "customgraphicspolygonitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPen>

CustomGraphicsPolygonItem::CustomGraphicsPolygonItem(QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    setPen(QPen(Qt::black, 40));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

CustomGraphicsPolygonItem::CustomGraphicsPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsPolygonItem(polygon, parent)
{
    setPen(QPen(Qt::black, 40));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void CustomGraphicsPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsPolygonItem::mousePressEvent(event);
}

void CustomGraphicsPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (lastMousePos != pos() && m_undoStack) {
            m_undoStack->push(new MoveCommand(this, lastMousePos, pos()));
        }
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

