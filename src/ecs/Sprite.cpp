#include "Sprite.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Acceleration.h"
#include "Collider.h"

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
    Direction d = acceleration->trajectory.getDirection();
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

    RT_Camera.project(target, transform->p.x, transform->p.y);
    if (!RT_Camera.visible(target)) {
        return;
    }

    Draw::instance().draw(texture->mem, source, target);

    if (Debug::drawBoundingBoxes) {
        if (parent.hasComponent<Collider>()) {
            auto collider = parent.getComponent<Collider>();
            Draw::instance().rect(color_Red, collider->boundingBox);
            // SDL_Rect source;
            // Gfx::pick(source, 53, texture->w);
            // Draw::instance().draw(texture->mem, source, collider->boundingBox);
        }
    }
}

void Sprite::update(float dt) {
    auto acceleration = parent.getComponent<Acceleration>();
    acceleration->accelerate(dt);
}