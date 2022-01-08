#include <ASSETS/Assets.h>

#include "Cc.h"
#include "Draw.h"
#include "Gfx.h"
#include "../config.h"
#include "alg/Text.h"
#include "Rt.h"
#include "ecs/Ai.h"
#include "ecs/Npc.h"

ScreenTransition::ScreenTransition(SceneType newScene) : newScene(newScene) {}

void ScreenTransition::update(float dt) {
    frames += 8;
    if (frames > 255 && switched == false) {
        RT_Context.setActiveScene(newScene);
        switched = true;
        frames = 0;
    } else if (frames > 255 && switched == true) {
        ready = true;
    }
}

void ScreenTransition::render() {
    r.x = 0;
    r.y = 0;
    r.w = configWindowWidth;
    r.h = configWindowHeight;

    c.r = 0;
    c.g = 0;
    c.b = 0;
    if (!switched) {
        c.a = frames;
        if (c.a > 255) c.a = 255;
    } else {
        if (frames > 255) frames = 255;
        c.a = 255 - frames;
        if (c.a > 255) c.a = 255;
    }

    Draw::instance().box(c, r);
}

bool ScreenTransition::done() {
    return ready;
}


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
    if (ev.key == GK_A || ev.key == GK_B) {
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

    bool speaker = false;
    for (int i = 0; i < index; i++) {
        // Newline
        char c = sequence.at(i);
        if (c == -1) {
            letter.y += 18;
            letter.x = background.x + 4;
            continue;
        }

        if (c == 8) speaker = true;

        Gfx::pickText(source, c, font->w);
        if (speaker) {
            Draw::instance().draw(font->mem, source, letter, color_Blue.toSdlColor());
        } else {
            Draw::instance().draw(font->mem, source, letter);
        }

        if (c == 9) speaker = false;
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

void NpcCommand::addNpc(std::shared_ptr<Entity> npc) {
    this->npcs.push_back(npc);
}

NpcCommand::NpcCommand() {}

NpcCommand::~NpcCommand() {
    for (auto npc: this->npcs) {
        npc->disable();
    }
}

bool NpcCommand::done() {
    bool done = true;
    for (auto npc: npcs) {
        auto n = npc->getComponent<Npc>();
        done = done && n->done();
    }
    return done;
}

void NpcCommand::update(float dt) {
    for (auto &npc: this->npcs) {
        npc->update(dt);
    }
}

void NpcCommand::render() {}