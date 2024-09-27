#include "customgraphicspolygonitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPen>
#include <QDebug>

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

QPolygonF CustomGraphicsPolygonItem::getGlobalPolygon()
{
    QPolygonF globalPolygon;
    QPolygonF localPolygon = this->polygon();

    // 将每个局部顶点映射到场景坐标（全局坐标）
    for (const QPointF &point : localPolygon) {
        globalPolygon << mapToScene(point);
    }

    return globalPolygon;
}

QList<QLineF> CustomGraphicsPolygonItem::getGlobalChildLine() const
{
    QList<QLineF> lines;
    for (const QLineF &line : child_line) {
        lines.append(QLineF(mapToScene(line.p1()), mapToScene(line.p2())));
    }
    return lines;
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

