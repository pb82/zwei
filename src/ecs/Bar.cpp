#include "Bar.h"

#include "Entity.h"
#include "Stats.h"
#include "Transform.h"

#include "../Draw.h"
#include "Attack.h"

Bar::Bar(Entity &parent) : Component(parent) {}

void Bar::setVisibility(float duration) {
    this->visible = duration;
}

void Bar::update(float dt) {
    if (visible <= 0) {
        visible = 0.0f;
    }
    visible -= dt;
}

void Bar::render(uint8_t) {
    if (visible <= 0.0f) {
        return;
    }

    auto stats = this->parent.getComponent<Stats>();
    auto transform = this->parent.getComponent<Transform>();
    auto hp = stats->character.getHitpoints();

    float cur = std::get<0>(hp);
    float max = std::get<1>(hp);
    float percent = cur / max;

    SDL_Rect target;
    Padding p = {0, 0, -0.25, 2};
    p.right = 2 - (2 * percent);

    RT_Camera.project(target, transform->p.x, transform->p.y, p);

    if (percent >= 0.5) {
        Draw::instance().box(color_EnemyGood, target);
    } else if (percent >= 0.2) {
        Draw::instance().box(color_EnemyMedium, target);
    } else {
        Draw::instance().box(color_EnemyBad, target);
    }

    if (stats->inventory.hasWeapon() && this->parent.hasComponent<Attack>()) {
        auto attack = this->parent.getComponent<Attack>();
        percent = 1.0f;

        if (attack->wait > 0.0f) {
            percent *= (attack->wait / stats->inventory.weapon->recharge());
        }

        p.top = -0.5;
        p.bottom = 2.25;
        p.left = 0;
        p.right = 2 - (2 * percent);
        RT_Camera.project(target, transform->p.x, transform->p.y, p);
        Draw::instance().box(color_EnemyBlue, target);
    }
}