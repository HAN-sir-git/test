#pragma once
#include <memory>
#include <QObject>
#include <QFile>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QPen>

//Text��
class PltText  {
public:
   QString text;
   double x, y;
};

//����ʵ�����
class PltPolyLine
{
    public:
        QPen pen;
        std::vector<std::pair<double, double>> Polygon; // ��ǰ����εĶ���
        // ��ʽĬ�Ϲ��캯��
        PltPolyLine() {
        // ������ԱĬ�ϳ�ʼ��
        }
};

struct  PltGraphicElement {
        int kind;
        PltPolyLine pltPolyLine;
        PltText text;
};




