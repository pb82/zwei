#include <ASSETS/Assets.h>
#include <SDL_ttf.h>

#include "Hud.h"
#include "Entity.h"
#include "../Rt.h"
#include "Stats.h"
#include "../alg/Text.h"
#include "../Gfx.h"
#include "../Draw.h"
#include "Attack.h"

#define BAR_SCREEN_RATIO 5

Hud::Hud(Entity &parent) : Component(parent) {
    this->texture = Assets::instance().getTexture(SPRITES);
}

void Hud::fastNums(int num, int *h, int *t, int *o) {
    int r = 0;
    *h = num / 100;
    r = num % 100;
    *t = r / 10;
    *o = r % 10;
}

void Hud::render() {
    auto player = RT_Context.getPlayer();
    auto stats = player->getComponent<Stats>();
    stats->inventory.render();

    // Health
    auto hp = stats->character.getHitpoints();
    float cur = std::get<0>(hp);
    float max = std::get<1>(hp);

    target.x = 10;
    target.y = 10;
    target.w = 32;
    target.h = 32;

    SDL_Rect source;
    // Hp symbol
    Gfx::pick(source, 90, texture->w);
    Draw::instance().draw(texture->mem, source, target);

    target.x += 28;
    target.h = 20;
    target.w = (configWindowWidth / BAR_SCREEN_RATIO) + 2;
    target.y = 16;

    Draw::instance().rect(color_White, target);

    float percent = cur / max;
    target.x += 1 ;
    target.y += 1;
    target.w = ((configWindowWidth / BAR_SCREEN_RATIO) * percent);
    target.h -= 2;

    if (percent >= 0.5) {
        Draw::instance().box(color_Good, target);
    } else if (percent >= 0.2) {
        Draw::instance().box(color_Medium, target);
    } else {
        Draw::instance().box(color_Bad, target);
    }

    if (!stats->inventory.hasWeapon()) {
        return;
    }

    auto attack = player->getComponent<Attack>();

    target.x = 10;
    target.y = 42;
    target.w = 32;
    target.h = 32;

    Gfx::pick(source, 107, texture->w);
    Draw::instance().draw(texture->mem, source, target);

    target.x += 28;
    target.h = 20;
    target.w = (configWindowWidth / BAR_SCREEN_RATIO) + 2;
    target.y = 48;

    Draw::instance().rect(color_White, target);

    float recharge = stats->inventory.weapon->recharge();
    float current = attack->wait;
    percent = 1.0f;

    if (current > 0.0f) {
        percent *= (current / recharge);
    }

    target.x += 1;
    target.y += 1;
    target.w = ((configWindowWidth / BAR_SCREEN_RATIO) * percent);
    target.h -= 2;

    Draw::instance().box(color_Blue, target);
}
