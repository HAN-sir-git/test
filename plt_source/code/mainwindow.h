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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "CommonDataForm/data.h"
#include "CPltFileDataAnalysis/pltfiledata.h"
#include "CustomGraphicsItem/customgraphicsheader.h"
#include "geometryAnalysis/cgeometryanalysis.h"
#include "view.h"
#include "CDxfFileDataAnalysis/cdxfwriter.h"
#include "ToolFunc/kdtree.hpp"

QT_BEGIN_NAMESPACE
class CGraphicsScene;
QT_END_NAMESPACE


struct RecognizedCutV
{
    int pre_A_angle = 90;
    int after_A_angle = 160;
    int pre_B_angle = 40;
    int after_B_angle = 80;
    int pre_C_angle = 90;
    int after_C_angle = 160;

};



class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void initConnect();
    // 重设置场景
    void resetSence();
    // 清理变量
    void clearVar();



protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

public slots:
    // 工具栏函数部分
    void openPltFile();
    // 合并相交的polyline
    void mergeIntersectedPolyline(PolyLinePtrList& polyLineList);

    // Greate KDtree
    void createEndPointKdTree(PolyLinePtrList polyLineList);

    // pltData 转换到 需要的数据结构
    void convertPltData(std::shared_ptr<ConvertData >& data);

    // 重叠图元Z轴调整
    void adjustZValueIfCovered( QGraphicsScene *scene);

    void dxfFileWrite(QList<QGraphicsItem *> items);

public:

    // 遍历data 转换其中图元数据结构为QGraphicsItem
    void populateSceneWithData(std::shared_ptr<ConvertData> data);

private:
    // 转换多种格式
    void ConvertPolyLine2Item(const PolyLinePtrList &polyLineList);

    void ConvertPolyLine2path(const PolyLinePtrList &polyLineList);
    // 识别单轮廓的V型剪口
    QList <QList <QLineF>> recognitionCutV( QGraphicsItem* item);
    // 识别单轮廓的I型剪口
    QList <QList <QLineF>> recognitionCutI( QGraphicsItem* item);

    // 增加ret外轮廓到场景
    void addOuterContourToScene(QMap<QGraphicsItem*, QGraphicsItemListPtr>& ret);

public:
    // 菜单栏函数部分 
    QMenu* createMenu();

public slots:
    // 识别外轮廓
    void populateScene();
    // 识别选中的图元的外轮廓
    void populateSelectedScene();
    // 清空外轮廓
    void clearOuterContour();
    // 导出所有图元到dxf文件
    void dxfFileWriteAllItems();
    // 导出选中的图元到dxf文件
    void dxfFileWriteSelectedItems();
    // 导出外轮廓到dxf文件
    void dxfFileWriteOuterContour();
    // 导出内轮廓到dxf文件
    void dxfFileWriteInnerContour();
    // 导出剪口和外轮廓到dxf文件
    void dxfFileWriteCutAndOuterContour();
    // 导出剪口和外轮廓和过切到dxf文件
    void dxfFileWriteCutAndOuterContourAndOverCut();

    // 识别V型剪口
    // 识别所有图元的V型剪口
    void recognitionCutAllV();
    // 识别选中的图元的V型剪口
    void recognitionCutSelectedV();
    // 识别所有图元的I型剪口
    void recognitionCutAllI();
    // 识别选中的图元的I型剪口
    void recognitionCutSelectedI();

    // 过切识别
    // 识别所有图元的过切
    void recognitionOverCutAll();
    // 识别选中的图元的过切
    void recognitionOverCutSelected();



public:

    QMenu* menu;
    CGeometryAnalysis* geometryParser;
    CGraphicsScene *scene;
    View *view;
    PltFileData *parser;
    CDxfWriter *dxfWriter;
    // kdtree struct
    Kdtree::KdNodeVector endpointNodes;
    Kdtree::KdTree* endpointTree = nullptr;

private:
    // 聚类结果
    QMap<QGraphicsItem*, QGraphicsItemListPtr> ret ;
    QList<QGraphicsItem*> outerContourList;  // 外轮廓
    // 剪口
    QList<QGraphicsItem*> cutList;

};

#endif // MAINWINDOW_H
