#include "filterdxfrw.h"
#include "libdwgr.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


FilterDXFRW::FilterDXFRW(dxfRW &dxfW): dxfW(dxfW) {
}

FileFormat FilterDXFRW::fileType(const QString &fileName)
{
    unsigned int found = fileName.lastIndexOf(".");
    QString fileExt = fileName.mid(found + 1);
    fileExt = fileExt.toUpper();
    if (fileExt == "DXF") {
        return FileFormat::FormatDXFRW;
    } else if (fileExt == "DWG") {
        return FileFormat::FormatDWG;
    } else {
        return FileFormat::FormatUnknown;
    }
}

bool FilterDXFRW::fileImport(const QString &fileI, const DrawDataPtr fData, FileFormat fileFormat, bool debug)
{
    hsData = fData;
    currentBlock = hsData->mBlock;

    if (fileFormat == FileFormat::FormatDWG)
    {
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
        dwgR* dwg = new dwgR(QFile::encodeName(fileI));
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        bool success = dwg->read(this, false);
        if (!success) {
            errorCode = dwg->getError();
            qDebug() << "DWG file error: format " << dwg->getVersion() << " error " << dwg->getError();
        }
        delete dwg;
        return success;
    }
    else if (fileFormat == FileFormat::FormatDXFRW)
    {
        //loads dxf
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
        dxfRW* dxf = new dxfRW(QFile::encodeName(fileI));
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        bool success = dxf->read(this, false);
        if (!success) {
            errorCode = dxf->getError();
             qDebug() << "DXF file error: format " << dxf->getVersion() << " error " << dxf->getError();
        }
        delete dxf;
        return success;
    }
    qDebug() << "file extension can be dxf or dwg";
    return false;
}

bool FilterDXFRW::fileExport(const string &file, DRW::Version v, bool binary, DrawDataPtr fData)
{
//    hsData = fData;
//    dxfW = new dxfRW(file.c_str());
//    bool success = dxfW->write(this, v, binary);
//    delete dxfW;
//    return success;
}

void FilterDXFRW::writeEntity(DRW_Entity *e)
{
    switch (e->eType) {
        case DRW::POINT:
            dxfW.writePoint(static_cast<DRW_Point*>(e));
            break;
        case DRW::LINE:
            dxfW.writeLine(static_cast<DRW_Line*>(e));
            break;
        case DRW::CIRCLE:
            dxfW.writeCircle(static_cast<DRW_Circle*>(e));
            break;
        case DRW::ARC:
            dxfW.writeArc(static_cast<DRW_Arc*>(e));
            break;
        case DRW::SOLID:
            dxfW.writeSolid(static_cast<DRW_Solid*>(e));
            break;
        case DRW::ELLIPSE:
            dxfW.writeEllipse(static_cast<DRW_Ellipse*>(e));
            break;
        case DRW::LWPOLYLINE:
            dxfW.writeLWPolyline(static_cast<DRW_LWPolyline*>(e));
            break;
        case DRW::POLYLINE:
            dxfW.writePolyline(static_cast<DRW_Polyline*>(e));
            break;
        case DRW::SPLINE:
            dxfW.writeSpline(static_cast<DRW_Spline*>(e));
            break;
    //    case RS2::EntitySplinePoints:
    //        writeSplinePoints(static_cast<DRW_Point*>(e));
    //        break;
    //    case RS2::EntityVertex:
    //        break;
        case DRW::INSERT:
            dxfW.writeInsert(static_cast<DRW_Insert*>(e));
            break;
        case DRW::MTEXT:
            dxfW.writeMText(static_cast<DRW_MText*>(e));
            break;
        case DRW::TEXT:
            dxfW.writeText(static_cast<DRW_Text*>(e));
            break;
        case DRW::DIMLINEAR:
        case DRW::DIMALIGNED:
        case DRW::DIMANGULAR:
        case DRW::DIMANGULAR3P:
        case DRW::DIMRADIAL:
        case DRW::DIMDIAMETRIC:
        case DRW::DIMORDINATE:
            dxfW.writeDimension(static_cast<DRW_Dimension*>(e));
            break;
        case DRW::LEADER:
            dxfW.writeLeader(static_cast<DRW_Leader*>(e));
            break;
        case DRW::HATCH:
            dxfW.writeHatch(static_cast<DRW_Hatch*>(e));
            break;
        case DRW::IMAGE:
            dxfW.writeImage(static_cast<DRW_Image*>(e), static_cast<dx_ifaceImg*>(e)->path);
            break;
        default:
            break;
        }
}


QString FilterDXFRW::lastError() const
{
    switch (errorCode) {
    case DRW::BAD_NONE:
        return (QObject::tr( "no DXF/DWG error", "HS_FilterDXFRW"));
    case DRW::BAD_OPEN:
        return (QObject::tr( "error opening DXF/DWG file", "HS_FilterDXFRW"));
    case DRW::BAD_VERSION:
        return (QObject::tr( "unsupported DXF/DWG file version", "HS_FilterDXFRW"));
    case DRW::BAD_READ_METADATA:
        return (QObject::tr( "error reading DXF/DWG meta data", "HS_FilterDXFRW"));
    case DRW::BAD_READ_FILE_HEADER:
        return (QObject::tr( "error reading DXF/DWG file header", "HS_FilterDXFRW"));
    case DRW::BAD_READ_HEADER:
        return (QObject::tr( "error reading DXF/DWG header dara", "HS_FilterDXFRW"));
    case DRW::BAD_READ_HANDLES:
        return (QObject::tr( "error reading DXF/DWG object map", "HS_FilterDXFRW"));
    case DRW::BAD_READ_CLASSES:
        return (QObject::tr( "error reading DXF/DWG classes", "HS_FilterDXFRW"));
    case DRW::BAD_READ_TABLES:
        return (QObject::tr( "error reading DXF/DWG tables", "HS_FilterDXFRW"));
    case DRW::BAD_READ_BLOCKS:
        return (QObject::tr( "error reading DXF/DWG blocks", "HS_FilterDXFRW"));
    case DRW::BAD_READ_ENTITIES:
        return (QObject::tr( "error reading DXF/DWG entities", "HS_FilterDXFRW"));
    case DRW::BAD_READ_OBJECTS:
        return (QObject::tr( "error reading DXF/DWG objects", "HS_FilterDXFRW"));
    case DRW::BAD_READ_SECTION:
        return (QObject::tr( "error reading DXF/DWG sections", "HS_FilterDXFRW"));
    case DRW::BAD_CODE_PARSED:
        return (QObject::tr( "error reading DXF/DWG code", "HS_FilterDXFRW"));
    default:
        return (QObject::tr( "undefined error", "HS_FilterInterface"));
    }
}
