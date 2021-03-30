#include <ASSETS/Assets.h>
#include "Collectable.h"
#include "Transform.h"
#include "../Gfx.h"
#include "../Draw.h"
#include "SelfDestruct.h"
#include "Acceleration.h"
#include "Bloat.h"
#include "Stats.h"

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

    auto stats = RT_Context.getPlayer()->getComponent<Stats>();

    // try to pick up the item
    if (stats->inventory.add(this->item)) {
        this->collected = true;
        this->parent.addComponent<SelfDestruct>(TIMER, 500);
        this->parent.addComponent<Acceleration>(4.0f, VM_50_PI);
        this->parent.addComponent<Bloat>();
        this->parent.getComponent<Acceleration>()->accelerate();
    }
}