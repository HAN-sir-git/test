#ifndef CGRAPHICSSENCE_H
#define CGRAPHICSSENCE_H

#include <QGraphicsScene>
#include <QUndoStack>

class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicsScene(QObject *parent = 0);

    QUndoStack* getUndoStack() { return undoStack; }
    
private:
    QUndoStack *undoStack;
};

#endif // CGRAPHICSSENCE_H
