#include "Inventory.h"

#include <cmath>

#include <ASSETS/Assets.h>

#include "../../Gfx.h"
#include "../../Draw.h"
#include "../../alg/Text.h"
#include "../Collectable.h"
#include "../Collider.h"
#include "../../snd/Player.h"
#include "../items/HealthPotion.h"
#include "TorchModifier.h"

Inventory::Inventory(Entity &parent) : parent(parent) {
    this->slots.resize(MAX_SLOTS);
}

void Inventory::serialize(JSON::Value &to) {
    JSON::Array items;
    for (int i = 0; i < this->slots.size(); i++) {
        JSON::Object o;
        o["type"] = this->slots.at(i).type;
        o["number"] = this->slots.at(i).number;
        if (this->slots.at(i).item) {
            o["equipped"] = this->slots.at(i).item->equipped;
        }
        items.push_back(o);
    }
    to["inventory"]["items"] = items;
    JSON::Array mods;
    for (int i = 0; i < this->modifiers.size(); i++) {
        JSON::Object m;
        m["type"] = this->modifiers.at(i)->type;
        m["lifetime"] = this->modifiers.at(i)->lifetime;
        m["max"] = this->modifiers.at(i)->max;
        m["tile"] = this->modifiers.at(i)->tile;
        mods.push_back(m);
    }
    to["inventory"]["modifiers"] = mods;
}

void Inventory::deserialize(JSON::Value &from) {
    auto loadedItems = from["inventory"]["items"].as<JSON::Array>();
    for (auto &item: loadedItems) {
        ItemType t = static_cast<ItemType>(item["type"].as<int>());
        int number = item["number"].as<int>();
        addItems(t, number);
    }
    auto loadedModifiers = from["inventory"]["modifiers"].as<JSON::Array>();
    for (auto &mod: loadedModifiers) {
        ModifierType t = static_cast<ModifierType>(mod["type"].as<int>());
        switch (t) {
            case CIRCLE_OF_LIGHT:
                float lifetime = mod["lifetime"].as<float>();
                float max = mod["max"].as<float>();
                int tile = mod["tile"].as<int>();
                addModifier<TorchModifier>(lifetime, tile);
                Manager::instance().addTimer(tile, (max - lifetime), max);
                break;
        }
    }
}

void Inventory::resetAll() {
    this->slots.clear();
    this->modifiers.clear();
    this->slots.resize(MAX_SLOTS);
}

bool Inventory::hasModifier(ModifierType type) {
    for (auto &m: modifiers) {
        if (m->type == type) return true;
    }
    return false;
}

void Inventory::update(float dt) {
    modifiers.erase(std::remove_if(modifiers.begin(), modifiers.end(), [](std::shared_ptr<Modifier> &m) {
        return !m->running();
    }), modifiers.end());

    for (auto &m: modifiers) {
        m->update(dt);
    }
}

void Inventory::next() {
    selectedSlot++;
    selectedSlot %= MAX_SLOTS;
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
    for (auto &slot: slots) {
        if (slot.type == item->type && slot.number < MAX_PER_SLOT) {
            slot.number++;
            return true;
        }
    }
    return addSingleSlotItem(item);
}

bool Inventory::addSingleSlotItem(std::shared_ptr<Item> item) {
    for (auto &slot: slots) {
        if (slot.type == EMPTY_SLOT) {
            slot.type = item->type;
            slot.number = 1;
            slot.item = item;
            return true;
        }
    }
    return false;
}

void Inventory::addItems(ItemType type, int number) {
    if (type == EMPTY_SLOT) return;
    for (auto &slot: slots) {
        if (slot.type == EMPTY_SLOT) {
            slot.type = type;
            slot.number = number;
            slot.item = Item::make(type);
            return;
        }
    }
}

void Inventory::drop() {
    if (this->slots.at(selectedSlot).type == EMPTY_SLOT) return;

    // Don't drop items when they're equipped
    if (this->slots.at(selectedSlot).item && this->slots.at(selectedSlot).item->equipped) {
        return;
    }


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
        if (slot.number > 0) {
            bool used = slot.item->use(RT_Context.getPlayer());
            if (!used) return;

            if (slot.number > 1) {
                slot.number--;
            } else {
                slot.item.reset();
                slot.type = EMPTY_SLOT;
                slot.number = 0;
            }
        }
    }
}

float Inventory::getCircleOfLight() {
    float base = 512.0f;
    if (!modifiers.empty()) {
        for (auto &m: modifiers) {
            if (m->type == CIRCLE_OF_LIGHT) {
                base = m->modify(base);
            }
        }
    }
    return base;
}

uint8_t Inventory::getAlphaForTileAt(Position &p) {
    auto t = parent.getComponent<Transform>();
    float d = std::abs(t->p.distance(p));
    float a = getCircleOfLight() / (d * d * d);
    a = std::min<float>(a, 255);
    return static_cast<uint8_t>(a);
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

    for (auto &slot: slots) {
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
            Gfx::pickText(source, Text::fromChar('E'), font->w);

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