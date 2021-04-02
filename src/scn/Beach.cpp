#include "Beach.h"

#include "../Api.h"
#include "../snd/Player.h"
#include "../Rt.h"

void Beach::init() {
    Api::initPlayer();
    Api::setPlayerPosition(7, 24);
    Api::showNewGameMenu(false);
    Api::loadMap("beach.json");
    Api::setGameState();
    Api::setEnableLights(true);
    Player::instance().playMusic(MUSIC_1);
}

void Beach::exit() {
    Api::unloadMap();
}