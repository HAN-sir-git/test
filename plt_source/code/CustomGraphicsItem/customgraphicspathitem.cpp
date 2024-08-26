#include "customgraphicspathitem.h"

#include <QPainterPath>

CustomGraphicsPathItem::CustomGraphicsPathItem(QGraphicsItem *parent): QGraphicsPathItem(parent)
{
}

CustomGraphicsPathItem::CustomGraphicsPathItem(const QPainterPath &path, QGraphicsItem *parent)
    : QGraphicsPathItem(path, parent)
{
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

