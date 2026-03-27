#ifndef ZWEI_INVENTORY_H
#define ZWEI_INVENTORY_H

#include <vector>

#include "../items/Item.h"
#include "../arms/Weapon.h"
#include "../Transform.h"

#define MAX_SLOTS 10
#define MAX_PER_SLOT 9

struct InventoryItem {
    ItemType type = EMPTY_SLOT;
    std::shared_ptr<Item> item = nullptr;
    int number = 0;
};

class Inventory {
public:
    Inventory(Entity &parent);

    bool add(std::shared_ptr<Item> item);

    void next();

    void prev();

    void drop();

    void render();

    void use();

    bool hasWeapon();

    void equip(std::shared_ptr<Weapon> weapon);

    void dropWeapon();

    void serialize(JSON::Value &to);

    void deserialize(JSON::Value &from);

    void resetAll();

    std::shared_ptr<Weapon> weapon = nullptr;

private:

    bool addStackableItem(std::shared_ptr<Item> item);

    bool addSingleSlotItem(std::shared_ptr<Item> item);

    void addItems(ItemType type, int number);

    std::vector<InventoryItem> slots;

    Entity &parent;

    int selectedSlot = 0;

};


#endif //ZWEI_INVENTORY_H
