#ifndef ZWEI_STATS_H
#define ZWEI_STATS_H

#include <memory>

#include "Component.h"
#include "arms/Weapon.h"
#include "char/Character.h"
#include "inv/Inventory.h"

class Stats : public Component {
public:
    Stats(Entity &parent, bool render = false);

    void update(float dt) override;

    bool serialize(JSON::Value &to) override;

    std::string name() { return "stats"; }

    // Properties

    Character character;

    Inventory inventory;

};


#endif
