#pragma once
#include <QObject>
#include <QFile>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QTextStream>
#include <memory> // 包含智能指针的头文件
#include "block_obj.h"
#include "PltGraphicElement.h"

#define DPI 300
#define HPGL_TO_INCH 25400.0

class CPLTFileData : public QObject
{
    Q_OBJECT

 public:
    CPLTFileData(QObject* parent = nullptr); // 构造函数
    ~CPLTFileData(); // 析构函数
    bool in(const QString& filePath);
    double m_ScaleX; // x轴的图形缩放比率
    double m_ScaleY; // y轴的图形缩放比率
    bool readPltGroups(QTextStream& stream);
    // 使用智能指针管理PltGraphicElement对象
    std::vector<PltGraphicElement> graphicElements;
    PltPolyLine  m_PltPolyLine;
};


