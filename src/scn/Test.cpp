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
    Api::loadMap("Beach.json");
    Api::setGameState();
}

void Test::exit() {
    Api::unloadMap();
}