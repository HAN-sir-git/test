#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include <QGraphicsItem>

class MoveCommand : public QUndoCommand {
public:
    MoveCommand(QGraphicsItem *item, const QPointF &oldPos, const QPointF &newPos, QUndoCommand *parent = nullptr)
        : QUndoCommand(parent), m_item(item), m_oldPos(oldPos), m_newPos(newPos) {
        setText("Move Item");
    }

    void undo() override {
        m_item->setPos(m_oldPos);
    }

    void redo() override {
        m_item->setPos(m_newPos);
    }

private:
    QGraphicsItem *m_item;
    QPointF m_oldPos;
    QPointF m_newPos;
};

#endif // MOVECOMMAND_H
