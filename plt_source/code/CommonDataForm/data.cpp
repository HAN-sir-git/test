#include "data.h"

//void ConvertData::refreshBox()
//{
//    box = QRectF();
//    for (auto point : point_list)
//    {
//        box = box.united(point->getBox());
//    }
//    // 多段线是否要打断作为普通线来处理
//}

void ConvertData::refreshBox()
{
    box = QRectF();
    for (auto polyline : polyline_list)
    {
        box = box.united(polyline->getBox());
    }
}
