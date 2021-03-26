#include "Entry.h"

#include "../Api.h"

void Entry::init() {
    Api::init();
    Api::showNewGameMenu(true);
    Api::setMenuState();
}

void Entry::exit() {}