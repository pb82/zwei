#include "Ctx.h"

#include <JSON/printer.h>

#include "snd/Player.h"
#include "scn/Start.h"
#include "scn/Entry.h"
#include "scn/Beach.h"
#include "ecs/Stats.h"
#include "io/Out.h"

GameStateMachine::GameStateMachine() {
    this->current.push(StateStart);
}

GameState GameStateMachine::currentState() {
    return this->current.top();
}

void GameStateMachine::popState() {
    if (this->current.size() > 1) this->current.pop();
}

bool GameStateMachine::toggleMenu() {
    // Can't get out of the initial start menu by pressing esc
    if (this->current.top() == StateStart) return false;
    if (this->current.top() == StateSaving) return false;
    if (this->current.top() == StateLoading) return false;

    // In-game allow to toggle the menu
    if (this->current.top() == StateMainMenu) {
        Player::instance().resume();
        this->current.pop();
    } else {
        Player::instance().pause();
        this->current.push(StateMainMenu);
        return true;
    }
    return false;
}

void GameStateMachine::pushState(GameState state) {
    if (this->currentState() != state) {
        this->current.push(state);
    }
}

Ctx::Ctx() {
    scenes.emplace(SceneStart, std::make_shared<Entry>());
    scenes.emplace(SceneDungeon, std::make_shared<Start>());
    scenes.emplace(SceneBeach, std::make_shared<Beach>());
}

void Ctx::setPlayer(std::shared_ptr<Entity> player) {
    this->player = player;
}

void Ctx::setMenu(std::shared_ptr<Entity> menu) {
    this->menu = menu;
}

std::shared_ptr<Entity> Ctx::getPlayer() {
    return this->player;
}

std::shared_ptr<Entity> Ctx::getMenu() {
    return this->menu;
}

void Ctx::save() {
    auto stats = getPlayer()->getComponent<Stats>();
    JSON::Value to;
    to["scene"] = activeScene->getSceneType();
    stats->inventory.serialize(to);
    stats->character.serialize(to);
    this->memory.serialize(to);

    Out f("savegame.json");
    if (f.open()) {
        JSON::PrettyPrinter p;
        std::string result = p.print(to);
        f.write(result);
    }
}