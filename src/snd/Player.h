#ifndef ZWEI_PLAYER_H
#define ZWEI_PLAYER_H

#include <SDL2/SDL_mixer.h>

#include <queue>
#include <ASSETS/Assets.h>

class Player {
public:
    static Player &instance() {
        static Player instance;
        return instance;
    }

    Player(Player const &) = delete;

    void operator=(Player const &) = delete;

    void playSound(Asset id);

    void playMusic(Asset id);

private:
    Player();

    ~Player();

    bool ready = false;
};

#endif //ZWEI_PLAYER_H
