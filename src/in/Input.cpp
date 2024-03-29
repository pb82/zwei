#include <iostream>
#include "Input.h"

std::unordered_map<SDL_Keycode, GameKey> Input::keyboardMapping;
std::unordered_map<SDL_GameControllerButton, GameKey> Input::controllerMapping;

Input::Input() : controller(nullptr) {
    initDefaultMapping();
}

Input::~Input() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
}

SDL_GameControllerButton Input::bound(GameKey button) {
    for (auto &pair : controllerMapping) {
        if (pair.second == button) {
            return pair.first;
        }
    }
    return SDL_CONTROLLER_BUTTON_INVALID;
}

SDL_Keycode Input::boundKey(GameKey key) {
    for (auto &pair : keyboardMapping) {
        if (pair.second == key) {
            return pair.first;
        }
    }
    return SDLK_UNKNOWN;
}

void Input::rebind(SDL_GameControllerButton button, GameKey key) {
    SDL_GameControllerButton bound = Input::bound(key);
    if (bound == SDL_CONTROLLER_BUTTON_INVALID) {
        controllerMapping[button] = key;
    } else {
        controllerMapping[bound] = controllerMapping[button];
        controllerMapping[button] = key;
    }
}

void Input::rebind(SDL_Keycode button, GameKey key) {
    SDL_Keycode bound = Input::boundKey(key);
    if (bound == SDLK_UNKNOWN) {
        keyboardMapping[button] = key;
    } else {
        keyboardMapping[bound] = keyboardMapping[button];
        keyboardMapping[button] = key;
    }
}

std::string Input::toString(GameKey key) {
    switch (key) {
        case GK_NONE:
            return "(unassigned)";
        case GK_SELECT:
            return "Select";
        case GK_START:
            return "Menu";
        case GK_R:
            return "Inventory >>";
        case GK_L:
            return "Inventory <<";
        case GK_Y:
            return "Drop Item";
        case GK_A:
            return "Attack/Confirm";
        case GK_DOWN:
            return "Down";
        case GK_UP:
            return "Up";
        case GK_RIGHT:
            return "Right";
        case GK_LEFT:
            return "Left";
        case GK_B:
            return "Use Item/Cancel";
        case GK_X:
            return "Direction Lock";
        default:
            return "(unknown)";
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
        if (keyboardMapping.find(e->key.keysym.sym) == keyboardMapping.end()) {
            key = GK_NONE;
        } else {
            key = keyboardMapping[e->key.keysym.sym];
        }
        g->state = e->type == SDL_KEYUP ? GK_RELEASED : GK_PUSHED;
    } else {
        auto b = SDL_GameControllerButton(e->cbutton.button);
        if (controllerMapping.find(b) == controllerMapping.end()) {
            key = GK_NONE;
        } else {
            key = controllerMapping[b];
        }
        g->state = e->type == SDL_CONTROLLERBUTTONUP ? GK_RELEASED : GK_PUSHED;
    }

    if (g->state == GK_RELEASED) {
        g->key = key;
        locked = false;
        return true;
    }

    if (g->key == key && locked) return false;
    locked = true;
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