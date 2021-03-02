#include "Inventory.h"

#include <ASSETS/Assets.h>

#include "../../Gfx.h"
#include "../../Draw.h"
#include "../../../config.h"
#include "../../alg/Text.h"
#include "../../Rt.h"
#include "../Transform.h"
#include "../Collectable.h"
#include "../Collider.h"
#include "../Manager.h"
#include "../../snd/Player.h"

Inventory::Inventory() {
    this->slots.resize(MAX_SLOTS);
}

void Inventory::next() {
    selectedSlot++;
    selectedSlot %= 10;
}

void Inventory::prev() {
    selectedSlot--;
    if (selectedSlot < 0) {
        selectedSlot = this->slots.size() - 1;
    }
}

bool Inventory::add(std::shared_ptr<Item> item) {
    bool success = false;
    if (item->stackable()) {
        success = addStackableItem(item);
    } else {
        success = addSingleSlotItem(item);
    }
    if (success) {
        Player::instance().playSound(SOUND_PICKUP);
    }
    return success;
}

bool Inventory::hasWeapon() {
    return this->weapon != nullptr;
}

void Inventory::equip(std::shared_ptr<Weapon> weapon) {
    this->weapon = weapon;
}

void Inventory::dropWeapon() {
    this->weapon = nullptr;
}

bool Inventory::addStackableItem(std::shared_ptr<Item> item) {
    for (auto &slot : slots) {
        if (slot.type == item->type && slot.number < MAX_PER_SLOT) {
            slot.number++;
            return true;
        }
    }
    return addSingleSlotItem(item);
}

bool Inventory::addSingleSlotItem(std::shared_ptr<Item> item) {
    for (auto &slot : slots) {
        if (slot.type == EMPTY_SLOT) {
            slot.type = item->type;
            slot.number = 1;
            slot.item = item;
            return true;
        }
    }
    return false;
}

void Inventory::drop() {
    if (this->slots.at(selectedSlot).type == EMPTY_SLOT) return;

    InventoryItem &slot = slots.at(selectedSlot);

    auto t = RT_Context.getPlayer()->getComponent<Transform>();

    Position p;
    t->p.nearestTile(p);
    Position dropPosition = RT_Context.getTopology().nearestAccessible(p, false);

    if (Manager::instance().hasEntities(dropPosition, ITEMS)) {
        return;
    }

    auto entity = Item::make(dropPosition, slot.item);

    if (slot.number > 1) {
        slot.number--;
    } else {
        if (slot.item->canEquip()) {
            slot.item->unequip(RT_Context.getPlayer());
        }
        slot.item.reset();
        slot.type = EMPTY_SLOT;
    }

    Manager::instance().enqueue(entity, ITEMS);
}

void Inventory::use() {
    if (this->slots.at(selectedSlot).type == EMPTY_SLOT) return;
    InventoryItem &slot = slots.at(selectedSlot);

    if (slot.item->canEquip()) {
        if (slot.item->equipped) {
            // If already equipped, unequip
            slot.item->unequip(RT_Context.getPlayer());
        } else {
            // Only one weapon can be equipped at the same time
            for (int i = 0; i < slots.size(); i++) {
                if (slots.at(i).item) slots.at(i).item->unequip(RT_Context.getPlayer());
            }
            slot.item->equip(RT_Context.getPlayer());
        }
    } else {
        slot.item->use(RT_Context.getPlayer());
    }
}

void Inventory::render() {
    int increment = configTileSize * 2;
    int inventoryWidth = MAX_SLOTS * increment;
    int x = (configWindowWidth / 2) - (inventoryWidth / 2);
    int y = configWindowHeight - increment;
    int slotTile = 14;
    int cursorTile = 13;
    int currentSlot = 0;

    auto texture = Assets::instance().getTexture(SPRITES);
    auto font = Assets::instance().getTexture(BITMAPFONT);

    for (auto &slot : slots) {
        SDL_Rect source;
        Gfx::instance().pick(source, slotTile, texture->w);

        SDL_Rect target = {x, y, increment, increment};
        Draw::instance().draw(texture->mem, source, target);

        if (slot.type != EMPTY_SLOT) {
            Gfx::pick(source, slot.item->tile(), texture->w);
            Draw::instance().draw(texture->mem, source, target);
        }

        if (currentSlot == selectedSlot) {
            Gfx::pick(source, cursorTile, texture->w);
            Draw::instance().draw(texture->mem, source, target);
        }

        if (slot.number > 1) {
            Gfx::pickText(source, Text::fromInt(slot.number), font->w);

            target.x += configTileSize;
            target.w = configTileSize;
            target.h = configTileSize;

            SDL_SetTextureColorMod(font->mem, 128, 255, 128);
            Draw::instance().draw(font->mem, source, target);
            SDL_SetTextureColorMod(font->mem, 255, 255, 255);
        }

        if (slot.item && slot.item->equipped) {
            Gfx::pickText(source, Text::fromChar('e'), font->w);

            target.x += configTileSize;
            target.w = configTileSize;
            target.h = configTileSize;

            SDL_SetTextureColorMod(font->mem, 128, 255, 128);
            Draw::instance().draw(font->mem, source, target);
            SDL_SetTextureColorMod(font->mem, 255, 255, 255);
        }

        x += increment;
        currentSlot++;
    }
}