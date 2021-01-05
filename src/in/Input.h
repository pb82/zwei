#ifndef ZWEI_INPUT_H
#define ZWEI_INPUT_H

#include <map>

#include <SDL2/SDL.h>

enum GameKeyState {
    GK_PUSHED,
    GK_RELEASED
};

enum GameKey {
    GK_NONE = 0,
    GK_UP,
    GK_DOWN,
    GK_LEFT,
    GK_RIGHT,
    GK_A,
    GK_B,
    GK_X,
    GK_Y,
    GK_START,
    GK_SELECT,
    GK_L,
    GK_R
};

struct GameKeyEvent {
    GameKeyState state;
    GameKey key;
    bool valid = false;
};

class Input {
public:
    Input();

    ~Input();

    bool scan();

    bool map(SDL_Event *e, GameKeyEvent *g);

private:
    void mapHatEvent(SDL_Event *e, GameKeyEvent *g);

    bool mapKeyboardEvent(SDL_Event *e, GameKeyEvent *g);

    bool mapGamepadEvent(SDL_Event *e, GameKeyEvent *g);

    GameKey fromSdlKey(const SDL_Event *e) const;

    GameKey fromSdlButton(const SDL_Event *e) const;

    SDL_GameController *controller;

    bool locked = false;
};

#endif
