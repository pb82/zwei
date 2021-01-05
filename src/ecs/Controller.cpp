#include "Controller.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Animation.h"
#include "Attack.h"

#define P_UP (activeKeys[GK_UP] == true)
#define P_DOWN (activeKeys[GK_DOWN] == true)
#define P_LEFT (activeKeys[GK_LEFT] == true)
#define P_RIGHT (activeKeys[GK_RIGHT] == true)
#define P_A (activeKeys[GK_A] == true)
#define P_B (activeKeys[GK_B] == true)
#define P_X (activeKeys[GK_X] == true)
#define P_Y (activeKeys[GK_Y] == true)
#define P_SELECT (activeKeys[GK_SELECT] == true)
#define P_START (activeKeys[GK_START] == true)

Controller::Controller(Entity &parent) : Component(parent) {
    resetKeys();
}

void Controller::resetKeys() {
    activeKeys[GK_UP] = false;
    activeKeys[GK_DOWN] = false;
    activeKeys[GK_LEFT] = false;
    activeKeys[GK_RIGHT] = false;
    activeKeys[GK_A] = false;
    activeKeys[GK_B] = false;
    activeKeys[GK_X] = false;
    activeKeys[GK_Y] = false;
    activeKeys[GK_START] = false;
    activeKeys[GK_SELECT] = false;
}

float Controller::angleFromKeys() {
    if (P_UP && P_LEFT) {
        return VM_75_PI;
    }
    if (P_UP && P_RIGHT) {
        return VM_25_PI;
    }
    if (P_DOWN && P_LEFT) {
        return VM_125_PI;
    }
    if (P_DOWN && P_RIGHT) {
        return VM_175_PI;
    }
    if (P_UP) {
        return VM_50_PI;
    }
    if (P_DOWN) {
        return VM_150_PI;
    }
    if (P_LEFT) {
        return VM_100_PI;
    }
    if (P_RIGHT) {
        return VM_0_PI;
    }
    return VM_0_PI;
}

void Controller::key(GameKeyEvent &key) {
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    if (key.state == GK_RELEASED) {
        if (key.key == GK_NONE) {
            resetKeys();
        } else {
            activeKeys[key.key] = false;
        }
    } else {
        activeKeys[key.key] = true;
    }

    if (P_SELECT) {
        if (parent.hasComponent<Stats>()) {
            auto stats = parent.getComponent<Stats>();
            stats->inventory.next();
        }
        return;
    }

    if (P_A) {
        if (parent.hasComponent<Attack>()) {
            auto attack = parent.getComponent<Attack>();
            acceleration->decelerate();
            attack->attack();
            return;
        }
    }

    if (P_B) {
        if (parent.hasComponent<Stats>()) {
            auto stats = parent.getComponent<Stats>();
            stats->inventory.use();
        }
        return;
    }

    if (P_X) {
        if (parent.hasComponent<Stats>()) {
            auto stats = parent.getComponent<Stats>();
            stats->inventory.drop();
        }
        return;
    }

    if (!P_UP && !P_DOWN && !P_LEFT && !P_RIGHT) {
        acceleration->decelerate();
        animation->stop();
        return;
    }

    acceleration->turn(angleFromKeys());
    acceleration->accelerate();
    animation->start();
}