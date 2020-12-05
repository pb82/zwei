#include "Sprite.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Acceleration.h"

#include "../src/Gfx.h"
#include "../src/Draw.h"
#include "../Debug.h"

Sprite::Sprite(Entity &parent, Asset id)
        : Component(parent),
          assetId(id) {}

void Sprite::pick(SDL_Rect &source) {
    auto texture = Assets::instance().getTexture(assetId);
    auto animation = parent.getComponent<Animation>();
    auto acceleration = parent.getComponent<Acceleration>();

    // Tile position in the tilemap
    Direction d = acceleration->getDirection();
    int scalar = animation->getCurrentFrame(d);
    Gfx::pick(source, scalar, texture->w);
}

void Sprite::render() {
    // Tilemap rect
    SDL_Rect source;
    pick(source);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();
    auto texture = Assets::instance().getTexture(assetId);

    // On screen?
    RT_Camera.project(target, transform->p.x, transform->p.y);
    if (!RT_Camera.visible(target)) {
        return;
    }

    // Render filter?
    if (!filters.empty()) filters.front()->render(texture->mem, &transform->p);

    // Draw
    Draw::instance().draw(texture->mem, source, target);
    SDL_SetTextureColorMod(texture->mem, 255, 255, 255);
}

void Sprite::update(float dt) {
    if (!filters.empty() && !filters.front()->upate(dt)) {
        filters.pop();
    }
}

void Sprite::addFilter(std::shared_ptr<Filter> f) {
    filters.push(f);
}