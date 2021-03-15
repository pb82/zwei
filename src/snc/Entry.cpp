#include "Entry.h"

#include "../Api.h"

void Entry::init() {
    Api::init();
    Api::setPlayerSpeed(3);
    Api::setPlayerPosition(7, 28);
    Api::setPlayerStats(20, 10, 10, 10);
    Api::setMenuState();
}

void Entry::exit() {
    Api::unloadMap();
}