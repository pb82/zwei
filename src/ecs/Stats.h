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

    void render() override;

    void equipWeapon(std::shared_ptr<Weapon> weapon);

    void dropWeapon();

    bool hasWeapon();

    // Properties

    std::shared_ptr<Weapon> weapon;

    Character character;

    Inventory inventory;

private:

    bool doRender = false;

    void renderHp();

};


#endif
