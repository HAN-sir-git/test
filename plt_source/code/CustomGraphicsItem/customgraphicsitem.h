#ifndef CUSTOMGRAPHICSITEM_H
#define CUSTOMGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QLineF>
#include <QUndoStack>
#include "movecommand.h"
class CustomGraphicsInterface  {
public:
    CustomGraphicsInterface() {}
    virtual ~CustomGraphicsInterface() {}

public:
    virtual void  setChildLine(const QList<QLineF> &line);
    virtual QList<QLineF> childLine() const;
    virtual void appendChildLine(const QLineF &line);
    virtual void clearChildLine();
    virtual void setUndoStack(QUndoStack *undoStack);


protected:
    QList<QLineF> child_line;

public:
    QUndoStack *m_undoStack;
};

#endif // CUSTOMGRAPHICSITEM_H
