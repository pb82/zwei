#include "Lighting.h"

#include <cmath>
#include <algorithm>

#include "Rt.h"
#include "Draw.h"
#include "ecs/Manager.h"

int Lighting::addDynamic(Position *pos, float radius, float lifetime, int tile) {
    LightSource src;
    src.tracked = pos;
    src.radius = radius;
    src.lifetime = lifetime;
    src.maxLifetime = lifetime;
    src.flickerPhase = 0;
    src.tile = tile;
    src.id = nextId++;
    src.dynamic = true;
    src.persistent = true;
    sources.push_back(src);
    return src.id;
}

int Lighting::addStatic(float x, float y, float radius, float lifetime, int tile) {
    LightSource src;
    src.fixed = Position(x, y);
    src.tracked = nullptr;
    src.radius = radius;
    src.lifetime = lifetime;
    src.maxLifetime = lifetime;
    src.flickerPhase = 0;
    src.tile = tile;
    src.id = nextId++;
    src.dynamic = false;
    src.persistent = false;
    sources.push_back(src);
    return src.id;
}

void Lighting::remove(int id) {
    sources.erase(std::remove_if(sources.begin(), sources.end(),
        [id](const LightSource &s) { return s.id == id; }), sources.end());
}

void Lighting::clear() {
    sources.erase(std::remove_if(sources.begin(), sources.end(),
        [](const LightSource &s) { return !s.persistent; }), sources.end());
}

void Lighting::clearAll() {
    sources.clear();
}

void Lighting::update(float dt) {
    for (auto &src : sources) {
        src.flickerPhase += dt;
        if (src.maxLifetime > 0) {
            src.lifetime -= dt;
        }
    }

    // Remove expired timed lights
    sources.erase(std::remove_if(sources.begin(), sources.end(),
        [](const LightSource &s) {
            return s.maxLifetime > 0 && s.lifetime <= 0;
        }), sources.end());
}

float Lighting::computeRadius(const LightSource &src) {
    float r = src.radius;
    float base = r * 3;
    base += std::abs(1024.0f * std::sin(src.flickerPhase / 200.0f));
    base = std::max(r * 2, base);
    return std::min(base, r * 5);
}

uint8_t Lighting::getAlphaAt(Position &p) {
    float maxAlpha = 0.0f;
    for (auto &src : sources) {
        Position &srcPos = src.getPosition();
        float d = std::abs(srcPos.distance(p));
        float r = computeRadius(src);
        float a = r / (d * d * d);
        maxAlpha = std::max(maxAlpha, a);
    }
    return static_cast<uint8_t>(std::min(maxAlpha, 255.0f));
}

void Lighting::renderDarkness() {
    if (!(Manager::instance().getRenderHints() & HINT_TURN_LIGHTS_OUT)) return;

    int tx0, ty0, tx1, ty1;
    RT_Camera.getVisibleTileRange(tx0, ty0, tx1, ty1);

    Color black{0, 0, 0, 0};

    for (int ty = ty0; ty <= ty1; ty++) {
        for (int tx = tx0; tx <= tx1; tx++) {
            SDL_Rect target;
            RT_Camera.project(target, (float)tx, (float)ty);

            Position p((float)tx, (float)ty);
            uint8_t brightness = sources.empty() ? 0 : getAlphaAt(p);
            uint8_t darkness = 255 - brightness;

            if (darkness > 0) {
                Draw::instance().box(black, target, darkness);
            }
        }
    }
}

bool Lighting::hasActiveTorch() {
    for (auto &src : sources) {
        if (src.dynamic && src.maxLifetime > 0) return true;
    }
    return false;
}

void Lighting::serialize(JSON::Value &to) {
    JSON::Array lights;
    for (auto &src : sources) {
        if (!src.dynamic || src.maxLifetime <= 0) continue;
        JSON::Object o;
        o["lifetime"] = src.lifetime;
        o["max"] = src.maxLifetime;
        o["tile"] = src.tile;
        o["radius"] = src.radius;
        lights.push_back(o);
    }
    to["lighting"] = lights;
}

void Lighting::deserialize(JSON::Value &from, Position *playerPos) {
    if (!from["lighting"].is(JSON::JSON_ARRAY)) return;
    auto loaded = from["lighting"].as<JSON::Array>();
    for (auto &light : loaded) {
        float lifetime = light["lifetime"].as<float>();
        float max = light["max"].as<float>();
        int tile = light["tile"].as<int>();
        float radius = light["radius"].as<float>();

        LightSource src;
        src.tracked = playerPos;
        src.radius = radius;
        src.lifetime = lifetime;
        src.maxLifetime = max;
        src.flickerPhase = max - lifetime;
        src.tile = tile;
        src.id = nextId++;
        src.dynamic = true;
        src.persistent = true;
        sources.push_back(src);

        Manager::instance().addTimer(tile, (max - lifetime), max);
    }
}
