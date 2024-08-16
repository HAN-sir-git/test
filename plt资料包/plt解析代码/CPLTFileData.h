#pragma once
#include <QObject>
#include <QFile>
#include <QString>
#include <QVector>
#include <QPointF>
#include <QTextStream>
#include <memory> // ��������ָ���ͷ�ļ�
#include "block_obj.h"
#include "PltGraphicElement.h"

#define DPI 300
#define HPGL_TO_INCH 25400.0

class CPLTFileData : public QObject
{
    Q_OBJECT

 public:
    CPLTFileData(QObject* parent = nullptr); // ���캯��
    ~CPLTFileData(); // ��������
    bool in(const QString& filePath);
    double m_ScaleX; // x���ͼ�����ű���
    double m_ScaleY; // y���ͼ�����ű���
    bool readPltGroups(QTextStream& stream);
    // ʹ������ָ�����PltGraphicElement����
    std::vector<PltGraphicElement> graphicElements;
    PltPolyLine  m_PltPolyLine;
};


