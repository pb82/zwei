#include "Sprite.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Acceleration.h"

#include "../src/Gfx.h"
#include "../src/Draw.h"
#include "../alg/Color.h"

Sprite::Sprite(Entity &parent, Asset id, float angle)
        : Component(parent),
          assetId(id),
          trajectory(0, angle) {}

void Sprite::pick(SDL_Rect &source) {
    auto texture = Assets::instance().getTexture(assetId);
    auto animation = parent.getComponent<Animation>();

    // Tile position in the tilemap
    Direction d = trajectory.getDirection();
    int scalar = animation->getCurrentFrame(d);
    source.x = (scalar * Gfx_Tile_Size) % texture->w;
    source.y = (scalar * Gfx_Tile_Size) / texture->w * Gfx_Tile_Size;
    source.w = Gfx_Tile_Size;
    source.h = Gfx_Tile_Size;
}

void Sprite::render() {
    // Tilemap rect
    SDL_Rect source;
    pick(source);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();
    auto texture = Assets::instance().getTexture(assetId);

    RT_Camera.project(target, transform->p.x, transform->p.y);
    if (!RT_Camera.visible(target)) {
        return;
    }

    Draw::instance().draw(texture->mem, source, target);
}

void Sprite::update(float dt) {
    auto transform = parent.getComponent<Transform>();
    auto acceleration = parent.getComponent<Acceleration>();
    acceleration->accelerate(dt);

    trajectory.radius = acceleration->speed * (dt / 1000);
    trajectory.translate(&transform->p.x, &transform->p.y);
}