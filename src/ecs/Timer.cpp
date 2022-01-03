#include "Timer.h"

#include <algorithm>

#include <ASSETS/Assets.h>

#include "../../config.h"
#include "../Draw.h"
#include "../Gfx.h"
#include "../alg/Text.h"

Timer::Timer(Entity &parent) : Component(parent) {}

TimerValue::TimerValue(float cur, float max, std::string &&text) {
    this->tile = tile;
    this->max = max;
    this->cur = cur;
    Text::toSequence(text.c_str(), letters);
}

void Timer::update(float dt) {
    if (timers.empty()) return;

    for (auto &t: timers) {
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

void Timer::addTimer(float cur, float max, const char *text) {
    timers.emplace_back(cur, max, text);
}

void Timer::render(uint8_t) {
    SDL_Rect target;
    SDL_Rect bar;

    auto texture = Assets::instance().getTexture(SPRITES);

    int y = 16;
    for (auto &t: timers) {
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
        // Tile
        if (t.letters.empty()) {
            Gfx::pick(source, t.tile, texture->w);
            Draw::instance().draw(texture->mem, source, icon);
        } else {
            auto font = Assets::instance().getTexture(BITMAPFONT);
            SDL_Rect letter = target;
            letter.x = (configWindowWidth - (t.letters.size() * 14) - 10);
            letter.w = 14;
            letter.h = 18;
            for (auto c: t.letters) {
                Gfx::pickText(source, c, font->w);
                Draw::instance().draw(font->mem, source, letter);
                letter.x += 14;
            }
            y += 32;
            continue;
        }

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