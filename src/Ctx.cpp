#include "Ctx.h"

#include "snd/Player.h"

GameStateMachine::GameStateMachine() {
    this->current.push(Game);
}

GameState GameStateMachine::currentState() {
    return this->current.top();
}

void GameStateMachine::toggleMenu() {
    if (this->current.top() == MainMenu) {
        Player::instance().resume();
        this->current.pop();
    } else {
        Player::instance().pause();
        this->current.push(MainMenu);
    }
}

void GameStateMachine::pushState(GameState state) {
    if (this->currentState() != state) {
        this->current.push(state);
    }
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