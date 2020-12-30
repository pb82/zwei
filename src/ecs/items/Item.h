#ifndef ZWEI_ITEM_H
#define ZWEI_ITEM_H

#include "../Entity.h"
#include "../../alg/Padding.h"
#include "../../alg/Position.h"

enum ItemType {
    EMPTY_SLOT,
    HEALTH_POTION,
    STICK,
};

class Item {
public:
    Item(ItemType type) : type(type) {}

    virtual bool stackable() { return false; }

    virtual bool canEquip() { return false; }

    virtual void use(std::shared_ptr<Entity> on) {}

    virtual void equip(std::shared_ptr<Entity> on) {}

    virtual void unequip(std::shared_ptr<Entity> on) {}

    virtual int tile() = 0;

    ItemType type;

    bool equipped = false;

    static std::shared_ptr<Entity> make(Position p, ItemType type);

    static std::shared_ptr<Entity> make(Position p, std::shared_ptr<Item> item);

};

#endif //ZWEI_ITEM_H
