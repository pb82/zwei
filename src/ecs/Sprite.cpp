#include "Sprite.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Acceleration.h"

#include "../src/Gfx.h"
#include "../src/Draw.h"
#include "Stats.h"

Sprite::Sprite(Entity &parent, Asset id)
        : Component(parent),
          assetId(id) {
    texture = Assets::instance().getTexture(assetId);
}

void Sprite::serialize(JSON::Value &to) {
    to["assetId"] = this->assetId;
}

float Sprite::pick(SDL_Rect &source) {
    auto animation = parent.getComponent<Animation>();

    // Tile position in the tilemap
    Direction d = N;

    if (parent.hasComponent<Acceleration>()) {
        auto acceleration = parent.getComponent<Acceleration>();
        d = acceleration->getDirection();
    }

    int scalar = animation->getCurrentFrame(d);
    Gfx::pick(source, scalar, texture->w);
    return animation->rotate;
}

void Sprite::render(uint8_t hints) {
    // Tilemap rect
    SDL_Rect source;
    auto rotate = pick(source);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();

    // On screen?
    RT_Camera.project(target, transform->p.x, transform->p.y, transform->padding);
    if (!RT_Camera.visible(target)) {
        return;
    }

    // Render filter?
    if (!filters.empty()) filters.front()->render(texture->mem, &transform->p);

    // Draw
    if ((hints & HINT_TURN_LIGHTS_OUT) == HINT_TURN_LIGHTS_OUT) {
        auto t = this->parent.getComponent<Transform>();
        uint8_t alpha = RT_Player->getComponent<Stats>()->inventory.getAlphaForTileAt(t->p);
        Draw::instance().pushAlpha(texture->mem);
        SDL_SetTextureAlphaMod(texture->mem, alpha);
        Draw::instance().draw(texture->mem, source, target, rotate);
        SDL_SetTextureColorMod(texture->mem, 255, 255, 255);
        Draw::instance().popAlpha();
    } else {
        Draw::instance().draw(texture->mem, source, target, rotate);
        SDL_SetTextureColorMod(texture->mem, 255, 255, 255);
    }
}

void Sprite::update(float dt) {
    if (!filters.empty() && !filters.front()->upate(dt)) {
        filters.pop();
    }
}

void Sprite::addFilter(std::shared_ptr<Filter> f) {
    if (filters.empty()) filters.push(f);
}