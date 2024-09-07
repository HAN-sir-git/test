#include "customgraphicslineitem.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>

CustomGraphicsLineItem::CustomGraphicsLineItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent) {
    setPen(QPen(Qt::black, 4));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

CustomGraphicsLineItem::CustomGraphicsLineItem(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent) {
    setPen(QPen(Qt::black, 4));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void CustomGraphicsLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsLineItem::mousePressEvent(event);
}

void CustomGraphicsLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (lastMousePos != pos() && m_undoStack) {
            m_undoStack->push(new MoveCommand(this, lastMousePos, pos()));
        }
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsLineItem::mouseReleaseEvent(event);
}
