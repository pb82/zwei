#include "Player.h"

#include <future>
#include <vector>
#include <tuple>

typedef std::future<std::shared_ptr<Sound>> background_audio;
typedef std::future<std::shared_ptr<Music>> background_music;

Sound::~Sound() {
    Mix_FreeChunk(this->sound);
}

Music::~Music() {
    Mix_FreeMusic(this->music);
}

std::shared_ptr<Sound> load_sound(const char *path) {
    auto sound = Mix_LoadWAV(path);
    return std::make_shared<Sound>(sound);
}

std::shared_ptr<Music> load_music(const char *path) {
    auto music = Mix_LoadMUS(path);
    return std::make_shared<Music>(music);
}

Player::Player() {
    init();
}

Player::~Player() {
    Mix_Quit();
}

void Player::playSound(Asset id) {
    if (sounds.find(id) == sounds.end()) {
        return;
    }

    if (!Mix_Playing(SoundEffects)) {
        Mix_PlayChannel(SoundEffects, sounds.at(id)->sound, 0);
    }
}

void Player::playMusic(Asset id) {
    if (music.find(id) == music.end()) {
        return;
    }

    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }

    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(music.at(id)->music, 0);
    }
}

void Player::init() {
    std::vector<std::tuple<Asset, background_audio>> sound_tasks;
    std::vector<std::tuple<Asset, background_music>> music_tasks;

    // sound effects
    sound_tasks.push_back({SOUND_PICKUP, std::async(load_sound, "assets/RAW/pickup.wav")});

    // musioc
    music_tasks.push_back({MUSIC_1, std::async(load_music, "assets/RAW/sample.ogg")});

    for (auto &task : sound_tasks) {
        Asset soundAsset = std::get<0>(task);
        auto soundData = std::get<1>(task).get();
        sounds.emplace(soundAsset, soundData);
    }

    for (auto &task : music_tasks) {
        Asset musicAsset = std::get<0>(task);
        auto musicData = std::get<1>(task).get();
        music.emplace(musicAsset, musicData);
    }

    this->ready = true;
}