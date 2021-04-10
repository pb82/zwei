#include "Start.h"

#include "../Api.h"
#include "../snd/Player.h"
#include "../Rt.h"
#include "../Cc.h"
#include "Beach.h"
#include "../ecs/Animation.h"

void Start::init() {
    Api::initPlayer();
    Api::setPlayerPosition(26, 15);
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
    Api::setInteractible(31, 12, [lights](Entity &p, JSON::Value &internalState) {
        bool pushed = !lights;
        auto button = internalState["pushed"];

        if (button.is(JSON::JSON_BOOL)) {
            pushed = button.as<bool>();
        }

        auto animation = p.getComponent<Animation>();
        if (pushed) animation->queueStateFramesForward();
        else animation->queueStateFramesBackward();

        Api::setEnableLights(pushed);
        internalState["pushed"] = !pushed;
    });

    Player::instance().playMusic(MUSIC_1);

    // Show message only when player enters from the south
    Api::setTrigger(34, 0, [](float) {
        Rt_Commands.push(std::make_shared<ScreenTransition>(std::make_shared<Beach>()));
    }, nullptr);
}

void Start::exit() {
    Api::unloadMap();
}