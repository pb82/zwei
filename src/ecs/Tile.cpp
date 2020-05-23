#include <cassert>
#include <algorithm>

#include "Tile.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"

#include "../Gfx.h"
#include "../Draw.h"
#include "../Debug.h"

Tile::Tile(Entity &parent, Asset id)
        : Component(parent),
          id(id) {}


void Tile::pick(SDL_Rect &source) {
    auto texture = Assets::instance().getTexture(id);
    auto animation = parent.getComponent<Animation>();

    // Tile position in the tilemap
    int scalar = animation->getCurrentFrame();
    Gfx::pick(source, scalar, texture->w);
}

void Tile::render() {
    // Tilemap rect
    SDL_Rect source;
    pick(source);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();
    auto texture = Assets::instance().getTexture(id);

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