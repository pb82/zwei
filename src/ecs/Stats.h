#ifndef ZWEI_STATS_H
#define ZWEI_STATS_H

#include <memory>

#include "Component.h"
#include "arms/Weapon.h"

class Stats : public Component {
public:
    Stats(Entity &parent);

    void equipWeapon(std::shared_ptr<Weapon> weapon);

    void dropWeapon();

    bool hasWeapon();

    std::shared_ptr<Weapon> weapon;

};


#endif
