#include "Ai.h"

#include "Entity.h"
#include "Acceleration.h"

Ai::Ai(Entity &parent) : Component(parent) {}

void Ai::update(float dt) {
    if (!mind) {
        return;
    }

    if (_pause > 0) {
        _pause -= dt;
        if (_pause < 0) {
            _pause = 0.0f;
        } else {
            return;
        }
    }

    time += dt;

    if (time < mind->delay()) {
        return;
    }

    time = 0.0f;
    if (mind->activate()) {
        mind->plan(dt);
    }
}

void Ai::pause(float duration) {
    this->_pause = duration;
}

void Ai::render(uint8_t) {
    if (mind) {
        mind->render();
    }
}