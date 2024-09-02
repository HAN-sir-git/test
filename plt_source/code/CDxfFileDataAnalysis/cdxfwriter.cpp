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
        if (QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item)) {
            QLineF line = lineItem->line();
            DRW_Line l;
            l.basePoint = DRW_Coord(line.x1(), line.y1());
            l.secPoint = DRW_Coord(line.x2(), line.y2());
            writer.addLine(l);
        }
        // 处理矩形
        else if (QGraphicsRectItem* rectItem = dynamic_cast<QGraphicsRectItem*>(item)) {

        }
        // 处理椭圆（用近似的多边形表示）
        else if (QGraphicsEllipseItem* ellipseItem = dynamic_cast<QGraphicsEllipseItem*>(item)) {

        }
        // 处理多边形
        else if (QGraphicsPolygonItem* polygonItem = dynamic_cast<QGraphicsPolygonItem*>(item)) {
            QPolygonF polygon = polygonItem->polygon();
            QPointF prevPoint = polygon.first();
            DRW_LWPolyline polyline;
            for (int i = 0; i < polygon.size(); ++i) {
                QPointF point = polygon.at(i);
                polyline.addVertex(DRW_Vertex2D(point.x(), point.y(),0.));
            }
            polyline.addVertex(DRW_Vertex2D(prevPoint.x(), prevPoint.y(),0.));
            polyline.flags = 1;
            polyline.vertexnum = polyline.vertlist.size();
            writer.addLWPolyline(polyline);

        }
        // 处理路径
        else if (CustomGraphicsPathItem* pathItem = dynamic_cast<CustomGraphicsPathItem*>(item)) {

            CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(pathItem);
            auto lines = p->childLine();
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
                writer.addLWPolyline(polyline);


        }
    }
    bool success = dxf.write(&writer, DRW::Version::AC1027, false);
    return success;
}


