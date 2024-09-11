/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chip.h"
#include "mainwindow.h"
#include "cgraphicssence.h"
#include "tool.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QImage>
#include <QSize>
#include <qmath.h>
#include <QRGB>
#include <QAction>
#include <QGraphicsItem>
#include <QToolButton>
#include <QVector2D>
#include <QVector>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    parser  = new PltFileData();
    scene = new CGraphicsScene(this);
    dxfWriter = new CDxfWriter;

    QAction *undoAction = scene->getUndoStack()->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcut(QKeySequence::Undo);  // Ctrl+Z
    addAction(undoAction);

    QAction *redoAction = scene->getUndoStack()->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcut(QKeySequence::Redo);  // Ctrl+Y
    addAction(redoAction);

    view = new View("view");
    view->view()->setScene(scene);

    geometryParser = new CGeometryAnalysis(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle(tr("Plt Example"));
    initConnect();
//    dxfWriter->writeDxfFile("XXX",scene->items());

}

void MainWindow::initConnect()
{
    connect(view->openFileButton, &QToolButton::clicked, this, &MainWindow::openPltFile);
    //connect(view->exportImageButton, &QToolButton::clicked, this, &MainWindow::saveSceneExample);
    connect(view->outlineRecognitionButton, &QToolButton::clicked, this, &MainWindow::populateScene);
    connect(view->cutRecognitionButtonV, &QToolButton::clicked, this, &MainWindow::recognitionCutAllV);
    connect(view->cutRecognitionButtonI, &QToolButton::clicked, this, &MainWindow::recognitionCutAllI);
}



void MainWindow::populateScene()
{

    auto items = scene->items();

    if(items.empty()) return;

    QList<QGraphicsItem *> filter;
    geometryParser->sortItemsByBoundingRectAreaDescending(items,geometryParser->m_Area2ItemMap,filter,parser->getBox());
    for(auto x :filter)
    {
        items.removeAll(x);
        scene->removeItem(x);
    }

    QMap<QGraphicsItem*, QGraphicsItemListPtr> ret =geometryParser->intersectItemsLoopCluster(items,scene,filter);

    QMap<QGraphicsItem*, QList<QLineF>> item2Lines = geometryParser->convertToLineMap(ret);


    // 显示聚类时候的相交路径判断

//    for(auto pl: geometryParser->paths)
//    {
//        QPen rrr(  Qt::red);
//        auto item = new CustomGraphicsPathItem(pl);
//        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
//        p->setUndoStack(scene->getUndoStack());
//        item->setPen(rrr);
//        scene->addItem(item);
//    }

//    for (auto pol: geometryParser->polygons)
//    {

//        pol.pop_back();
//        QPainterPath path;
//        if(!pol.empty()) path.moveTo(pol.takeFirst());
//        for(auto p: pol)
//        {
//            path.lineTo(p);
//        }

//        QPen rrr(  Qt::red);
//        auto item = new CustomGraphicsPathItem(path);
//        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
//        p->setUndoStack(scene->getUndoStack());
//        item->setPen(rrr);
//        scene->addItem(item);
//    }


     //显示聚类后的分类边框
//    for(int i = 0 ;i < ret.keys().size();i++)
//    {
//        QPen rrr( Qt::red);
//        auto key = ret.keys().at(i);
//        QRectF r;
//        for(auto item: ret[key])
//        {
//            //转换item的类型，根据类型绘制不同的图元
//            int itemType = item->type();


//            if (itemType == QGraphicsPolygonItem::Type) {
//                QGraphicsPolygonItem *polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
//               // polygonItem->setPen(rrr);
//                r = r.united(polygonItem->sceneBoundingRect());
//            } else if (itemType == QGraphicsLineItem::Type) {
//                QGraphicsLineItem *lineItem = qgraphicsitem_cast<QGraphicsLineItem *>(item);
//               // lineItem->setPen(rrr);
//                r = r.united(lineItem->sceneBoundingRect());
//            } else if (itemType == QGraphicsPathItem::Type) {
//                QGraphicsPathItem *pathItem = qgraphicsitem_cast<QGraphicsPathItem *>(item);
//                //pathItem->setPen(rrr);
//                r = r.united(pathItem->sceneBoundingRect());
//            } else {

//            }
//        }
//        // 绘制边框
//         QPolygonF polygon;
//          polygon << r.topLeft()    // 左上角
//                  << r.topRight()   // 右上角
//                  << r.bottomRight()// 右下角
//                  << r.bottomLeft();// 左下角
//        auto item = new CustomGraphicsPolygonItem(polygon);
//        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
//        p->setUndoStack(scene->getUndoStack());
//        item->setPen(rrr);
//        scene->addItem(item);
//    }

     //绘制聚类后一组集合的轮廓

    for(auto itemkey: item2Lines.keys())
    {
        QMap<QPointF, QSet<QPointF> > neighborhood;
        auto breaklines = geometryParser->breakLinesByIntersections(item2Lines[itemkey],neighborhood);
        auto startp = geometryParser->findBottomLeftPoint(neighborhood.keys());
        auto contourPoints = geometryParser->findContour(startp,neighborhood);

        if(contourPoints.empty()) continue;

        QPainterPath path(contourPoints[0]/10);
        QList<QLineF> ls;
        for(int i = 1; i <  contourPoints.size();i++)
        {
            path.lineTo(contourPoints[i]/10);
            ls.append(QLineF(contourPoints[i-1],contourPoints[i]));
        }
        auto item = new CustomGraphicsPathItem(path);
        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
        p->setUndoStack(scene->getUndoStack());
        p->setChildLine(ls);
        QPen rrr( Qt::red,4);
        item->setPen(rrr);
        scene->addItem(item);
    }
    QList<QGraphicsItem*> outitems = scene->items();
    dxfFileWrite(outitems);

}


void MainWindow::recognitionCutV( QGraphicsItem *item)
{
    QList <QList <QLineF>> cutVs;
    RecognizedCutV v;
    // 获取 item的所有线
    auto  p = dynamic_cast<CustomGraphicsInterface*>(item);
    if(p == nullptr) return;
    auto lines = p->childLine();

    // lines 是有序的 判断连续的两条线a、b是否满足 长度和夹角限制, a线和a线前一条线的夹角大于90度，小于160度
    for (int i= 0; i < lines.size()-1;i++)
    {
        auto line1 = lines[ (i-1 + lines.size()) % lines.size()];
        auto line2 = lines[i];
        auto line3 = lines[(i+1) % lines.size()];
        auto line4 = lines[(i+2) % lines.size()];

        
        QPointF p12;
        line1.intersects(line2,&p12);
        // 作p12到另外两个点的向量 先判断p12 是哪个点
        QVector2D v1 = QVector2D( p12 == line2.p1() ? line2.p2() : line2.p1() - p12);
        QVector2D v2 = QVector2D( p12 == line3.p1() ? line3.p2() : line3.p1() - p12);

        // 计算夹角
        double angleA = Tool::angleBetweenVectors(v1,v2);

        QPointF p23;
        line2.intersects(line3,&p23);
        // 作p23到另外两个点的向量 先判断p23 是哪个点
        v1 = QVector2D( p23 == line3.p1() ? line3.p2() : line3.p1() - p23);
        v2 = QVector2D( p23 == line4.p1() ? line4.p2() : line4.p1() - p23);

        // 计算夹角
        double angleB = Tool::angleBetweenVectors(v1,v2);

        QpointF p34;
        line3.intersects(line4,&p34);
        // 作p34到另外两个点的向量 先判断p34 是哪个点
        v1 = QVector2D( p34 == line4.p1() ? line4.p2() : line4.p1() - p34);
        v2 = QVector2D( p34 == line1.p1() ? line1.p2() : line1.p1() - p34);

        // 计算夹角
        double angleC = Tool::angleBetweenVectors(v1,v2);

        if(angleA >= v.pre_A_angle && angleA <= v.after_A_angle 
            && angleB >= v.pre_B_angle && angleB <= v.after_B_angle 
            && angleC >= v.pre_C_angle && angleC <= v.after_C_angle)
        {
            // 保存line2和line3 作为V剪口
            cutVs.append({line2,line3});
        }  
    }

}
void MainWindow::adjustZValueIfCovered(QGraphicsScene *scene) {

    auto items =  scene->items();
    QSet<QGraphicsItem*> filter;
    for(auto item: items)
    {
        if(filter.contains(item))
            continue;
        // 获取和该图元重叠的其他图元
        QList<QGraphicsItem*> collidingItems = scene->items(item->sceneBoundingRect(),Qt::ContainsItemShape);

        if (!collidingItems.isEmpty()) {
            // 如果有其他图元与当前图元重叠
            for (QGraphicsItem* collidingItem : collidingItems) {
                    // 如果重叠的图元 Z 值高于当前图元，则调整当前图元的 Z 值
                filter.insert(collidingItem);
                collidingItem->setZValue(item->zValue() + 1);
            }
        }
    }

}

void MainWindow::dxfFileWrite(QList<QGraphicsItem *> items)
{
    // 获取文件保存路径
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "untitled.dxf",
                                                    tr("DXF Files (*.dxf)"));
    if(fileName.isEmpty())
    {
        return;
    }

    // 保存文件
    dxfWriter->writeDxfFile(fileName, items);

}

void MainWindow::recognitionCutAllV()
{
    auto items = scene->selectedItems();
    for(auto item: items)
    {
        recognitionCutV(item);
    }
}

void MainWindow::recognitionCutAllI()
{
}

void MainWindow::mergeIntersectedPolyline(PolyLinePtrList& polyLineList)
{
    PolyLinePtrList ret;
    // plt文件中的一定是point 类型
    QList<int> endPointPolIndex;
    // 1. 遍历polylineList,找到相交的polyline,存储不闭合的polyline的首位端点
    for (int i = 0; i < polyLineList.size() ;i++)
    {
        if(polyLineList[i]->getClosed())
        {
            ret.append(polyLineList[i]);
        }else {

            endPointPolIndex.append(i);
        }

    }

    // 2. 找到首尾相接的polyline，记录使用过的polyline
    QList<int> usedIndex;
    for(auto index: endPointPolIndex)
    {
        if(usedIndex.contains(index))
        {
            continue;
        }
        auto  pl = polyLineList[index];
        auto ps = pl->getPoints();
        if(ps.empty())
            continue;

        QList<Point> retPoints =ps;
        QVector<Point> points = {ps.first(),ps.last()};

        while(!points.isEmpty())
        {
            auto point1 = points.takeFirst().getPointF();
            std::vector<double> coord_point(2);
            Kdtree::KdNodeVector result;
            coord_point[0] = point1.x();
            coord_point[1] = point1.y();
            endpointTree->k_nearest_neighbors(coord_point, 2, &result);

                for(auto res: result)
                {
                    QPointF p =  QPointF(res.point[0], res.point[1]);
                    // 计算 p和point的长度
                    if(QVector2D(p-point1).length() < 2)
                    {
                        // 首尾相接
                        if(index != res.index && !usedIndex.contains(res.index))
                        {
                            auto polylinePtr =  polyLineList[res.index];
                            if(p == polylinePtr->getPoints().first().getPointF())
                            {
                                if(retPoints.first().getPointF() == point1)
                                {
                                    // 11
                                    auto tmppoint = polylinePtr->getPoints();
                                    points.append(tmppoint.last());
                                    std::reverse(tmppoint.begin(),tmppoint.end());
                                    tmppoint.append(retPoints);
                                    retPoints = tmppoint;

                                }else {
                                    //12
                                    retPoints.append(polylinePtr->getPoints());
                                    points.append(polylinePtr->getPoints().last());
                                }

                            }else{
                                // 尾
                                if(retPoints.first().getPointF() == point1)
                                {
                                    // 11
                                    auto tmppoint = polylinePtr->getPoints();
                                    points.append(tmppoint.first());
                                    tmppoint.append(retPoints);
                                    retPoints = tmppoint;

                                }else {
                                    //12
                                    auto tmppoint = polylinePtr->getPoints();
                                    points.append(tmppoint.first());
                                    std::reverse(tmppoint.begin(),tmppoint.end());
                                    retPoints.append(tmppoint);

                                }

                            }
                            usedIndex.append(res.index);
                        }
                    }



            }
        }

        bool bclose = false;
        if(QVector2D(retPoints.last().getPointF() - retPoints.first().getPointF()).length() < 2)
        {
            if(retPoints.last().getPointF() != retPoints.first().getPointF())
            {
                retPoints.append(retPoints.first());
            }
            bclose = true;
        }

        auto pline = std::make_shared<PolyLine>(retPoints, bclose);
        pline->appendVertexs(retPoints);
        ret.append(pline);
    }
    polyLineList = ret;
}

void MainWindow::createEndPointKdTree(PolyLinePtrList polyLineList)
{
   for (int i = 0;i < polyLineList.size();i++)
   {
       auto& pl = polyLineList[i];
       if(pl->getClosed())continue;
       if(pl->getPoints().size() < 2) continue;

       auto points = pl->getPoints();
       std::vector<double> point(2);
       // start
       point[0] = points.front().x();
       point[1] = points.front().y();
       Kdtree::KdNode node1(point,nullptr,i);
       endpointNodes.push_back(node1);
       // end
       point[0] = points.back().x();
       point[1] = points.back().y();
       Kdtree::KdNode node2(point,nullptr,i);
       endpointNodes.push_back(node2);
   }
   endpointTree = new Kdtree::KdTree(&endpointNodes);
}

void MainWindow::convertPltData(std::shared_ptr<ConvertData>& data)
{
    createEndPointKdTree(data->polyline_list);
   // 合并端点相交polyline
    mergeIntersectedPolyline(data->polyline_list);
}

void  MainWindow::openPltFile()
{
    scene->clear();
    if(parser->ParsePltFile())
    {
       auto data = parser->getPltData();

       // 增加对data的处理环节
       convertPltData(data);
       populateSceneWithData(data);
    }
    view->view()->setSceneRect(scene->sceneRect());
    view->view()->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

}

void MainWindow::populateSceneWithData(std::shared_ptr<ConvertData> data)
{
   ConvertPolyLine2Item(data->polyline_list);
    //ConvertPolyLine2path(data->polyline_list);

}

void MainWindow::ConvertPolyLine2Item(const PolyLinePtrList &polyLineList)
{
    for(auto pl : polyLineList)
    {
        QGraphicsItem *item = nullptr;
        auto points = pl->getQPointFs();
        if(points.size() < 2)
        {
            continue;
        }
        if(pl->getClosed())
        {
            item = new CustomGraphicsPolygonItem(points);

        }
        else 
        {
            if(points.size() == 2)
            {
                item = new CustomGraphicsLineItem(QLineF(points[0],points[1]));
            }else
            {
                QPainterPath path(points[0]);
                for(int i = 1; i <  points.size();i++)
                {
                    path.lineTo(points[i]);
                }
                item = new CustomGraphicsPathItem(path);

            }
        }

        for (auto child : pl->getChilds())
        {
           //根据child的类型，构造不同的图元
            switch (child->rtti())
            {
                case COMMON_TYPE::LINE:
                {
                    auto line = std::dynamic_pointer_cast<Line>(child);

                    // B方案 直接追加QLineF类型到item,指针类型强转公共接口基类
                    CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
                    p->appendChildLine(line->getLineF());   
                }
                break;
            }
        }
        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
        p->setUndoStack(scene->getUndoStack());
        scene->addItem(item);
    }

}

void MainWindow::ConvertPolyLine2path(const PolyLinePtrList &polyLineList)
{
    for(auto pl : polyLineList)
    {
        QGraphicsItem *item = nullptr;
        auto points = pl->getQPointFs();

        QPainterPath path(points[0]);
        for(int i = 1; i <  points.size();i++)
        {
            path.lineTo(points[i]);
        }
        item = new CustomGraphicsPathItem(path);
        scene->addItem(item);

    }

}



