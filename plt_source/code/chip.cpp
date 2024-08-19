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

#include <QtWidgets>

Chip::Chip(const QColor &color, int x, int y)
{
    this->x = x;
    this->y = y;
    this->color = color;
    // 设置了图形项的 Z 值，用于确定图形项在堆叠顺序中的位置,X+Y取余，这样相邻项永远Z轴不一致，
    // (x + y) % 2 的计算结果将根据 x 和 y 的值确定图形项的 Z 值。这可能是为了使芯片对象在棋盘或网格上有不同的堆叠顺序
    // 当多个图形项具有相同的 Z 值时，它们可能会在绘制时发生重叠，这会导致视觉上的混乱。
    // 通过使用 (x + y) % 2 这样的计算方式，可以尽量减少相邻图形项之间的 Z 值冲突，减少重叠问题的发生。
    setZValue((x + y) % 2);
    //表示图形项可以被选择.图形项可以被移动。
    setFlags(ItemIsSelectable | ItemIsMovable);
    //启用了图形项的鼠标悬停事件。如果用户将鼠标悬停在此图形项上，它可以捕获并响应鼠标悬停事件。
    setAcceptHoverEvents(true);
}
/*boundingRect 和 shape 都用于绘图和碰撞检测的参考区域，但它们在不同的上下文和目的下使用：
boundingRect 用于指定图形项的边界矩形，它定义了图形项的外包围矩形，通常是一个矩形区域。
主要用途是在图形项被绘制时，提供一个用于确定图形项的绘制区域的参考框架。例如，当绘制一个 QGraphicsItem 子类的对象时，
可以使用 boundingRect 来确定要绘制的区域，确保不会浪费时间和资源在边界之外的区域上。
在碰撞检测方面，boundingRect 通常用于粗略的碰撞检测，例如确定两个图形项的边界矩形是否相交，以快速排除不可能碰撞的情况。
shape：

shape 用于定义图形项的形状，这不一定是一个矩形。形状可以是任何复杂的几何形状，包括不规则形状。
主要用途是在精确的碰撞检测和鼠标事件处理等情况下，提供一个更精确的图形项形状的参考。
例如，如果您希望某个图形项只在其实际可见部分内响应鼠标事件，而忽略边界矩形之外的部分，那么您可以使用 shape 来定义该形状。
在碰撞检测方面，shape 用于更准确地检测两个图形项之间的碰撞，包括复杂形状的相交情况。
通常情况下，如果您只需要进行粗略的绘图或碰撞检测，可以使用 boundingRect，因为它比 shape 的计算更快速。
但如果您需要更精确的形状表示和碰撞检测，那么可以使用 shape 来定义更复杂的形状。
*/
QRectF Chip::boundingRect() const
{
    return QRectF(0, 0, 110, 70);
}

QPainterPath Chip::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}

void Chip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    // 如果项被选中，那么 fillColor 将设置为 color 的深色版本（通过 .dark(150) 方法），深色版本是原颜色的较暗变化。
    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    // 如果项处于鼠标悬停状态，它将当前的 fillColor 变量更改为更亮的颜色版本（通过 .light(125) 方法），亮色版本是原颜色的较亮变化。
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    // levelOfDetailFromTransform根据传递给它的绘图变换（painter->worldTransform()）来计算级别细节水平（LOD）
    // worldTransform() 是一个函数或方法，用于获取与绘图相关的世界变换矩阵。
    // 这个矩阵通常包含了缩放、旋转和平移等变换信息，用于确定图形的最终渲染效果。

    /*
     * 通常表现为一个浮点数，范围在 0 到 1 之间，其中 0 表示最低细节，1 表示最高细节。
    LOD 是一个用于优化绘制的概念，通常用于处理在不同缩放级别下如何绘制图形的问题。
    通过根据当前的绘图变换计算 LOD，可以决定是否以高分辨率或低分辨率来渲染图形，以提高性能
    并在不同缩放级别下获得合适的绘制效果。这种技术常用于处理大规模地图、图像浏览器和其他需要动态调整绘制精度的应用程序。

    当放大视图时，LOD 值通常会变大，因为用户需要更多的细节。
    反之，当缩小视图时，LOD 值通常会变小，因为用户关心的细节较少。
    通过调整 LOD 值，可以在不同缩放级别下优化图形的绘制，以提高性能和用户体验。
    */
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.2) {
        if (lod < 0.125) {
            // 全绘制，填充缝隙
            painter->fillRect(QRectF(0, 0, 110, 70), fillColor);
            return;
        }
        // 部分绘制，凸显
        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(13, 13, 97, 57);
        painter->setBrush(b);
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawRect(QRect(14, 14, 79, 39));
    painter->setBrush(b);

    if (lod >= 1) {
        painter->setPen(QPen(Qt::gray, 1));
        painter->drawLine(15, 54, 94, 54);
        painter->drawLine(94, 53, 94, 15);
        painter->setPen(QPen(Qt::black, 0));
    }

    // Draw text
    if (lod >= 2) {
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        painter->drawText(170, 180, QString("Model: VSC-2000 (Very Small Chip) at %1x%2").arg(x).arg(y));
        painter->drawText(170, 200, QString("Serial number: DLWR-WEER-123L-ZZ33-SDSJ"));
        painter->drawText(170, 220, QString("Manufacturer: Chip Manufacturer"));
        painter->restore();
    }

    // Draw lines
    QVarLengthArray<QLineF, 36> lines;
    if (lod >= 0.5) {
        for (int i = 0; i <= 10; i += (lod > 0.5 ? 1 : 2)) {
            lines.append(QLineF(18 + 7 * i, 13, 18 + 7 * i, 5));
            lines.append(QLineF(18 + 7 * i, 54, 18 + 7 * i, 62));
        }
        for (int i = 0; i <= 6; i += (lod > 0.5 ? 1 : 2)) {
            lines.append(QLineF(5, 18 + i * 5, 13, 18 + i * 5));
            lines.append(QLineF(94, 18 + i * 5, 102, 18 + i * 5));
        }
    }
    if (lod >= 0.4) {
        const QLineF lineData[] = {
            QLineF(25, 35, 35, 35),
            QLineF(35, 30, 35, 40),
            QLineF(35, 30, 45, 35),
            QLineF(35, 40, 45, 35),
            QLineF(45, 30, 45, 40),
            QLineF(45, 35, 55, 35)
        };
        lines.append(lineData, 6);
    }
    painter->drawLines(lines.data(), lines.size());

    // Draw red ink
    // shift绘制部分
    if (stuff.size() > 1) {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(stuff.first());
        for (int i = 1; i < stuff.size(); ++i)
            path.lineTo(stuff.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }
}

void Chip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Chip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 边刷新坐标 边绘制
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Chip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
