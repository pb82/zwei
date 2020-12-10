#ifndef ZWEI_WEAPON_H
#define ZWEI_WEAPON_H

class Weapon {
public:

    virtual float range() { return 0.0f; }

    virtual float speed() { return 1.0f; }

    virtual int recharge() { return 500; }

    virtual float power() { return 0.0f; }

    virtual bool isProjectile() { return false; }

private:
};

#endif
