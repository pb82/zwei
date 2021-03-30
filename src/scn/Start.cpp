#include "Start.h"

#include "../Api.h"
#include "../snd/Player.h"
#include "../Rt.h"

void Start::init() {
    Api::initPlayer();
    Api::setPlayerPosition(26, 15);
    Api::showNewGameMenu(false);
    Api::loadMap("dungeon2.json");

    // Turn lights off initialls
    bool lights = false;
    Api::setEnableLights(lights);

    // Switch to game mode
    Api::setGameState();

    // Doors
    Api::setDoor(29, 9);
    Api::setDoor(34, 1);

    // Button to switch lights
    Api::setInteractible(31, 12, [lights](Entity &, JSON::Value &internalState) {
        bool pushed = !lights;
        auto button = internalState["pushed"];

        if (button.is(JSON::JSON_BOOL)) {
            pushed = button.as<bool>();
        }

        Api::setEnableLights(pushed);
        internalState["pushed"] = !pushed;
    });

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

    // Api::createSingleSpeechBubble("It's really dark in here.", true);
    // Api::createSingleSpeechBubble("Maybe I can find a torch somewhere?", false);

}

void Start::exit() {
    Api::unloadMap();
}