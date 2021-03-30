#ifndef ZWEI_INVENTORY_H
#define ZWEI_INVENTORY_H

#include <vector>

#include "../items/Item.h"
#include "../arms/Weapon.h"
#include "../Transform.h"
#include "Modifier.h"

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

    void update(float dt);

    void use();

    bool hasWeapon();

    void equip(std::shared_ptr<Weapon> weapon);

    void dropWeapon();

    // Why is this here?
    // To allow adding 'torches' to the game that can expire.
    // If the player uses a torch, which is inside the inventory, then
    // the value returned here will be different than otherwise.
    uint8_t getAlphaForTileAt(Position &p);

    std::shared_ptr<Weapon> weapon = nullptr;

    template<typename T>
    void addModifier(float lifetime) {
        auto m = std::make_shared<T>(lifetime);
        modifiers.push_back(m);
    }

    bool hasModifier(ModifierType type);

private:

    float getCircleOfLight();

    bool addStackableItem(std::shared_ptr<Item> item);

    bool addSingleSlotItem(std::shared_ptr<Item> item);

    std::vector<InventoryItem> slots;

    std::vector<std::shared_ptr<Modifier>> modifiers;

    Entity &parent;

    int selectedSlot = 0;

};


#endif //ZWEI_INVENTORY_H
