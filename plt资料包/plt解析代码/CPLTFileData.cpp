#include "pltplot.h"
#include "CPLTFileData.h"
#include <QDebug> // 确保包含 qDebug 的头文件

using namespace std;


CPLTFileData::CPLTFileData(QObject* parent) {}

CPLTFileData::~CPLTFileData()
{

}

bool CPLTFileData::in(const QString& filePath ) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false; // 文件打开失败
    }

    // 保存当前的全局默认区域设置
    QLocale oldLocale = QLocale::c();
    // 设置全局区域设置为C，使用点作为数字分隔符
    QLocale::setDefault(QLocale::c());

    QTextStream in(&file);
    // 假设readDxfGroups已经被修改为接受QTextStream&而不是FILE*
    while (readPltGroups(in)) {
        // 循环直到文件结束或readDxfGroups返回false
    }

    // 恢复原来的全局区域设置
    QLocale::setDefault(oldLocale);
    file.close();
    return true;
}

//因为plt文档，没有行概念，是采用“;”来区分
bool getStrippedPltQt(QString& line, QTextStream& stream) {
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

    //qDebug() << line;

    // 如果成功读取到了非空行，则返回true
    return !line.isEmpty();
}

bool CPLTFileData::readPltGroups(QTextStream& stream) {
    QString pltReadTmp;
    PltPolyLine mPltPolyLine;
    QString splitChar;
    double xOffset = 0; //x轴的偏移，用于翻页
    double xMin = 0;
    double xMax = 0;
    bool isPageFirst = true;

    while (getStrippedPltQt(pltReadTmp, stream)) {
        //qDebug() << "Read line:" << pltReadTmp; // 输出当前行内容

        if (pltReadTmp.startsWith("PU")) {
            //qDebug() << "Detected PU command"; // 输出检测到 PU 指令
            if (!mPltPolyLine.Polygon.empty()) {
                PltGraphicElement element;
                element.kind = 0;  // 假设 kind=0 表示 polyline
                element.pltPolyLine = mPltPolyLine;
                graphicElements.push_back(element);
            }
            mPltPolyLine = PltPolyLine();
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
                mPltPolyLine.Polygon.push_back(std::make_pair(x, y));
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
            //qDebug() << "Detected PG command"; // 输出检测到 PG 指令，翻页
            if (!mPltPolyLine.Polygon.empty()) {
                PltGraphicElement element;
                element.kind = 0;
                element.pltPolyLine = mPltPolyLine;
                graphicElements.push_back(element);
            }
            xOffset += xMax - xMin;
            isPageFirst = true;
            continue;
        }
    }

    if (!mPltPolyLine.Polygon.empty()) {
        PltGraphicElement element;
        element.kind = 0;
        element.pltPolyLine = mPltPolyLine;
        graphicElements.push_back(element);
    }

    return !stream.atEnd();
}



