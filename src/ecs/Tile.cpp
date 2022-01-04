#include <cmath>

#include "Tile.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Acceleration.h"
#include "Stats.h"

#include "../Gfx.h"
#include "../Draw.h"

Tile::Tile(Entity &parent, Asset id)
        : Component(parent),
          id(id) {
    texture = Assets::instance().getTexture(id);
}


void Tile::pick(SDL_Rect &source) {
    auto animation = parent.getComponent<Animation>();

    // Tile position in the tilemap
    int scalar = animation->getCurrentFrame();
    Gfx::pick(source, scalar, texture->w);
}

void Tile::render(uint8_t hints) {
    // Tilemap rect
    SDL_Rect source;
    pick(source);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();

    if ((hints & HINT_PARALLAX_LAYER) == HINT_PARALLAX_LAYER) {
        RT_Camera.projectParallax(target, transform->p.x, transform->p.y);
    } else {
        RT_Camera.project(target, transform->p.x, transform->p.y);
    }

    if (!RT_Camera.visible(target)) {
        return;
    }

    if ((hints & HINT_TURN_LIGHTS_OUT) == HINT_TURN_LIGHTS_OUT) {
        auto t = this->parent.getComponent<Transform>();
        uint8_t alpha = RT_Player->getComponent<Stats>()->inventory.getAlphaForTileAt(t->p);
        Draw::instance().pushAlpha(texture->mem);
        SDL_SetTextureAlphaMod(texture->mem, alpha);
        Draw::instance().draw(texture->mem, source, target);
        Draw::instance().popAlpha();
    } else {
        Draw::instance().draw(texture->mem, source, target);
    }
}