#ifndef ZWEI_WEAPON_H
#define ZWEI_WEAPON_H

class Weapon {
public:

    virtual float range() { return 0.0f; }

    virtual int recharge() { return 100; }

    virtual float power() { return 0.0f; }

private:
};

#endif
