#include "Controller.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Animation.h"

Controller::Controller(Entity &parent) : Component(parent) {
    activeKeys[GK_UP] = false;
    activeKeys[GK_DOWN] = false;
    activeKeys[GK_LEFT] = false;
    activeKeys[GK_RIGHT] = false;
    activeKeys[GK_A] = false;
}

float Controller::angleFromKeys(float angle) {
    if (P_UP) {
        angle = VM_50_PI;
    }
    if (P_DOWN) {
        angle = VM_150_PI;
    }
    if (P_LEFT) {
        angle = VM_100_PI;
    }
    if (P_RIGHT) {
        angle = VM_0_PI;
    }
    if (P_UP && P_LEFT) {
        angle = VM_75_PI;
    }
    if (P_UP && P_RIGHT) {
        angle = VM_25_PI;
    }
    if (P_DOWN && P_LEFT) {
        angle = VM_125_PI;
    }
    if (P_DOWN && P_RIGHT) {
        angle = VM_175_PI;
    }
    if (P_A) {

    }
    return angle;
}

void Controller::key(GameKeyEvent &key) {
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    if (key.state == GK_RELEASED) {
        if (key.key == GK_NONE) {
            activeKeys[GK_UP] = false;
            activeKeys[GK_DOWN] = false;
            activeKeys[GK_LEFT] = false;
            activeKeys[GK_RIGHT] = false;
        } else {
            activeKeys[key.key] = false;
        }
    } else {
        activeKeys[key.key] = true;
    }

    if (!P_UP && !P_DOWN && !P_LEFT && !P_RIGHT) {
        acceleration->maxSpeed = 0;
        acceleration->speed = 0;
        animation->paused = true;
        return;
    }

    animation->paused = false;
    acceleration->speed = acceleration->speed * 0.6;
    acceleration->maxSpeed = acceleration->resetSpeed;
    acceleration->trajectory.angle = angleFromKeys(acceleration->trajectory.angle);
}