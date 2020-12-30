#include "StickItem.h"
#include "../Stats.h"
#include "../arms/Stick.h"

StickItem::StickItem() : Item(STICK) {}

bool StickItem::stackable() {
    return false;
}

bool StickItem::canEquip() {
    return true;
}

int StickItem::tile() {
    return 94;
}

void StickItem::equip(std::shared_ptr<Entity> on) {
    if (on->hasComponent<Stats>()) {
        auto stats = on->getComponent<Stats>();
        stats->inventory.equip(std::make_shared<Stick>());
        this->equipped = true;
    }
}

void StickItem::unequip(std::shared_ptr<Entity> on) {
    if (on->hasComponent<Stats>()) {
        auto stats = on->getComponent<Stats>();
        stats->inventory.dropWeapon();
        this->equipped = false;
    }
}