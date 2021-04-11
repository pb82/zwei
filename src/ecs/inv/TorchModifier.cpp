#include "TorchModifier.h"

TorchModifier::TorchModifier(float lifetime) : Modifier(CIRCLE_OF_LIGHT, lifetime) {}

void TorchModifier::update(float dt) {
    Modifier::update(dt);
}

float TorchModifier::modify(float in) {
    float base = in * 3;
    base += abs(1024 * sin(lifetime / 1000));
    base = std::max<float>(in * 2, base);
    return std::min<float>(base, in * 5);
}