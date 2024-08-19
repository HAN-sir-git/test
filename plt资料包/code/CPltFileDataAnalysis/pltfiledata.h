#ifndef PLTFILEDATA_H
#define PLTFILEDATA_H

#include <QObject>
#include <QTextStream>

#include "CommonDataForm/data.h"
// plt 解析类
class PltFileData  : public QObject
{
   Q_OBJECT
public:
    PltFileData(QObject* parent = nullptr);
    ~PltFileData();
    
public:
    // 解析plt文件
    bool ParsePltFile();
    // 获取解析后的数据
    std::shared_ptr<ConvertData >getPltData();
private:
    bool getStrippedPltQt(QString& line, QTextStream& stream);
    bool readPltGroups(QTextStream& stream);

public:
    std::shared_ptr<ConvertData> m_convertData{nullptr};
    QString m_fileName;
    
};

#endif // PLTFILEDATA_H
