#include "Entry.h"
#include "../Api.h"

Entry::Entry() : Scene(SceneEntry) {}

void Entry::init() {
    Api::init();
}

void Entry::exit() {}