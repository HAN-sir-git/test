#ifndef DATA_H
#define DATA_H

#include "libdxfrw.h"
#include "point.h"
#include "line.h"
#include "polyline.h"
#include "block.h"

#include <QVector>
#include <QMap>
#include <QDebug>

using DRW_ImagePtr = std::shared_ptr<DRW_Image>;
using DRW_LinePtr = std::shared_ptr<DRW_Line>;
using DRW_ArcPtr = std::shared_ptr<DRW_Arc>;
using DRW_CirclePtr = std::shared_ptr<DRW_Circle>;
using DRW_EllipsePtr = std::shared_ptr<DRW_Ellipse>;
using DRW_TracePtr = std::shared_ptr<DRW_Trace>;
using DRW_TextPtr = std::shared_ptr<DRW_Text>;
using DRW_PolylinePtr = std::shared_ptr<DRW_Polyline>;
using DRW_LWPolylinePtr = std::shared_ptr<DRW_LWPolyline>;
using DRW_SolidPtr = std::shared_ptr<DRW_Solid>;
using DRW_HatchPtr = std::shared_ptr<DRW_Hatch>;

using DRW_PointPtrList = QList<std::shared_ptr<DRW_Point>>;
using DRW_LinePtrList = QList<DRW_LinePtr>;
using DRW_CirclePtrList = QList<std::shared_ptr<DRW_Circle>>;
using DRW_ArcPtrList = QList<std::shared_ptr<DRW_Arc>>;
using DRW_EllipsePtrList = QList<std::shared_ptr<DRW_Ellipse>>;
using DRW_TracePtrList = QList<std::shared_ptr<DRW_Trace>>;
using DRW_TextPtrList = QList<std::shared_ptr<DRW_Text>>;
using DRW_PolylinePtrList = QList<std::shared_ptr<DRW_Polyline>>;
using DRW_LWPolylinePtrList = QList<std::shared_ptr<DRW_LWPolyline>>;
using DRW_SolidPtrList = QList<std::shared_ptr<DRW_Solid>>;
using DRW_HatchPtrList = QList<std::shared_ptr<DRW_Hatch>>;


using BlockPtr = std::shared_ptr<Block>;
using EntityPtr = std::shared_ptr<Entity>;
using PointPtr = std::shared_ptr<Point>;
using LinePtr = std::shared_ptr<Line>;
using PolylinePtr = std::shared_ptr<PolyLine>;
//using ArcPtr = std::shared_ptr<Arc>;
//using CirclePtr = std::shared_ptr<Circle>;
//using EllipsePtr = std::shared_ptr<Ellipse>;
//using TextPtr = std::shared_ptr<Text>;
//using MTextPtr = std::shared_ptr<MText>;
//using SolidPtr = std::shared_ptr<Solid>;
//using HatchPtr = std::shared_ptr<Hatch>;
//using SplinePtr = std::shared_ptr<Spline>;
//using PathPtr = std::shared_ptr<PainterPath>;
//using InsertPtr = std::shared_ptr<Insert>;

using EntityPtrList = QList<EntityPtr>;
using PointPtrList = QList<PointPtr>;
using LinePtrList =QList<LinePtr>;
using PolyLinePtrList = QList<PolylinePtr>;
//using HS_CirclePtrList = QList<std::shared_ptr<Circle>>;
//using HS_ArcPtrList = QList<std::shared_ptr<Arc>>;
//using HS_EllipsePtrList = QList<std::shared_ptr<Ellipse>>;
//using HS_TextPtrList = QList<std::shared_ptr<Text>>;
//using HS_MTextPtrList = QList<std::shared_ptr<MText>>;
//using HS_SolidPtrList = QList<std::shared_ptr<Solid>>;
//using HS_HatchPtrList = QList<std::shared_ptr<Hatch>>;
//using HS_SplinePtrList = QList<std::shared_ptr<Spline>>;
//using HS_PathPtrList = QList<std::shared_ptr<PainterPath>>;
//using HS_InsertPtrList = QList<std::shared_ptr<Insert>>;





struct ConvertData
{
    PointPtrList point_list;
    LinePtrList line_list;
    PolyLinePtrList polyline_list;
public:
    QRectF box; // data box

    void refreshBox();
};



class dx_ifaceImg : public DRW_Image {
public:
    dx_ifaceImg(){}
    dx_ifaceImg(const DRW_Image& p):DRW_Image(p){}
    ~dx_ifaceImg(){}
    std::string path; //stores the image path
};

//container class to store entites.
class dx_ifaceBlock : public DRW_Block {
public:
    dx_ifaceBlock(){}
    dx_ifaceBlock(const DRW_Block& p):DRW_Block(p){}
    ~dx_ifaceBlock(){
        for (std::list<DRW_Entity*>::const_iterator it=ent.begin(); it!=ent.end(); ++it)
            delete *it;
    }
    std::list<DRW_Entity*>ent; //stores the entities list
};


class DrawData {

public:
    DrawData(){
        mBlock = std::make_shared<Block>();
    }
    ~DrawData(){

    }

    DRW_Header headerC;
    QList<DRW_LType>        lineTypes;
    QList<DRW_Layer>        layers;
    QList<DRW_Dimstyle>     dimStyles;
    QList<DRW_Vport>        VPorts;
    QList<DRW_Textstyle>    textStyles;
    QList<DRW_AppId>        appIds;
    QList<BlockPtr>       blocks;
    QList<DRW_ImagePtr>       images;
    BlockPtr             mBlock;
public:
    ConvertData convertData;
public:

    // refactor code
    void convertEntityHS();

    /*********************static fun**********************************/

    std::shared_ptr<Point> Static_DRW_Point_2_HS_Point(const DRW_Point& data);
    std::shared_ptr<Line> Static_DRW_Line_2_HS_Line(const DRW_Line& data);
    std::shared_ptr<PolyLine> Static_DRW_Polyline_2_HS_Polyline(const DRW_Polyline& data);
    std::shared_ptr<PolyLine> Static_DRW_LWPolyline_2_HS_Polyline(const DRW_LWPolyline& data);
//    std::shared_ptr<Circle> Static_DRW_Circle_2_HS_Circle(const DRW_Circle& data,bool isFill = false);
//    std::shared_ptr<Arc> Static_DRW_Arc_2_HS_Arc(const DRW_Arc& data);
//    std::shared_ptr<Ellipse> Static_DRW_Ellipse_2_HS_Ellipse(const DRW_Ellipse& data,bool isFill = false);
//    std::shared_ptr<Text> Static_DRW_Text_2_HS_Text(const DRW_Text& data);
//    std::shared_ptr<MText> Static_DRW_MText_2_HS_MText(const DRW_MText& data);
//    std::shared_ptr<Solid> Static_DRW_Solid_2_HS_Solid(const DRW_Solid& data);
//    std::shared_ptr<Solid> Static_DRW_Trace_2_HS_Solid(const DRW_Trace& data);
//    std::shared_ptr<Hatch> Static_DRW_Hatch_2_HS_Hatch(const DRW_Hatch& data);
//    std::shared_ptr<Spline> Static_DRW_Spline_2_HS_Spline(const DRW_Spline& data);
//    std::shared_ptr<Insert> Static_DRW_Insert_2_HS_Insert(const DRW_Insert& data);

    /***************************************************************************/

    bool DRW_Point_2_HS_Point_R(const DRW_Point& data);
    bool DRW_Line_2_HS_Line_R(const DRW_Line& data);
    bool DRW_Polyline_2_HS_Polyline_R(const DRW_Polyline& data);
    bool DRW_LWPolyline_2_HS_Polyline_R(const DRW_LWPolyline& data);
//    bool DRW_Circle_2_HS_Circle_R(const DRW_Circle& data,bool isFill = false);
//    bool DRW_Arc_2_HS_Arc_R(const DRW_Arc& data);
//    bool DRW_Ellipse_2_HS_Ellipse_R(const DRW_Ellipse& data,bool isFill = false);
//    bool DRW_Text_2_HS_Text_R(const DRW_Text& data);
//    bool DRW_MText_2_HS_MText_R(const DRW_MText& data);
//    bool DRW_Solid_2_HS_Solid_R(const DRW_Solid& data);
//    bool DRW_Trace_2_HS_Solid_R(const DRW_Trace& data);
//    bool DRW_Hatch_2_HS_Hatch_R(const DRW_Hatch& data);
//    bool DRW_Spline_2_HS_Spline_R(const DRW_Spline& data);
//    bool DRW_Insert_2_HS_Insert_R(const DRW_Insert& data);


    /********************************************************************/

//    void convertPolyline_2_Shape();
//    void convertInsert_2_Shape();
//    void convertInsert(const HS_InsertPtr& ptr);
//    void convertHatch_2_Shape();

    ConvertData getModelSpaceEntity();

    void refreshBox();

    QRectF getBox() const { return convertData.box; }

    /**************************tool func***********************************/
    static QString toNativeString(const QString& data);

};


#endif // DATA_H
