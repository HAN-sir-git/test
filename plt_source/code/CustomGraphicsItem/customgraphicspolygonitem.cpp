#include "customgraphicspolygonitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>

CustomGraphicsPolygonItem::CustomGraphicsPolygonItem(QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

CustomGraphicsPolygonItem::CustomGraphicsPolygonItem(const QPolygonF &polygon, QGraphicsItem *parent)
    : QGraphicsPolygonItem(polygon, parent)
{
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

void CustomGraphicsPolygonItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        qreal scaleFactor = 1.15;
        if (event->delta() < 0) {
            scaleFactor = 1.0 / scaleFactor;
        }
        setScale(scale() * scaleFactor);
    } else {
        int angle = event->delta() / 8;
        setRotation(rotation() + angle);
    }
    event->accept();
}
