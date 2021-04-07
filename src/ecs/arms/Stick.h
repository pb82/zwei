#ifndef ZWEI_STICK_H
#define ZWEI_STICK_H

#include "Weapon.h"

class Stick : public Weapon {
public:
    Stick();

    ~Stick() {}

    float range() override;

    int recharge() override;

    int damage(const Character &c) override;

    float speed() override;

    float throwback() override;

    bool isProjectile() override;

    int id() { return 1; }

private:

    int damageTable[10] = {3, 3, 3, 3, 3, 4, 5, 5, 5, 6};

};

#endif //ZWEI_STICK_H
