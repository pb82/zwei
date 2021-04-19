#include "Timer.h"

#include <algorithm>

#include <ASSETS/Assets.h>

#include "../../config.h"
#include "../Draw.h"
#include "../Gfx.h"

Timer::Timer(Entity &parent) : Component(parent) {}

void Timer::update(float dt) {
    if (timers.empty()) return;

    for (auto &t : timers) {
        t.update(dt);
    }

    timers.erase(std::remove_if(timers.begin(), timers.end(), [](TimerValue &t) {
        return !t.running();
    }), timers.end());

    std::sort(timers.begin(), timers.end(), [](TimerValue &a, TimerValue &b) {
        float pa = a.cur / a.max;
        float pb = b.cur / b.max;
        return pb < pa;
    });
}

void Timer::addTimer(int tile, float max) {
    timers.emplace_back(tile, max);
}

void Timer::addTimer(int tile, float cur, float max) {
    timers.emplace_back(tile, cur, max);
}

void Timer::render(uint8_t) {
    SDL_Rect target;
    SDL_Rect bar;

    auto texture = Assets::instance().getTexture(SPRITES);

    int y = 16;
    for (auto &t : timers) {
        target.w = (configWindowWidth / 5) + 2;
        target.h = 20;
        target.x = 10 + configWindowWidth - target.w - 32;
        target.y = y;

        SDL_Rect icon;
        icon = target;
        icon.w = 32;
        icon.h = 32;
        icon.x -= 32;
        icon.y -= 6;

        SDL_Rect source;
        // Hp symbol
        Gfx::pick(source, t.tile, texture->w);
        Draw::instance().draw(texture->mem, source, icon);

        float percent = 1.0f;
        if (t.cur > 0.0f) {
            percent -= t.cur / t.max;
        }

        bar = target;
        bar.x += 1;
        bar.y += 1;
        bar.h -= 2;
        bar.w = ((configWindowWidth / 5) * percent);
        Draw::instance().rect(color_White, target);
        Draw::instance().box(color_Blue, bar);
        y += 32;
    }
}