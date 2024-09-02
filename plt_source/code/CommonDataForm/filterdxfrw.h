#ifndef FILTERDXFRW_H
#define FILTERDXFRW_H

#include "drw_interface.h"
#include "libdxfrw.h"
#include "data.h"
#include <QPointF>

using DrawDataPtr = std::shared_ptr<DrawData>;

enum FileFormat {
    FormatDXFRW,
    FormatDWG,
    FormatUnknown
};


class FilterDXFRW  : public DRW_Interface
{
public:
    FilterDXFRW(dxfRW &dxfW);
    ~FilterDXFRW(){}

    FileFormat fileType(const QString& fileName);
    bool fileImport(const QString& fileI, const DrawDataPtr fData,FileFormat fileFormat, bool debug = false);
    bool fileExport(const std::string& file, DRW::Version v, bool binary, DrawDataPtr fData);
    void writeEntity(DRW_Entity* e);

    QString lastError() const ;

    // DRW_Interface interface

    void addHeader(const DRW_Header* data) override
    {
        hsData->headerC = *data;
    }

    void addLType(const DRW_LType& data) override
    {
        hsData->lineTypes.append(data);
    }

    void addLayer(const DRW_Layer& data) override
    {
        hsData->layers.append(data);
    }

    void addDimStyle(const DRW_Dimstyle& data) override
    {
        hsData->dimStyles.append(data);
    }

    void addVport(const DRW_Vport& data) override
    {
        hsData->VPorts.append(data);
    }

    void addTextStyle(const DRW_Textstyle& data) override
    {
        hsData->textStyles.append(data);
    }

    void addAppId(const DRW_AppId& data) override
    {
        hsData->appIds.append(data);
    }

    // blocks
    void addBlock(const DRW_Block& data) override
    {
        QString name = QString::fromUtf8(data.name.c_str());
        Point  bp(data.basePoint.x, data.basePoint.y);
        BlockPtr block = std::make_shared<Block>(BlockData{name,bp});
        hsData->blocks.append(block);
        currentBlock = block;
    }

    void endBlock() override
    {
        currentBlock = hsData->mBlock;
    }

    virtual void setBlock(const int /*handle*/){}//unused


    void addPoint(const DRW_Point& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Point>(data));
    }

    void addLine(const DRW_Line& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Line>(data));
    }

    void addRay(const DRW_Ray& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Ray>(data));
    }

    void addXline(const DRW_Xline& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Xline>(data));
    }

    void addArc(const DRW_Arc& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Arc>(data));
    }

    void addCircle(const DRW_Circle& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Circle>(data));
    }

    void addEllipse(const DRW_Ellipse& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Ellipse>(data));
    }

    void addLWPolyline(const DRW_LWPolyline& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_LWPolyline>(data));
    }

    void addPolyline(const DRW_Polyline& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Polyline>(data));
    }

    void addSpline(const DRW_Spline* data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Spline>(*data));
    }

    void addKnot(const DRW_Entity& data) override {}//unused

    void addInsert(const DRW_Insert& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Insert>(data));
    }

    void addTrace(const DRW_Trace& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Trace>(data));
    }

    void add3dFace(const DRW_3Dface& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_3Dface>(data));
    }

    void addSolid(const DRW_Solid& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Solid>(data));
    }

    void addMText(const DRW_MText& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_MText>(data));
    }

    void addText(const DRW_Text& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Text>(data));
    }

    void addDimAlign(const DRW_DimAligned *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimAligned>(*data));
    }

    void addDimLinear(const DRW_DimLinear *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimLinear>(*data));
    }

    void addDimRadial(const DRW_DimRadial *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimRadial>(*data));
    }

    void addDimDiametric(const DRW_DimDiametric *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimDiametric>(*data));
    }

    void addDimAngular(const DRW_DimAngular *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimAngular>(*data));
    }

    void addDimAngular3P(const DRW_DimAngular3p *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimAngular3p>(*data));
    }

    void addDimOrdinate(const DRW_DimOrdinate *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_DimOrdinate>(*data));
    }

    void addLeader(const DRW_Leader *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Leader>(*data));
    }

    void addHatch(const DRW_Hatch *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Hatch>(*data));
    }

    void addViewport(const DRW_Viewport& data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Viewport>(data));
    }

    void addImage(const DRW_Image *data) override
    {
        currentBlock->ents.append(std::make_shared<DRW_Image>(*data));
        hsData->images.append(std::make_shared<DRW_Image>(*data));
    }

    void linkImage(const DRW_ImageDef *data) override {
//        duint32 handle = data->handle;
//                std::string path(data->name);
//                for (std::list<DRW_Image*>::iterator it=hsData->images.begin(); it != hsData->images.end(); ++it){
//                    if ((*it)->ref == handle){
//                        DRW_Image *img = *it;
//                        img->path = path;
//                    }
//                }
    }//unused

// writer part no need to implement
    void addComment(const char* comment) override {} //unused

    void addPlotSettings(const DRW_PlotSettings *data) override {} //unused

    void writeHeader(DRW_Header& data) override {
//        data = hsData->headerC;

    }

    void writeBlocks() override {
//        for (auto  bk : hsData->blocks){
//            DRW_Block block;
//            block.name =bk->getName().toStdString();
//            block.basePoint.x =bk->getBasePoint().x();
//             block.basePoint.y =bk->getBasePoint().y();
//              block.basePoint.z =0.;
//            block.flags = 1;//flag for unnamed block
//            dxfW->writeBlock(&block);
//            //and write each entity in block
//             for (auto ent :bk->ents)
//                 writeEntity(ent.get());
//            }
//        DRW_Block block;
//        block.name =currentBlock->getName().toStdString();
//        block.basePoint.x =currentBlock->getBasePoint().x();
//         block.basePoint.y =currentBlock->getBasePoint().y();
//          block.basePoint.z =0.;
//        block.flags = 1;//flag for unnamed block
//        dxfW->writeBlock(&block);
//        for (auto ent :currentBlock->ents)
//            writeEntity(ent.get());

    } //unused

    void writeBlockRecords() override {
//        for (auto blockRecord : hsData->blocks)
//             dxfW->writeBlockRecord(blockRecord->getName().toStdString());
//        dxfW->writeBlockRecord(currentBlock->getName().toStdString());
    } //unused

    void writeLTypes() override {
//        for (auto& lineType: hsData->lineTypes)
//            dxfW->writeLineType(&lineType);
    } //unused

    void writeLayers() override {
//        for (auto& layer: hsData->layers)
//            dxfW->writeLayer(&layer);
    } //unused

    void writeTextstyles() override {
//        for (auto &textStyle: hsData->textStyles)
//            dxfW->writeTextstyle(&textStyle);
    } //unused

    void writeVports() override {
//        for (auto& vport: hsData->VPorts)
//            dxfW->writeVport(&vport);
    } //unused

    void writeDimstyles() override {
//        for (auto& dimStyle: hsData->dimStyles)
//            dxfW->writeDimstyle(&dimStyle);
    } //unused

    void writeAppId() override {
//        for (auto& appId: hsData->appIds)
//            dxfW->writeAppId(&appId);
    } //unused

    void writeEntities() override {
//        for (auto e : hsData->mBlock->ents)
//            writeEntity(e.get());
        for (auto e : currentBlock->ents)
            writeEntity(e.get());
//        DRW_Line line;
//           line.basePoint.x = 10;
//           line.basePoint.y = 20;
//           line.secPoint.x = 30;
//           line.secPoint.y = 30;
//           dxfW.writeLine(&line);

//           DRW_Line line2;
//           line2.basePoint.x = 0;
//           line2.basePoint.y = 0;
//           line2.secPoint.x = 0;
//           line2.secPoint.y = 30;
//           dxfW.writeLine(&line2);
    } //unused

    void writeObjects() override {} //unused


public:
    dxfRW &dxfW;
    DrawDataPtr  hsData{nullptr}; // 用于存储DXF/DWG文件的数据
    BlockPtr  currentBlock{nullptr};
protected:
    int errorCode{0};
};

#endif // FILTERDXFRW_H
