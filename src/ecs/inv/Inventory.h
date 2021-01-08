#ifndef ZWEI_INVENTORY_H
#define ZWEI_INVENTORY_H

#include <vector>

#include "../items/Item.h"
#include "../arms/Weapon.h"

#define MAX_SLOTS 10
#define MAX_PER_SLOT 9

struct InventoryItem {
    ItemType type = EMPTY_SLOT;
    std::shared_ptr<Item> item = nullptr;
    int number = 0;
};

class Inventory {
public:
    Inventory();

    bool add(std::shared_ptr<Item> item);

    void next();

    void drop();

    void render();

    void use();

    bool hasWeapon();

    void equip(std::shared_ptr<Weapon> weapon);

    void dropWeapon();

    std::shared_ptr<Weapon> weapon = nullptr;
    
private:

    bool addStackableItem(std::shared_ptr<Item> item);

    bool addSingleSlotItem(std::shared_ptr<Item> item);

    std::vector<InventoryItem> slots;

    int selectedSlot = 0;

};


#endif //ZWEI_INVENTORY_H
