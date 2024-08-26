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
#include "view.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QImage>
#include <QSize>
#include <qmath.h>
#include <QRGB>
#include <QGraphicsItem>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    parser  = new PltFileData();
    scene = new QGraphicsScene(this);

    View *view = new View("view");
    view->view()->setScene(scene);

    geometryParser = new CGeometryAnalysis(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle(tr("Plt Example"));

    populateScene();

//    view->view()->setSceneRect(scene->sceneRect());
//    view->view()->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

   // saveSceneExample(scene,this);

    //exportSceneExample(scene,this);

    //opencvtest1("D:/Plt_code/test/plt_source/image/ww.png_1_0.png");

}

void MainWindow::populateScene()
{
    openPltFile();
    auto items = scene->items();
    QList<QGraphicsItem *> filter;
    geometryParser->sortItemsByBoundingRectAreaDescending(items,geometryParser->m_Area2ItemMap,filter);
    for(auto x :filter)
    {
        items.removeAll(x);
        scene->removeItem(x);
    }

    QMap<QGraphicsItem*, QGraphicsItemListPtr> ret =geometryParser->intersectItemsLoopCluster(items,scene,filter);

    QMap<QGraphicsItem*, QList<QLineF>> item2Lines = geometryParser->convertToLineMap(ret);

    for(auto itemkey: item2Lines.keys())
    {
        QMap<QPointF, QSet<QPointF> > neighborhood;
        auto breaklines = geometryParser->breakLinesByIntersections(item2Lines[itemkey],neighborhood);
        auto startp = geometryParser->findBottomLeftPoint(neighborhood.keys());
        auto contourPoints = geometryParser->findContour(startp,neighborhood);

        if(contourPoints.empty()) continue;
        QPainterPath path(contourPoints[0]/10);
        for(int i = 1; i <  contourPoints.size();i++)
        {
            path.lineTo(contourPoints[i]/10);
        }
        auto item = new CustomGraphicsPathItem(path);
        QPen rrr( Qt::red);
        item->setPen(rrr);
        scene->addItem(item);

//        for(int i = 0; i <  breaklines.size();i++)
//        {
//            auto item = new CustomGraphicsLineItem(QLineF(breaklines[i].p1()/10,breaklines[i].p2()/10));
//            QPen rrr( Qt::red);
//            item->setPen(rrr);
//            scene->addItem(item);
//        }



//        break;

    }


//    for(int i = 0 ;i < ret.keys().size();i++)
//    {
//        QPen rrr( i == 0 ? Qt::red:Qt::green);
//        auto key = ret.keys().at(i);

//        // 绘制边框
//        auto rectitem = new QGraphicsRectItem(key->boundingRect());

//        rectitem->setPen(rrr);
//        scene->addItem(rectitem);


//        for(auto item: ret[key])
//        {
//           //转换item的类型，根据类型绘制不同的图元
//            int itemType = item->type();


//            if (itemType == QGraphicsPolygonItem::Type) {
//                QGraphicsPolygonItem *polygonItem = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
//                polygonItem->setPen(rrr);
//            } else if (itemType == QGraphicsLineItem::Type) {
//                QGraphicsLineItem *lineItem = qgraphicsitem_cast<QGraphicsLineItem *>(item);
//                lineItem->setPen(rrr);
//            } else if (itemType == QGraphicsPathItem::Type) {
//                QGraphicsPathItem *pathItem = qgraphicsitem_cast<QGraphicsPathItem *>(item);
//                pathItem->setPen(rrr);
//            } else {
                
//            }
//        }
//    }
}

void  MainWindow::openPltFile()
{
    if(parser->ParsePltFile())
    {
        auto data = parser->getPltData();
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
                    //A方案 追加作为孩子项，但是不绘制，会影响图元的获取问题
                    //CustomGraphicsLineItem *item1 = new CustomGraphicsLineItem(line->getLineF());
                    //item1->setParentItem(item);

                    // B方案 直接追加QLineF类型到item,指针类型强转公共接口基类
                    CustomGraphicsInterface *p = dynamic_cast<CustomGraphicsInterface*>(item);
                    p->appendChildLine(line->getLineF());   
                }
                break;
            }
        }
        scene->addItem(item);
        // QPen www(Qt::white);
        // //item->setPen(www);
        // if(!pl->getClosed())
        // {
        //     QPen rrr(Qt::red);
        //     QPen ggg(Qt::green);
        //     item->setPen(0 == 0 ? rrr:ggg);
        // }
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



