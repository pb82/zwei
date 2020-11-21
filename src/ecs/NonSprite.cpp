#include "NonSprite.h"
#include "Entity.h"
#include "Animation.h"
#include "Acceleration.h"
#include "Collider.h"
#include "../src/Gfx.h"
#include "../src/Draw.h"
#include "../Debug.h"

NonSprite::NonSprite(Entity &parent) : Component(parent) {}

void NonSprite::render() {
    auto texture = Assets::instance().getTexture(TILES);

    if (Debug::drawBoundingBoxes) {
        if (parent.hasComponent<Collider>()) {
            auto collider = parent.getComponent<Collider>();
            SDL_Rect source;
            Gfx::pick(source, 53, texture->w);
            Draw::instance().draw(texture->mem, source, collider->boundingBox);
        }
    }
}

void NonSprite::update(float dt) {
}