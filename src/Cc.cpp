#include <ASSETS/Assets.h>
#include <sstream>

#include "Cc.h"
#include "Draw.h"
#include "Font.h"
#include "Gfx.h"
#include "../config.h"
#include "alg/Text.h"
#include "Rt.h"
#include "ecs/Ai.h"
#include "ecs/Npc.h"

static const int BUBBLE_MAX_LINES = 3;

ScreenTransition::ScreenTransition(SceneType newScene) : newScene(newScene) {}

void ScreenTransition::update(float dt) {
    frames += 8;
    if (frames > 255 && switched == false) {
        RT_Context.setActiveScene(newScene);
        RT_Context.autosave();
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

// -- SpeechBubble (TTF-based) --

int SpeechBubble::getWrapWidth() {
    int margin = static_cast<int>(8 * configZoomFactor);
    int inset = static_cast<int>(4 * configZoomFactor);
    return configWindowWidth - (margin * 2) - (inset * 2);
}

int SpeechBubble::getLineHeight() {
    return Font::instance().lineHeight();
}

SpeechBubble::SpeechBubble(const char *text, bool last) : text(text), last(last) {
    buildTexture();
}

SpeechBubble::~SpeechBubble() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
    }
}

void SpeechBubble::buildTexture() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    int wrapWidth = getWrapWidth();

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(Font::instance().get(), text.c_str(), white, wrapWidth);
    if (!surface) return;

    textTexture = SDL_CreateTextureFromSurface(Gfx_Renderer, surface);
    textW = surface->w;
    textH = surface->h;
    SDL_FreeSurface(surface);
}

void SpeechBubble::split(const char *text, std::vector<std::shared_ptr<SpeechBubble>> &target) {
    int wrapWidth = getWrapWidth();

    // Split input into words
    std::istringstream stream(text);
    std::string word;
    std::vector<std::string> words;
    while (stream >> word) {
        words.push_back(word);
    }

    // Build bubbles of BUBBLE_MAX_LINES lines each by measuring
    // actual rendered height via TTF
    TTF_Font *f = Font::instance().get();
    int maxH = BUBBLE_MAX_LINES * TTF_FontLineSkip(f);
    std::string current;

    for (size_t i = 0; i < words.size(); i++) {
        std::string candidate = current.empty() ? words[i] : current + " " + words[i];

        // Measure actual rendered height
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(f, candidate.c_str(), white, wrapWidth);
        int h = surface ? surface->h : 0;
        if (surface) SDL_FreeSurface(surface);

        if (h > maxH && !current.empty()) {
            // Would exceed max lines — flush current bubble
            target.emplace_back(std::make_shared<SpeechBubble>(current.c_str(), false));
            current = words[i];
        } else {
            current = candidate;
        }
    }

    if (!current.empty()) {
        target.emplace_back(std::make_shared<SpeechBubble>(current.c_str(), true));
    }

    // Mark the last bubble
    if (!target.empty()) {
        target.back()->last = true;
    }
}

void SpeechBubble::key(GameKeyEvent &ev) {
    if (!ev.valid) return;
    if (ev.state != GK_PUSHED) return;
    if (ev.key == GK_A || ev.key == GK_B) {
        if (visibleChars < (int)text.size()) {
            visibleChars = text.size();
        } else {
            this->read = true;
        }
    }
}

void SpeechBubble::update(float dt) {
    time += dt;
    charTimer += dt;
    if (charTimer > 30.0f) {
        if (visibleChars < (int)text.size()) visibleChars++;
        charTimer = 0;
    }
}

bool SpeechBubble::done() {
    return read;
}

void SpeechBubble::render() {
    int margin = static_cast<int>(8 * configZoomFactor);
    int inset = static_cast<int>(4 * configZoomFactor);
    int bubbleH = BUBBLE_MAX_LINES * getLineHeight() + (inset * 2);

    // Background box: full width minus margin, at top of screen
    SDL_Rect background;
    background.x = margin;
    background.y = margin;
    background.w = configWindowWidth - (margin * 2);
    background.h = bubbleH;

    Draw::instance().box(color_Bubble, background);

    if (!textTexture) return;

    if (visibleChars < (int)text.size() && visibleChars > 0) {
        std::string partial = text.substr(0, visibleChars);
        int wrapWidth = background.w - (inset * 2);
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(Font::instance().get(), partial.c_str(), white, wrapWidth);
        if (surface) {
            SDL_Texture *partialTex = SDL_CreateTextureFromSurface(Gfx_Renderer, surface);
            SDL_Rect target;
            target.x = background.x + inset;
            target.y = background.y + inset;
            target.w = surface->w;
            target.h = surface->h;
            SDL_RenderCopy(Gfx_Renderer, partialTex, nullptr, &target);
            SDL_DestroyTexture(partialTex);
            SDL_FreeSurface(surface);
        }
    } else if (visibleChars >= (int)text.size()) {
        SDL_Rect target;
        target.x = background.x + inset;
        target.y = background.y + inset;
        target.w = textW;
        target.h = textH;
        SDL_RenderCopy(Gfx_Renderer, textTexture, nullptr, &target);
    }

    // "More" indicator
    if (!last) {
        int indicatorSize = static_cast<int>(8 * configZoomFactor);
        SDL_Rect indicator;
        indicator.x = background.x + background.w - inset - indicatorSize;
        indicator.y = background.y + background.h - inset - indicatorSize;
        indicator.w = indicatorSize;
        indicator.h = indicatorSize;

        if (visibleChars >= (int)text.size()) {
            bool show = static_cast<int>(time / 500) % 2 == 0;
            if (show) {
                Color white{255, 255, 255, 255};
                Draw::instance().box(white, indicator);
            }
        }
    }
}

// -- NpcCommand --

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
