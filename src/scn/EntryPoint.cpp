#include "EntryPoint.h"
#include "../Api.h"

EntryPoint::EntryPoint() : Scene(Scene_Game) {}

void EntryPoint::init() {
    Api::init();
}

void EntryPoint::exit() {}