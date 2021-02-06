#include <iostream>
#include "Input.h"

Input::Input() : controller(nullptr) {
    initDefaultMapping();
}

Input::~Input() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
}

void Input::initDefaultMapping() {
    keyboardMapping[SDLK_LEFT] = GameKey::GK_LEFT;
    keyboardMapping[SDLK_RIGHT] = GameKey::GK_RIGHT;
    keyboardMapping[SDLK_UP] = GameKey::GK_UP;
    keyboardMapping[SDLK_DOWN] = GameKey::GK_DOWN;
    keyboardMapping[SDLK_a] = GameKey::GK_B;
    keyboardMapping[SDLK_s] = GameKey::GK_A;
    keyboardMapping[SDLK_x] = GameKey::GK_X;
    keyboardMapping[SDLK_y] = GameKey::GK_Y;
    keyboardMapping[SDLK_q] = GameKey::GK_L;
    keyboardMapping[SDLK_w] = GameKey::GK_R;
    keyboardMapping[SDLK_ESCAPE] = GameKey::GK_START;
    keyboardMapping[SDLK_TAB] = GameKey::GK_SELECT;

    controllerMapping[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = GameKey::GK_LEFT;
    controllerMapping[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = GameKey::GK_RIGHT;
    controllerMapping[SDL_CONTROLLER_BUTTON_DPAD_UP] = GameKey::GK_UP;
    controllerMapping[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = GameKey::GK_DOWN;
    controllerMapping[SDL_CONTROLLER_BUTTON_B] = GameKey::GK_B;
    controllerMapping[SDL_CONTROLLER_BUTTON_A] = GameKey::GK_A;
    controllerMapping[SDL_CONTROLLER_BUTTON_X] = GameKey::GK_X;
    controllerMapping[SDL_CONTROLLER_BUTTON_Y] = GameKey::GK_Y;
    controllerMapping[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = GameKey::GK_L;
    controllerMapping[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = GameKey::GK_R;
    controllerMapping[SDL_CONTROLLER_BUTTON_START] = GameKey::GK_START;
    controllerMapping[SDL_CONTROLLER_BUTTON_BACK] = GameKey::GK_SELECT;
}

bool Input::mapEvent(SDL_Event *e, GameKeyEvent *g, GameEventType type) {
    GameKey key;
    if (type == KEYBOARD) {
        if (keyboardMapping.find(e->key.keysym.sym) == keyboardMapping.end()) return false;
        g->state = e->type == SDL_KEYUP ? GK_RELEASED : GK_PUSHED;
        key = keyboardMapping[e->key.keysym.sym];
    } else {
        auto b = SDL_GameControllerButton(e->cbutton.button);
        if (controllerMapping.find(b) == controllerMapping.end()) return false;
        g->state = e->type == SDL_CONTROLLERBUTTONUP ? GK_RELEASED : GK_PUSHED;
        key = controllerMapping[b];
    }

    if (g->state == GK_RELEASED) {
        g->key = key;
        locked = false;
        return true;
    }

    if (g->key == key && locked) return false;
    g->key = key;
    return true;
}

bool Input::map(SDL_Event *e, GameKeyEvent *g) {
    g->source = e;
    if (e->type == SDL_CONTROLLERBUTTONDOWN || e->type == SDL_CONTROLLERBUTTONUP) {
        g->valid = mapEvent(e, g, GAMEPAD);
        return g->valid;
    } else if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        g->valid = mapEvent(e, g, KEYBOARD);
        return g->valid;
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