#include <ASSETS/Assets.h>

#include "Cc.h"
#include "Draw.h"
#include "Gfx.h"
#include "../config.h"
#include "alg/Text.h"

SpeechBubble::SpeechBubble(std::vector<int> &sequence, bool last) : sequence(sequence), last(last) {}

SpeechBubble::SpeechBubble(const char *text, bool last) : last(last) {
    Text::toSequence(text, sequence);
}

void SpeechBubble::split(const char *text, std::vector<std::shared_ptr<SpeechBubble>> &target) {
    std::vector<int> sequence;
    Text::toSequence(text, sequence);

    std::vector<int> m;
    for (int i = 0; i < sequence.size(); i++) {
        int j = sequence.at(i);
        if (j == -2) {
            target.emplace_back(std::make_shared<SpeechBubble>(m, false));
            m.clear();
            continue;
        }
        m.push_back(j);
    }

    if (!m.empty()) {
        target.emplace_back(std::make_shared<SpeechBubble>(m, true));
    }
}

void SpeechBubble::key(GameKeyEvent &ev) {
    if (!ev.valid) return;
    if (ev.state != GK_PUSHED) return;
    if (ev.key == GK_A) {
        if (this->index < this->sequence.size()) {
            this->index = this->sequence.size();
        } else {
            this->read = true;
        }
    }
}

void SpeechBubble::update(float dt) {
    time += dt;
    if (time - lastIndex > 30.0f) {
        if (index < sequence.size()) index++;
        lastIndex = time;
    }

    if (time >= 500) {
        tick = !tick;
        time = 0;
        lastIndex = 0;
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

    SDL_Rect letter;
    letter.x = background.x + 4;
    letter.y = background.y + 4;
    letter.w = 14;
    letter.h = 18;

    SDL_Rect source;
    auto font = Assets::instance().getTexture(BITMAPFONT);

    for (int i = 0; i < index; i++) {
        // Newline
        char c = sequence.at(i);
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