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


    view->view()->setSceneRect(scene->sceneRect());
    view->view()->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

}

void MainWindow::initConnect()
{
    connect(view->openFileButton, &QToolButton::clicked, this, &MainWindow::openPltFile);
    //connect(view->exportImageButton, &QToolButton::clicked, this, &MainWindow::saveSceneExample);
    connect(view->outlineRecognitionButton, &QToolButton::clicked, this, &MainWindow::populateScene);
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

    for (auto pol: geometryParser->polygons)
    {

        pol.pop_back();
        QPainterPath path;
        if(!pol.empty()) path.moveTo(pol.takeFirst());
        for(auto p: pol)
        {
            path.lineTo(p);
        }

        QPen rrr(  Qt::red);
        auto item = new CustomGraphicsPathItem(path);
        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
        p->setUndoStack(scene->getUndoStack());
        item->setPen(rrr);
        scene->addItem(item);
    }


     //显示聚类后的分类边框
//    for(int i = 0 ;i < ret.keys().size();i++)
//    {
//        QPen rrr( i%2 == 0 ? Qt::red : Qt::green);
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
//        QPolygonF polygon;
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

//    for(auto itemkey: item2Lines.keys())
//    {
//        QMap<QPointF, QSet<QPointF> > neighborhood;
//        auto breaklines = geometryParser->breakLinesByIntersections(item2Lines[itemkey],neighborhood);
//        auto startp = geometryParser->findBottomLeftPoint(neighborhood.keys());
//        auto contourPoints = geometryParser->findContour(startp,neighborhood);

//        if(contourPoints.empty()) continue;

//        QPainterPath path(contourPoints[0]/10);
//        QList<QLineF> ls;
//        for(int i = 1; i <  contourPoints.size();i++)
//        {
//            path.lineTo(contourPoints[i]/10);
//            ls.append(QLineF(contourPoints[i-1],contourPoints[i]));
//        }
//        auto item = new CustomGraphicsPathItem(path);
//        CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
//        p->setUndoStack(scene->getUndoStack());
//        p->setChildLine(ls);
//        QPen rrr( Qt::red);
//        item->setPen(rrr);
//        scene->addItem(item);
//    }
    QList<QGraphicsItem*> outitems = scene->items();
    dxfFileWrite(outitems);

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


void MainWindow::mergeIntersectedPolyline(PolyLinePtrList& polyLineList)
{
    PolyLinePtrList ret;
    // plt文件中的一定是point 类型
     QList<QList<Point>> pointList;
    // 1. 遍历polylineList,找到相交的polyline,存储不闭合的polyline的首位端点
    for (auto pl : polyLineList)
    {
        if(pl->getClosed())
        {
            ret.append(pl);
            continue;
        }
        auto points = pl->getPoints();
        if(points.size() < 2)
        {
            continue;
        }
        pointList.append(points);
    }

    // 2. 找到首尾相接的polyline，记录使用过的polyline
    QList<int> usedIndex;
    for(int i = 0; i < pointList.size();i++)
    {
        if(usedIndex.contains(i))
        {
            continue;
        }
        QList<Point> pl = pointList[i];
        for(int j = 0; j < pointList.size();j++)
        {
            if(i == j || usedIndex.contains(j))
            {
                continue;
            }
            QList<Point> pl2 = pointList[j];

            if(pl.first().data.point == pl2.last().data.point)
            {
                pl2.pop_back();
                pl2.append(pl);
                usedIndex.append(j);
            }
            else if(pl.first().data.point == pl2.first().data.point)
            {
                pl.pop_front();
                reverse(pl.begin(),pl.end());
                pl2.append(pl);
                usedIndex.append(j);
            }else if(pl.last().data.point == pl2.first().data.point)
            {
                pl.pop_back();
                pl.append(pl2);
                usedIndex.append(j);
            }else if(pl.last().data.point == pl2.last().data.point)
            {
                pl.pop_back();
                reverse(pl2.begin(),pl2.end());
                pl.append(pl2);
                usedIndex.append(j);
            }
        }
        // 判断pl 是否闭合,如果闭合，直接添加到ret，如果两点的距离小于阈值2，认为闭合

        if(QVector2D(pl.front().data.point-pl.back().data.point).length() < 2)
        {
            if(pl.front().data.point != pl.back().data.point)
                pl.append(pl.front());
            auto pline = std::make_shared<PolyLine>(pl,true);
            auto pts = pl;
            pline->appendVertexs(pts);
            ret.append(pline);
        }else
        {
            auto pline = std::make_shared<PolyLine>(pl,false);
            pline->appendVertexs(pl);
            ret.append(pline);
        }
    }
    polyLineList = ret;
}

void MainWindow::createEndPointKdTree(PolyLinePtrList polyLineList)
{
    QList<QList<Point>> pointList;
   // 1. 遍历polylineList,找到相交的polyline,存储不闭合的polyline的首位端点
   Kdtree::KdNodeVector nodes;
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
       Kdtree::KdNode node(point,nullptr,i);
       nodes.push_back(node);
       // end
       point[0] = points.back().x();
       point[1] = points.back().y();
       Kdtree::KdNode node(point,nullptr,i);
       nodes.push_back(node);
   }
   endpointKdtree = Kdtree::KdTree tree(&nodes);

   // 2. 找到首尾相接的polyline，记录使用过的polyline
   QList<int> usedIndex;
   for(int i = 0; i < pointList.size();i++)
   {
       if(usedIndex.contains(i))
       {
           continue;
       }
       QList<Point> pl = pointList[i];
       for(int j = 0; j < pointList.size();j++)
       {
           if(i == j || usedIndex.contains(j))
           {
               continue;
           }
           QList<Point> pl2 = pointList[j];

           if(pl.first().data.point == pl2.last().data.point)
           {
               pl2.pop_back();
               pl2.append(pl);
               usedIndex.append(j);
           }
           else if(pl.first().data.point == pl2.first().data.point)
           {
               pl.pop_front();
               reverse(pl.begin(),pl.end());
               pl2.append(pl);
               usedIndex.append(j);
           }else if(pl.last().data.point == pl2.first().data.point)
           {
               pl.pop_back();
               pl.append(pl2);
               usedIndex.append(j);
           }else if(pl.last().data.point == pl2.last().data.point)
           {
               pl.pop_back();
               reverse(pl2.begin(),pl2.end());
               pl.append(pl2);
               usedIndex.append(j);
           }
       }
       // 判断pl 是否闭合,如果闭合，直接添加到ret，如果两点的距离小于阈值2，认为闭合

       if(QVector2D(pl.front().data.point-pl.back().data.point).length() < 2)
       {
           if(pl.front().data.point != pl.back().data.point)
               pl.append(pl.front());
           auto pline = std::make_shared<PolyLine>(pl,true);
           auto pts = pl;
           pline->appendVertexs(pts);
           ret.append(pline);
       }else
       {
           auto pline = std::make_shared<PolyLine>(pl,false);
           pline->appendVertexs(pl);
           ret.append(pline);
       }
   }
   polyLineList = ret;
}

void MainWindow::convertPltData(std::shared_ptr<ConvertData>& data)
{
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



