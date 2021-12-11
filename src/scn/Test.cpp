#include "Test.h"

#include "../Api.h"
#include "../Rt.h"

const uint8_t ID_Ally_1 = 28;

const uint8_t ID_Enemy_1 = 55;
const uint8_t ID_Enemy_2 = 56;
const uint8_t ID_Enemy_3 = 57;
const uint8_t ID_Enemy_4 = 58;
const uint8_t ID_Enemy_5 = 59;
const uint8_t ID_Enemy_6 = 60;

const uint8_t ID_Bow_1 = 30;

Test::Test() : Scene(SceneTesting) {}

void Test::addItems(JSON::Value &collected) {
}

void Test::addEnemies(JSON::Value &resumed) {

}

void Test::init() {
    Api::initPlayer();
    Api::setPlayerPosition(2, 14);
    Api::loadMap("testing_area.json");
    Api::setGameState();

    Api::addAlly(7, 7, ID_Ally_1, 100);

    Api::addKakta(13, 3, ID_Enemy_1);
    Api::addKakta(13, 4, ID_Enemy_2);
    Api::addKakta(13, 5, ID_Enemy_3);

    Api::addSpider(5, 3, ID_Enemy_4);
    Api::addSpider(6, 4, ID_Enemy_5);
    Api::addSpider(7, 5, ID_Enemy_6);

    Api::addItem(5, 14, ID_Bow_1, STICK);



    // Api::setEnableLights(false);

    Api::setTrigger(7, 7, [](float, Entity &parent) {
        Api::createSpeechBubble("I will help you defeat the enemies!");
        RT_Context.getMemory().setBool("ally.joined", true);
        parent.disable();
    }, nullptr, 2);
}

void Test::exit() {
    Api::unloadMap();
}