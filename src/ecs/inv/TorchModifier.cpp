#include "TorchModifier.h"

TorchModifier::TorchModifier(float lifetime) : Modifier(CIRCLE_OF_LIGHT, lifetime) {}

void TorchModifier::serialize(JSON::Value &to) {
    to["type"] = this->type;
    to["duration"] = this->duration;
}

void TorchModifier::update(float dt) {
    Modifier::update(dt);
    duration += dt;
}

float TorchModifier::modify(float in) {
    float base = in * 3;
    base += abs(1024 * sin(duration / 1000));
    base = std::max<float>(in * 2, base);
    return std::min<float>(base, in * 5);
}