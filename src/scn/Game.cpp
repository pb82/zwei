//
// Created by peter on 11.02.24.
//

#include "Game.h"
#include "../Api.h"

Game::Game() : Scene(Scene_Game) {}

void Game::init() {
    Api::setGameState();
    Api::loadMap("t.json");
}

void Game::exit() {}