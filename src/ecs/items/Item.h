#ifndef ZWEI_ITEM_H
#define ZWEI_ITEM_H

#include "../Entity.h"
#include "../../alg/Padding.h"
#include "../../alg/Position.h"

enum ItemType {
    EMPTY_SLOT,
    HEALTH_POTION,
    STICK,
    BOW,
    TORCH,
    HEART,
};

class Item {
public:
    Item(ItemType type) : type(type) {}

    virtual ~Item() {}

    virtual bool stackable() { return false; }

    virtual bool canEquip() { return false; }

    virtual bool use(std::shared_ptr<Entity> on) { return false; }

    virtual void equip(std::shared_ptr<Entity> on) {}

    virtual void unequip(std::shared_ptr<Entity> on) {}

    virtual int tile() = 0;

    virtual bool useOnPickup() { return false; }

    virtual bool notification(std::string &s) { return false; }

    ItemType type;

    bool equipped = false;

    static std::shared_ptr<Item> make(ItemType type);

    static std::shared_ptr<Entity> make(Position p, ItemType type);

    static std::shared_ptr<Entity> make(Position p, std::shared_ptr<Item> item);

};

#endif //ZWEI_ITEM_H
