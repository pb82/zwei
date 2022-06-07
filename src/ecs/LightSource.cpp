#include "LightSource.h"

#include "Entity.h"

LightSource::LightSource(Entity &parent) : Component(parent), expires(false) {}

LightSource::LightSource(Entity &parent, float lifetime) : Component(parent), lifetime(lifetime), expires(true) {}

void LightSource::update(float dt) {
    age += dt;
    if (expires) {
        if (lifetime - age <= 0) {
            this->parent.removeComponent<LightSource>();
        }
    }
}

uint8_t LightSource::getAlpha(float d) {
    float dd = 255 - std::min<float>(255, d * (50 + sin(age / 100)));
    return static_cast<uint8_t>(dd);
}