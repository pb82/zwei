#include "Test.h"

#include "SceneConstants.h"

#include "../Api.h"
#include "../Rt.h"
#include "../alg/Vector.h"

Test::Test() : Scene(SceneTesting) {}

void Test::addItems(JSON::Value &collected) {
}

void Test::addEnemies(JSON::Value &resumed) {
    if (resumed.is(JSON::JSON_ARRAY)) {
        auto enemies = resumed.as<JSON::Array>();
        for (auto &enemy: enemies) {
            uint16_t id = enemy["id"].as<uint16_t>();
            float x = enemy["x"].as<float>();
            float y = enemy["y"].as<float>();
            int hp = enemy["hp"].as<int>();
            Api::addKakta(x, y, id, hp);
        }
    }
}

void Test::init() {
    Api::initPlayer();
    Api::setPlayerPosition(14, 30);
    Api::loadMap("Beach.json");
    Api::setGameState();
    Api::setDoor(14, 17, SceneConstants::ID_Door_1);
    Api::setRoofHideTrigger(14, 16);
    Api::setRoofShowTrigger(14, 18);

    if (!RT_Memory.getBool(SceneConstants::KEY_IntroductionDialog, false)) {
        Api::createSingleSpeechBubble("(Sailors) Wait here for us and don't move!", true);
        Api::createSingleSpeechBubble("(Sailors) We are going to investigate.", true);

        Api::createSingleSpeechBubble("(Arnold) Ok.", false);
        RT_Memory.setBool(SceneConstants::KEY_IntroductionDialog, true);

        auto npc = Api::addNpc(14, 26, SceneConstants::ID_Npc_1);
        Api::addTurnToNpc(npc, VM_50_PI, 8000, 1);

        auto npc2 = Api::addNpc(16, 26, SceneConstants::ID_Npc_2);
        Api::addTurnToNpc(npc2, VM_50_PI, 2000, 1);
        Api::addTurnToNpc(npc2, VM_150_PI, 1000, 0);
        Api::addTurnToNpc(npc2, VM_50_PI, 5000, 1);

        auto cmd = std::make_shared<NpcCommand>();
        cmd->addNpc(npc);
        cmd->addNpc(npc2);

        Rt_Commands.push(cmd);

        Api::createSingleSpeechBubble("(Arnold) I'm getting bored.", true);
        Api::createSingleSpeechBubble("(Arnold) A quick look around can't hurt.", false);
    }

    Api::setInteractible(15, 14, SceneConstants::ID_InteractibleNotes, [](Entity &on) {
        Api::createSingleSpeechBubble("(Arnold) Something is here.", true);
        Api::createSingleSpeechBubble("(Arnold) Looks like something is hidden under the floor.", true);
        Api::createSingleSpeechBubble("(Sailors) Ahhhhhh!", true);
        Api::createSingleSpeechBubble("(Arnold) What was that?", true);

        Api::addKakta(4, 12, SceneConstants::ID_Enemy_Kakta_1);
        Api::addKakta(5, 11, SceneConstants::ID_Enemy_Kakta_2);
        Api::addKakta(4, 7, SceneConstants::ID_Enemy_Kakta_3);
        Api::addKakta(4, 6, SceneConstants::ID_Enemy_Kakta_4);
    });
}

void Test::exit() {
    Api::unloadMap();
}