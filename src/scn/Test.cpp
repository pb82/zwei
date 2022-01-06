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

    /*
    if (!RT_Memory.getBool(SceneConstants::KEY_IntroductionDialog, false)) {
        Api::createSingleSpeechBubble("(Sailors) Wait here for us and don't move!", true);
        Api::createSingleSpeechBubble("(Arnold) Ok.", false);
        RT_Memory.setBool(SceneConstants::KEY_IntroductionDialog, true);

        auto npc = Api::addNpc(14, 26, 3);
        npc->addTurn(VM_50_PI, 4000, 2.0f);
        npc->go();

        Api::createSingleSpeechBubble("(Arnold) I'm getting bored.", true);
        Api::createSingleSpeechBubble("(Arnold) A quick look around can't hurt.", false);
    }

    Api::setInteractible(15, 14, SceneConstants::ID_InteractibleNotes, [](Entity &on) {
        Api::createSingleSpeechBubble("(Arnold) Something is here.", true);
        Api::createSingleSpeechBubble("(Arnold) Looks like something is hidden under the floor.", true);
        Api::createSingleSpeechBubble("(Sailors) Ahhhhhh!", true);
        Api::createSingleSpeechBubble("(Arnold) What was that?", true);

        Api::addKakta(4, 12, SceneConstants::ID_Kakta_1);
        Api::addKakta(5, 11, SceneConstants::ID_Kakta_2);
    });
    */

    // Api::addKakta(4, 12, SceneConstants::ID_Kakta_1);
    // Api::addKakta(5, 11, SceneConstants::ID_Kakta_2);
    Api::addItem(14, 26, 4, STICK);
    Api::addItem(15, 25, 4, HEART);

}

void Test::exit() {
    Api::unloadMap();
}