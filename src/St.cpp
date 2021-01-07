#include "St.h"
#include "snd/Player.h"

int St::getMusicVolume() {
    return musicVolume;
}

int St::incMusicVolume() {
    musicVolume += 10;
    if (musicVolume > 100) {
        musicVolume = 100;
    }
    Player::instance().setMusicVolume(musicVolume);
    return musicVolume;
}

int St::decMusicVolume() {
    musicVolume -= 10;
    if (musicVolume < 0) {
        musicVolume = 0;
    }
    Player::instance().setMusicVolume(musicVolume);
    return musicVolume;
}

int St::getEffectsVolume() {
    return this->effectsVolume;
}

int St::incEffectsVolume() {
    effectsVolume += 10;
    if (effectsVolume > 100) {
        effectsVolume = 100;
    }
    Player::instance().setEffectsVolume(effectsVolume);
    return effectsVolume;
}

int St::decEffectsVolume() {
    effectsVolume -= 10;
    if (effectsVolume < 0) {
        effectsVolume = 0;
    }
    Player::instance().setEffectsVolume(effectsVolume);
    return effectsVolume;
}

void St::initAll() {
    Player::instance().setMusicVolume(musicVolume);
    Player::instance().setEffectsVolume(effectsVolume);
}