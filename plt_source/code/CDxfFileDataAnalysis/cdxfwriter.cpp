#include "cdxfwriter.h"
#include "CustomGraphicsItem/customgraphicsheader.h"
#include <QPainterPath>

CDxfWriter::CDxfWriter(QObject *parent)
{
    Q_UNUSED(parent);
}

CDxfWriter::~CDxfWriter()
{
}

bool CDxfWriter::writeDxfFile(const QString &filename,QList<QGraphicsItem*>  items)
{
    dxfRW dxf(filename.toStdString().c_str());
    FilterDXFRW writer(dxf);

    auto data = std::make_shared<DrawData>();
    // 这些都是输出时候的扩展容器
    writer.hsData = data;
    writer.currentBlock = std::make_shared<Block>();
    writer.currentBlock->setName("cs");
    for (QGraphicsItem* item : items) {
        // 处理线条
        if (CustomGraphicsLineItem* lineItem = dynamic_cast<CustomGraphicsLineItem*>(item)) {
            QLineF line = lineItem->getGlobalLine();
            DRW_Line l;
            l.basePoint = DRW_Coord(line.x1(), line.y1());
            l.secPoint = DRW_Coord(line.x2(), line.y2());

            // 颜色设置
            QColor color = lineItem->pen().color();
            if(color != QColor(Qt::black))
                l.color24 = color.red() << 16 | color.green() << 8 | color.blue();
            writer.addLine(l);
        }
        // 处理多边形
        else if (CustomGraphicsPolygonItem* polygonItem = dynamic_cast<CustomGraphicsPolygonItem*>(item)) {
            QPolygonF polygon = polygonItem->getGlobalPolygon();
            QPointF prevPoint = polygon.first();
            DRW_LWPolyline polyline;
            for (int i = 0; i < polygon.size(); ++i) {
                QPointF point = polygon.at(i);
                polyline.addVertex(DRW_Vertex2D(point.x(), point.y(),0.));
            }
            polyline.addVertex(DRW_Vertex2D(prevPoint.x(), prevPoint.y(),0.));
            polyline.flags = 1;
            polyline.vertexnum = polyline.vertlist.size();
            // 颜色设置
            QColor color = polygonItem->pen().color();
            if(color != QColor(Qt::black))
                polyline.color24 = color.red() << 16 | color.green() << 8 | color.blue();
            writer.addLWPolyline(polyline);

        }
        // 处理路径
        else if (CustomGraphicsPathItem* pathItem = dynamic_cast<CustomGraphicsPathItem*>(item)) {

            CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(pathItem);
            auto lines = p->getGlobalChildLine();
            DRW_LWPolyline polyline;
            if(lines.empty()) continue;
            // 多断线绘制
            polyline.addVertex(DRW_Vertex2D(lines[0].x1(), lines[0].y1(), 0.));
            for (const QLineF& line : lines) {
                polyline.addVertex(DRW_Vertex2D(line.x2(), line.y2(), 0.));

            }
            // 不闭合
             polyline.flags = 0;
             polyline.vertexnum = polyline.vertlist.size();

             // 颜色设置
             QColor color = pathItem->pen().color();
             if(color != QColor(Qt::black))
                polyline.color24  = color.blue() << 16 | color.green() << 8 | color.red();
             writer.addLWPolyline(polyline);
        }
    }
    bool success = dxf.write(&writer, DRW::Version::AC1027, false);
    return success;
}


