#include "customgraphicsitemgroup.h"
#include "customgraphicsheader.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>

CustomGraphicsItemGroup::CustomGraphicsItemGroup(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

CustomGraphicsItemGroup::~CustomGraphicsItemGroup()
{
}

void CustomGraphicsItemGroup::setPen(const QPen &pen)
{
    // 设置组内所有图元的画笔
    for (QGraphicsItem *item : childItems()) {
        switch (item->type()) {
        case QGraphicsItemGroup::Type:
            static_cast<CustomGraphicsItemGroup *>(item)->setPen(pen);
            break;
        case QGraphicsLineItem::Type:
            static_cast<QGraphicsLineItem *>(item)->setPen(pen);
            break;
        case QGraphicsEllipseItem::Type:
            static_cast<QGraphicsEllipseItem *>(item)->setPen(pen);
            break;
        case QGraphicsPathItem::Type:
            static_cast<QGraphicsPathItem *>(item)->setPen(pen);
            break;
        case QGraphicsPolygonItem::Type:
            static_cast<QGraphicsPolygonItem *>(item)->setPen(pen);
            break;
        default:
            break;
        }   
    }
}

QList<QLineF> CustomGraphicsItemGroup::getGlobalChildLine() const
{
    QList<QLineF> lines;
    for (QGraphicsItem *item : childItems()) {
        switch (item->type()) {
        case QGraphicsItemGroup::Type:
            lines.append(static_cast<CustomGraphicsItemGroup *>(item)->getGlobalChildLine());
            break;
        case QGraphicsLineItem::Type:
            lines.append(static_cast<CustomGraphicsLineItem *>(item)->getGlobalLine());
            break;
        case QGraphicsEllipseItem::Type:
//            lines.append(static_cast<CustomGraphicsEllipseItem *>(item)->getGlobalChildLine());
            break;
        case QGraphicsPathItem::Type:
            lines.append(static_cast<CustomGraphicsPathItem *>(item)->getGlobalChildLine());
            break;
        case QGraphicsPolygonItem::Type:
            lines.append(static_cast<CustomGraphicsPolygonItem *>(item)->getGlobalChildLine());
            break;
        default:
            break;
        }
    }
    return lines;
}

void CustomGraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastMousePos = pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsItemGroup::mousePressEvent(event);
}

void CustomGraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (lastMousePos != pos() && m_undoStack) {
            m_undoStack->push(new MoveCommand(this, lastMousePos, pos()));
        }
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItemGroup::mouseReleaseEvent(event);
}
