#include <ASSETS/Assets.h>

#include "Cc.h"
#include "Draw.h"
#include "Gfx.h"
#include "../config.h"
#include "alg/Text.h"

SpeechBubble::SpeechBubble(const char *text, bool last) : text(text), last(last) {}

void SpeechBubble::key(GameKeyEvent &ev) {
    if (!ev.valid) return;
    if (ev.state != GK_PUSHED) return;
    if (ev.key == GK_A) {
        this->read = true;
    }
}

void SpeechBubble::update(float dt) {
    time += dt;
    if (time >= 500) {
        tick = !tick;
        time = 0;
    }
}

bool SpeechBubble::done() {
    return read;
}

void SpeechBubble::render() {
    SDL_Rect background;

    background.x = (configWindowWidth / 2) - 400;
    background.y = 4;
    background.w = 800;
    background.h = 240;

    Draw::instance().box(color_Bubble, background);

    sequence.clear();
    Text::toSequence(text, sequence);

    SDL_Rect letter;
    letter.x = background.x + 4;
    letter.y = background.y + 4;
    letter.w = 14;
    letter.h = 18;

    SDL_Rect source;
    auto font = Assets::instance().getTexture(BITMAPFONT);

    for (int c: sequence) {
        // Newline
        if (c == -1) {
            letter.y += 18;
            letter.x = background.x + 4;
            continue;
        }
        Gfx::pickText(source, c, font->w);
        Draw::instance().draw(font->mem, source, letter);
        letter.x += 14;
    }

    letter.x = background.x + 784;
    letter.y = background.y + 222;
    letter.w = 14;
    letter.h = 18;

    if (!last) {
        if (tick) {
            Gfx::pickText(source, 95, font->w);
            Draw::instance().draw(font->mem, source, letter);
        } else {
            Gfx::pickText(source, 0, font->w);
            Draw::instance().draw(font->mem, source, letter);
        }
    }
}