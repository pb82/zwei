#include <ASSETS/Assets.h>

#include "Hud.h"
#include "Entity.h"
#include "../Rt.h"
#include "Stats.h"
#include "../alg/Text.h"
#include "../Gfx.h"
#include "../Draw.h"


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
    int cur = std::get<0>(hp);
    int max = std::get<1>(hp);

    // render cur
    int h, t, o;
    fastNums(cur, &h, &t, &o);

    SDL_Rect target;
    target.x = 10;
    target.y = 10;
    target.w = 32;
    target.h = 32;

    SDL_Rect source;
    // Hp symbol
    Gfx::pick(source, 90, texture->w);
    Draw::instance().draw(texture->mem, source, target);
    target.x += 24;

    // Current Hp
    fastNums(cur, &h, &t, &o);
    renderNumber(target, cur, h, t, o);

    // Divider
    Gfx::pick(source, 89, texture->w);
    Draw::instance().draw(texture->mem, source, target);
    target.x += 24;

    // Max hp
    fastNums(max, &h, &t, &o);
    renderNumber(target, max, h, t, o);

    // Experience
    target.y += 32;
    target.x = 10;

    // Exp symbol
    Gfx::pick(source, 91, texture->w);
    Draw::instance().draw(texture->mem, source, target);
    target.x += 24;

    auto exp = stats->character.getExperience();
    cur = std::get<0>(exp);
    max = std::get<1>(exp);

    // Current exp
    fastNums(cur, &h, &t, &o);
    renderNumber(target, cur, h, t, o);

    // Divider
    Gfx::pick(source, 89, texture->w);
    Draw::instance().draw(texture->mem, source, target);
    target.x += 24;

    // Next level
    fastNums(max, &h, &t, &o);
    renderNumber(target, max, h, t, o);


}

void Hud::renderNumber(SDL_Rect &target, int num, int h, int t, int o) {
    SDL_Rect source;
    if (h > 0) {
        Gfx::pick(source, Text::fromInt(h), texture->w);
        Draw::instance().draw(texture->mem, source, target);
    }
    target.x += 24;

    if (t > 0 || num >= 100) {
        Gfx::pick(source, Text::fromInt(t), texture->w);
        Draw::instance().draw(texture->mem, source, target);
    }
    target.x += 24;

    Gfx::pick(source, Text::fromInt(o), texture->w);
    Draw::instance().draw(texture->mem, source, target);
    target.x += 24;
}