#ifndef ENTITY_H
#define ENTITY_H

#include "common_type.h"
#include "common_mathfunc.h"
#include <QPen>
#include <memory>
using namespace std;

struct Attrib{
     QPen pen;
};


class Entity
{
public:
    Entity(){}
    Entity(const Attrib& a ) : attribute(a){}
    //虚析构函数
    virtual ~Entity(){}

public:
    virtual COMMON_TYPE::EntityType rtti() const { return COMMON_TYPE::ENTITY; }
    virtual QList<std::shared_ptr<Entity>> getChilds() const { return QList<std::shared_ptr<Entity>>(); }

public:
    void setEntityAttributes(const Attrib& a) {attribute = a;}

public:
    Attrib attribute;
};

#endif // ENTITY_H
