#include "EntryPoint.h"
#include "../Api.h"

EntryPoint::EntryPoint() : Scene(Scene_Game) {}

void EntryPoint::init() {
    Api::init();
    Api::setStartState();
}

void EntryPoint::exit() {}