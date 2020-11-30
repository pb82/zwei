#ifndef ZWEI_STICK_H
#define ZWEI_STICK_H

#include "Weapon.h"

class Stick : public Weapon {
public:
    Stick();

    float range() override;

    int recharge() override;

    float power() override;
};


#endif //ZWEI_STICK_H
