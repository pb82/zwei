#ifndef ZWEI_INPUT_H
#define ZWEI_INPUT_H

#include <unordered_map>

#include <SDL.h>

enum GameKeyState {
    GK_PUSHED,
    GK_RELEASED
};

enum GameEventType {
    KEYBOARD,
    GAMEPAD,
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
    SDL_Event *source = nullptr;
};

class Input {
public:
    Input();

    ~Input();

    bool scan();

    bool map(SDL_Event *e, GameKeyEvent *g);

    void initDefaultMapping();

    static SDL_GameControllerButton bound(GameKey button);

    static SDL_Keycode boundKey(GameKey key);

    static void rebind(SDL_GameControllerButton button, GameKey key);

    static void rebind(SDL_Keycode button, GameKey key);

    static std::string toString(GameKey key);

    static std::unordered_map<SDL_Keycode, GameKey> keyboardMapping;
    static std::unordered_map<SDL_GameControllerButton, GameKey> controllerMapping;

private:

    bool mapEvent(SDL_Event *e, GameKeyEvent *g, GameEventType type);

    GameKey fromSdlButton(const SDL_Event *e) const;

    SDL_GameController *controller = nullptr;

    bool locked = false;
};

#endif
