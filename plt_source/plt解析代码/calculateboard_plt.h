#ifndef CALCULATEBOARD_PLT_H
#define CALCULATEBOARD_PLT_H
#include "analyse_entities_plt.h"
#include <QPolygonF>

using namespace std;

QPolygonF calcuPltBoard(const Plt_PointData & data);
QPolygonF calcuPltBoard(const Plt_LineData & data);

QPolygonF calculatorPltBoard(const vector<void*> &list);//计算图形边界
#endif // CALCULATEBOARD_PLT_H
