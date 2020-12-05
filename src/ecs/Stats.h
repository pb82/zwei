#ifndef ZWEI_STATS_H
#define ZWEI_STATS_H

#include <memory>

#include "Component.h"
#include "arms/Weapon.h"

class Stats : public Component {
public:
    Stats(Entity &parent);

    void equipWeapon(const Weapon* w);

private:

    const Weapon* weapon;

};


#endif
