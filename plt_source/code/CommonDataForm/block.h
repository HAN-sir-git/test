#ifndef BLOCK_H
#define BLOCK_H

#include "entity.h"
#include "point.h"
#include "libdxfrw.h"

using DRW_EntityPtr = std::shared_ptr<DRW_Entity>;
using DRW_EntityPtrList = QList<DRW_EntityPtr>;

struct BlockData
{
    BlockData() {}

    BlockData(QString name, Point basePoint):
        name(name),
        basePoint(basePoint)
    {}


    QString name;
    Point basePoint;
};

class Block: public Entity
{
public:
    Block(){}
    Block(const BlockData& data): data(data) {}
    ~Block(){}

    COMMON_TYPE::EntityType rtti() const { return COMMON_TYPE::BLOCK; }

    /*********************************SET*************************************/
    void setData(const BlockData& data) { this->data = data; }

    void setName(const QString& name) { data.name = name; }

    void setBasePoint(const Point& basePoint) { data.basePoint = basePoint; }

    /*********************************GET*************************************/

    const BlockData& getData() const { return data; }

    const QString& getName() const { return data.name; }

    const Point& getBasePoint() const { return data.basePoint; }

public:
    BlockData data;
    DRW_EntityPtrList ents;
};

#endif // BLOCK_H
