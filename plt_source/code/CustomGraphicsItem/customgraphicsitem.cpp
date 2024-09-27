#include "customgraphicsitem.h"


void CustomGraphicsInterface::setChildLine(const QList<QLineF> &line)
{
    child_line = line;
}

QList<QLineF> CustomGraphicsInterface::childLine() const
{
    return child_line;
}

void CustomGraphicsInterface::appendChildLine(const QLineF &line)
{
    child_line.append(line);
}

QList<QLineF> CustomGraphicsInterface::getGlobalChildLine() const
{
    return QList<QLineF>();
}

void CustomGraphicsInterface::clearChildLine()
{
    child_line.clear();
}

void CustomGraphicsInterface::setUndoStack(QUndoStack *undoStack) {
    m_undoStack = undoStack;
}
