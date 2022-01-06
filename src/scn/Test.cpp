#include "Test.h"

#include "SceneConstants.h"

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

    if (!RT_Memory.getBool(SceneConstants::key_IntroductionDialog, false)) {
        Api::createSingleSpeechBubble("(Sailors) Wait here for us and don't move!", true);
        Api::createSingleSpeechBubble("(Arnold) Ok.", false);
        RT_Memory.setBool(SceneConstants::key_IntroductionDialog, true);
    }

    auto npc = Api::addNpc(14, 26, 3);
    npc->addTurn(VM_50_PI, 4000, 2.0f);
    npc->go();

    Api::createSingleSpeechBubble("(Arnold) I'm getting bored.", true);
    Api::createSingleSpeechBubble("(Arnold) A quick look around can't hurt.", true);

}

void Test::exit() {
    Api::unloadMap();
}