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


ConvertData DrawData::getModelSpaceEntity()
{
    return convertData;
}

void DrawData::refreshBox()
{
    convertData.refreshBox();
}

QString DrawData::toNativeString(const QString &data)
{
    QString res;

    // Ignore font tags:
    int j = 0;
    for (int i=0; i<data.length(); ++i) {
        if (data.at(i).unicode() == 0x7B){ //is '{' ?
            if (data.at(i+1).unicode() == 0x5c){ //and is "{\" ?
                //check known codes
                if ( (data.at(i+2).unicode() == 0x66) || //is "\f" ?
                     (data.at(i+2).unicode() == 0x48) || //is "\H" ?
                     (data.at(i+2).unicode() == 0x43)    //is "\C" ?
                     ) {
                    //found tag, append parsed part
                    res.append(data.mid(j,i-j));
                    int pos = data.indexOf(0x7D, i+3);//find '}'
                    if (pos <0) break; //'}' not found
                    QString tmp = data.mid(i+1, pos-i-1);
                    do {
                        tmp = tmp.remove(0,tmp.indexOf(0x3B, 0)+1 );//remove to ';'
                    } while(tmp.startsWith("\\f") || tmp.startsWith("\\H") || tmp.startsWith("\\C"));
                    res.append(tmp);
                    i = j = pos;
                    ++j;
                }
            }
        }
    }
    res.append(data.mid(j));

    // Line feed:
    res = res.replace(QRegExp("\\\\P"), "\n");
    // Space:
    res = res.replace(QRegExp("\\\\~"), " ");
    // Tab:
    res = res.replace(QRegExp("\\^I"), "    ");//RLZ: change 4 spaces for \t when mtext have support for tab
    // diameter:
    res = res.replace(QRegExp("%%[cC]"), QChar(0x2300));//RLZ: Empty_set is 0x2205, diameter is 0x2300 need to add in all fonts
    // degree:
    res = res.replace(QRegExp("%%[dD]"), QChar(0x00B0));
    // plus/minus
    res = res.replace(QRegExp("%%[pP]"), QChar(0x00B1));

    return res;

}
