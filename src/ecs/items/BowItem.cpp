#include "BowItem.h"
#include "../Stats.h"
#include "../arms/Bow.h"

BowItem::BowItem() : Item(BOW) {}

bool BowItem::stackable() {
    return false;
}

bool BowItem::canEquip() {
    return true;
}

int BowItem::tile() {
    return 104;
}

void BowItem::equip(std::shared_ptr<Entity> on) {
    if (on->hasComponent<Stats>()) {
        auto stats = on->getComponent<Stats>();
        stats->inventory.equip(std::make_shared<Bow>());
        this->equipped = true;
    }
}

void BowItem::unequip(std::shared_ptr<Entity> on) {
    if (on->hasComponent<Stats>()) {
        auto stats = on->getComponent<Stats>();
        stats->inventory.dropWeapon();
        this->equipped = false;
    }
}