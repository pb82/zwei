#include "EntryPoint.h"
#include "../Api.h"

EntryPoint::EntryPoint() : Scene(Scene_EntryPoint) {}

void EntryPoint::init() {
    Api::init();
    Api::setStartState();
}

void EntryPoint::exit() {}