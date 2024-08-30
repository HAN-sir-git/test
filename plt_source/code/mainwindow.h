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

QT_BEGIN_NAMESPACE
class CGraphicsScene;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:
    void setupMatrix();
    void populateScene();


public:
    // 工具栏函数部分
    void openPltFile();
    // 遍历data 转换其中图元数据结构为QGraphicsItem
    void populateSceneWithData(std::shared_ptr<ConvertData> data);

    void saveSceneToImage(QGraphicsScene *scene, const QString &filePath);

    // 直接导出jpg或png
    void saveSceneExample(QGraphicsScene *scene, QWidget *parentWidget);

    // 分块导出后合并
    void exportSceneInChunks(QGraphicsScene *scene, const QString &baseFilePath, const QSize &chunkSize);

    // 示例用法
    void exportSceneExample(QGraphicsScene *scene, QWidget *parentWidget);

private:
    // 转换多种格式
    void ConvertPolyLine2Item(const PolyLinePtrList &polyLineList);

    void ConvertPolyLine2path(const PolyLinePtrList &polyLineList);


public:

    //opencv做轮廓检测
   // void opencvtest1(const QString& path);
    CGeometryAnalysis* geometryParser;
    CGraphicsScene *scene;
    PltFileData *parser;
};

#endif // MAINWINDOW_H
