#include <ASSETS/Assets.h>
#include "Analytics.h"
#include "../Debug.h"
#include "../Gfx.h"
#include "../Draw.h"
#include "Entity.h"
#include "Collider.h"

Analytics::Analytics(Entity &parent) : Component(parent) {}

void Analytics::update(float dt) {}

void Analytics::render(uint8_t) {
    if (Debug::drawBoundingBoxes) {
        if (parent.hasComponent<Collider>()) {
            auto collider = parent.getComponent<Collider>();
            SDL_Rect source;

            auto texture = Assets::instance().getTexture(SPRITES);

            Gfx::pick(source, 15, texture->w);
            Draw::instance().draw(texture->mem, source, collider->boundingBox);
        }
    }
}