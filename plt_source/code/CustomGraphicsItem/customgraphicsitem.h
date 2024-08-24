#ifndef CUSTOMGRAPHICSITEM_H
#define CUSTOMGRAPHICSITEM_H

#include <QGraphicsItem>
class CustomGraphicsInterface {
public:
    CustomGraphicsInterface() {}
    virtual ~CustomGraphicsInterface() {}

public:
    virtual void  setChildLine(const QList<QLineF> &line);
    virtual QList<QLineF> childLine() const;
    virtual void appendChildLine(const QLineF &line);
    virtual void clearChildLine();


protected:
    QList<QLineF> child_line;
};

#endif // CUSTOMGRAPHICSITEM_H