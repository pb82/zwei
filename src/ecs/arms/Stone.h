#ifndef ZWEI_STONE_H
#define ZWEI_STONE_H

#include "Weapon.h"

class Stone : public Weapon {
public:
    Stone();

    ~Stone() {}

    float range() override;

    int recharge() override;

    int damage(const Character &c) override;

    float speed() override;

    float throwback() override;

    bool isProjectile() override;

    int getProjectileTile() override;

    void getParams(float *power, float *weight, float *decay) override;

private:
    int damageTable[10] = {1, 0, 2, 0, 2, 3, 3, 3, 0, 4};

};


#endif //ZWEI_STONE_H
