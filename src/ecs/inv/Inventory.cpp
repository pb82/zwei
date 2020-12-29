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
#include "../Acceleration.h"

Inventory::Inventory() {
    this->slots.resize(MAX_SLOTS);
}

void Inventory::next() {
    selectedSlot++;
    selectedSlot %= 10;
}

bool Inventory::add(std::shared_ptr<Item> item) {
    if (item->stackable()) return addStackableItem(item);
    return addSingleSlotItem(item);
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
        slot.item.reset();
        slot.type = EMPTY_SLOT;
    }

    Manager::instance().enqueue(entity, ITEMS);
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
            Gfx::pick(source, Text::fromInt(slot.number), texture->w);

            target.x += configTileSize;
            target.w = configTileSize;
            target.h = configTileSize;

            SDL_SetTextureColorMod(texture->mem, 128, 255, 128);
            Draw::instance().draw(texture->mem, source, target);
            SDL_SetTextureColorMod(texture->mem, 255, 255, 255);
        }

        x += increment;
        currentSlot++;
    }
}