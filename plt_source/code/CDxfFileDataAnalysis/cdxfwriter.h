#ifndef CDXFWRITER_H
#define CDXFWRITER_H

#include "CommonDataForm/filterdxfrw.h"

#include <QString>
#include <QGraphicsItem>
#include <QObject>
#include "CommonDataForm/data.h"
#include <QPointF>

class CDxfWriter: public QObject
{
    Q_OBJECT
public:
    CDxfWriter( QObject *parent = nullptr);

    ~CDxfWriter();


    bool writeDxfFile(const QString &filename,QList<QGraphicsItem*>);
};

#endif // CDXFWRITER_H
