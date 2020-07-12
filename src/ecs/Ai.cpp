#include "Ai.h"

#include <vector>
#include <iostream>
#include <ASSETS/Assets.h>

#include "../Gfx.h"
#include "../Rt.h"

#include "Entity.h"
#include "Transform.h"
#include "Acceleration.h"
#include "Animation.h"

Ai::Ai(Entity &parent) : Component(parent) {}

void Ai::update(float dt) {
    if (!mind) {
        return;
    }

    time += dt;

    if (time < mind->delay()) {
        return;
    }

    time = 0.0f;
    mind->plan(dt);
}

void Ai::collide(std::shared_ptr<Collider> other) {
    if (mind) {
        mind->collide(other);
    }
}
