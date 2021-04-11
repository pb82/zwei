#include "Entry.h"

#include "../Api.h"

Entry::Entry() : Scene(SceneStart) {}

void Entry::init() {
    Api::init();
}

void Entry::exit() {}