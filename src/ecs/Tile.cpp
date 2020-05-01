#include <cassert>

#include "Tile.h"
#include "Entity.h"
#include "Transform.h"

#include "../src/Gfx.h"
#include "../src/Draw.h"

void Tile::pick(SDL_Rect &source) {
    auto texture = Assets::instance().getTexture(id);

    // Tile position in the tilemap
    int scalar = pos - 1;
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