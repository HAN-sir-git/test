#include "pltplot.h"
#include "CPLTFileData.h"
#include <QDebug> // ȷ������ qDebug ��ͷ�ļ�

using namespace std;


CPLTFileData::CPLTFileData(QObject* parent) {}

CPLTFileData::~CPLTFileData()
{

}

bool CPLTFileData::in(const QString& filePath ) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false; // �ļ���ʧ��
    }

    // ���浱ǰ��ȫ��Ĭ����������
    QLocale oldLocale = QLocale::c();
    // ����ȫ����������ΪC��ʹ�õ���Ϊ���ַָ���
    QLocale::setDefault(QLocale::c());

    QTextStream in(&file);
    // ����readDxfGroups�Ѿ����޸�Ϊ����QTextStream&������FILE*
    while (readPltGroups(in)) {
        // ѭ��ֱ���ļ�������readDxfGroups����false
    }

    // �ָ�ԭ����ȫ����������
    QLocale::setDefault(oldLocale);
    file.close();
    return true;
}

//��Ϊplt�ĵ���û���и���ǲ��á�;��������
bool getStrippedPltQt(QString& line, QTextStream& stream) {
    if (stream.atEnd()) {
        // ����Ѿ������ļ�ĩβ���򷵻�false
        return false;
    }

    // ���ı����ж�ȡһ��
    QString pltLine;
    while (true) {
        QString token = stream.read(1); // ����ַ���ȡ
        if (token.isEmpty() || token == ";") {
            break;
        }
        pltLine += token;
    }

    // ʹ��QString��trimmed()����ȥ����β�ո�
    line = pltLine.trimmed();

    //qDebug() << line;

    // ����ɹ���ȡ���˷ǿ��У��򷵻�true
    return !line.isEmpty();
}

bool CPLTFileData::readPltGroups(QTextStream& stream) {
    QString pltReadTmp;
    PltPolyLine mPltPolyLine;
    QString splitChar;
    double xOffset = 0; //x���ƫ�ƣ����ڷ�ҳ
    double xMin = 0;
    double xMax = 0;
    bool isPageFirst = true;

    while (getStrippedPltQt(pltReadTmp, stream)) {
        //qDebug() << "Read line:" << pltReadTmp; // �����ǰ������

        if (pltReadTmp.startsWith("PU")) {
            //qDebug() << "Detected PU command"; // �����⵽ PU ָ��
            if (!mPltPolyLine.Polygon.empty()) {
                PltGraphicElement element;
                element.kind = 0;  // ���� kind=0 ��ʾ polyline
                element.pltPolyLine = mPltPolyLine;
                graphicElements.push_back(element);
            }
            mPltPolyLine = PltPolyLine();
            continue;
        }

        if (pltReadTmp.startsWith("PD")) {
            //qDebug() << "Detected PD command"; // �����⵽ PD ָ��
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
            //qDebug() << "Detected PG command"; // �����⵽ PG ָ���ҳ
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



