#include "Player.h"

Player::Player() {}

Player::~Player() {
    Mix_Quit();
}

void Player::playSound(Asset id) {
    auto sound = Assets::instance().getSound(id);
    Mix_PlayChannel(-1, sound->sound, 0);
}

void Player::playMusic(Asset id) {
}