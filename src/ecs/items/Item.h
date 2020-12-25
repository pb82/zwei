#ifndef ZWEI_ITEM_H
#define ZWEI_ITEM_H

#include "../Entity.h"
#include "../../alg/Padding.h"

enum ItemType {
    EMPTY_SLOT,
    HEALTH_POTION,
};

class Item {
public:
    Item(ItemType type) : type(type) {}

    virtual bool stackable() { return false; }

    virtual void use(Entity &on) {}

    virtual int tile() = 0;

    ItemType type;

};

#endif //ZWEI_ITEM_H
