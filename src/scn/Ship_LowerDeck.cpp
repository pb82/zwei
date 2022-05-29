#include "Ship_LowerDeck.h"
#include "../Api.h"

Ship_LowerDeck::Ship_LowerDeck() : Scene(Scene_Ship_LowerDeck) {}

void Ship_LowerDeck::init() {
    Api::init();
    Api::initPlayer();
    Api::setPlayerPosition(18, 4);
    Api::loadMap("Ship_LowerDeck.json");
    Api::setEnableLights(false);
    Api::setGameState();
}

void Ship_LowerDeck::exit() {}