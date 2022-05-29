#include "EntryPoint.h"
#include "../Api.h"

EntryPoint::EntryPoint() : Scene(Scene_EntryPoint) {}

void EntryPoint::init() {
    Api::init();
}

void EntryPoint::exit() {}