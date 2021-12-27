#include "Test.h"

#include "../Api.h"
#include "../Rt.h"

uint16_t ID_Door_1 = 1;

Test::Test() : Scene(SceneTesting) {}

void Test::addItems(JSON::Value &collected) {
}

void Test::addEnemies(JSON::Value &resumed) {

}

void Test::init() {
    Api::initPlayer();
    Api::setPlayerPosition(14, 30);
    Api::loadMap("Beach.json");
    Api::setGameState();
    Api::setDoor(14, 17, ID_Door_1);
    Api::setRoofHideTrigger(14, 16);
    Api::setRoofShowTrigger(14, 18);
}

void Test::exit() {
    Api::unloadMap();
}