#ifndef BOXSELECTIONRECT_H
#define BOXSELECTIONRECT_H

#include <QRectF>
#include <QObject>
#include <QGraphicsRectItem>
#include "../ElectricalSymbolRecognition/common.h"

/* 鼠标之于ROI的位置 */
enum class ROI_EmDiRection
{
    DIR_TOP = 0,
    DIR_BOTTOM,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_LEFTTOP,
    DIR_LEFTBOTTOM,
    DIR_RIGHTTOP,
    DIR_RIGHTBOTTOM,
    DIR_MIDDLE,
    DIR_NONE
};

// 宽度随缩放比例lod调整
#define EDGPADDING       5        //四周边缘可拉伸宽度

#define POINT_WIDTH      6        //边缘9点的宽度
#define POINT_HEIGHT     6        //边缘9点的高度

#define EDGE_WIDTH       1        //边框的宽度
#define MIDDLELINE_WIDTH 1        //辅助线的宽度

#define DRAW_TEN_POINT   0         //绘制十个点
#define DRAW_SUB_LINE    1         //绘制辅助线

class QAction;
class QMouseEvent;
class QContextMenuEvent;


class BoxSelectionRect :public QObject ,public QGraphicsRectItem
{

    Q_OBJECT // 自定义信号
public:
    explicit BoxSelectionRect(QObject *parent = nullptr);

    // 初始化
    void init();
    // 获取ROI区域
    QRectF getROIBox() const;
    // clear
    void clear();

public:
    /*****************************************MouseShape******************************************/
    Qt::CursorShape RoiPressCursorShape(ROI_EmDiRection dir);
    Qt::CursorShape RoiMoveCursorShape(ROI_EmDiRection dir);
    Qt::CursorShape RoiReleaseCursorShape(ROI_EmDiRection dir);

    /*****************************************Paint*********************************************/
    // 绘制ROI区域
    void drawROIBox(QPainter *painter);

    /*****************************************PressEvent*********************************************/
    // 鼠标位置计算
    ROI_EmDiRection mousePressPos(QPointF pos);
    // 设置鼠标按下状态
    void setPressStatus(const ROI_EmDiRection dir, const QPointF &pos);
    // 鼠标是否按下
    bool isPressed() const;

    /*****************************************MoveEvent*********************************************/
    // 鼠标移动刷新ROI区域
    void refreshROIBox(const QPointF& pos);

    /*****************************************ReleaseEvent*********************************************/
    // 鼠标释放状态重设
    void setReleaseStatus();


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) ;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    // Esc按键将ROI区域归零
    void keyPressEvent(QKeyEvent *ev);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

// 信号
signals:
    void sigRecognitionType(OnceFacilitySymbolRecognition type);

private:

    qreal lod {1.0};
    bool m_bPainterPressed;        //是否正在绘制
    bool m_bMovedPressed;          //是否正在拖动
    bool m_bScalePressed;          //是否正在缩放大小
    ROI_EmDiRection m_pressDir;    //拖动的方向

    QRectF m_roiRect;               //绘制的ROI区域
    QPointF m_StartPoint;            //初始按压位置

};

#endif // BOXSELECTIONRECT_H
