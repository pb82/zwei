#include "Ctx.h"

GameStateMachine::GameStateMachine() {
    this->current.push(Game);
}

GameState GameStateMachine::currentState() {
    return this->current.top();
}

void GameStateMachine::toggleMenu() {
    if (this->current.top() == MainMenu) {
        this->current.pop();
    } else {
        this->current.push(MainMenu);
    }
}

void GameStateMachine::pushState(GameState state) {
    this->current.push(state);
}

void Ctx::setPlayer(std::shared_ptr<Entity> player) {
    this->player = player;
}

std::shared_ptr<Entity> Ctx::getPlayer() {
    return this->player;
}