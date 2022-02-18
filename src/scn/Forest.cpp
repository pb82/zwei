#include "Forest.h"
#include "../Api.h"
#include "../Cc.h"
#include "../Rt.h"

Forest::Forest() : Scene(SceneForest) {}

void Forest::init() {
    Api::init();
    Api::initPlayer();
    Api::setPlayerPosition(2, 30);
    Api::loadMap("Forest.json");
    Api::setGameState();

    Api::setTrigger(2, 31, [](float angle, Entity &){
        RT_Spawn.push({25, 1});
        auto t = std::make_shared<ScreenTransition>(SceneTesting);
        Rt_Commands.push(t);
    }, nullptr);
}

void Forest::exit() {
    Api::unloadMap();
}