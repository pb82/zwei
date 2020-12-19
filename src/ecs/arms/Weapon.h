#ifndef ZWEI_WEAPON_H
#define ZWEI_WEAPON_H

#include "../char/Character.h"

class Weapon {
public:

    virtual float range() { return 0.0f; }

    virtual float speed() { return 1.0f; }

    virtual int recharge() { return 500; }

    virtual int damage(const Character& c) { return 0.0f; }

    virtual float throwback() { return 0.0f; }

    virtual bool isProjectile() { return false; }

private:
};

#endif
