#include "pltfiledata.h"
#include <QFileDialog>
#include <QLocale>
#include <QString>
#include <QStringList>

#include "CommonDataForm/data.h"

PltFileData::PltFileData(QObject *parent)
{
    Q_UNUSED(parent);
}

PltFileData::~PltFileData()
{
}

bool PltFileData::ParsePltFile()
{
     m_convertData = std::make_shared<ConvertData>();
    // 打开文件
    QString _lastFileDir = "../";
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("打开文件"), _lastFileDir, tr("*.plt"));

    if (fileName.isEmpty())
        return false;
    
    //m_fileName = fileName;
    QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false; // 文件打开失败
    }

    QTextStream in(&file);
    // 假设readDxfGroups已经被修改为接受QTextStream&而不是FILE*
    while (readPltGroups(in)) {
        // 循环直到文件结束或readDxfGroups返回false
    }
    file.close();
    return true;

}

//因为plt文档，没有行概念，是采用“;”来区分
bool PltFileData::getStrippedPltQt(QString &line, QTextStream &stream) {
    if (stream.atEnd()) {
        // 如果已经到达文件末尾，则返回false
        return false;
    }
    // 从文本流中读取一段
    QString pltLine;
    while (true) {
        QString token = stream.read(1); // 逐个字符读取
        if (token.isEmpty() || token == ";") {
            break;
        }
        pltLine += token;
    }
    // 使用QString的trimmed()方法去除首尾空格
    line = pltLine.trimmed();

    // 如果成功读取到了非空行，则返回true
    return !line.isEmpty();
}

bool PltFileData::readPltGroups(QTextStream& stream)
{
    ConvertData cc;
    QString pltReadTmp;
    QString splitChar;
    double xOffset = 0; //x轴的偏移，用于翻页
    double xMin = 0;
    double xMax = 0;
    bool isPageFirst = true;

    PolylineData PolyLinedata;
    while (getStrippedPltQt(pltReadTmp, stream)) {


        if (pltReadTmp.startsWith("PU")) {

            if (!PolyLinedata.points.empty()) {
                // 闭合性判断
                if(PolyLinedata.points.size() > 2 && PolyLinedata.points.first().getPointF() == PolyLinedata.points.last().getPointF())
                {
                   // PolyLinedata.closed = true;
                   // PolyLinedata.points.removeLast();
                }
                auto points = PolyLinedata.points;
                std::shared_ptr<PolyLine> t = std::make_shared<PolyLine>(PolyLinedata.points,PolyLinedata.closed);
                //t->appendVertexs(points);
                m_convertData->polyline_list.push_back(std::move(t));
            }
            PolyLinedata = PolylineData();
            pltReadTmp.remove(0, 2);
            if (pltReadTmp.contains(",")) splitChar = ",";
            else splitChar = " ";
            QStringList subTokens = pltReadTmp.split(splitChar, QString::SkipEmptyParts);
            for (int i = 0; i < subTokens.size(); i += 2) {
                double x = subTokens[i].toDouble() + xOffset;
                double y = subTokens[i + 1].toDouble();
                PolyLinedata.points.push_back(Point(x, y));
                if (isPageFirst) {
                    isPageFirst = false;
                    xMin = x;
                    xMax = x;
                }
                else {
                    if (xMin > x) xMin = x;
                    if (xMax < x) xMax = x;
                }
            }

            continue;
        }

        if (pltReadTmp.startsWith("PD")) {
            //qDebug() << "Detected PD command"; // 输出检测到 PD 指令
            pltReadTmp.remove(0, 2);
            if (pltReadTmp.contains(",")) splitChar = ",";
            else splitChar = " ";
            QStringList subTokens = pltReadTmp.split(splitChar, QString::SkipEmptyParts);
            for (int i = 0; i < subTokens.size(); i += 2) {
                double x = subTokens[i].toDouble() + xOffset;
                double y = subTokens[i + 1].toDouble();
                PolyLinedata.points.push_back(Point(x, y));
                if (isPageFirst) {
                    isPageFirst = false;
                    xMin = x;
                    xMax = x;
                }
                else {
                    if (xMin > x) xMin = x;
                    if (xMax < x) xMax = x;
                }
            }
            continue;
        }

        if (pltReadTmp.startsWith("PG")) {
            if (!PolyLinedata.points.empty()) {
                // 闭合性判断
                if(PolyLinedata.points.size() > 2 && PolyLinedata.points.first().getPointF() == PolyLinedata.points.last().getPointF())
                {
                   // PolyLinedata.closed = true;
                   // PolyLinedata.points.removeLast();
                }
                auto points = PolyLinedata.points;
                std::shared_ptr<PolyLine> t = std::make_shared<PolyLine>(PolyLinedata.points,PolyLinedata.closed);
                //PolyLinedata = PolylineData();
               // t->appendVertexs(points);
                //m_convertData->polyline_list.push_back(std::move(t));
            }

            xOffset += (xMax - xMin);
            isPageFirst = true;
            continue;
        }
    }

    if (!PolyLinedata.points.empty()) {
        // 闭合性判断
        if(PolyLinedata.points.size() > 2 && PolyLinedata.points.first().getPointF() == PolyLinedata.points.last().getPointF())
        {
           // PolyLinedata.closed = true;
            //PolyLinedata.points.removeLast();
        }
        auto points = PolyLinedata.points;
        std::shared_ptr<PolyLine> t = std::make_shared<PolyLine>(PolyLinedata.points,PolyLinedata.closed);
        //t->appendVertexs(points);
        //m_convertData->polyline_list.push_back(std::move(t));
    }

    return !stream.atEnd();
}
        

std::shared_ptr<ConvertData > PltFileData::getPltData()
{
    return m_convertData;
}
