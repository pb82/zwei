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

GameKey Input::fromSdlKey(const SDL_Event *e) const {
    switch (e->key.keysym.sym) {
        case SDLK_UP:
            return GK_UP;
        case SDLK_DOWN:
            return GK_DOWN;
        case SDLK_LEFT:
            return GK_LEFT;
        case SDLK_RIGHT:
            return GK_RIGHT;
        default:
            return GK_NONE;
    }
}

bool Input::mapKeyboardEvent(SDL_Event *e, GameKeyEvent *g) {
    g->state = e->type == SDL_KEYUP ? GK_RELEASED : GK_PUSHED;
    if (g->state == GK_RELEASED) {
        g->key = fromSdlKey(e);
        locked = false;
        return true;
    }

    switch (e->key.keysym.sym) {
        case SDLK_UP:
            if (g->key == GK_UP && locked) return false;
            locked = true;
            g->key = GK_UP;
            return true;
        case SDLK_RIGHT:
            if (g->key == GK_RIGHT && locked) return false;
            locked = true;
            g->key = GK_RIGHT;
            return true;
        case SDLK_DOWN:
            if (g->key == GK_DOWN && locked) return false;
            locked = true;
            g->key = GK_DOWN;
            return true;
        case SDLK_LEFT:
            if (g->key == GK_LEFT && locked) return false;
            locked = true;
            g->key = GK_LEFT;
            return true;
        default:
            g->key = GK_NONE;
            return false;
    }
}

bool Input::map(SDL_Event *e, GameKeyEvent *g) {
    if (e->type == SDL_JOYHATMOTION) {
        mapHatEvent(e, g);
        return true;
    }
    if (e->type == SDL_JOYBUTTONDOWN) {
        return false;
    }
    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        return mapKeyboardEvent(e, g);
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