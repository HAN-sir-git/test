#include "customgraphicslineitem.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>

CustomGraphicsLineItem::CustomGraphicsLineItem(QGraphicsItem *parent)
    : QGraphicsLineItem(parent) {
    setPen(QPen(Qt::black, 40));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

CustomGraphicsLineItem::CustomGraphicsLineItem(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent) {
    setPen(QPen(Qt::black, 40));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

QLineF CustomGraphicsLineItem::getGlobalLine() const
{
    return QLineF(mapToScene(line().p1()), mapToScene(line().p2()));
}

QList<QLineF> CustomGraphicsLineItem::getGlobalChildLine() const
{
    QList<QLineF> lines;
    for (const QLineF &line : child_line) {
        lines.append(QLineF(mapToScene(line.p1()), mapToScene(line.p2())));
    }
    return lines;
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
