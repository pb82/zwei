#ifndef ZWEI_LIGHTING_H
#define ZWEI_LIGHTING_H

#include <vector>
#include <cstdint>

#include "alg/Position.h"
#include <JSON/value.h>

struct LightSource {
    Position fixed;       // Storage for static lights (used when dynamic == false)
    Position *tracked;    // Points to external Position (used when dynamic == true)
    float radius;         // Base light radius
    float lifetime;       // Remaining seconds; <= 0 means permanent
    float maxLifetime;    // Original lifetime (for serialization)
    float flickerPhase;   // Accumulated time for flicker effect
    int tile;             // UI tile icon (for timer display)
    int id;               // Unique handle for removal
    bool dynamic;         // true = follows tracked pointer, false = uses fixed
    bool persistent;      // true = survives scene clear (player torch)

    Position &getPosition() {
        return dynamic ? *tracked : fixed;
    }
};

class Lighting {
public:
    static Lighting &instance() {
        static Lighting instance;
        return instance;
    }

    Lighting(Lighting const &) = delete;
    void operator=(Lighting const &) = delete;

    // Add a dynamic light that follows a live Position pointer. Returns handle.
    int addDynamic(Position *pos, float radius, float lifetime, int tile);

    // Add a static light at fixed coordinates. Returns handle.
    // lifetime <= 0 means permanent.
    int addStatic(float x, float y, float radius, float lifetime = 0, int tile = 0);

    // Remove a light by handle.
    void remove(int id);

    // Remove non-persistent lights (call on scene teardown).
    void clear();

    // Remove all lights including persistent ones.
    void clearAll();

    // Called once per frame.
    void update(float dt);

    // Render darkness overlay — call after all world layers are drawn.
    void renderDarkness();

    // Compute alpha for a world position considering all light sources.
    uint8_t getAlphaAt(Position &p);

    // Check if any active torch light exists.
    bool hasActiveTorch();

    // Serialization (only dynamic timed lights).
    void serialize(JSON::Value &to);
    void deserialize(JSON::Value &from, Position *playerPos);

private:
    Lighting() = default;

    float computeRadius(const LightSource &src);

    std::vector<LightSource> sources;
    int nextId = 1;
};

#define RT_Lighting Lighting::instance()

#endif //ZWEI_LIGHTING_H
