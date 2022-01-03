#include "Heart.h"
#include "../Stats.h"

Heart::Heart() : Item(HEART) {}

int Heart::tile() {
    return 110;
}

bool Heart::stackable() {
    return false;
}

bool Heart::useOnPickup() {
    return true;
}

bool Heart::use(std::shared_ptr<Entity> on) {
    if (on->hasComponent<Stats>()) {
        auto stats = on->getComponent<Stats>();
        stats->character.bumpHitpoints();
        Manager::instance().addTimer(0, 5000, "Health increased");
        return true;
    }
    return false;
}