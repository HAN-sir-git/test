#include "boxselectionrect.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QStyleOptionGraphicsItem>



BoxSelectionRect::BoxSelectionRect(QObject *parent)
{
    init();
}

void BoxSelectionRect::init()
{
    m_bPainterPressed = false;
    m_bMovedPressed = false;
    m_bScalePressed = false;
    m_roiRect = QRect(0, 0, 0, 0);
    m_pressDir = ROI_EmDiRection::DIR_NONE;
    //setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

QRectF BoxSelectionRect::getROIBox() const
{
    return m_roiRect;
}

void BoxSelectionRect::clear()
{
    setRect(0,0,0,0);
    m_roiRect = QRectF(0,0,0,0);
}

Qt::CursorShape BoxSelectionRect::RoiPressCursorShape(ROI_EmDiRection dir)
{
    switch (dir)
    {
    case ROI_EmDiRection::DIR_MIDDLE:
        return Qt::ClosedHandCursor;
        break;
    case ROI_EmDiRection::DIR_LEFTTOP:
    case ROI_EmDiRection::DIR_RIGHTBOTTOM:
        return Qt::SizeFDiagCursor;
        break;
    case ROI_EmDiRection::DIR_RIGHTTOP:
    case ROI_EmDiRection::DIR_LEFTBOTTOM:
        return Qt::SizeBDiagCursor;
        break;
    case ROI_EmDiRection::DIR_TOP:
    case ROI_EmDiRection::DIR_BOTTOM:
        return Qt::SizeVerCursor;
        break;
    case ROI_EmDiRection::DIR_LEFT:
    case ROI_EmDiRection::DIR_RIGHT:
        return Qt::SizeHorCursor;
        break;
    case ROI_EmDiRection::DIR_NONE:
        return Qt::ArrowCursor;
        break;
    default:
        break;
    }
}

Qt::CursorShape BoxSelectionRect::RoiMoveCursorShape(ROI_EmDiRection dir)
{
    switch (dir)
    {
    case ROI_EmDiRection::DIR_MIDDLE:
        return Qt::OpenHandCursor;
        break;
    case ROI_EmDiRection::DIR_LEFTTOP:
    case ROI_EmDiRection::DIR_RIGHTBOTTOM:
        return Qt::SizeFDiagCursor;
        break;
    case ROI_EmDiRection::DIR_RIGHTTOP:
    case ROI_EmDiRection::DIR_LEFTBOTTOM:
        return Qt::SizeBDiagCursor;
        break;
    case ROI_EmDiRection::DIR_TOP:
    case ROI_EmDiRection::DIR_BOTTOM:
        return Qt::SizeVerCursor;
        break;
    case ROI_EmDiRection::DIR_LEFT:
    case ROI_EmDiRection::DIR_RIGHT:
        return Qt::SizeHorCursor;
        break;
    case ROI_EmDiRection::DIR_NONE:
        return Qt::ArrowCursor;
        break;
    default:
        break;
    }
}

Qt::CursorShape BoxSelectionRect::RoiReleaseCursorShape(ROI_EmDiRection dir)
{
    switch (dir)
    {
    case ROI_EmDiRection::DIR_MIDDLE:
        return Qt::OpenHandCursor;
        break;
    case ROI_EmDiRection::DIR_LEFTTOP:
    case ROI_EmDiRection::DIR_RIGHTBOTTOM:
    case ROI_EmDiRection::DIR_RIGHTTOP:
    case ROI_EmDiRection::DIR_LEFTBOTTOM:
    case ROI_EmDiRection::DIR_TOP:
    case ROI_EmDiRection::DIR_BOTTOM:
    case ROI_EmDiRection::DIR_LEFT:
    case ROI_EmDiRection::DIR_RIGHT:
    case ROI_EmDiRection::DIR_NONE:
        return Qt::ArrowCursor;
        break;
    default:
        break;
    }
    return Qt::ArrowCursor;
}

ROI_EmDiRection BoxSelectionRect::mousePressPos(QPointF pos)
{

    double edgpadding = EDGPADDING / lod < EDGPADDING ? EDGPADDING / lod : EDGPADDING;

    ROI_EmDiRection direction = ROI_EmDiRection::DIR_NONE;
    auto margin_rect = m_roiRect.adjusted(-edgpadding,-edgpadding,edgpadding,edgpadding);

    if (margin_rect.contains(pos))
    {
        if (qAbs(pos.x() - m_roiRect.left()) < edgpadding)
        {
            if (qAbs(pos.y() - m_roiRect.top()) < edgpadding)
            {
                direction = ROI_EmDiRection::DIR_LEFTTOP;
            }
            else if (qAbs(pos.y() - m_roiRect.bottom()) < edgpadding)
            {
                direction = ROI_EmDiRection::DIR_LEFTBOTTOM;
            }
            else
            {
                direction = ROI_EmDiRection::DIR_LEFT;
            }
        }
        else if (qAbs(pos.x() - m_roiRect.right()) < edgpadding)
        {
            if (qAbs(pos.y() - m_roiRect.top()) < edgpadding)
            {
                direction = ROI_EmDiRection::DIR_RIGHTTOP;
            }
            else if (qAbs(pos.y() - m_roiRect.bottom()) < edgpadding)
            {
                direction = ROI_EmDiRection::DIR_RIGHTBOTTOM;
            }
            else
            {
                direction = ROI_EmDiRection::DIR_RIGHT;
            }
        }
        else if (qAbs(pos.y() - m_roiRect.top()) < edgpadding)
        {
            direction = ROI_EmDiRection::DIR_TOP;
        }
        else if (qAbs(pos.y() - m_roiRect.bottom()) < edgpadding)
        {
            direction = ROI_EmDiRection::DIR_BOTTOM;
        }
        else
        {
            direction = ROI_EmDiRection::DIR_MIDDLE;
        }
    }
    return direction;
}

void BoxSelectionRect::setPressStatus(const ROI_EmDiRection dir, const QPointF &pos)
{
    if(dir == ROI_EmDiRection::DIR_NONE){
        m_bPainterPressed = true;
    }
    else if(dir == ROI_EmDiRection::DIR_MIDDLE){
        m_bMovedPressed = true;
    }
    else{
        m_bScalePressed = true;
        m_pressDir = dir;
    }
    m_StartPoint = pos;

}

bool BoxSelectionRect::isPressed() const
{
    return m_bPainterPressed || m_bMovedPressed || m_bScalePressed;
}

void BoxSelectionRect::refreshROIBox(const QPointF &pos)
{
    if(m_bPainterPressed){
        auto tl = QPointF(std::min(m_StartPoint.x(), pos.x()), std::min(m_StartPoint.y(), pos.y()));
        auto br = QPointF(std::max(m_StartPoint.x(), pos.x()), std::max(m_StartPoint.y(), pos.y()));
        m_roiRect = QRectF(tl,br);
    }else if(m_bMovedPressed){
        m_roiRect.moveCenter(m_roiRect.center() + pos - m_StartPoint);
        m_StartPoint = pos;
    }else if(m_bScalePressed){
        m_StartPoint = pos;
        switch (m_pressDir)
        {
        case ROI_EmDiRection::DIR_LEFT:
            m_roiRect.setLeft(pos.x());
            break;
        case ROI_EmDiRection::DIR_RIGHT:
            m_roiRect.setRight(pos.x());
            break;
        case ROI_EmDiRection::DIR_TOP:
            m_roiRect.setTop(pos.y());
            break;
        case ROI_EmDiRection::DIR_BOTTOM:
            m_roiRect.setBottom(pos.y());
            break;
        case ROI_EmDiRection::DIR_LEFTTOP:
            m_roiRect.setTopLeft(pos);
            break;
        case ROI_EmDiRection::DIR_LEFTBOTTOM:
            m_roiRect.setBottomLeft(pos);
            break;
        case ROI_EmDiRection::DIR_RIGHTTOP:
            m_roiRect.setTopRight(pos);
            break;
        case ROI_EmDiRection::DIR_RIGHTBOTTOM:
            m_roiRect.setBottomRight(pos);
            break;
        default:
            break;
        }
    }
    this->setRect(m_roiRect);
    update();
}

void BoxSelectionRect::drawROIBox(QPainter *painter)
{

    // 动态计算值
    double edge_width = EDGE_WIDTH / lod < EDGE_WIDTH ? EDGE_WIDTH / lod : EDGE_WIDTH;
    double point_width = POINT_WIDTH / lod < POINT_WIDTH ? POINT_WIDTH / lod : POINT_WIDTH ;
    double point_height = POINT_HEIGHT / lod < POINT_HEIGHT ? POINT_HEIGHT / lod : POINT_HEIGHT;
    double middleline_width = MIDDLELINE_WIDTH / lod < MIDDLELINE_WIDTH ? MIDDLELINE_WIDTH / lod : MIDDLELINE_WIDTH;

    //绘制ROI区域
    painter->save();
    painter->setPen(QPen(QColor(0, 255, 0), edge_width));
    painter->setBrush(QBrush(QColor(0, 0, 200, 120)));
    painter->drawRect(m_roiRect);
    //painter->drawRect(boundingRect());
    painter->restore();


    //绘制辅助线
#if DRAW_SUB_LINE
    painter->save();
    painter->setPen(QPen(QColor(Qt::white), middleline_width,Qt::DashDotLine));
    painter->drawLine(m_roiRect.topLeft().x() + m_roiRect.width() / 2, m_roiRect.topLeft().y() + edge_width, m_roiRect.bottomRight().x() - m_roiRect.width() / 2, m_roiRect.bottomRight().y());
    painter->drawLine(m_roiRect.topLeft().x() + edge_width, m_roiRect.topLeft().y() + m_roiRect.height() / 2, m_roiRect.bottomRight().x(), m_roiRect.bottomRight().y() - m_roiRect.height() / 2);
    painter->restore();
#endif

//绘制9个点
#if DRAW_TEN_POINT
    painter->save();
    painter->setPen(QPen(Qt::NoPen));
    painter->setBrush(Qt::red);
    painter->drawRect(m_roiRect.topLeft().x(), m_roiRect.topLeft().y(), point_width, point_height); //左上角
    painter->drawRect(m_roiRect.topLeft().x(), m_roiRect.topLeft().y() + m_roiRect.height() / 2 - point_width / 2, point_width, point_height); //左边中心点
    painter->drawRect(m_roiRect.bottomLeft().x(), m_roiRect.bottomLeft().y()- point_width, point_width, point_height); //左下角
    painter->drawRect(m_roiRect.topLeft().x() + m_roiRect.width() / 2 - point_width / 2, m_roiRect.topLeft().y(), point_width, point_height);  //顶部中心
    painter->drawRect(m_roiRect.topLeft().x() + m_roiRect.width() / 2 - point_width /2, m_roiRect.topLeft().y() + m_roiRect.height() / 2 - point_width / 2, point_width, point_height);  //中心点
    painter->drawRect(m_roiRect.bottomLeft().x() + m_roiRect.width() / 2 - point_width / 2, m_roiRect.bottomLeft().y() - point_width, point_width, point_height); //底部中心点
    painter->drawRect(m_roiRect.topRight().x() - point_width, m_roiRect.topRight().y(), point_width, point_height); //右上角
    painter->drawRect(m_roiRect.topRight().x() - point_width / 2, m_roiRect.topRight().y() + m_roiRect.height() / 2 - point_width /2, point_width, point_height); //右边中心点
    painter->drawRect(m_roiRect.bottomRight().x() - point_width, m_roiRect.bottomRight().y() - point_width, point_width, point_height); //右下角点
    painter->restore();
#endif
}

void BoxSelectionRect::setReleaseStatus()
{
     qDebug() << "Shape:" << shape();
    m_StartPoint = QPointF();
    m_bPainterPressed = false;
    m_bMovedPressed = false;
    m_bScalePressed = false;
}

void BoxSelectionRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(widget);
    lod = option->levelOfDetailFromTransform(painter->worldTransform());
    drawROIBox(painter);
}

void BoxSelectionRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->pos();
    auto dir = mousePressPos(pos);
    setCursor(RoiPressCursorShape(dir));
    event->accept();
    QGraphicsRectItem::mousePressEvent(event);
}

void BoxSelectionRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    auto pos = event->pos();
    auto dir = mousePressPos(pos);
    setCursor(RoiReleaseCursorShape(dir));
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void BoxSelectionRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    auto pos = event->pos();
    auto dir = mousePressPos(pos);
    setCursor(RoiMoveCursorShape(dir));
}


void BoxSelectionRect::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Escape)
    {
        clear();
        update();
    }
}

void BoxSelectionRect::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.setStyleSheet("QMenu{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(182,202,240, 255), stop:1 rgba(208,222,240, 255));}");

    QAction* del = menu.addAction(QString("删除"));
    QAction* action1 = menu.addAction(QString("接地识别"));
    QAction* action2 = menu.addAction(QString("开关识别"));
    QAction* action3 = menu.addAction(QString("主变识别"));
    QAction* action4 = menu.addAction(QString("刀闸识别"));
    // 显示菜单，并获取用户选择的操作
    QAction* selectedItem = menu.exec(event->screenPos());

    // 处理用户选择的操作
    if (selectedItem == del)
    {
        clear();
    }
    else if (selectedItem == action1)
    {
        // 发送信号告知场景接地识别
        emit sigRecognitionType(OnceFacilitySymbolRecognition::GroundingSymbol);
    }
    else if (selectedItem == action2)
    {
        emit sigRecognitionType(OnceFacilitySymbolRecognition::SwitchOn);
    }
    else if (selectedItem == action3)
    {
        emit sigRecognitionType(OnceFacilitySymbolRecognition::Transformer);
    }
    else if(selectedItem == action4)
    {
        emit sigRecognitionType(OnceFacilitySymbolRecognition::Knife);
    }

}
