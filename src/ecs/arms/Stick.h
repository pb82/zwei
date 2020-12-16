#ifndef ZWEI_STICK_H
#define ZWEI_STICK_H

#include "Weapon.h"


class Stick : public Weapon {
public:
    Stick();

    float range() override;

    int recharge() override;

    int power() override;

    float speed() override;

    float throwback() override;

    bool isProjectile() override;

private:

    int damageTable[10] = {3, 3, 3, 3, 3, 4, 5, 5, 5, 6};

};

#endif //ZWEI_STICK_H
