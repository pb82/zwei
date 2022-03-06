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

    Api::addItem(2, 15, 0, ItemType::STICK);
    Api::addItem(2, 16, 0, ItemType::HEART);


    if (GET_BOOL(SceneConstants::KEY_OnTheRun)) {
        Api::createSingleSpeechBubble("(Arnold) I better find something to defend myself.", true);
        Api::createSingleSpeechBubble("(Arnold) Who knows what creatures are running around here.", false);
    }

    Api::setTrigger(2, 31, [](float angle, Entity &){
        if (GET_BOOL(SceneConstants::KEY_OnTheRun)) {
            Api::createSingleSpeechBubble("(Arnold) No way I'm going back there now.", false);
            return;
        }

        RT_Spawn.push({25, 1});
        auto t = std::make_shared<ScreenTransition>(SceneTesting);
        Rt_Commands.push(t);
    }, nullptr);
}

void Forest::exit() {
    Api::unloadMap();
}