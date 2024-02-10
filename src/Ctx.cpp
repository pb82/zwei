#include "Ctx.h"

#include <JSON/printer.h>

#include "snd/Player.h"
#include "ecs/Stats.h"
#include "io/Out.h"
#include "ecs/Acceleration.h"
#include "ecs/Animation.h"
#include "ecs/Interactible.h"
#include "ecs/Id.h"
#include "ecs/Collider.h"
#include "scn/EntryPoint.h"

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
    scenes.emplace(Scene_Game, std::make_shared<EntryPoint>());
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
