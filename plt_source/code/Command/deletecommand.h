#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>

class DeleteCommand : public QUndoCommand {
public:
    DeleteCommand(QGraphicsScene *scene, QGraphicsItem *item)
        : m_scene(scene), m_item(item) {
        setText("Delete Item");
    }

    void undo() override {
        // 撤销删除操作
        m_scene->addItem(m_item);
    }

    void redo() override {
        // 执行删除操作
        m_scene->removeItem(m_item);
    }

private:
    QGraphicsScene *m_scene;
    QGraphicsItem *m_item;
};
#endif // DELETECOMMAND_H
