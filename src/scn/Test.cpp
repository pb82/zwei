#include "Test.h"

#include "../Api.h"
#include "../Rt.h"

const uint8_t ID_Ally_1 = 28;
const uint8_t ID_Enemy_1 = 29;

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

    Api::addAlly(7, 7, ID_Ally_1);
    // Api::addKakta(13, 3, ID_Enemy_1);

    Api::setTrigger(7, 7, [](float, Entity &parent) {
        Api::createSpeechBubble("Hello. I'm a friendly Kakta. I'm going to join you!");
        RT_Context.getMemory().setBool("ally.joined", true);

        // Auto remove trigger after invocation
        parent.removeComponent<Trigger>();
    }, nullptr, 2);
}

void Test::exit() {
    Api::unloadMap();
}