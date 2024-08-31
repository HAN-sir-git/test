#ifndef CGRAPHICSSENCE_H
#define CGRAPHICSSENCE_H

#include <QGraphicsScene>
#include <QUndoStack>
#include "Command/deletecommand.h"

class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicsScene(QObject *parent = 0);

    QUndoStack* getUndoStack() { return undoStack; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    QUndoStack *undoStack;
};

#endif // CGRAPHICSSENCE_H
