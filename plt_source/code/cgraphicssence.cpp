#include "cgraphicssence.h"
#include <QKeyEvent>

CGraphicsScene::CGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(100);
}

void CGraphicsScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem *> selectedItems = this->selectedItems();
        for (QGraphicsItem *item : selectedItems) {
            // 创建删除命令，并将其推入撤销堆栈
            undoStack->push(new DeleteCommand(this, item));
        }
    } else {
        QGraphicsScene::keyPressEvent(event);
    }
}
