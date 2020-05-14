#include <iostream>
#include "Input.h"

Input::Input() : controller(nullptr) {}

Input::~Input() {
    if (controller) {
        SDL_JoystickClose(controller);
        controller = nullptr;
    }
}

void Input::mapHatEvent(SDL_Event *e, GameKeyEvent *g) {
    g->state = e->jhat.value == 0 ? GK_RELEASED : GK_PUSHED;
    if (g->state == GK_RELEASED) g->key = GK_NONE;
    if (e->jhat.value == 1) g->key = GK_UP;
    else if (e->jhat.value == 2) g->key = GK_RIGHT;
    else if (e->jhat.value == 4) g->key = GK_DOWN;
    else if (e->jhat.value == 8) g->key = GK_LEFT;
}

bool Input::map(SDL_Event *e, GameKeyEvent *g) {
    if (e->type == SDL_JOYHATMOTION) {
        mapHatEvent(e, g);
        return true;
    }
    if (e->type == SDL_JOYBUTTONDOWN) {
        return false;
    }
    return false;
}

bool Input::scan() {
    int num = SDL_NumJoysticks();
    if (num <= 0) {
        return false;
    }


    for (int i = 0; i < num; ++i) {
        controller = SDL_JoystickOpen(i);
        if (controller) {
            return true;
        }
    }

    return false;
}