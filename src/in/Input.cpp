#include <iostream>
#include "Input.h"

Input::Input() : controller(nullptr) {}

Input::~Input() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
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
        case SDLK_a:
            return GK_A;
        case SDLK_TAB:
            return GK_SELECT;
        case SDLK_x:
            return GK_X;
        default:
            return GK_NONE;
    }
}

GameKey Input::fromSdlButton(const SDL_Event *e) const {
    switch (e->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            return GK_UP;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            return GK_DOWN;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            return GK_LEFT;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            return GK_RIGHT;
        case SDL_CONTROLLER_BUTTON_A:
            return GK_A;
        case SDL_CONTROLLER_BUTTON_GUIDE:
            return GK_SELECT;
        case SDL_CONTROLLER_BUTTON_X:
            return GK_X;
        default:
            return GK_NONE;
    }
}

bool Input::mapGamepadEvent(SDL_Event *e, GameKeyEvent *g) {
    g->state = e->type == SDL_CONTROLLERBUTTONUP ? GK_RELEASED : GK_PUSHED;
    if (g->state == GK_RELEASED) {
        g->key = fromSdlButton(e);
        locked = false;
        return true;
    }

    switch (e->cbutton.button) {
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            if (g->key == GK_UP && locked) return false;
            locked = true;
            g->key = GK_UP;
            return true;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            if (g->key == GK_RIGHT && locked) return false;
            locked = true;
            g->key = GK_RIGHT;
            return true;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            if (g->key == GK_DOWN && locked) return false;
            locked = true;
            g->key = GK_DOWN;
            return true;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            if (g->key == GK_LEFT && locked) return false;
            locked = true;
            g->key = GK_LEFT;
            return true;
        case SDL_CONTROLLER_BUTTON_A:
            if (g->key == GK_A && locked) {
                return false;
            }
            locked = true;
            g->key = GK_A;
            return true;
        case SDL_CONTROLLER_BUTTON_GUIDE:
            if (g->key == GK_SELECT && locked) {
                return false;
            }
            locked = true;
            g->key = GK_SELECT;
            return true;
        case SDL_CONTROLLER_BUTTON_X:
            if (g->key == GK_X && locked) {
                return false;
            }
            locked = true;
            g->key = GK_X;
            return true;
        default:
            g->key = GK_NONE;
            return false;
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
        case SDLK_a:
            if (g->key == GK_A && locked) {
                return false;
            }
            locked = true;
            g->key = GK_A;
            return true;
        case SDLK_TAB:
            if (g->key == GK_SELECT && locked) {
                return false;
            }
            locked = true;
            g->key = GK_SELECT;
            return true;
        case SDLK_x:
            if (g->key == GK_X && locked) {
                return false;
            }
            locked = true;
            g->key = GK_X;
            return true;
        default:
            g->key = GK_NONE;
            return false;
    }
}

bool Input::map(SDL_Event *e, GameKeyEvent *g) {
    if (e->type == SDL_CONTROLLERBUTTONDOWN || e->type == SDL_CONTROLLERBUTTONUP) {
        return mapGamepadEvent(e, g);
    }
    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        return mapKeyboardEvent(e, g);
    }
    return false;
}

bool Input::scan() {
    if (SDL_GameControllerAddMappingsFromFile("./gamecontrollerdb.txt") == -1) {
        return false;
    }

    int num = SDL_NumJoysticks();
    if (num <= 0) {
        return false;
    }

    SDL_GameControllerEventState(SDL_ENABLE);
    SDL_JoystickEventState(SDL_ENABLE);

    for (int i = 0; i < num; ++i) {
        if (!SDL_IsGameController(i)) {
            continue;
        }

        controller = SDL_GameControllerOpen(i);

        if (controller) {
            return true;
        }
    }

    return false;
}