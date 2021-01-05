#ifndef ZWEI_PLAYER_H
#define ZWEI_PLAYER_H

#include <SDL2/SDL_mixer.h>

#include <queue>
#include <ASSETS/Assets.h>

enum AudioChannel {
    SoundEffects = 0,
};

struct Sound {
    Sound(Mix_Chunk *sound) : sound(sound) {}

    ~Sound();

    Mix_Chunk *sound;
};

struct Music {
    Music(Mix_Music *music) : music(music) {}

    ~Music();

    Mix_Music *music;
};

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

    void pause();

    void resume();

private:
    Player();

    ~Player();

    void init();

    std::unordered_map<Asset, std::shared_ptr<Sound>> sounds;

    std::unordered_map<Asset, std::shared_ptr<Music>> music;

    Asset nowPlaying = UNDEFINED;

    bool ready = false;
};

#endif //ZWEI_PLAYER_H
