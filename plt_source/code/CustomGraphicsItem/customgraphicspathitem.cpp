#include "customgraphicspathitem.h"

#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPen>

CustomGraphicsPathItem::CustomGraphicsPathItem(QGraphicsItem *parent): QGraphicsPathItem(parent)
{
    setPen(QPen(Qt::black, 40));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

CustomGraphicsPathItem::CustomGraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent)
    : QGraphicsPathItem(path, parent)
{
    setPen(QPen(Qt::black, 40));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

QPainterPath CustomGraphicsPathItem::getGlobalPath() const
{
    return mapToScene(path()); 
}

QList<QLineF> CustomGraphicsPathItem::getGlobalChildLine() const
{
    QList<QLineF> lines;
    for (const QLineF &line : child_line) {
        lines.append(QLineF(mapToScene(line.p1()), mapToScene(line.p2())));
    }
    return lines;
}

void CustomGraphicsPathItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsPathItem::mousePressEvent(event);
}

void CustomGraphicsPathItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (lastMousePos != pos() && m_undoStack) {
            m_undoStack->push(new MoveCommand(this, lastMousePos, pos()));
        }
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}



QVector<QLineF> CustomGraphicsPathItem::pathToLines(const QPainterPath &path) {
//    QVector<QLineF> lines;

//    QList<QPainterPath::Element> elements = path.elementList();
//    QPointF previousPoint = QPointF();

//    for (int i = 0; i < elements.size(); ++i) {
//        const QPainterPath::Element& element = elements[i];

//        if (element.type == QPainterPath::MoveToElement) {
//            if (i > 0) {
//                // 处理前一个线段
//                lines.append(QLineF(previousPoint, path.elementAt(i)()));
//            }
//            previousPoint = element();
//        } else if (element.type == QPainterPath::LineToElement) {
//            lines.append(QLineF(previousPoint, element()));
//            previousPoint = element.point;
//        } else if (element.type == QPainterPath::CurveToElement) {
//            // 处理曲线部分（如果需要）
//            // 曲线可以拆分为线段
//        } else  {
//             // 关闭子路径
//            path.closeSubpath();


//        }
//    }

//    return lines;
}

