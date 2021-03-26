#include "Start.h"

#include "../Api.h"
#include "../snd/Player.h"
#include "../Rt.h"

void Start::init() {
    Api::initPlayer();
    Api::setPlayerPosition(26, 15);
    Api::showNewGameMenu(false);
    Api::loadMap("dungeon2.json");
    Api::setGameState();

    // Doors
    Api::setDoor(29, 9);
    Api::setDoor(34, 1);

    Player::instance().playMusic(MUSIC_1);

    // Show message only when player enters from the south
    Api::setTrigger(34, 2, [](float) {
        if (!RT_Memory.getBool("start.bubble", false)) {
            Api::createSpeechBubble("Maybe this is the way out?");
            RT_Memory.setBool("start.bubble", true);
        }
    }, nullptr);

    Api::setTrigger(34, 0, [](float angle) {
        RT_Memory.del("start.bubble");
    }, nullptr);
}

void Start::exit() {
    Api::unloadMap();
}