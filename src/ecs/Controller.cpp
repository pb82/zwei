#include "Controller.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Animation.h"

Controller::Controller(Entity &parent) : Component(parent) {}

void Controller::key(GameKeyEvent &key) {
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    if (key.state == GK_RELEASED) {
        acceleration->maxSpeed = 0;
        acceleration->speed = 0;
        animation->paused = true;
    } else {
        if (key.key == GK_UP) {
            animation->paused = false;
            acceleration->speed = 0;
            acceleration->maxSpeed = acceleration->resetSpeed;
            acceleration->trajectory.angle = VM_50_PI;
        } else if (key.key == GK_DOWN) {
            animation->paused = false;
            acceleration->speed = 0;
            acceleration->maxSpeed = acceleration->resetSpeed;
            acceleration->trajectory.angle = VM_150_PI;
        } else if (key.key == GK_LEFT) {
            animation->paused = false;
            acceleration->speed = 0;
            acceleration->maxSpeed = acceleration->resetSpeed;
            acceleration->trajectory.angle = VM_100_PI;
        } else if (key.key == GK_RIGHT) {
            animation->paused = false;
            acceleration->speed = 0;
            acceleration->maxSpeed = acceleration->resetSpeed;
            acceleration->trajectory.angle = VM_0_PI;
        }
    }
}