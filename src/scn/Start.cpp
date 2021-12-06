#include "Start.h"

#include "../Api.h"
#include "../snd/Player.h"
#include "../Rt.h"
#include "../Cc.h"
#include "Beach.h"
#include "../ecs/Animation.h"

Start::Start() : Scene(SceneDungeon) {}

const uint8_t ID_Door_1 = 0;
const uint8_t ID_Door_2 = 1;
const uint8_t ID_Button = 2;

const uint8_t ID_enemy_1 = 3;

const uint8_t ID_item_torch_1 = 4;

void Start::addItems(JSON::Value &collected) {
    if (!RT_Memory.arrayContains("collectedItems", ID_item_torch_1)) {
        Api::addItem(32, 13, ID_item_torch_1, TORCH);
    }
}

void Start::addEnemies(JSON::Value &resumed) {

}

void Start::init() {
    Api::initPlayer();
    Api::setPlayerPosition(26, 15);
    Api::loadMap("dungeon2.json");

    // Turn lights off initially
    bool lights = false;
    Api::setEnableLights(lights);

    // Switch to game mode
    Api::setGameState();

    // Doors
    Api::setDoor(29, 9, ID_Door_1);
    Api::setDoor(34, 1, ID_Door_2);

    // Button to switch lights
    Api::setInteractible(31, 12, ID_Button, [lights](Entity &p) {
        bool pushed = RT_Memory.getBool("scene.dungeon.button", false);
        auto animation = p.getComponent<Animation>();
        if (!pushed) animation->queueStateFramesForward();
        else animation->queueStateFramesBackward();
        Api::setEnableLights(!pushed);
        RT_Memory.setBool("scene.dungeon.button", !pushed);
    });

    Player::instance().playMusic(MUSIC_1);

    // Show message only when player enters from the south
    Api::setTrigger(34, 0, [](float, Entity&) {
        Rt_Commands.push(std::make_shared<ScreenTransition>(SceneBeach));
    }, nullptr);
}

void Start::exit() {
    Api::unloadMap();
}