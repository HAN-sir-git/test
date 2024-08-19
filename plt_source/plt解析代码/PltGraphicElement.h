#pragma once
#include <memory>
#include <QObject>
#include <QFile>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QPen>

//Text类
class PltText  {
public:
   QString text;
   double x, y;
};

//多线实体对象
class PltPolyLine
{
    public:
        QPen pen;
        std::vector<std::pair<double, double>> Polygon; // 当前多边形的顶点
        // 显式默认构造函数
        PltPolyLine() {
        // 其他成员默认初始化
        }
};

struct  PltGraphicElement {
        int kind;
        PltPolyLine pltPolyLine;
        PltText text;
};




