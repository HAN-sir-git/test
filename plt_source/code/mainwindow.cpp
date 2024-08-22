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


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    parser  = new PltFileData();
    scene = new QGraphicsScene(this);

    View *view = new View("view");
    view->view()->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle(tr("Plt Example"));
        showMaximized();
   // scene->setBackgroundBrush(Qt::black);
    populateScene();
    // 设置初始视口区域

//    view->view()->setSceneRect(scene->sceneRect());
//    view->view()->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    saveSceneExample(scene,this);

    //exportSceneExample(scene,this);

    //opencvtest1("D:/Plt_code/test/plt_source/image/ww.png_1_0.png");

}

void MainWindow::populateScene()
{
    openPltFile();
}

void  MainWindow::openPltFile()
{
    static int cs = 0;
    bool ret = parser->ParsePltFile();
    if(ret)
    {
        auto data = parser->getPltData();

        for(auto pl : data->polyline_list)
        {
            auto pointfs = pl->getQPointFs();
            QPainterPath path(pointfs[0]);
            for(int i = 1; i <  pointfs.size();i++)
            {
                path.lineTo(pointfs[i]);
            }

            QAbstractGraphicsShapeItem *item = new QGraphicsPathItem(path);
            QPen www(Qt::white);
            //item->setPen(www);
//            if(!pl->getClosed())
//            {
//                QPen rrr(Qt::red);
//                QPen ggg(Qt::green);
//                item->setPen(cs%2 == 0 ? rrr:ggg);
//                cs++;
//            }
            scene->addItem(item);
        }
    }

}

void MainWindow::saveSceneToImage(QGraphicsScene *scene, const QString &filePath) {
    // 创建一个QImage对象，大小与场景相同
    QRectF sceneRect = scene->sceneRect();
    QSize imageSize(sceneRect.width(), sceneRect.height());
    QImage* image = new QImage(imageSize, QImage::Format_Grayscale8);
    image->fill(Qt::white); // 可选：设置背景色

    // 创建一个QPainter对象用于绘制
    QPainter painter(image);

    // 将QGraphicsScene内容渲染到QImage
    scene->render(&painter);

    // 保存图片到文件
    image->save(filePath);
}

void MainWindow::saveSceneExample(QGraphicsScene *scene, QWidget *parentWidget) {
    QString filePath = QFileDialog::getSaveFileName(parentWidget, "Save Image", "", "Images (*.jpg)");
    if (!filePath.isEmpty()) {
        saveSceneToImage(scene, filePath);
    }
}

void MainWindow::exportSceneInChunks(QGraphicsScene *scene, const QString &baseFilePath, const QSize &chunkSize) {
    QRectF sceneRect = scene->sceneRect();
    int chunksX = qCeil(sceneRect.width() / chunkSize.width());
    int chunksY = qCeil(sceneRect.height() / chunkSize.height());
    QRect sschunkRect(
        0,
        0,
        chunkSize.width(),
        chunkSize.height()
        );


    for (int x = 0; x < chunksX; ++x) {
        for (int y = 0; y < chunksY; ++y) {
            QRect chunkRect(
                x * chunkSize.width(),
                y * chunkSize.height(),
                chunkSize.width(),
                chunkSize.height()
                );

            // 创建QImage对象
            QSize imageSize(chunkSize);

            QImage* image = new QImage(imageSize, QImage::Format_Grayscale8);
            image->fill(Qt::white); // 可选：设置背景色

            // 创建一个QPainter对象用于绘制
            QPainter painter(image);


            // 渲染场景的一部分到QImage
            QGraphicsView view(scene);
            view.setSceneRect(chunkRect);
            view.render(&painter, sschunkRect, chunkRect);

            // 保存每个分块的图像
            QString filePath = baseFilePath + QString("_%1_%2.png").arg(x).arg(y);
            image->save(filePath);

        }
    }
}

void MainWindow::exportSceneExample(QGraphicsScene *scene, QWidget *parentWidget) {
    QString baseFilePath = QFileDialog::getSaveFileName(parentWidget, "Save Image Base Path", "", "Images (*.png)");
    if (baseFilePath.isEmpty()) return;

    QSize chunkSize(5000, 5000); // 定义每个分块的大小

    exportSceneInChunks(scene, baseFilePath, chunkSize);
}


