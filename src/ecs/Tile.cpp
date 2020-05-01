#include <cassert>
#include <algorithm>

#include "Tile.h"
#include "Entity.h"
#include "Transform.h"

#include "../src/Gfx.h"
#include "../src/Draw.h"

Tile::Tile(Entity &parent, Asset id, int tile)
        : Component(parent),
          animationSpeed(1000.0f),
          id(id) {
    tiles.push_back(tile);
    currentTile = tile;
}

void Tile::update(float dt) {
    animationCount += dt;
    currentTile = tiles.at(long(animationCount / animationSpeed) % tiles.size());
}

void Tile::addAnimationFrame(int tile) {
    tiles.push_back(tile);
}

void Tile::setAnimationSpeed(float ms) {
    this->animationSpeed = std::max(50.0f, ms);
}

void Tile::pick(SDL_Rect &source) {
    auto texture = Assets::instance().getTexture(id);

    // Tile position in the tilemap
    int scalar = currentTile - 1;
    source.x = (scalar * Gfx_Tile_Size) % texture->w;
    source.y = (scalar * Gfx_Tile_Size) / texture->w * Gfx_Tile_Size;
    source.w = Gfx_Tile_Size;
    source.h = Gfx_Tile_Size;
}

void Tile::render() {
    assert(parent.hasComponent<Transform>());

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
}