#include "Forest.h"
#include "../Api.h"
#include "../Cc.h"
#include "../Rt.h"

#include "SceneConstants.h"

Forest::Forest() : Scene(SceneForest) {}

void Forest::init() {
    Api::init();
    Api::initPlayer();
    Api::setPlayerPosition(2, 30);
    Api::loadMap("Forest.json");
    Api::setGameState();

    if (!GET_BOOL(SceneConstants::KEY_Forest_StickTaken))
        Api::addItem(2, 15, ItemType::STICK, []() {
            RT_Memory.setBool(SceneConstants::KEY_Forest_StickTaken, true);
        });

    if (!GET_BOOL(SceneConstants::KEY_Forest_HeartTaken))
        Api::addItem(2, 16, ItemType::HEART, []() {
            RT_Memory.setBool(SceneConstants::KEY_Forest_HeartTaken, true);
        });

    Api::addCaterpillar(16, 23, 0, 30);

    Api::setTrigger(2, 31, [](float angle, Entity &) {
        RT_Spawn.push({25, 1});
        auto t = std::make_shared<ScreenTransition>(SceneTesting);
        Rt_Commands.push(t);
    }, nullptr);

    /*
    if (GET_BOOL(SceneConstants::KEY_OnTheRun)) {
        Api::createSingleSpeechBubble("(Arnold) I better find something to defend myself.", true);
        Api::createSingleSpeechBubble("(Arnold) Who knows what creatures are running around here.", false);
    }

    Api::setTrigger(13, 25, [](float angle, Entity &){
        Api::createSingleSpeechBubble("(Arnold) I can hear something moving there.", true);
        Api::createSingleSpeechBubble("(Arnold) Some sort of weapon would be useful.", false);
    }, nullptr, 1,1);

    Api::setTrigger(2, 31, [](float angle, Entity &){
        if (GET_BOOL(SceneConstants::KEY_OnTheRun)) {
            Api::createSingleSpeechBubble("(Arnold) No way I'm going back there now.", false);
            return;
        }

        RT_Spawn.push({25, 1});
        auto t = std::make_shared<ScreenTransition>(SceneTesting);
        Rt_Commands.push(t);
    }, nullptr);
    */
}

void Forest::exit() {
    Api::unloadMap();
}