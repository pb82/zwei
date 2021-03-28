#include "Tile.h"
#include "Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Acceleration.h"

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
    if ((hints & HINT_LINE_OF_SIGHT) == HINT_LINE_OF_SIGHT) {
        auto playerTransform = RT_Player->getComponent<Transform>();
        auto playerCollider = RT_Player->getComponent<Collider>();
        auto tileTransform = this->parent.getComponent<Transform>();

        path.clear();

        Position from = tileTransform->p;
        Position to = playerTransform->p;

        to.x += playerTransform->padding.left / 2;
        to.y += playerTransform->padding.top / 2;

        to.nearestTile(to);

        if (RT_Topology.accessible(to.x, to.y) == false) {
            float angle = playerTransform->p.angle(to);
            Vector v(0, angle);
            Direction d = v.getDirection();
            switch (d) {
                case N:
                    to.y += 1;
                    break;
                case S:
                    to.y -= 1;
                    break;
                case E:
                    to.x -= 1;
                    break;
                case W:
                    to.x += 1;
                default:
                    break;
            }
        }

        from.nearestTile(from);
        from.bresenham(to, path);

        if (!RT_Topology.allAccessible(path, from, to)) {
            return;
        }
    }

    // Tilemap rect
    SDL_Rect source;
    pick(source);

    // Screen rect
    SDL_Rect target;
    auto transform = parent.getComponent<Transform>();

    RT_Camera.project(target, transform->p.x, transform->p.y);
    if (!RT_Camera.visible(target)) {
        return;
    }

    Draw::instance().draw(texture->mem, source, target);
}