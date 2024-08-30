#include "cgraphicssence.h"

CGraphicsScene::CGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(100);
}
