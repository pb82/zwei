#include "Timer.h"

#include <algorithm>

#include <ASSETS/Assets.h>
#include <SDL_ttf.h>

#include "../../config.h"
#include "../Draw.h"
#include "../Font.h"
#include "../Gfx.h"
#include "../alg/Text.h"

Timer::Timer(Entity &parent) : Component(parent) {}

TimerValue::TimerValue(float cur, float max, std::string &&text) {
    this->tile = 0;
    this->max = max;
    this->cur = cur;
    this->text = std::move(text);
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
        target.w = (configWindowWidth / 5) + 4;
        target.h = 40;
        target.x = 10 + configWindowWidth - target.w - 64;
        target.y = y;

        SDL_Rect icon;
        icon = target;
        icon.w = 64;
        icon.h = 64;
        icon.x -= 64;
        icon.y -= 12;

        SDL_Rect source;
        // Tile
        if (t.text.empty()) {
            Gfx::pick(source, t.tile, texture->w);
            Draw::instance().draw(texture->mem, source, icon);
        } else {
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface *surface = TTF_RenderUTF8_Blended(Font::instance().get(), t.text.c_str(), white);
            if (surface) {
                SDL_Texture *tex = SDL_CreateTextureFromSurface(Gfx_Renderer, surface);
                SDL_Rect textTarget;
                textTarget.w = surface->w;
                textTarget.h = surface->h;
                textTarget.x = configWindowWidth - surface->w - 10;
                textTarget.y = y + (target.h - surface->h) / 2;
                SDL_RenderCopy(Gfx_Renderer, tex, nullptr, &textTarget);
                SDL_DestroyTexture(tex);
                SDL_FreeSurface(surface);
            }
            y += 64;
            continue;
        }

        float percent = 1.0f;
        if (t.cur > 0.0f) {
            percent -= t.cur / t.max;
        }

        bar = target;
        bar.x += 2;
        bar.y += 2;
        bar.h -= 4;
        bar.w = ((configWindowWidth / 5) * percent);
        Draw::instance().rect(color_White, target);
        Draw::instance().box(color_Blue, bar);
        y += 64;
    }
}