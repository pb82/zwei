#include <ASSETS/Assets.h>
#include "Collectable.h"
#include "Transform.h"
#include "../Gfx.h"
#include "SelfDestruct.h"
#include "Stats.h"
#include "Id.h"
#include "../Draw.h"

Collectable::Collectable(Entity &parent, std::shared_ptr<Item> item) : Component(parent), item(item) {}

void Collectable::render(uint8_t hints) {
    auto texture = Assets::instance().getTexture(SPRITES);

    // Tilemap rect
    SDL_Rect source;
    Gfx::pick(source, item->tile(), texture->w);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();

    // On screen?
    RT_Camera.project(target, transform->p.x, transform->p.y, transform->padding);
    if (!RT_Camera.visible(target)) {
        return;
    }

    if ((hints & HINT_TURN_LIGHTS_OUT) == HINT_TURN_LIGHTS_OUT) {
        auto t = this->parent.getComponent<Transform>();
        uint8_t a = RT_Player->getComponent<Stats>()->inventory.getAlphaForTileAt(t->p);
        Draw::instance().pushAlpha(texture->mem);
        SDL_SetTextureAlphaMod(texture->mem, a + alpha);
        Draw::instance().draw(texture->mem, source, target);
        Draw::instance().popAlpha();
    } else {
        Draw::instance().pushAlpha(texture->mem);
        SDL_SetTextureAlphaMod(texture->mem, alpha);
        Draw::instance().draw(texture->mem, source, target);
        Draw::instance().popAlpha();
    }
}

void Collectable::collect() {
    if (this->collected) return;

    if (this->_onCollect) {
        _onCollect();
    }

    if (this->item->useOnPickup()) {
        this->item->use(RT_Context.getPlayer());
        this->collected = true;
        this->parent.disable();
        return;
    }

    auto stats = RT_Context.getPlayer()->getComponent<Stats>();

    // try to pick up the item
    if (stats->inventory.add(this->item)) {
        this->collected = true;
        this->parent.disable();
        if (this->parent.hasComponent<Id>()) {
            auto id = this->parent.getComponent<Id>();
            RT_Memory.addToArray("collectedItems", id->id);
        }

        std::string s;
        if (this->item->notification(s)) {
            Manager::instance().addTimer(0, 5000, s.c_str());
        }
    }
}